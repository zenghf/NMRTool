__author__ = 'Haifeng Zeng'
from numpy import *
import math
import scipy
from matplotlib.pyplot import plot


def expm(m):
    """
    Calculate the matrix exponential of square array m
    """
    d, v = linalg.eig(m)
    return dot(dot(v, diag(exp(d))), linalg.inv(v))

class Pool:
    def __init__(self, name='', T1=inf, T2=inf, lifeTime=1., chemicalShift=0., concentration=1.):
        self.name = name
        self.T1 = T1
        self.T2 = T2
        self.lifeTime = lifeTime
        self.chemicalShift = chemicalShift
        self.concentration = concentration

def equilibriumMag(poolList):
    """
    Calculate the thermal equilbium magnetization of poolList
    Input parameter:
        poolList: (class Pool pool1, pool2, ...)
    Output parameter:
        mInf: array of magnetization
            ([x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...])
    """
    nPools = len(poolList)
    concList = array([pool.concentration for pool in poolList])
    # mInf: array of thermal equilibrium magnetization,
    # ([x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...])
    mInf = hstack([zeros(nPools*2), concList / concList[0]])
    return mInf


def blochSimu(m0, pulse, freqList, T1, T2):
    '''
    Bloch simulation of magnetization evolution ver 1.0
    Input parameters
      m: initial magnetization in (3,) array
      pulse: nPulse x 3 array, 
            [strength1,phase1,duration1;strength2,phase2,duration2;...]
            strength: in Hz 
            phase: in radian (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
            duration: in s
      freqList: offest in Hz
      T1: in s, default value is Inf
      T2: in s, default value is Inf
    Output parameters
      mMat: 3 dimensional matrix 
        dim 1: [x y z] magnetization;
        dim 2: frequencies
        dim 3: elements of pulses, initial magnetization, then magnetization
               after each pulse element, total (nPulse+1) elements
    
    # Calculate the evolution matrix for the pulse
    # http://en.wikipedia.org/wiki/Rotation_matrix
    # section "9.3 Exponential map"
    #
    # Calculate the evolution matrix
    # dm/dt = (exchMat + chemicalShiftMat + pulseMat)m + rMat(m-mInf)
    # m is the vector of magnetization, mInf is the vector of magnetization at
    #     thermal equilibrium
    # change the equation into the following form
    # d(m+mp)/dt = (exchMat + chemicalShiftMat + pulseMat + rMat)*(m+mp)
    #         = evolMat*(m+mp)
    #    evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
    #    mp      = linalg.solve(evolMat, -rMat*yInf);
    # the solution for m is
    # m = expm(evolMat*t)*(m(0)+mp) - mp
    
    '''
    m = m0.reshape((3,1))
    nPulse = pulse.shape[0]
    nFreq = len(freqList)
    mMat = zeros((3, len(freqList), nPulse+1))
    mMat[:,:,0] = tile(m, (1, nFreq))
    rMat = diag(array((1/T2,1/T2,1/T1)),0)
    mp = array((0,0,0))
    
    for kFreq in range(nFreq):
        for kPul in range(nPulse):
            t = pulse[kPul, 2]
            x = pulse[kPul, 0] * cos(pulse[kPul,1]) * pi * 2
            y = pulse[kPul, 0] * sin(pulse[kPul,1]) * pi * 2
            z = freqList[kFreq] * pi * 2
            pulseMat = array([[0, -z, y],\
                              [z, 0, -x],\
                              [-y, x, 0]])
            if not math.isinf(T1):
                mp = linalg.solve(pulseMat+rMat, array((0,0,1/T1)))
            mMat[:,kFreq, kPul+1] = dot(expm((rMat+pulseMat)*t), mMat[:,kFreq,kPul]+mp) - mp
    return mMat
    

