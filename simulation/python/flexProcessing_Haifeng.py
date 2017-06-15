
# coding: utf-8

# In[1]:

import nylib
from numpy import *
from matplotlib.pyplot import *


# In[108]:

## 50 ms exchange time
# read and process ser file
dataPath_101 = '/Users/hzeng/Data/140414_folic_acid/34'
TD = int(nylib.BrukerPar(dataPath_101, 'acqu2', 'TD'))
dataCos_101 = nylib.BrukerFid(dataPath_101)
tevol_101 = loadtxt('%s/vdlist'%dataPath_101)[:TD]

# reshape ser file
dataCos_101 = dataCos_101.reshape((TD,len(dataCos_101)/TD))


# In[113]:

len(dataCos_101)


# In[52]:

plot(dataCos_101[0])


# In[53]:

# integrate the water projection and get FLEX signal
FlexFid_101 = zeros((len(dataCos_101)))
for ii in range(len(dataCos_101)):
    ImageCos_101 = abs(fft.fftshift(fft.fft(dataCos_101[ii])))
    FlexFid_101[ii] = sum(ImageCos_101[20:220])
FlexFid_101 = FlexFid_101/FlexFid_101.max()


# In[54]:

# example projection
ExampleProjection = abs(fft.fftshift(fft.fft(dataCos_101[0])))
plot(ExampleProjection)


# In[55]:

# display FLEX data
figure(2).show()
clf()
plot(tevol_101[:len(FlexFid_101)], FlexFid_101, 'bo-')
figure(2).canvas.draw()


# In[117]:

# fit out baseline
p0 = array( [0.1, 0.7, 10., 1., 500., 0.1, 1000., 1., 500.])
p1 = nylib.FlexLSFit(tevol_101, FlexFid_101, p0)
nylib.PrintResults(p1)
baseline = nylib.fitfunc(r_[p1[0], p1[1:5]], tevol_101)
figure(2).hold(True)
plot(tevol_101*1e3, FlexFid_101, 'b')
plot(tevol_101*1e3, nylib.fitfunc(p1, tevol_101),'g')
plot(tevol_101*1e3, baseline, 'r')
figure(2).hold(False)
fidFiltered_101 = FlexFid_101 - baseline


# In[58]:

# plot baseline corrected fid
plot(tevol_101*1e3, fidFiltered_101)


# In[59]:

import scipy
def getSpectrum(fid, tevol, o1, phase0, phase1, fill, apodize):
	area_hilbert = scipy.fftpack.hilbert(fid*exp(1j*phase0)*exp(1j*phase1*o1))
	area_pre = area_hilbert*cos(linspace(0,pi/2,len(tevol)))
	#area_pre = area_hilbert*exp(-apodize*pi*tevol)
	fid2 = concatenate( ( area_pre, zeros(fill-len(fid)) ), axis=0 )
	FLEXSpect = real(fft.fft(fid2))
	return fid2, FLEXSpect


# In[82]:

# transform to Fourier domain
o1 = float(nylib.BrukerPar(dataPath_101, 'acqu', 'O2')) - float(nylib.BrukerPar(dataPath_101, 'acqu', 'O1'))
#fid2, FlexSpectrum = nylib.getSpectrum((fidFiltered - nylib.fitfunc(r_[0, p1[1:5]], tevol)), tevol, o1, 0.0, 0., 8192*2, 1)
fid2, FlexSpectrum_101 = getSpectrum(fidFiltered_101, tevol_101, o1, 3.8, 0., 8192*2, 1)

ppm_101 = nylib.PpmScale(FlexSpectrum_101, o1, 750, tevol_101[2]-tevol_101[1], '1D')


# In[83]:

# fid before FT
plot(fid2[:500])


# In[92]:

# display spectrum
figure(3).show()
clf()
plot(ppm_101,FlexSpectrum_101)
xlim(10,-5)
#ylim(-0.05,0.15)
grid('on')
figure(3).canvas.draw()


# In[90]:

### 8 ms exchange time
# read and process ser file
dataPath_100 = 'ny/flexTest/100'
dataCos_100 = nylib.BrukerFid(dataPath_100)
tevol_100 = loadtxt('%s/vdlist'%dataPath_100)
# reshape ser file
dataCos_100 = dataCos_100.reshape((len(tevol_100),len(dataCos_100)/len(tevol_100)))


# In[88]:

dataCos_100 = dataCos_100[:142]
tevol_100 = tevol_100[:142]


# In[72]:

# integrate the water projection and get FLEX signal
FlexFid_100 = zeros((len(tevol_100)))
for ii in range(len(tevol_100)):
    ImageCos_100 = abs(fft.fftshift(fft.fft(dataCos_100[ii])))
    FlexFid_100[ii] = sum(ImageCos_100[20:220])
FlexFid_100 = FlexFid_100/FlexFid_100.max()


# In[73]:

# example projection
ExampleProjection = abs(fft.fftshift(fft.fft(dataCos_100[0])))
plot(ExampleProjection)


# In[74]:

# display FLEX data
figure(2).show()
clf()
plot(tevol_100, FlexFid_100, 'bo-')
figure(2).canvas.draw()


# In[75]:

# fit out baseline
p0 = array( [0.9, 0.05, 9680., 1., 30., 0.03, 8300, 1., 500., 0.03, 100, 1., 100.])
p1 = nylib.FlexLSFit(tevol_100, FlexFid_100, p0)
nylib.PrintResults(p1)
baseline = nylib.fitfunc(r_[p1[0], p1[9:13]], tevol_100)
figure(2).hold(True)
plot(tevol_100*1e3, FlexFid_100, 'b')
plot(tevol_100*1e3, nylib.fitfunc(p1, tevol_100))
plot(tevol_100*1e3, baseline, 'r')
figure(2).hold(False)
fidFiltered_100 = FlexFid_100 - baseline


