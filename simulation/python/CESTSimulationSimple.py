# This is to see if I can simulate the problem with the CEST 
# spectrum if there is no delay from one saturation 
# frequency to the next.

from __future__ import division
import bloch
from hzlib import *
from numpy import *
import matplotlib.pyplot as plt

'''
freewater = bloch.Pool('freewater', t1 = 1., t2 = 0.08, lifetime = 1., chemical_shift = -0.0001, concentration = 111200.)
#macromolecular = bloch.Pool('macromolecular', t1 = 0.7, t2 = 0.00001, lifetime = 1/3., chemical_shift = -3., concentration = 7150.)
amide = bloch.Pool('amide', t1 = 1.0, t2 = 0.055, lifetime = 1/30.,  chemical_shift = 3.6, concentration = 72.)
amine = bloch.Pool('amine', t1 = 1.0, t2 = 0.055, lifetime = 1/500.,  chemical_shift = 2.5, concentration = 72.)
hydroxyl = bloch.Pool('hydroxyl', t1 = 1.0, t2 = 0.055, lifetime = 1/1000.,  chemical_shift = 1.3, concentration =60.)
NOE1 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -1.0, concentration =72.)
NOE2 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -1.5, concentration =72.)
NOE3 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -2.0, concentration =72.)
NOE4 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -2.5, concentration =72.)
NOE5 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -3.0, concentration =72.)
NOE6 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -3.5, concentration =72.)
NOE7 = bloch.Pool('NOE', t1 = 1.0, t2 = 0.055, lifetime = 1/15.,  chemical_shift = -4.0, concentration =72.)
'''

water = bloch.Pool('water', 3., 1., 1., 0., 112000.)
amide = bloch.Pool('amide', 1., 0.4, 1./30., 3.6, 10.)
amine = bloch.Pool('amine', 1., 0.4, 1./300., 2.6, 10.)
poolList = (water, amide, amine)
poolList2 = (water, amine, amide)
pulse = array([[20., 1 , 2]], dtype=float)
freqdata = linspace(2.6,2.6,1)

a = bloch.solve(poolList, freqdata, pulse, crusher=False, magnetic_field=3, pulse_repeat=1, post_dynamic_delay=inf,verbose=0 )
b = bloch.solve(poolList2, freqdata, pulse, crusher=False, magnetic_field=3, pulse_repeat=1, post_dynamic_delay=inf,verbose=0 )
#print a.reshape((3,3))

print a.reshape((3,3))
print b.reshape((3,3))
print



water_hz = Pool('water', 3., 1., 1., 0., 112000.)
amide_hz = Pool('amide', 1., 0.4, 1./30., 3.6, 10.)
amine_hz = Pool('amine', 1., 0.4, 1./300., 2.6, 10.)
poolList_hz = (water_hz, amide_hz, amine_hz)
poolList_hz2 = (water_hz, amine_hz, amide_hz)
a2 = blochSolve(poolList_hz, freqdata, pulse, crusher=False, magneticField=3, nPulseRepeat=1, postDynamicDelay=inf )
b2 = blochSolve(poolList_hz2, freqdata, pulse, crusher=False, magneticField=3, nPulseRepeat=1, postDynamicDelay=inf )
print a2.reshape((3,3))
print b2.reshape((3,3))


