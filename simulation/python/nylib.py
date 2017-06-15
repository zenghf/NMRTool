# from __future__ import division
from numpy import *
from matplotlib.pyplot import *
import scipy.fftpack
from scipy import optimize
import os
import re
import struct
#import cjlib
from scipy import ndimage
#from matplotlib.nxutils import pnpoly
import mmap

#############################################################
########################## Read in data #####################
#############################################################


## Read Bruker Topspin data

    # read in Bruker fid
def BrukerFid(directory, file='ser'):
	data = fromfile(file='%s/%s' % (directory, file), dtype=dtype('i4'))
	fid=data.astype(float32).view(complex64)
	return fid
	
	# get spectrum
def BrukerSpectrum(fid, fill):
	fid2 = concatenate( ( fid, zeros(fill-len(fid)) ), axis=0 )
	Spect1 = scipy.fftpack.fft(fid2)
	Spect2 = abs(scipy.fftpack.fftshift(Spect1))
	return Spect2
	
	# plot spectrum
def ppmScale(spect, directory = '.'):
	SW = float(BrukerPar(directory, 'acqu', 'SW'))
	o1 = float(BrukerPar(directory, 'acqu', 'O1'))
	field = float(BrukerPar(directory, 'acqu', 'SFO1'))
	offset=o1/field
	n = len(spect)
	ppm = linspace(-SW/2 + offset, SW/2 + offset, len(spect))
	return ppm

def ParavisionMultiFolder(directory, exp0, expF, x, y, DataType):
	fns = arange(exp0, expF+1)
	d = zeros((len(fns), x,y ))
	
	for ii,fn in enumerate( fns ):
		## Read in the reco data
		d[ii] = cjlib.readraw( '%s/%d/pdata/1/2dseq' % ( directory, fn ), r_[ x, y  ], DataType )
		bb = BrukerPar('%s/%s'%(directory,fn), 'reco', 'RECO_map_slope')
		Doffset = BrukerPar('%s/%s'%(directory,fn), 'reco', 'RECO_map_offset')
		d[ii] = (d[ii] / bb) - Doffset
	return d


def Paravision2dseqNew(directory):
	## check if 2dseq file present
	try:
		with open('%s/pdata/1/2dseq' % directory) as f: pass
	except IOError as e:
		print '2dseq file not detected'
	
	## Read in acqusition paramters and reconstruction parameters
	NI = int(BrukerPar(directory, 'acqp', 'NI'))
	NR = int(BrukerPar(directory, 'acqp', 'NR'))
	ReadoutDir = BrukerPar(directory, 'method', 'PVM_SPackArrReadOrient=', 'string')
	acqMatrix = ones((3))
	PVM_Matrix = BrukerPar(directory, 'method', 'PVM_Matrix=')
	acqMatrix[acqMatrix.shape[0] - PVM_Matrix.shape[0]:] = PVM_Matrix
	zSize, ySize, xSize = int(acqMatrix[0]), int(acqMatrix[1]), int(acqMatrix[2])

	## Read in recostruction parameters
	bb = BrukerPar(directory, 'reco', 'RECO_map_slope')
	Doffset = BrukerPar(directory, 'reco', 'RECO_map_offset')
	filetype = BrukerPar(directory, 'reco', 'RECO_wordtype')[1:3]
	

	dd = cjlib.readraw( '%s/pdata/1/2dseq' % ( directory), r_[ NR*NI*zSize, ySize, xSize ], 'int%s' % (filetype) )
	d = zeros((dd.shape))
	if len(bb)>1:
		for ii in range(len(bb)):
			d[ii,:,:] = dd[ii,:,:] / bb[ii] - Doffset[ii]
	
	d = d.reshape((NR, NI, zSize, ySize, xSize))
	print d.shape
	return squeeze(d)
	
#	dd = zeros((NR,NI,zSize,ySize*xSize))
#	NormIndex = nonzero([x-1 for x in array(dd.shape[:-1])])
#	for ii,di in enumerate(dd[NormIndex]):
#		dd[NormIndex[ii]] = (cjlib.readraw( '%s/pdata/1/2dseq' % ( directory), r_[ NR, NI, zSize, ySize, xSize ], 'int%s' % (filetype) ))/bb[ii] - Doffset[ii]
#		dd = dd.reshape((NR, NI, zSize, ySize,xSize))
#	print dd.shape
#	return squeeze(dd)