def blochSolveSimple(poolList, ppm, pulse, magneticField, crusher, gyro, m0):
    '''
    bloch simulation of the magnetization transfer 
    Author: Haifeng Zeng, hzeng6@jhmi.edu
    Input parameter:
        pool    = Pool class with fields {name, T1, T2, lifetime, chemicalShift, concentration}
            name: pool name
            T1: T1 of the pool
            T2: T2 of the pool
            lifetime: lifetime of the exchangable proton in that pool, 
                      1/(exchange rate), 1 for free water 
            chemicalShift: chemical shift the exchangable proton, in ppm
            concentration: concentration of the exchangable proton, in mM
        poolList = [pool1; pool2; pool3; pool4;...]; pool1 is always free water pool
        ppm: carrier frequency of the pulse, in ppm
        pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
                strength: in Hz 
                phase: in radian, (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
                duration in s
        magneticField: magnetic field in Tesla
        crusher: true of false, whether to destroy the transverse magnetization
        gyro: relative gyromagnetic ratio to 1H
        m0: initial magnetization vector in the follwoing order
             [x(freewater); x(exchangable1); x(exchangable2);...
              y(freewater); y(exchangable1); y(exchangable2);...
              z(freewater); z(exchangable1); z(exchangable2);...]
              z(freewater) = 1;
             m0 = array([0]) menas thermal equilibrium
    Output parameter:
        m: magnetization vector after the pulse
    Test code:
        water = Pool(name = 'water', T1=3., T2=1., lifeTime=1., chemicalShift=0., concentration=112000.)
        amide = Pool(name = 'amide', T1=10., T2=0.4, lifeTime=1./30., chemicalShift=3.6, concentration=10.)
        amine = Pool(name = 'amine', T1=1., T2=0.04, lifeTime=1./300., chemicalShift=2.6, concentration=10.)
        poolList = (water, amide, amine)
        ppm = 3.6
        pulse = array([[40, 0 , 2]])
        magneticField = 3.
        crusher = False
        gyro = 1.
        m0 = array([0.])
        m = blochSolveSimple(poolList, ppm, pulse, magneticField, crusher, gyro, m0)
        print "m", m.shape, real(m)   
    '''    

    # start of the function
    # define of local parameters
    m0 = array(m0)
    gyro1H = 42.576  # gyromagnetic ratio of 1H, MHz/T
    freeWater = poolList[0]
    otherPools = poolList[1:]
    nPools = len(poolList)
    nPulse = pulse.shape[0]
    
    
    # mInf: array of thermal equalibrium magnetizations, 
    #     ([x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...])
    mInf = equilibriumMag(poolList)
    if (prod(mInf.shape) == prod(m0.shape)):
        pass
    else:
        m0 = mInf.copy()
        print('Set initial magnetizations to thermal equilibrium magnetizations')
    
    
    # Calculate the exchange matrix
    other2water = [1/pool.lifeTime for pool in otherPools] 
    water2other = [pool.concentration/pool.lifeTime/freeWater.concentration for pool in otherPools]
    exchMat = zeros((nPools*3, nPools*3))
    blockExchMat = zeros((nPools, nPools))
    blockExchMat[0,1:] = other2water
    blockExchMat[1:,0] = water2other
    for k in range(3):
        exchMat[k*nPools:(k+1)*nPools, k*nPools:(k+1)*nPools] = blockExchMat
    exchMat = exchMat - diag(sum(exchMat,0))
    
    # Calculate the relaxation matrix
    r1List = array([1./pool.T1 for pool in poolList])
    r2List = array([1./pool.T2 for pool in poolList])
    rMat = -diag(hstack([r2List,r2List,r1List]))
    
    # Calculate the chemical shift matrix
    chemicalShiftList = array([pool.chemicalShift for pool in poolList])
    # wList: chemical shift evolution frequency in radian
    wList = 2*pi*magneticField*gyro1H*gyro*(chemicalShiftList - ppm)        
    chemicalShiftMat = diag(hstack([-wList,zeros(nPools)]),nPools) + \
                       diag(hstack([wList,zeros(nPools)]),-nPools)
    
    
    pulseMatList = zeros((nPools*3, nPools*3, pulse.shape[0]))
    for kPul in range(pulse.shape[0]):
        xList = ones(nPools) * pulse[kPul,0] * cos(pulse[kPul,1]) * pi * 2
        yList = ones(nPools) * pulse[kPul,0] * sin(pulse[kPul,1]) * pi * 2
        zList = zeros(nPools)
        pulseMat = diag(hstack([-zList,-xList]), nPools) + \
                    diag(yList, nPools*2) + \
                   diag(hstack([zList,xList]), -nPools) + \
                   diag(-yList, -nPools*2)
        pulseMatList[:,:,kPul] = pulseMat
        
    
    # Calculae the evolution of the magnetization
    # ref function blochSim for the algorithm
    m = m0.copy()
    for kPul in range(nPulse):
        t = pulse[kPul,2]
        if math.isinf(t):
            m = mInf.copy()
        else:
            pulseMat = pulseMatList[:,:,kPul]
            evolMat = exchMat + chemicalShiftMat + pulseMat + rMat
            mp = linalg.solve(evolMat, -dot(rMat, mInf))
            m = dot(expm(evolMat*t), (m+mp)) - mp
        
    # Crush the transverse magnetization
    if crusher:
        for k in range(nPools*2):
            m[k] = 0.
    return m


