addpath(strcat(pwd,'/utils'));
%%{
%datapath='/Users/hzeng/Projects/CSMRI/data/CSTEST.qw1/';
datapath='/Users/hzeng/Data/CSTEST.qw1/';
expno=13;
data0 = readRareFID(datapath, expno);
%}
fid = data0(:,:,1,1);
fid = permute(fid, [2 1]);
ny = size(fid,1);
nx = size(fid,2);
data = fid;
data = flipud(data);
%return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L1 Recon Parameters 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pctg = [0.375];  	% undersampling factor
P = 5;              % Variable density polymonial degree

N = size(data); 	% image Size
DN = size(data); 	% data Size
TVWeight = 0.002*1; 	% Weight for TV penalty
xfmWeight = 0.005*1;	% Weight for Transform L1 penalty
Itnlim = 8;         % Number of iterations

pdf1d = ones(ny,1);
mask1d = true(ny,1);

%mask1d = genSampling(pdf1d,100,2);		% generates a sampling pattern
pdf = repmat(pdf1d(:),[1,nx]);
mask = repmat(mask1d(:),[1,nx]);

%generate Fourier sampling operator
%mask = mask | true;
FT = p2DFT(mask, N, 1, 2);
FTall = p2DFT(true(size(mask)), N, 1, 2);
img  = FTall'*data;

% scale data
%%{
im_dc = FT'*(data.*mask./pdf);
scale = max(abs(im_dc(:)));
data = data/scale;
im_dc = im_dc/scale;
img = img/scale;
%}

% figure
% imagesc(abs(img))
% colormap(gray);

figure, imshow(abs(img));
colormap('default');
colormap(gray);
img2 = img;