def GetRoi(d, ROI=None):
	figure(1)
	clf()
	figure(1).subplots_adjust(left=0.0, right=1.0, top=1.0, bottom=0)
	cjlib.mimage( d[0] )
	
	## Get an ROI
		# If ROI given
	if not ROI == None:
		XY, mask = roipolyXYny(ROI)
	else:
		XY, mask = cjlib.roipolyXY()
		print XY

	## Get the mean value through d
	Ints = cjlib.applymask( d, mask )
	print Ints
	## Replot the ROI
	XY=array(XY)
	XY = concatenate ((XY, [XY[0,:]]), axis=0 )
	plot(XY.transpose()[0], XY.transpose()[1], 'r-*', linewidth = 4)
	savefig('ROI.png', bbox_inches='tight')
	figure(1).canvas.draw()
	#return Ints
	return Ints[0]


## Read in Varian fdf data
# Written by Craig Jones, code taken from his website

class Varian:

    def __init__(self):
        pass

    def read( self, filename ):
        if filename.endswith('.fdf'):
            data = self.readFDF( filename )
        elif filename.endswith('.img'):
            data = self.readIMG( filename )
        else:
            print "Unknown filename %s " % (filename)

        return data


    def readFDF(self, filename ):
        
        fp = open( filename, 'rb' )

        xsize = -1
        ysize = -1
        zsize = 1
        bigendian = -1
        done = False

        while not done :

            line = fp.readline()    

            if( len( line ) >= 1 and line[0] == chr(12) ):
                break

            if( len( line ) >= 1 and line[0] != chr(12) ):

                if( line.find('bigendian') > 0 ):
                    endian = line.split('=')[-1].rstrip('\n; ').strip(' ')

                if( line.find('echos') > 0 ):
                    nechoes = line.split('=')[-1].rstrip('\n; ').strip(' ')

                if( line.find('echo_no') > 0 ):
                    echo_no = line.split('=')[-1].rstrip('\n; ').strip(' ')

                if( line.find('nslices') > 0 ):
                    nslices = line.split('=')[-1].rstrip('\n; ').strip(' ')

                if( line.find('slice_no') > 0 ):
                    sl = line.split('=')[-1].rstrip('\n; ').strip(' ')

                if( line.find('matrix') > 0 ):
                    m = re.findall('(\d+)', line.rstrip())
                    
                    if len(m) == 2:
                        xsize, ysize = int(m[0]), int(m[1])
                    elif len(m) == 3:
                        xsize, ysize, zsize = int(m[0]), int(m[1]), int(m[2])

        fp.seek(-xsize*ysize*zsize*4,2)

        if bigendian == 1:
            fmt = ">%df" % (xsize*ysize*zsize)
        else:
            fmt = "<%df" % (xsize*ysize*zsize)

        data = struct.unpack(fmt, fp.read(xsize*ysize*zsize*4))
        data = array( data ).reshape( [xsize, ysize, zsize ] ).squeeze()

        fp.close()

        return data

    def readIMG(self, directory):
        
        # Get a list of all the FDF files in the directory
        try:
            files = os.listdir(directory)
        except: 
            print "Could not find the directory %s" % directory
            return

        files = [ file for file in files if file.endswith('.fdf') ]

        data = [] 
        for file in files:
            data.append( self.readFDF( directory+'/'+file ) )

#        data = transpose( array( data ), (1,2,0) ) 

        return array( data )


