% This is a demonstration of a real randomly undersampled 3DFT acquisition
% It includes normalization, phase correction and separable reconstruction of each slice
% 
% In each step, the previous slice is used as an initial image, since they are very close 
% in content.
%
% The full recon takes ~ 430 seconds on an AMD64 3700+ processor with 2.5Gb of RAM
%
% The data was obtained with collaboration with Tolga Cukur.
% The original data was 192x128x128. Here only 64 slices in the middle of the calf
% are given because of the large file size
%
% The data was undersampled in the acquisition by a factor of 2
% the zero filling with density compensation, not surprizingly, result in a good MIP, but has
% significantly more "noise" the slices than the CS recon.
%
% (c) Michael Lustig 2007

addpath(strcat(pwd,'/utils'));

load calf_data_cs.mat


% take ifft in the fully sampled dimension
data = fftshift(ifft(fftshift(data,1),[],1),1);
data = permute(data,[2,3,1]);

im_zfwdc = zeros(size(data));
for n=1:size(data,3)
	im_zfwdc(:,:,n) = ifft2c(data(:,:,n)./pdf);
end

% scale data such that the maximum image pixel in zf-w/dc is around 1
% this way, we can use similar lambda for different problems
data = data/max(abs(im_zfwdc(:)));
im_zfwdc = im_zfwdc/max(abs(im_zfwdc(:)));


%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L1 Recon Parameters 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

N = [size(data,1),size(data,2)]; 		% image Size
DN = N;		 	% data Size
TVWeight = [0.01] ; 	% Weight for TV penalty - only TV is on, but I encourage you to try wavelets as well.
xfmWeight = [0.0];	% Weight for Transform L1 penalty
Itnlim = 45;		% Number of iterations
OuterIt = length(TVWeight);

%generate transform operator

%XFM = Wavelet('Daubechies',4,4);	% Wavelet
%XFM = TIDCT(8,4);			% DCT
XFM = 1;				% Identity transform 	

% initialize Parameters for reconstruction
phmask = zpad(hamming(6)*hamming(6)',N(1),N(2)); %mask to grab center frequency
phmask = phmask/max(phmask(:));			 %for low-order phase estimation and correction
res = zeros(N);
RES = zeros(size(data));

param = init;
param.XFM = XFM;
param.TV = TVOP;
param.Itnlim = Itnlim;

tic
for slice = 1:size(data,3)

	param.data = data(:,:,slice);
	ph = exp(i*angle((ifft2c(data(:,:,slice).*phmask)))); % estimate phase for phase correction
	param.FT = p2DFT(mask, DN, ph, 2); 

	for n=1:OuterIt
		param.TVWeight =TVWeight(n);     % TV penalty 
		param.xfmWeight = xfmWeight(n);  % L1 wavelet penalty
		res = fnlCg(res, param);
		figure(100), imshow(cat(2,abs(im_zfwdc(:,:,slice)),abs(XFM'*res)),[]), drawnow;
	end
	RES(:,:,slice) = XFM'*res;
	if mod(slice,10)==0
		figure(101), imshow(cat(2,max(abs(permute(im_zfwdc,[3,1,2])),[],3),max(abs(permute(RES,[3,1,2])),[],3)),[]), drawnow;
	end
end

toc

disp('Done!, now computing MIPs');

MIP_dc = zeros(size(data,3),N(1),36);
MIP_cs = zeros(size(data,3),N(1),36);

for n=1:36
	MIP_dc(:,:,n) = max(abs(permute(imrotate(im_zfwdc,10*(n-1),'bilinear','crop'),[3,1,2])),[],3);
	MIP_cs(:,:,n) = max(abs(permute(imrotate(RES,10*(n-1),'bilinear','crop'),[3,1,2])),[],3);
	figure(101), imshow(cat(2,MIP_dc(:,:,n),MIP_cs(:,:,n)),[],'InitialMagnification',150), title('zf-w/dc                           CS'), drawnow;
end



