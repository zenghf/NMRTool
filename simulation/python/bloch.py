from __future__ import division
from numpy import zeros, pi, cos, exp, sin, sqrt, array, dot, concatenate, inf, r_, diag, tile, ones, savetxt, nonzero, inf#, rot90
from numpy.linalg import pinv
import scipy.integrate
import scipy.linalg
from progressbar import ProgressBar, Percentage, Bar, ETA
from scipy.optimize import fmin_l_bfgs_b, leastsq, fmin_tnc
from matplotlib.pyplot import *
import time

def createA(k1, k2, Ca, C, w1, wtis, w):

    npools = len(k1)

    A = zeros(( npools*3, npools*3 ) )

    ##
    ##  Create diagonal
    ##
    A = A + diag( r_[ -k2, -k2, -k1 ] )

    ##
    ## Create the lower off diagonals
    ##
    for pooli in range(1, npools ):

        A = A + diag( tile( r_[Ca[pooli], zeros( (npools-1,) )], 3 )[:-pooli], k=-pooli )

    A = A + diag( r_[ wtis-w, w1*ones( (npools,1) ) ].ravel(), k=-(npools) )

    ##
    ## Create the upper off diagonals
    ##
    for pooli in range(1, npools ):
        A = A + diag( tile( r_[C[pooli], zeros( (npools-1,) )], 3 )[:-pooli], k=pooli )

    A = A + diag( r_[ w-wtis, -w1*ones( (npools,1) ) ].ravel(), k=npools )

    return A

def updateA(A, w1, lineshape):

    npools = A.shape[0]/3 

    A[ nonzero( diag( r_[ zeros( (npools, 1)), ones( (npools,1) ) ].ravel(), k=-npools ) > 0 ) ] = w1

    A[ nonzero( diag( r_[ zeros( (npools, 1)), ones( (npools,1) ) ].ravel(), k=npools ) > 0 ) ] = -w1

    A[-1,-1] = lineshape

    return A

class Pool():
    name = ''
    t1 = 0          # in s
    t2 = 0          # in s
    lifetime = 0        # in s  ==  1 / exchange rate
    chemical_shift = 0  # in ppm
    concentration = 0   # in mM

    def __init__(self, name, t1, t2, lifetime, chemical_shift, concentration):
        self.name = name
        self.t1 = t1
        self.t2 = t2
        self.lifetime = lifetime
        self.chemical_shift = chemical_shift
        self.concentration = concentration

    def copy(self):
        return Pool( self.name, self.t1, self.t2, self.lifetime, self.chemical_shift, self.concentration )

    def textify(self):
        return '%s: T1=%3.2g s, T2=%3.2g s, lifetime=%3.2g s \n %s cs=%3.2g ppm, []=%3.2g mM' % (self.name, self.t1, self.t2, self.lifetime, ' '*len(self.name), self.chemical_shift, self.concentration)

class Pulse():
    name = ''
    amplitude = 0
    phase = 0
    duration = 0
        
    def __init__(self, name, amplitude, phase=1):
        self.name = name
        self.amplitude = amplitude
        self.phase = phase

    def getpulse(self, B1, duration):
        out_amplitude = self.amplitude / self.amplitude.max() * B1
        out_time = linspace( 0, duration, len(self.amplitude) )

        return (out_time, out_amplitude)

def solve(pools, frequencies, pulse, pulse_repeat=1, magnetic_field=3, verbose=1, crusher=True, post_dynamic_delay=inf):
    ''' Solve the bloch equation with exchange
    return value: z = zeros((len(pools)*3, len(frequencies), pulse_repeat));
                      dim 1: [x_pool_water, x_pool1, ..., y_pool_water, y_pool1, ... z_pool_water, z_pool1, ...]
    pulse : array of nPulse * 3, [[pulseStrength, phase(1:x, -1:-x), duration]...]
            pulseStrength: in unit of Hz
            phase: only 1 or -1
            duration: in s

    '''