def BrukerPar(loc, file, parameter, parameterType=float):
	
	def findPath(file):
		if file == 'reco':
			path = '%s/pdata/1/%s'%(loc,file)
		else:
			path = '%s/%s'%(loc, file)
		return path
	
	def returnParameter(path, parameter, parameterType):
		with open (path) as f:
			for line in f:
	 			if "##$%s" %(parameter) in line:
	 				if "(" in line:
	 					# find number of elements
	 					num2find = int( re.search( '\(\s*(\d+)\s*\)', line ).group(1) )		
	 					# read in all lines until number of elements are found
	 					numbers = array([]) 
	 					
	 					while( len(numbers) < num2find ):
	 						string = next(f)
	 						if parameterType == float:
	 							numbers = r_[ numbers, [float(s) for s in string.split()] ]
	 						else:
	 							numbers = string.rstrip('\n')
	 					return numbers
	  	  			return line.split("=")[-1]
	  	  				
	path = findPath(file)
	parameterValue = returnParameter(path, parameter, parameterType)
	return parameterValue
		
def stop(): raise StopIteration()

def searchFile(file, string):
	with open(file) as f:
		map = mmap.mmap(f.fileno(), 0, prot=mmap.PROT_READ)
		for line in iter(map.readline, ""):
			if string in line:
				print line

#############################################################
######################## Hacks of cjlib #####################
#############################################################
 
#def roipolyXYny(XY):
#	blah1, blah2, ncols, nrows = gca().dataLim.bounds
#	print nrows, ncols
#	nrows, ncols = int(nrows), int(ncols)
#	mask = array([ pnpoly(j,i,XY) for i in range(nrows) for j in range(ncols)  ]).reshape(ncols,nrows)
#	return XY,mask


#############################################################################
########################## Calculate parameters #############################
#############################################################################

## Calculate excitation efficiency of rectangular rf pulse
# o1 is the offset for the rf pulse in Hz
# field is the 1H resonace frequency in MHz
# PulseWidth is the duration of a pi/2 block rf pulse
# PeakOffset is the location of the excitation efficiency in ppm from the water resonance
 
def ExcitEff(o1, field, PulseWidth, PeakOffset):
	
	# Calculate PeakOffset in Hz
	PeakOffsetHz = PeakOffset*field
	
	# offset relative to o1 
	offset = PeakOffsetHz - o1
	
	# Transverse magnetisation calcualtions
	theta = arctan((1/(4*PulseWidth))/offset)
	alpha = 2*pi*(1/(4*PulseWidth))*PulseWidth/sin(theta)

	Mx = sin(theta)*sin(alpha)
	My = ((1-cos(alpha))*sin(theta)*cos(theta))
	Mtransverse = ((Mx**2)+(My**2))**0.5
	
	return Mtransverse

