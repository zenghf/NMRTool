% dir_name = '../ASL/';
% filename=strcat(dir_name,'M0_3D4mm_complex_SENSE_14_1.REC'); 
% [v, header] = Philips_readrec(filename);
addpath(strcat(pwd,'/utils'));
load M0_3D4mm_complex_SENSE_14_1
image_m=squeeze(v);
img0 = complex(image_m(:,:,:,2), image_m(:,:,:,3));
img0 = permute(img0, [2 3 1]);
scale = max(abs(img0(:)));
img0 = img0 / scale;
img_recon = zeros(size(img0));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L1 Recon Parameters 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

D1 = size(img0,1);
D2 = size(img0,2);
D3 = size(img0,3);
I1 = 2^nextpow2(max(D1,D2));
I2 = 2^nextpow2(max(D1,D2));


pctg = [0.5];  	% undersampling factor
P = 5;              % Variable density polymonial degree

N = [I1, I2]; 	% image Size
NFid = size(img0, 3); % number of data point in the direct dimension;
DN = [D1, D2]; 	% data Size
TVWeight = 0.002*0; 	% Weight for TV penalty
xfmWeight = 0.005*0.1;	% Weight for Transform L1 penalty
Itnlim = 4;         % Number of iterations

load mask_80_30_0.5.mat
% load mask, pdf, pctg;


%generate Fourier sampling operator
%mask = mask | true;
FT = p2DFT(mask, N, 1, 2); % p2DFT(mask,imSize [ ,phase,mode])
FTall = p2DFT(true(size(mask)), N, 1, 2);
%generate transform operator
% XFM = Wavelet('Daubechies',4,4);	% Wavelet
XFM = Wavelet('Haar',4,4);	% Wavelet
% initialize Parameters for reconstruction
param = init;
param.FT = FT;
param.XFM = XFM;
param.TV = TVOP;
%param.data = data;
param.TVWeight =TVWeight;     % TV penalty 
param.xfmWeight = xfmWeight;  % L1 wavelet penalty
param.Itnlim = Itnlim;

tic
figure(100);
figure(99);
for k = 40
%for k = 1:NFid
    img = img0(:,:,k);
    img = zpad(img, I1, I2);
    data = FTall * img;
    im_dc = FT'*(data.*mask./pdf);
    param.data = data;

    res = XFM*im_dc;

    disp(['------------',int2str(k),'---------------']);
    % do iterations

    for n=1:20
        param.xfmWeight = 0.01 * (1^n);
        res = fnlCg(res,param);
        figure(99),imshow(mat2gray(abs(res))), colormap('default'), drawnow;
        im_res = XFM'*res;
        figure(100), imshow(mat2gray(abs([crop(img, D1, D2), crop(im_res, D1, D2), crop(img, D1, D2) - crop(im_res, D1, D2)]))), 
            colormap('default'), title(int2str(k)), drawnow
    end
    img_recon(:,:,k) = crop(im_res, D1, D2);
    toc
end