#  z = zeros((len(pools)*3, len(frequencies), pulse_repeat));
#  For the pools parameter, the first one (index=0) is always the freewater 
#
    if magnetic_field > 25:
        print 'bloch::solve: Magnetic field should be in T (you passed in %3.2f)' % (magnetic_field)
        return None

    if len(pools) < 1:
        print 'bloch::solve: Must have at least one pool !! '
        return None

    magnet_freq = magnetic_field * 42.576

    ##
    ##  Calculate teh initial pool concentrations
    ##
    #  tt: array of concentrations of all pools
    tt = array( [ pool.concentration for pool in pools ] )
    #  y0: array of initial magnetizations, (x1, x2, ...y1, y2,... z1, z2, ...)
    y0 = r_[  zeros( len(pools), ), zeros( len(pools), ), tt ] / pools[0].concentration

    ##
    ##  Calculate the frequencies and offset of the pools.
    ##
    # freq:  in ppm
    # w_pools: in radian
    freq = zeros((len(pools),1))   
    w_pools = zeros((len(pools),1))
    for ii in range(len(pools)):
        freq[ii] = pools[ii].chemical_shift;
        w_pools[ii] = 2*magnet_freq*pi*freq[ii] 
    

    ##### NOW CALLED W_POOLS ########
    #wa = 2*magnet_freq*pi*freq[0] 
    #wb = 2*magnet_freq*pi*freq[1]
    #wc = 2*magnet_freq*pi*freq[2]
    #wd = 2*magnet_freq*pi*freq[3]

    # C: exchange rate, from exchangable proton to free water
    C = zeros((len(pools),))
    for ii in range(1,len(pools)):
        C[ii] = 1/pools[ii].lifetime    
    # Ca: exchange rate, from free water to exchangable proton
    Ca = y0[ len(pools)*2: ] / y0[len(pools)*2] * C

    ######## USED TO BE CALLED Ca ############ 
    C[0] = sum(Ca)
    
    ##
    ##  Calculate the pseudo exchange rates
    ##
    t1 = array( [ pool.t1 for pool in pools ] )
    k1 = zeros((len(pools),))
    k2 = zeros((len(pools),))
    for ii in range( len(pools) ):
        k1[ii] = 1/pools[ii].t1 + C[ii];
        k2[ii] = 1/pools[ii].t2 + C[ii];
    
    # nyadav changed to return x, y z mags for all pools
    z = zeros((len(pools)*3, len(frequencies), pulse_repeat));
#   z = zeros((len(frequencies), pulse_repeat));
#   mx = zeros((len(frequencies), pulse_repeat));
#   my = zeros((len(frequencies), pulse_repeat));

    ##
    ##  Create a copy of the initial signal intensities
    ##
    y = y0.copy()

    ##
    ##  Create the b vector
    ##
    b =  concatenate( ( zeros( len(pools), ), zeros( len(pools), ), y0[ len(pools)*2: ] / t1  ) )

#progress bar
    if verbose > 0:
        pbar = ProgressBar(widgets=['Frequencies: ', Percentage(), Bar(), ETA()], maxval=len(frequencies)).start()

    ##
    ##  Loop over the frequenices
    ##
    for k in range(len(frequencies)):

        ## 
        ##  Determine if we are going to do a delay between
        ##  dynamics or if we are going to assume an infinite delay
        ##
        if post_dynamic_delay == inf:
            y=y0.copy() 
        else:
            ## If the post_dynamic_delay is not infinite then we need to take into account a
            ## delay between the dynamics.  This isn't pretty but I believe it works correctly.
            w1 = 0
            w = 2*magnet_freq*pi*frequencies[k]
            lineshape=0
            
            A = createA(k1, k2, Ca, C, 0, w_pools, w)

            A = updateA( A, w1,  -k1[-1])

            Ainvb = scipy.linalg.solve( A, b )

            y = dot(scipy.linalg.expm(A*post_dynamic_delay),(y+Ainvb))-Ainvb # Analytical solution
            
        w = 2*magnet_freq*pi*frequencies[k]
        A = createA(k1, k2, Ca, C, 0, w_pools, w)

        F = lambda th,T2a,w,wa: ((T2a /abs(3.0*cos(th) *cos(th)-1)) *exp(-2*(((wa-w) *T2a)/(3*cos(th) *cos(th)-1))**2)) * sin(th)
        Q = scipy.integrate.quad(F,0,pi/2, args=(pools[-1].t2,w,w_pools[-1]))[0]

        ##
        ##  Pre compute expm(A*t)
        ##
        expmA = zeros( r_[len(pulse), A.shape ])
        Ainvbm = zeros( r_[len(pulse), A.shape[0] ])
        for m in range(pulse.shape[0]):

            w1 = 2*pi*pulse[m,0]*pulse[m,1]
            lineshape=-pi*w1*w1*sqrt(2/pi)*Q
            
            A = createA(k1, k2, Ca, C, 0, w_pools, w)
            A = updateA(A, w1, -k1[-1]+lineshape)

            expmA[m] = scipy.linalg.expm(A*pulse[m,2])

            Ainvbm[m] = scipy.linalg.solve( A, b )

        ##
            ##  Repeat the pulse.
        ##
        for pulsei in range( pulse_repeat ):
            
            ##
            ##  Loop over the parts of the pulse
            ##
            for m in range(pulse.shape[0]):

                ## **** ALL THIS STUFF BELOW IS PRECALCULATED ABOVE NOW ****

                #w1 = 2*pi*pulse[m,0]*pulse[m,1]
                
                #A = updateA(A, w1)

                #Ainvb = scipy.linalg.solve( A, b )

                #y = dot(scipy.linalg.expm2(A*pulse[m,2]),(y+Ainvb))-Ainvb # Analytical solution
                y = dot(expmA[m],(y+Ainvbm[m]))-Ainvbm[m] # Analytical solution

            ##
            ##  Crusher 
            ##
            if crusher == True:
                y[0:2*len(pools)]= zeros(( 2*(len(pools)), ))
            #print y    
            ##
            ##  Store the solution
            ##
            # nyadav modified to give x,y,z mags of all pools
            z[:,k,pulsei] = y