def blochSolve(poolList, ppmList, pulse, magneticField, postDynamicDelay=inf, nPulseRepeat=1, crusher = False, gyro=1., m0 = array([0])):
    """
    bloch simulation of the magnetization transfer of exchangable pools
    Author: Haifeng Zeng, hzeng6@jhmi.edu
    Input parameter:
        poolList = (class Pool pool1, pool2, pool3,...); pool1 is always free water pool
        ppmList: list of carrier frequencies of the pulses, in ppm
        pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
                strength: in Hz 
                phase: in radian, (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
                duration in s
        magneticField: magnetic field in Tesla
        postDynamicDelay: delay after acquisition, in s; 
            inf means every scan starts from thermal equilibrium magnetization
        nPulseRepeat: number of repeatition for signal avaraging
        crusher: true of false, whether to destroy the transverse magnetization
        gyro: relative gyromagnetic ratio to 1H
        m0: initial magnetization vector in the follwoing order
             [x(freewater); x(exchangable1); x(exchangable2);...
              y(freewater); y(exchangable1); y(exchangable2);...
              z(freewater); z(exchangable1); z(exchangable2);...]
              z(freewater) = 1;
             m0 = array([0]) menas thermal equilibrium
        sequence: m0->[pulse @ ppm1, Acquisition, postDynamicDelay] -> [pulse @ ppm2,Acquisition, postDynamicDelay] ...
                        \_________________________________________/      \_______________________________________/
                               repeat nPulseRepeat times                         repeat nPulseRepeat times
        The calculation is in a rotating frame with a frequency of the pulse
    Output parameter:
        mMat: three dimensional matrix of magnetization vector after the pulse
            dim 1: [x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...]
            dim 2: [ppm1; ppm2; ppm3; ...]
            dim 3: [after repeat 1; after repeat 2; after repeat 3;...]
    Test Code:
    # Test parameters
        water = Pool(name = 'water', T1=3., T2=1., lifeTime=1., chemicalShift=0., concentration=112000.)
        amide = Pool(name = 'amide', T1=1., T2=0.4, lifeTime=1./30., chemicalShift=3.6, concentration=10.)
        amine = Pool(name = 'amine', T1=1., T2=0.4, lifeTime=1./300., chemicalShift=2.6, concentration=10.)
        poolList = [water, amide, amine]
        ppmList = linspace(-10,10,101)
        pulse1 = array([[20., 0 , 2]], dtype=float)
        pulse2 = array([[0.,0.,0.01]])
        pulse3 = array([[0.,0.,0.01]])
        pulseList = [pulse1, pulse2, pulse3]
        pulse = vstack((pulse1))
        magneticField = 3.
        nPulseRepeat = 1
        crusher = False
        gyro = 1.
        postDynamicDelay = inf
        m0 = equilibriumMag(poolList)
        #print "mMat", mMat.shape
        #print mMat.reshape((3,3))
        
        blochSolve(poolList, ppmList, pulse, magneticField, postDynamicDelay=inf, nPulseRepeat=1, crusher = False, gyro=1., m0=m0)
        plt.figure()
        plt.plot(ppmList, mMat[-3,-1,:], ppmList[50:],mMat[-3,-1,50::-1]-mMat[-3,-1,50:])
        print(mMat.shape)
        print(mMat[-3,0,:])
        plt.show()
    """


    # define of local parameters
    m0 = array(m0)
    gyro1H = 42.576  # gyromagnetic ratio of 1H, MHz/T
    freeWater = poolList[0]
    otherPools = poolList[1:]
    nPulse = pulse.shape[0]
    nPools = len(poolList)
    nPPM = len(ppmList)
    if isinf(postDynamicDelay):
        nPulseRepeat = 1
        print('postDynamicDelay = inf, set nPulseRepeat 1')
    
    
    # mInf: array of thermal equalibrium magnetizations, 
    mInf = equilibriumMag(poolList)
    if (prod(mInf.shape) == prod(m0.shape)):
        pass
    else:
        m0 = mInf.copy()
        print('Set initial magnetizations to thermal equilibrium magnetizations')
    
    
    # Calculate the exchange matrix
    other2water = [1/pool.lifeTime for pool in otherPools] 
    water2other = [pool.concentration/pool.lifeTime/freeWater.concentration for pool in otherPools]
    exchMat = zeros((nPools*3, nPools*3),dtype=float)
    blockExchMat = zeros((nPools, nPools))
    blockExchMat[0,1:] = other2water
    blockExchMat[1:,0] = water2other
    for k in range(3):
        exchMat[k*nPools:(k+1)*nPools, k*nPools:(k+1)*nPools] = blockExchMat
    exchMat = exchMat - diag(sum(exchMat,0))
    
    # Calculate the relaxation matrix
    r1List = array([1./pool.T1 for pool in poolList])
    r2List = array([1./pool.T2 for pool in poolList])
    rMat = -diag(hstack([r2List,r2List,r1List]))
    
    # Calculate the chemical shift matrix
    chemicalShiftList = array([pool.chemicalShift for pool in poolList])
    chemicalShiftMatList = zeros((nPools*3, nPools*3, nPPM), dtype=float)
    for kPPM in range(nPPM):
        # wList: chemical shift evolution frequency in radian
        ppm = ppmList[kPPM]
        wList = 2*pi*magneticField*gyro1H*gyro*(chemicalShiftList - ppm)
        chemicalShiftMat = diag(hstack([-wList,zeros(nPools)]),nPools) + \
                           diag(hstack([wList,zeros(nPools)]),-nPools)
        chemicalShiftMatList[:,:,kPPM] = chemicalShiftMat
    
    # Calculate the pulse matrix
    pulseMatList = zeros((nPools*3, nPools*3, nPulse), dtype=float)
    for kPul in range(nPulse):
        xList = ones(nPools) * pulse[kPul,0] * cos(pulse[kPul,1]) * pi * 2
        yList = ones(nPools) * pulse[kPul,0] * sin(pulse[kPul,1]) * pi * 2
        zList = zeros(nPools)
        pulseMat = diag(hstack([-zList,-xList]), nPools) + \
                    diag(yList, nPools*2) + \
                   diag(hstack([zList,xList]), -nPools) + \
                   diag(-yList, -nPools*2)
        pulseMatList[:,:,kPul] = pulseMat
    
    # Calculate the transfer matrix
    # ref function blochSim for the algorithm
    expMatMat = zeros((nPools*3,nPools*3, nPulse, nPPM), dtype = float)
    mpMat = zeros((nPools*3, nPulse, nPPM), dtype = float)
    for kPPM in range(nPPM):
        chemicalShiftMat = chemicalShiftMatList[:,:,kPPM] 
        for kPulse in range(nPulse):
            pulseMat = pulseMatList[:,:,kPulse]
            t = pulse[kPulse, 2]
            evolMat = exchMat + chemicalShiftMat + pulseMat + rMat
            mp = linalg.solve(evolMat, -dot(rMat, mInf))
            expMat = expm(evolMat*t)
            expMatMat[:,:,kPulse,kPPM] = expMat
            mpMat[:,kPulse,kPPM] = mp
    
    # Calculate the transfer matrix for postDynamicDelay
    expMatDelay = eye(nPools*3, dtype=float)
    expMatDelayList = zeros((nPools*3,nPools*3,nPPM), dtype=float)
    mpDelay = zeros((nPools*3))
    mpDelayList = zeros((nPools*3,nPPM), dtype=float)
    for kPPM in range(nPPM):
        chemicalShiftMat = chemicalShiftMatList[:,:,kPPM] 
        if not isinf(postDynamicDelay):
            expMatDelay = expm((exchMat + chemicalShiftMat + rMat)*postDynamicDelay)
            mpDelay = linalg.solve(exchMat + chemicalShiftMat + rMat, -dot(rMat, mInf))
        expMatDelayList[:,:,kPPM] = expMatDelay
        mpDelayList[:,kPPM] = mpDelay
    
    
    # Calculate the evolution of the magnetization
    mMat = zeros((nPools*3, nPPM, nPulseRepeat), dtype = float)
    m = m0.copy()
    for kPPM in range(nPPM):
        expMatDelay = expMatDelayList[:,:,kPPM]
        mpDelay = mpDelayList[:,kPPM]
        for kPulseRepeat in range(nPulseRepeat):
            for kPulse in range(nPulse):
                expMat = expMatMat[:, :, kPulse, kPPM]
                mp = mpMat[:, kPulse, kPPM]
                m = dot(expMat, m+mp) - mp
            if crusher:
                for k in range(nPools*2):
                    m[k] = 0.
            mMat[:,kPPM,kPulseRepeat] = m
            if isinf(postDynamicDelay):
                m = mInf
            else:
                m = dot(expMatDelay, m+mpDelay) - mpDelay
    return mMat


