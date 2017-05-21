% this simulation simulates an angio image and reconstructs for different 
% undersampling patterns in the same way as in the accompanying paper
%
% (c) Michael Lustig 2007


clear all
addpath(pwd);
addpath(strcat(pwd,'/utils'));

disp('this simulation simulates an angio image and reconstructs for different ')
disp('undersampling patterns in the same way as in the accompanying paper.')

cd simulation


automate = 1;

% parameters for the simulation
Nmag = 3;		% number of different "vessel" magnitudes
sizes = [ 1,3,6]	% sizes of vessels in pixel radius
N = [100,100]; 		% size of image
DN = [100,100]; 	% size of k-space 


%parameters for recon
TVWeight = 0.05;
xfmWeight = 0.05;
Itnlim = 30;
maxN = 30;	% repeat each recon many times while reducing lambda
Show = 1;


% the undersampling factor and the sampling polynomial variable density parameters
PCTG = [1/8 , 1/12 , 1/20, 1/8 , 1/12, 1/20];
PVEC = [20, 20, 20 , 100, 100, 100];

for iteration=1:length(PCTG)
	pctg = PCTG(iteration);
	P = PVEC(iteration);

	angioSim;
	IM_LR(:,:,iteration) = im_lr;
	IM_DC(:,:,iteration) = im_dc;
	IM_L1(:,:,iteration) = res;

end

cd ..




