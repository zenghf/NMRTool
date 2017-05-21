function [k,g,s,time] = vdSpiralDesign(Nitlv, res, fov,radius , Gmax, Smax, T, interpType)

% [k,g,s,time] = vdSpiralDesign(Nitlv, res, fov,radius , Gmax, Smax, T, interpType)
%
% This function designs a variable density spiral
%
%	Input:
%		Nitlv	-	Number of interleves
%		res	-	resolution (in mm)
%		fov	- 	vector of fov (in cm)
%		radius	-	vector of radius corresponding to the fov
%		Gmax	-	max gradient (default 4 G/CM)
%		Smax	-	max slew (default 15)
%		T	-	sampling rate (in ms)
%		interpType- 	type of interpolation used to interpolate the fov
%				accept: linear, cubic, spline
%	
%	Output:
%		k	-	the k-space trajectory
%		g	-	the gradient waveform
%		s	-	the slew rate
%		time	-	total time
%
%	example:
%		design a dual density spiral
%		
%	[k,g,s,time] = vdSpiralDesign(16, 1,[35,35,10,10],[0,0.1,0.13,1],4,15,4e-3,'cubic');
%	figure, plot(k(:)*exp(i*2*pi*[1:16]/16));
%
%
%
%	(c) Michael Lustig 2007


kmax = 5/res;

if length(radius)<2
	error(' radius must be at least length=2');
end

dr = 1/1500/max(fov/Nitlv);
r = 0:dr:kmax;   kmax = max(r);

fov =  interp1(radius*kmax,fov,r,interpType);
dtheta = 2*pi*dr.*fov/Nitlv;
theta = cumsum(dtheta);

C = r.*exp(i*theta);
[C,time,g,s,k] = minTimeGradient(C, [], [], Gmax, Smax,T,0);


k = C;