## Split an arrayinto the desired number of parts
def split_list(alist, wanted_parts=1):
    length = len(alist)
    return [ alist[i*length // wanted_parts: (i+1)*length // wanted_parts] 
             for i in range(wanted_parts) ]

#############################################################################
########################### Send email via gmail ############################
#############################################################################
import smtplib
from datetime import datetime
def noticeEMail(message, toaddr='nnyadav@gmail.com', fromaddr='nypython@gmail.com', usr='nypython', psw='zaq1ZAQ!' ):
#    Taken and modified from http://www.drewconway.com/zia/?p=2707
#    	message : a string containing message body
#   	toaddr : a email address, or a list of addresses, to send the message to
#   	fromaddr : the email address the message will be from, as a string
#    	usr : the GMail username, as a string
#    	psw : the GMail password, as a string 
    
    # Initialize SMTP server
    server=smtplib.SMTP('smtp.gmail.com:587')
    server.starttls()
    server.login(usr,psw)
    
    # Send email
    senddate=datetime.strftime(datetime.now(), '%Y-%m-%d')
    subject="Your job has completed"
    m="Date: %s\r\nFrom: %s\r\nTo: %s\r\nSubject: %s\r\nX-Mailer: My-Mail\r\n\r\n" % (senddate, fromaddr, toaddr, subject)
    msg='''
    
    Message: '''+str(message)
    msg.attach(file)
    server.sendmail(fromaddr, toaddr, m+msg)
    server.quit()


#############################################################################
############################## FLEX Processing ##############################
#############################################################################

## Define FLEX Equation
def fitfunc(p, x): 
	
	tfid = zeros( size(x) ) + p[0]
	
	for pooli in range( (len(p)-1)/4 ):
		firstVal = pooli * 4 + 1
		tfid = tfid + ((p[firstVal]*cos(2*pi*p[firstVal+1]*x+p[firstVal+2]))*exp(-p[firstVal+3]*x))
	
	return tfid

def fitfuncAbs(p, x): 
	
	tfid = zeros( size(x) ) + p[0]
	
	for pooli in range( (len(p)-1)/5 ):
		firstVal = pooli * 5 + 1
		tfid = tfid + ((p[firstVal]*cos(2*pi*p[firstVal+1]*x+p[firstVal+2]))*exp(-p[firstVal+3]*fabs(x))*exp(-(p[firstVal+4]*fabs(x))**2))
		
	return tfid

## Define FLEX Equation in log form
def fitfuncLog(p, x, field): 
	
	tfid = zeros( size(x) ) + p[0]
	
	for pooli in range( (len(p)-1)/4 ):
		firstVal = pooli * 4 + 1
		tfid = tfid + ((p[firstVal]*cos(2*pi*(p[firstVal+1]/field)*x+p[firstVal+2]))*(-p[firstVal+3]*x))
	
	return tfid

def PrintResults(p):
	print 'DC correction:', round(p[0], 3)
	for pooli in range((len(p)-1)/4):
		firstVal = pooli * 4 + 1
		print 'Frequency: {freq:3.0f} Hz \t Amplitude: {amp:5.2f} % \t Phase: {phase:3.2f} \t k+R2: {decay:4.0f} s-1'.format(freq = p[firstVal+1], amp = p[firstVal]*100, phase = p[firstVal+2], decay = p[firstVal+3])
		
def PrintResults5(p):
	print 'DC correction:', round(p[0], 3)
	for pooli in range((len(p)-1)/5):
		firstVal = pooli * 5 + 1
		print 'Frequency: {freq:3.0f} Hz \t Amplitude: {amp:5.2f} % \t Phase: {phase:3.2f} \t k+R2: {decay1:4.0f} s-1 \t k+R2: {decay2:4.0f} s-1'.format(freq = p[firstVal+1], amp = p[firstVal]*100, phase = p[firstVal+2], decay1 = p[firstVal+3], decay2 = p[firstVal+4])

def FlexFuncMin(tevol, mm, p0, bounds, weighting = 500 ):

	def errfunc(p, x, y):
		p = p*scalings
		w = tevol[-1]*weighting - tevol*weighting + 0.1
		dd = sum( w * (fitfunc(p, x) - y)**2 ) # Distance to the target function
		return dd

	scalings = abs(p0)
	
	# re-scale the bounds
	bounds_rescaled = []
	for bndi, bnd in enumerate( bounds ):
		bounds_rescaled.append( (bnd[0]/scalings[bndi], bnd[1]/scalings[bndi] ) )
	p1 = optimize.fmin_l_bfgs_b(errfunc, p0/scalings, args=(tevol, mm), bounds=bounds_rescaled, approx_grad=True, factr = 1e-4)[0]
	return p1*scalings

def FlexLSFit(tevol, mm, p0):
	# Ensure type(p0) == list else do p0.tolist()
	errfunc = lambda p, x, y: fitfunc(p, x) - y # Distance to the target function
	p1, success = optimize.leastsq(errfunc, p0[:], args=(tevol, mm))
	return p1
	
## Define FLEX Equation
def fitfuncComplex(p, x): 
	
	tfid1 = zeros( size(x) ) + p[0]
	tfid2 = zeros( size(x) ) + p[0]
	
	for pooli in range( (len(p)-1)/5 ):
		firstVal = pooli * 5 + 1
		tfid1 = tfid1 + ((p[firstVal]*cos(2*pi*p[firstVal+1]*x+p[firstVal+2]))*exp(-p[firstVal+3]*fabs(x))*exp(-(p[firstVal+4]*(x))**2))
		tfid2 = tfid2 + ((p[firstVal]*cos(2*pi*p[firstVal+1]*x+(p[firstVal+2]-pi/2)))*exp(-p[firstVal+3]*fabs(x))*exp(-(p[firstVal+4]*(x))**2))
	
	return tfid1, tfid2

def FlexLSFitComplex(tevol, mm1, mm2, p0):
	# Ensure type(p0) == list else do p0.tolist()
	errfunc = lambda p, x, y1, y2: (fitfuncComplex(p, x)[0] - y1)*(fitfuncComplex(p, x)[1] - y2)
	p1, success = optimize.leastsq(errfunc, p0[:], args=(tevol, mm1, mm2))
	return p1

def FlexFuncMinComplex(tevol, mm1, mm2, p0, bounds, weighting = 500 ):

	def errfunc(p, x, y1, y2):
		p = p*scalings
#		w = tevol*weighting + 0.1
		w = tevol[-1]*weighting - tevol*weighting + 0.1
		dd = sum( w * (((fitfuncComplex(p, x)[0] - y1) + (fitfuncComplex(p, x)[1] - y2))**2)) # Distance to the target function
		return dd

	scalings = abs(p0)
	
	# re-scale the bounds
	bounds_rescaled = []
	for bndi, bnd in enumerate( bounds ):
		bounds_rescaled.append( (bnd[0]/scalings[bndi], bnd[1]/scalings[bndi] ) )
	p1 = optimize.fmin_l_bfgs_b(errfunc, p0/scalings, args=(tevol, mm1, mm2), bounds=bounds_rescaled, approx_grad=True, factr = 1e-4)[0]
	return p1*scalings

# Function for plotting spectrum of FID
def getSpectrum(fid, tevol, o1, phase0, phase1, fill, apodize):
	area_hilbert = scipy.fftpack.hilbert(fid*exp(1j*phase0)*exp(1j*phase1*o1))
#	area_pre = area_hilbert*cos(linspace(0,pi/2,len(tevol)))
	area_pre = area_hilbert*exp(-apodize*pi*tevol)
	fid2 = concatenate( ( area_pre, zeros(fill-len(fid)) ), axis=0 )
	FLEXSpect = real(fft.fft(fid2))
	return fid2, FLEXSpect

def getSpectrumCombineNew(fid1, fid2, tevol, o1, phase0, phase1, fill, apodize):
	u = fid1*exp(1j*phase0)*exp(1j*phase1*o1)
	v = fid2*exp(1j*phase0)*exp(1j*phase1*o1)
	fidComplex = u+1j*v
#	if cos == 'True':
	area_pre = fidComplex*exp(-apodize*pi*tevol)
#	else:
#	area_pre = fidComplex*cos(linspace(0,pi/2,len(tevol)))
	fid2 = concatenate( ( area_pre, zeros(fill-len(fid1)) ), axis=0 )
	FLEXSpect = fft.fftshift(fft.fft(fid2))#*exp(1j*phase1)
	return fid2, FLEXSpect

def getSpectrumCombine(fid1, fid2, tevol, phase0, phase1, fill, apodize):
	u = cos(phase0)*fid1+sin(phase0)*fid2
	v = -sin(phase0)*fid1+cos(phase0)*fid2
	fidComplex = u+1j*v
#	if cos == 'True':
	area_pre = fidComplex*exp(-apodize*pi*tevol)
#	else:
#	area_pre = fidComplex*cos(linspace(0,pi/2,len(tevol)))
	fid2 = concatenate( ( area_pre, zeros(fill-len(fid1)) ), axis=0 )
	FLEXSpect = fft.fftshift(fft.fft(fid2))#*exp(1j*phase1)
	return fid2, FLEXSpect

def FlexBaselineFit(FreqPpm, FLEXSpect, FitInds, PolyOrder = 7):
	FitCoeff = scipy.polyfit(FreqPpm[FitInds], FLEXSpect[FitInds], PolyOrder)
	FitttedPoly = scipy.polyval(list(FitCoeff), FreqPpm)
	return FitttedPoly

# Calculate the ppm scale
def PpmScale(FLEXSpect, o1, field, dwt, dimension='1D'):
	n = len(FLEXSpect)
	sw=1/dwt
	if dimension == '1D':
		if o1<=0:
			ppm=arange(o1,sw+o1,sw/n)/field
		else:
			ppm=arange(o1,-sw+o1,-sw/n)/field
	else:
		ppm = (arange(sw/2, -sw/2, -sw/n)+o1)/field
	return ppm