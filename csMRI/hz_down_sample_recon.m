%WavePath;
addpath(strcat(pwd,'/utils'));
%datapath='/Users/hzeng/Projects/CSMRI/data/CSTEST.qw1/';
datapath = '/Users/hzeng/Data/hzeng.r52/';
expno = 32;
ref_expno = 31;
data0 = readRareFidScale(datapath, expno, 0.0);
fid = data0(:,:,1,1);
fid = permute(fid, [2 1]);
ny = size(fid,1);
nx = size(fid,2);

ref_data = readRareFID(datapath, ref_expno);
ref_fid = ref_data(:,:,1,1);
ref_fid = permute(ref_fid, [2 1]);
ref_fid = flipud(ref_fid);

%%%% filling zeros to the fid %%%%
load 128_0.5.mat
data = zeros(length(pdf1d), size(fid,2));
data(mask1d,:) = fid;
data = flipud(data);
%return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L1 Recon Parameters 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pctg = [0.5];  	% undersampling factor
P = 5;              % Variable density polymonial degree

N = size(data); 	% image Size
DN = size(data); 	% data Size
TVWeight = 0.002*1; 	% Weight for TV penalty
xfmWeight = 0.005*1;	% Weight for Transform L1 penalty
Itnlim = 8;         % Number of iterations

pdf = repmat(pdf1d(:),[1,nx]);
mask = repmat(mask1d(:),[1,nx]);

%generate Fourier sampling operator
FT = p2DFT(mask, N, 1, 2);
FTall = p2DFT(true(size(mask)), N, 1, 2);
img  = FTall'*data;
%figure
%imagesc(abs(img))

% scale data
im_dc = FT'*(data.*mask./pdf);
scale = max(abs(im_dc(:)));
data = data/scale;
im_dc = im_dc/scale;
img = img/scale;

%generate transform operator
%XFM = Wavelet('Daubechies',4,4);	% Wavelet
XFM = Wavelet('Daubechies',4,2);	% Wavelet
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

figure(1);
imshow(abs(res));

%return;
% do iterations
tic
for n=1:5
	res = fnlCg(res,param);
	im_res = XFM'*res;
	figure(100), imshow(abs(im_res),[]), colormap('default'), drawnow
end
toc


ref_img = FTall' * ref_fid;
%ref_img = ref_img / max(abs(ref_img(:)));
ref_img = ref_img / scale;


%fun = @(p) sum(abs((ref_img(:) * complex(cos(p), sin(p)) - im_res(:))) .^ 2);
%p = fminsearch(fun, 0);
%ref_img = ref_img * complex(cos(p), sin(p));


%figure(2), imagesc(abs([im_dc,ref_img,im_res, (ref_img-im_res)]));
figure(2), imagesc(abs([im_dc,ref_img,im_res]));
colormap('default');
title('  zf-w/dc, full sample, l_1 Wavelet, difference*5', 'fontsize',20);
im_res = im_res / max(abs(im_res(:)));


figure(3);
imshow(abs(im_res));