# Flex process functions
def fftFlex(data, intRange = None):
    flexFid = zeros((len(data)))
    if intRange is None:
        intRange = range(data.shape[1])
    for ii in range(len(data)):
        spec = abs(fft.fftshift(fft.fft(data[ii])))
        flexFid[ii] = sum(spec[intRange])
    flexFid = flexFid / flexFid.max()
    return flexFid

def getFlexSpec(fid, tevol, o1, phase0, phase1, fill, apodize):
    area_hilbert = scipy.fftpack.hilbert(fid*exp(1j*phase0)*exp(1j*phase1*o1))
    area_pre = area_hilbert*cos(linspace(0,pi/2,len(tevol)))
    #area_pre = area_hilbert*exp(-apodize*pi*tevol)
    fid2 = concatenate( ( area_pre, zeros(fill-len(fid)) ), axis=0 )
    flexSpec = fft.fft(fid2)
    return fid2, flexSpec

def getFlexSpec2(fid, tevol, o1, phase0, phase1, fill, apodize):
    area_hilbert = fid*exp(1j*phase0)*exp(1j*phase1*o1)
    area_pre = area_hilbert*cos(linspace(0,pi/2,len(tevol)))
    #area_pre = area_hilbert*exp(-apodize*pi*tevol)
    fid2 = concatenate( ( area_pre, zeros(fill-len(fid)) ), axis=0 )
    flexSpec = fft.fft(fid2)
    return fid2, flexSpec    

def filterFlex(fid, tevol, p0):
    p = nylib.FlexLSFit(tevol, fid, p0)
    baseline = nylib.fitfunc(r_[p[0:5]], tevol)
    fidFiltered = fid - baseline
    return fidFiltered, p

#def processFlex(fid, tevol, p0, o1, phase0, phase1, fill, apodize):
#    fidFiltered, p = filterFlex(fid, tevol, p0)
#    nylib.PrintResults(p)
#    plot(tevol, fid, tevol, fidFiltered)
#    fid2, flexSpec = getFlexSpec(fidFiltered, tevol, o1, phase0, phase1, fill, apodize)
#    plot(t)



if False or __name__ == '__main__':
    m = r_[0, 0, 1]
    pulse = array([[10000, 0, 0.25e-4],\
             [10000, pi, 0.25e-14]])
    freqList = linspace(-1000,1000)
    T1 = Inf
    T2 = Inf
    mMat =  blochSimu(m, pulse, freqList, T1, T2);
    #from matplotlib import figure, pyplot.plot, pyplot.show
    #from pylab import *
    import matplotlib.pyplot as plt
    fig,ax = plt.subplots()
    plot1 = ax.plot(freqList, mMat[:,:,-1].transpose())
    ax.legend(["x", "y", "z"], loc=2)
    plt.show()
    print plot1
    