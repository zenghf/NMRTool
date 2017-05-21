% This script demonstare a CS reconstruction from 
% Randomly undersmapled phase encodes of a 2D GRE 
% Phantom scan, were the phantom data is noisy


addpath(strcat(pwd,'/utils'));


load noisy_phantom


%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L1 Recon Parameters 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

N = size(data); 	% image Size
DN = size(data); 	% data Size
TVWeight = 0.05; 	% Weight for TV penalty
xfmWeight = 0.0;	% Weight for Transform L1 penalty
Itnlim = 15;		% Number of iterations



phmask = zpad(hamming(6)*hamming(6)',N(1),N(2)); %mask to grab center frequency
phmask = phmask/max(phmask(:));			 %for low-order phase estimation and correction
ph = exp(i*angle((ifft2c(data.*phmask)))); % estimate phase for phase correction

%generate Fourier sampling operator
FT = p2DFT(mask, N, ph, 2);

% scale data
im_dc = FT'*(data.*mask./pdf);
data = data/max(abs(im_dc(:)));
im_dc = im_dc/max(abs(im_dc(:)));

%generate transform operator
%XFM = Wavelet('Daubechies',6,4);	% Wavelet
XFM = 1;

% initialize Parameters for reconstruction

param = init;
param.FT = FT;
param.XFM = XFM;
param.TV = TVOP;
param.data = data;
param.TVWeight =TVWeight;     % TV penalty 
param.xfmWeight = xfmWeight;  % L1 wavelet penalty
param.Itnlim = Itnlim;

figure(100), imshow(abs(im_dc),[]);drawnow;

res = XFM*im_dc;

% do iterations
tic
for n=1:5
	res = fnlCg(res,param);
	im_res = XFM'*res;
	figure(100), imshow(abs(im_res),[]), drawnow
end
toc


figure, imshow(abs(cat(2,im_dc,im_res)),[]);
title(' zf-w/dc              CS');