# In[76]:

# plot baseline corrected fid
plot(tevol_100*1e3, fidFiltered_100)


# In[77]:

# transform to Fourier domain
o1 = float(nylib.BrukerPar(dataPath_100, 'acqu', 'O2')) - float(nylib.BrukerPar(dataPath_100, 'acqu', 'O1'))
#fid2, FlexSpectrum = nylib.getSpectrum((fidFiltered - nylib.fitfunc(r_[0, p1[1:5]], tevol)), tevol, o1, 0.0, 0., 8192*2, 1)
fid2, FlexSpectrum_100 = getSpectrum((fidFiltered_100 - nylib.fitfunc(r_[0, p1[1:5]], tevol_100)), tevol_100, o1, 0.0, 0., 8192*2, 1)

ppm_100 = nylib.PpmScale(FlexSpectrum_100, o1, 750, tevol_100[2]-tevol_100[1], '1D')


# In[78]:

# fid before FT
plot(fid2[:170])


# In[79]:

# display spectrum
figure(3).show()
clf()
plot(ppm_100,FlexSpectrum_100)
xlim(10,-10)
#ylim(-0.05,0.15)
grid('on')
figure(3).canvas.draw()


# In[114]:

# read in watergate spectrum
dataPath_watergate = 'ny/InvivoProteinSpect/11'


# In[115]:

data_wg = nylib.BrukerFid(dataPath_watergate, file='fid')
field = float(nylib.BrukerPar(dataPath_watergate, 'acqu', 'SFO1'))
sw = float(nylib.BrukerPar(dataPath_watergate, 'acqu', 'SW_h'))
o1 = float(nylib.BrukerPar(dataPath_watergate, 'acqu', 'O1'))
TD = int(nylib.BrukerPar(dataPath_watergate, 'acqu', 'TD'))


# In[116]:

#plot(data_wg[:100])
where(data_wg == data_wg.max())


# In[117]:

acq_t = linspace(0, (TD/2)/sw, (TD/2))


# In[118]:

data_wg_cropped = data_wg[69:]
tevol = acq_t[69:]


# In[130]:

plot(tevol, imag(data_wg_cropped))
plot(tevol, real(data_wg_cropped))


# In[103]:

dataS = nylib.BrukerSpectrum(data_wg_cropped, 1024*32)
ppm = nylib.ppmScale(dataS, dataPath_watergate) - o1/field


# In[104]:

plot(ppm, dataS)
xlim(10, -10)
ylim(0, 1e7)


# In[ ]:




# In[144]:

# fit out baseline
"""
p0 = array( [0.9, 0.05, 9680., 1., 30., 0.03, 8300, 1., 500., 0.03, 100, 1., 100.])
p1 = nylib.FlexLSFit(tevol, real(data_wg_cropped), p0)
nylib.PrintResults(p1)
baseline = nylib.fitfunc(r_[p1[0], p1[9:13]], tevol)
figure(2).hold(True)
plot(tevol*1e3, data_wg_cropped, 'b')
plot(tevol*1e3, nylib.fitfunc(p1, tevol))
plot(tevol*1e3, baseline, 'r')
figure(2).hold(False)
"""
p0_2 = array( [-1e5,  1e5, 100, 1., 100.])
p2 = nylib.FlexLSFit(tevol, imag(data_wg_cropped), p0_2)
nylib.PrintResults(p2)
fidFiltered_wg_real = real(data_wg_cropped) - baseline
baseline2 = nylib.fitfunc(p2, tevol)
fidFiltered_wg_imag = imag(data_wg_cropped) - baseline2


# In[148]:

plot(tevol*1e3, imag(data_wg_cropped), 'b')
plot(tevol*1e3, nylib.fitfunc(p2, tevol), 'r')
plot(tevol*1e3, baseline2, 'r')


# In[146]:

figure()
subplot(121)
plot(fidFiltered_wg_real)
subplot(121)
plot(fidFiltered_wg_imag)


# In[149]:

fidFiltered_wg = fidFiltered_wg_real + 1j * fidFiltered_wg_imag
dataS = nylib.BrukerSpectrum(fidFiltered_wg, 1024*32)


# In[154]:

plot(ppm, dataS)
xlim(10, -10)
ylim(-1e6, 3e6)


# In[177]:

# overlay the three images
figure(figsize=(10,10))
plot(ppm, dataS/dataS.max()/2, label='1D watergate')
plot(ppm_100,FlexSpectrum_100, label = 'FLEX texch = 8 ms')
plot(ppm_101,FlexSpectrum_101, label = 'FLEX texch = 50 ms')

xlabel('chemical shift (ppm)')
xlim(10, -10)
ylim(-0.15, 0.6)
legend(loc='upper right')
grid('on')
savefig('EggWhiteFLEX.pdf')


# In[178]:

# overlay the three images
figure(figsize=(10,10))
plot(ppm, dataS/dataS.max()*10-0.7, label='1D watergate')
plot(ppm_100,FlexSpectrum_100, label = 'FLEX texch = 8 ms')
plot(ppm_101,FlexSpectrum_101, label = 'FLEX texch = 50 ms')

xlabel('chemical shift (ppm)')
xlim(10, 0)
ylim(-0.15, 0.6)
legend(loc='upper right')
grid('on')
savefig('EggWhiteFLEX_zoomed.pdf')


# In[8]:

# load exported data
from matplotlib import gridspec
figure(figsize=(6, 10))
gs = gridspec.GridSpec(3, 1)
ax1 = subplot(gs[0, 0])
ax2 = subplot(gs[1, 0])
ax3 = subplot(gs[2, 0])


# In[ ]:



