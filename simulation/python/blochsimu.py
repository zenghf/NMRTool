__author__ = 'Haifeng Zeng'
from numpy import *
import math
import scipy
from hzlib import expm, Pool, equilibriumMag
'''
function y1 = blochSolveSimple(pools, frequency, pulse, magneticField, crusher, gyro, m0)
% bloch simulation of the magnetization transfer 
% Author: Haifeng Zeng, hzeng6@jhmi.edu
% pool    = Pool class with fields {name, T1, T2, lifetime, chemicalShift, concentration};
%    name: pool name
%    T1: T1 of the pool
%    T2: T2 of the pool
%    lifetime: lifetime of the exchangable proton in that pool, 
%              1/(exchange rate), 1 for free water 
%    chemicalShift: chemical shift the exchangable proton, in ppm
%    concentration: concentration of the exchangable proton, in mM
% pools = [pool1; pool2; pool3; pool4;...]; pool1 is always free water pool
% ppm: carrier frequency of the pulse, in ppm
% pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
%        strength: in Hz 
%        phase: (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
%        duration in s
% magneticField: magnetic field in Tesla
% crusher: true of false, whether to destroy the transverse magnetization
% gyro: relative gyromagnetic ratio to 1H
% m0: initial magnetization vector in the follwoing order
%     [x(freewater); x(exchangable1); x(exchangable2);...
%      y(freewater); y(exchangable1); y(exchangable2);...
%      z(freewater); z(exchangable1); z(exchangable2);...]
%      z(freewater) = 1;
%     y1 = 0 menas thermal equilibrium
% m: magnetization vector after the pulse
%     The calculation is in a rotating frame with a frequency of the pulse


'''

# Test parameters
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

def blochSolveSimple(poolList, ppm, pulse, magneticField, crusher, gyro, m0):
    '''
    function y1 = blochSolveSimple(pools, frequency, pulse, magneticField, crusher, gyro, m0)
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
    
    # Calculate the evolution matrix for the pulse
    # http://en.wikipedia.org/wiki/Rotation_matrix
    # section "9.3 Exponential map"
    
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
    
    
    # Calculate the evolution matrix
    # dy/dt = (exchMat + chemicalShiftMat + pulseMat)y + rMat(y-yInf)
    # y is the vector of magnetization, yInf is the vector of magnetization at
    #     thermal equilibrium
    # change the equation into the following form
    # d(y+yp)/dt = (exchMat + chemicalShiftMat + pulseMat + rMat)*(y+yp)
    #         = evolMat*(y+yp)
    #    evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
    #    yp      = mldivide(evolMat, -rMat*yInf);
    # the solution for y is 
    # y = expm(evolMat*t)*(y(0)+yp) - yp
    
    # Calculae the evolution of the magnetization
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
    
    
    # Calculate the crusher matrix
    if crusher:
        for k in range(nPools*2):
            m[k] = 0.
    return m


m = blochSolveSimple(poolList, ppm, pulse, magneticField, crusher, gyro, m0)
print "m", m.shape, real(m)