#           mx[k,pulsei], my[k,pulsei], z[k,pulsei] = y[0], y[ len(pools) ], y[ len(pools)*2  ]
            # Have to output y[1], y[len(pools)+1], y[len(pools)+2]

            

        if verbose > 0:
            pbar.update(k)


    if verbose > 0 :
        pbar.finish()

    return z

def solve0(pools, frequency, pulse, magnetic_field=3, y0 = 0, crusher=True, gyromagnetic_ratio_r = 1):
###  pools in thermal equalibrium <-> [pulse at frequency]         
###  crush transverse magnetization if crusher = True
###      
###  frequency : in ppm   
###  pulse: n by 3 matrix
###       pulse[,0]:  phase, 1 mean x, -1 mean -x
###       pulse[,1]:  pulse strength in Hz
###       pulse[,2]: duration in second
###  return value
###     [x_freewater, x_hydroxyl,... y_freewater, y_hydroxyl,... z_freewater, z_hydroxyl...;]

#
#  For the pools parameter, the first one (index=0) is always the freewater 
#
    if magnetic_field > 25:
        print 'bloch::solve: Magnetic field should be in T (you passed in %3.2f)' % (magnetic_field)
        return None

    if len(pools) < 1:
        print 'bloch::solve: Must have at least one pool !! '
        return None

    magnet_freq = magnetic_field * gyromagnetic_ratio_r * 42.57

    ##
    ##  Calculate teh initial pool concentrations
    ##
    tt = array( [ pool.concentration for pool in pools ] )
    if (type(y0) != type(array(1)) or len(y0) != len(pools)*3):
        y0 = r_[  zeros( len(pools), ), zeros( len(pools), ), tt ] / pools[0].concentration
    #print(y0)
    print '[' + ', '.join('%10.3e' % v for v in y0) + ']'
    ##
    ##  Calculate the frequencies and offset of the pools.
    ##
    freq = zeros((len(pools),1))
    w_pools = zeros((len(pools),1))
    for ii in range(len(pools)):
        freq[ii] = pools[ii].chemical_shift;
        w_pools[ii] = 2*magnet_freq*pi*freq[ii] 

    C = zeros((len(pools),))
    for ii in range(1,len(pools)):
        C[ii] = 1/pools[ii].lifetime    

    Ca = y0[ len(pools)*2: ] / y0[len(pools)*2] * C

    ######## USED TO BE CALLED Ca ############ 
    C[0] = sum(Ca)
    
    ##
    ##  Calculate the pseudo exchange rates
    ##
    t1 = array( [ pool.t1 for pool in pools ] )
    k1 = zeros((len(pools),))
    k2 = zeros((len(pools),))
    for ii in range( len(pools) ):
        k1[ii] = 1/pools[ii].t1 + C[ii];
        k2[ii] = 1/pools[ii].t2 + C[ii];

    y = y0.copy()

    ##
    ##  Create the b vector
    ##
    b =  concatenate( ( zeros( len(pools), ), zeros( len(pools), ), y0[ len(pools)*2: ] / t1  ) )
    
    w = 2*magnet_freq*pi*frequency
    A = createA(k1, k2, Ca, C, 0, w_pools, w)

    F = lambda th,T2a,w,wa: ((T2a /abs(3.0*cos(th) *cos(th)-1)) *exp(-2*(((wa-w) *T2a)/(3*cos(th) *cos(th)-1))**2)) * sin(th)
    Q = scipy.integrate.quad(F,0,pi/2, args=(pools[-1].t2,w,w_pools[-1]))[0]

    ##
    ##  Pre compute expm(A*t)
    ##
    expmA = zeros( r_[len(pulse), A.shape ])
    Ainvbm = zeros( r_[len(pulse), A.shape[0] ])
    for m in range(pulse.shape[0]):

        w1 = 2*pi*pulse[m,0]*pulse[m,1]
        lineshape=-pi*w1*w1*sqrt(2/pi)*Q
        
        A = createA(k1, k2, Ca, C, 0, w_pools, w)
        A = updateA(A, w1, -k1[-1]+lineshape)

        expmA[m] = scipy.linalg.expm(A*pulse[m,2])

        Ainvbm[m] = scipy.linalg.solve( A, b )
        
    ##
    ##  Loop over the parts of the pulse
    ##
    for m in range(pulse.shape[0]):
        y = dot(expmA[m],(y+Ainvbm[m]))-Ainvbm[m] # Analytical solution

    ##
    ##  Crusher 
    ##
    if crusher == True:
        y[0:2*len(pools)]= zeros(( 2*(len(pools)), ))
    #print('\n**********************\n')
    return y



