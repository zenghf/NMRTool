__author__ = 'Haifeng Zeng'
from numpy import *
import math
import scipy
import matplotlib.pyplot as plt
from hzlib import expm, Pool, equilibriumMag

#pool    = {name, T1, T2, lifetime, chemicalShift, concentration};



'''
% bloch simulation of the magnetization transfer, ver1.1
% Author: Haifeng Zeng, hzeng6@jhmi.edu
% pool    = {name, T1, T2, lifetime, chemicalShift, concentration};
%    name: pool name
%    T1: T1 of the pool
%    T2: T2 of the pool
%    lifetime: lifetime of the exchangable proton in that pool,
%              1/(exchange rate), 1 for free water
%    chemicalShift: chemical shift of the exchangable proton, in ppm
%    concentration: concentration of the exchangable proton, in mM
% pools = {pool1; pool2; pool3; pool4;...}; pool1 is always free water pool
% frequency: carrier frequency of the pulse, in ppm
% pulseCell: {pulse1, pulse2, pulse3,...}
%     pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
%         strength: in Hz
%             0: delay, no pulse
%         phase: (0:x, pi/2:y, pi:-x, 3*pi/2:-y)
%         duration: in s
%             -inf: crush transverse magnetization
%             inf:  return to thermal equlibrium magnetization
% magneticField: magnetic field in Tesla
% nPulseRepeat: number of repeat of the pulses in pulseCell
% gyro: relative gyromagnetic ratio to 1H
% m0: initial magnetization vector in the follwoing order
%     [x(freewater); x(exchangable1); x(exchangable2);...
%      y(freewater); y(exchangable1); y(exchangable2);...
%      z(freewater); z(exchangable1); z(exchangable2);...]
%      z(freewater) = 1;
%     0: thermal equilibrium
% mMat: three dimensional matrix of magnetization vector after the pulse
%     dim 1: [x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...]
%     dim 2: [ppm1; ppm2; ppm3; ...]
%     dim 3: [after repeat 1; after repeat 2; after repeat 3;...]
% sequence: m1 -> [pulse @ ppm1, postDynamicDelay] -> [pulse @ ppm2, postDynamicDelay] ...
%                 \_____________________________/      \_____________________________/
%                   repeat nPulseRepeat times              repeat nPulseRepeat times
%     The calculation is in a rotating frame with a frequency of the pulse

'''



# Test parameters
water = Pool(name = 'water', T1=3., T2=1., lifeTime=1., chemicalShift=0., concentration=112000.)
amide = Pool(name = 'amide', T1=1., T2=0.4, lifeTime=1./30., chemicalShift=3.6, concentration=10.)
amine = Pool(name = 'amine', T1=1., T2=0.4, lifeTime=1./300., chemicalShift=2.6, concentration=10.)
poolList = [water, amide, amine]
ppmList = linspace(2.6,2.6,1)
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

def blochSolve(poolList, ppmList, pulse, magneticField, postDynamicDelay=inf, nPulseRepeat=1, gyro=1., crusher = False, m0 = array([0])):
    # start of the function
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
    
    # Calculate the evolution matrix for the pulse
    # http://en.wikipedia.org/wiki/Rotation_matrix
    # section "9.3 Exponential map"
    #
    # Calculate the evolution matrix
    # dm/dt = (exchMat + chemicalShiftMat + pulseMat)y + rMat(m-mInf)
    # m is the vector of magnetization, mInf is the vector of magnetization at
    #     thermal equilibrium
    # change the equation into the following form
    # d(m+mp)/dt = (exchMat + chemicalShiftMat + pulseMat + rMat)*(m+mp)
    #         = evolMat*(m+mp)
    #    evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
    #    mp      = linalg.solve(evolMat, -rMat*yInf);
    # the solution for m is
    # m = expm(evolMat*t)*(m(0)+mp) - mp
    
    
    # Calculate the transfer matrix
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


mMat = blochSolve(poolList, ppmList, pulse, magneticField, postDynamicDelay=inf, nPulseRepeat=1, gyro=1., crusher = False, m0 = array([0]))

print "mMat", mMat.shape
print mMat.reshape((3,3))

'''
plt.figure()
plt.plot(ppmList, mMat[-3,-1,:], ppmList[50:],mMat[-3,-1,50::-1]-mMat[-3,-1,50:])
print(mMat.shape)
print(mMat[-3,0,:])
plt.show()
'''

