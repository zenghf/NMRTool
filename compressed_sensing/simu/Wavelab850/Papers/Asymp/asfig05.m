% asfig05 -- Asymptopia Figure 05: AutoSpline Reconstructions from Noisy Data
%
% Here we depict a reconstruction using penalized thin-plate splines,
% in which periodic thin-plate splines are used to smooth the data,
% with roughness penalty proportional to the energy stored in the
% second derivative.  The constant of proportionality (or Lagrangian
% tuning parameter) is the empirical minimizer of the Stein Unbiased
% Estimate of Risk.
%
global yblocks ybumps yheavi yDoppler
global t
% 
%clf;
SplineInit
%
	[xhat, c] = SplineUChoose(yblocks,2.);
	versaplot(221,t,xhat,[],' 5 (a) AutoSpline[Blocks]',[],[])
%
	[xhat,c] = SplineUChoose(ybumps,2.);
	versaplot(222,t,xhat,[],' 5 (b) AutoSpline[Bumps]',[],[])
%
	[xhat,c] = SplineUChoose(yheavi,2.);
	versaplot(223,t,xhat,[],' 5 (c) AutoSpline[HeaviSine]',[],[])
%
	[xhat,c] = SplineUChoose(yDoppler,2.);
	versaplot(224,t,xhat,[],' 5 (d) AutoSpline[Doppler]',[],[])
%  
% Prepared for the paper Wavelet Shrinkage: Asymptopia?
% Copyright (c) 1994 David L. Donoho and Iain M. Johnstone
%  
    
    
    
 
 
%
%  Part of Wavelab Version 850
%  Built Tue Jan  3 13:20:41 EST 2006
%  This is Copyrighted Material
%  For Copying permissions see COPYING.m
%  Comments? e-mail wavelab@stat.stanford.edu 
