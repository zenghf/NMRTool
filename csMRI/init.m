function res = init()
% param = init()
%
% function returns a structure with the entries that are needed for the reconstruction.
% The user MUST supply operators afterwords!
%
% See:
%	demo_SheppLoganTV demo_angio_simulation demo_Brain_2D.m
% 
% (c) Michael Lustig 2007

res.FT = []; % The measurement operator (undersmapled Fourier for example)
res.XFM = []; % Sparse transform operator
res.TV = []; 	% the Total variation operator
res.data = []; % measurements to reconstruct from

res.TVWeight = 0.01;	% TV penalty
res.xfmWeight = 0.01;   % transform l1 penalty

res.Itnlim = 20;	% default number of iterations
res.gradToll = 1e-30	% step size tollerance stopping criterea (not used)

res.l1Smooth = 1e-15;	% smoothing parameter of L1 norm
res.pNorm = 1;  % type of norm to use (i.e. L1 L2 etc)

% line search parameters
res.lineSearchItnlim = 150;
res.lineSearchAlpha = 0.01;
res.lineSearchBeta = 0.6;
res.lineSearchT0 = 1 ; % step size to start with