def fit(signal, pools, frequencies, pulse, magnetic_field=3, crusher=False, post_dynamic_delay=inf, pulse_repeat=1, verbose=0):


    ##
    ##  First, calculate the approximate center of the water peak
    ##  and correct the frequencies based on that.
    ##

    fitinds = nonzero( abs( frequencies ) < 1 )[0]

    if len(fitinds) < 3:
        print 'bloch.fit:  It seems the frequencies are in Hz.  Might want to correct that.'

    offset = calcOffset( frequencies[fitinds], signal[fitinds] )
    #frequencies = frequencies - offset

    #
    #  The pools need to be defined such that if a variable is 
    #  a singla value (e.g., pools[0].t1 = 1.5 ) then it is 
    #  considered static.  If a pool is defined as a triple
    #  of values (e.g., 1.5, 0.5, 3) then the first is the inital
    #  guess, the second is the lower bound and the third is the
    #  upper bound.
    #

    x0 = []
    bounds = []
    index = []
    for pi,pool in enumerate(pools):
        if type(pool.t1).__name__ == 'tuple':
            x0.append( pool.t1[0] )
            bounds.append( pool.t1[1:3] )
            if pool.t1[1] > pool.t1[2]:
                print 'fit: ERROR: bounds are reversed for T1 for %s' % ( pool.name )
            index.append(  (pi, 't1') )

        if type(pool.t2).__name__ == "tuple":
            x0.append( pool.t2[0] )
            bounds.append( pool.t2[1:3] )
            if pool.t2[1] > pool.t2[2]:
                print 'fit: ERROR: bounds are reversed for T2 for %s' % ( pool.name )
            index.append(  (pi, 't2') )

        if type(pool.chemical_shift).__name__ == "tuple":
            x0.append( pool.chemical_shift[0] )
            bounds.append( pool.chemical_shift[1:3] )
            if pool.chemical_shift[1] > pool.chemical_shift[2]:
                print 'fit: ERROR: bounds are reversed for Chemical Shift for %s' % ( pool.name )
            index.append(  (pi, 'chemical_shift') )

        if type(pool.concentration).__name__ == "tuple":
            x0.append( pool.concentration[0] )
            bounds.append( pool.concentration[1:3] )
            if pool.concentration[1] > pool.concentration[2]:
                print 'fit: ERROR: bounds are reversed for Concentration for %s' % ( pool.name )
            index.append(  (pi, 'concentration') )

        if type(pool.lifetime).__name__ == "tuple":
            x0.append( pool.lifetime[0] )
            bounds.append( pool.lifetime[1:3] )
            if pool.lifetime[1] > pool.lifetime[2]:
                print 'fit: ERROR: bounds are reversed for Lifetime for %s' % ( pool.name )
            index.append(  (pi, 'lifetime') )

    ##  This is the frequency offset that we are going to try and fit out as well.
    x0.append( offset )
    bounds.append( (-0.2, 0.2) )
        
    ##
    ##  Now do the actual minimization
    ##
    x = fmin_tnc( fit_residuals, x0, args=(signal, index, pools, frequencies, pulse, magnetic_field, pulse_repeat, crusher, post_dynamic_delay), bounds=bounds, approx_grad=True, messages=0, epsilon=1e-1)[0]
    #x = fmin_tnc( fit_residuals, x0, args=(signal, index, pools, frequencies, pulse, magnetic_field, pulse_repeat, crusher, post_dynamic_delay), bounds=bounds, approx_grad=True, messages=0, epsilon=0e1, ftol=1.0, xtol=1.0)[0]
    #x = fmin_tnc( fit_residuals, x0, args=(signal, index, pools, frequencies, pulse, magnetic_field, pulse_repeat, crusher, post_dynamic_delay), bounds=bounds, approx_grad=True, messages=0, epsilon=0.1)[0]
    #x = fmin_l_bfgs_b( fit_residuals, x0, args=(signal, index, pools, frequencies, pulse, magnetic_field, pulse_repeat, crusher, post_dynamic_delay), bounds=bounds, approx_grad=True, factr=1e8)[0]

    print 'Frequency offset: ', x[-1]
    offset = x[-1]
    x = x[:-1]

    ## Calculate what we are going to return
    pools_pass = []
    for pool in pools:
        pools_pass.append( pool.copy() )

    for ii, ind in enumerate( index ):
        if ind[1] == 't1':
            pools_pass[ ind[0] ].t1 = x[ii]
        if ind[1] == 't2':
            pools_pass[ ind[0] ].t2 = x[ii]
        if ind[1] == 'chemical_shift':
            pools_pass[ ind[0] ].chemical_shift = x[ii]
        if ind[1] == 'concentration':
            pools_pass[ ind[0] ].concentration = x[ii]
        if ind[1] == 'lifetime':
            pools_pass[ ind[0] ].lifetime = x[ii]

    print 'The solution is: '
    for p in pools_pass:
        print p.textify()

    ##  Now return the calculated signal
    #calculated_signal = solve(pools_pass, frequencies, pulse, magnetic_field=3, verbose=1, pulse_repeat=pulse_repeat, crusher=crusher, post_dynamic_delay=post_dynamic_delay)[:,-1] * 100
    
    return pools_pass, offset

def fit_residuals( x, meas_signal, index, pools, frequencies, pulse, magnetic_field, pulse_repeat, crusher, post_dynamic_delay):
    ## 
    ##  This will calculate the residuals between the measuresd signal and the
    ##  calcualted signal based on the parameters.
    ##

    offset = x[-1]
    x = x[:-1]
    
    ##  Create the set of pools and fill in the variable parts based on 
    ##  the current value of 'x'
    pools_pass = []
    for pool in pools:
        pools_pass.append( pool.copy() )

    for ii, ind in enumerate( index ):
        if ind[1] == 't1':
            pools_pass[ ind[0] ].t1 = x[ii]
        if ind[1] == 't2':
            pools_pass[ ind[0] ].t2 = x[ii]
        if ind[1] == 'lifetime':
            pools_pass[ ind[0] ].lifetime = x[ii]
        if ind[1] == 'chemical_shift':
            pools_pass[ ind[0] ].chemical_shift = x[ii]
        if ind[1] == 'concentration':
            pools_pass[ ind[0] ].concentration = x[ii]

    ##  Calculate the signal based on the static variables
    ##  and the current value of the current paramters.
    calculated_signal = solve(pools_pass, frequencies-offset, pulse, magnetic_field=3, pulse_repeat=pulse_repeat, crusher=crusher, post_dynamic_delay=post_dynamic_delay, verbose=0)[:,-1] * 100

    fig2 = figure(2)
    clf()
    plot( frequencies, meas_signal, 'bo-' )
    plot( frequencies, calculated_signal, 'rx-' )
    fig2.canvas.draw()

    dd = sqrt( sum( ( meas_signal - calculated_signal )**2 ) )
    print dd
    return dd

def calcOffset( freq, mm ):
    def lorentzian(freq, A, x0, w, b, k):  
        return A * ( 1 - ( w**2 / ( k* ( w**2 + (x0-freq)**2 ) ) ) ) + b  

    def lorentzian_min(x, freq, data):
        return sum( (lorentzian(freq, x[0], x[1], x[2], x[3], x[4]) - (data) )**2 )  

    def lorentzianFit(freq, data):

        A = max(data)
        x0 = freq[ nonzero( data == min(data) )[0][0] ] # Hz 
        w = 1 # Hz 
        b = 0  
        k = 1

        x = fmin_l_bfgs_b(lorentzian_min, fprime=[], x0=(A,x0,w, b, k), args=(freq,data), approx_grad=True, bounds=((None, None),(-1, 1),(None, None),(None, None),(None,None)))[0]

        return x

    A,x0,w,b,k = lorentzianFit( freq, mm )

    return x0

