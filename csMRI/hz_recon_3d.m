%% preparation and configuration
%WavePath;
addpath(strcat(pwd,'/utils'));
%datapath = '/Users/hzeng/Data/hzeng.rN1/'; expno = 3; conf = 'cs_64_64_0.5_8.txt'; load 64_64_0.5.mat;
%datapath = '/Users/hzeng/Data/hzeng.r52/'; expno = 46; conf = 'cs_64_64_0.5_8.txt'; load 64_64_0.5.mat;
%datapath = '/Users/hzeng/Data/hzeng.r52/'; expno = 47; conf = 'cs_64_64_0.25_8.txt'; load 64_64_0.25.mat;
%datapath = '/Users/hzeng/Data/hzeng.r52/'; expno = 48; conf = 'cs_64_64_0.125_8.txt'; load 64_64_0.125.mat;
datapath = '/Users/hzeng/Data/hzeng.sJ1/'; conf = 'cs_64_32_0.25_8.txt';  expno = 21; ref_expno = 19; load 64_32_0.25.mat;
display_image = 1;

%% read data
fid = readRareFID3D(datapath, expno, 'NoSorting');
n4d = 1;
if (length(size(fid)) == 4)
    n4d = size(fid, 4);
end

%% read compressed sensing configuration file
f = fopen(conf, 'r');
% read comments
c = '#';
while (c == '#')
    line = fgets(f);
    c = line(1);
end
% read comments
CS_EncMatrix = sscanf(line, '%*s %d %d');
CS_ACT_EncMatrix =sscanf(fgets(f), '%*s %d %d');
PVM_RareFactor = sscanf(fgets(f), '%*s %d');
fclose(f);
% read encoding steps
encInd = importdata(conf, ' ', 4);
encInd = encInd.data;

%% prepare fit data
nx = size(fid, 1);
ny = CS_ACT_EncMatrix(1);
nz = CS_ACT_EncMatrix(2);
data3d = zeros(nx, ny, nz, n4d);
data3dft1 = zeros(nx, ny, nz, n4d);
img3d = zeros(nx, ny, nz, n4d);
for k4d = 1:n4d
    for k = 1:size(encInd, 1)
        k1 = mod(k - 1, CS_EncMatrix(1)) + 1;
        k2 = floor((k - 1) / CS_EncMatrix(1)) + 1;
        kk1 = encInd(k, 1) + CS_ACT_EncMatrix(1) / 2 + 1;
        kk2 = encInd(k, 2) + CS_ACT_EncMatrix(2) / 2 + 1;
        data3d(:, kk1, kk2, k4d) = fid(:, k1, k2, k4d);
        data3dft1(:, kk1, kk2, k4d) = fftshift(fft(fid(:, k1, k2, k4d)));
    end
end


%mask = mask';
%return;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% L1 Recon Parameters 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pctg = sum(mask) / numel(mask);  	% undersampling factor
P = 5;              % Variable density polymonial degree

N = [ny, nz]; 	% image Size
DN = [ny, nz]; 	% data Size
TVWeight = 0.002*1; 	% Weight for TV penalty
xfmWeight = 0.005*1;	% Weight for Transform L1 penalty
Itnlim = 8;         % Number of iterations

%generate Fourier sampling operator
FT = p2DFT(mask, N, 1, 2);
FTall = p2DFT(true(size(mask)), N, 1, 2);

%generate transform operator
%XFM = Wavelet('Daubechies',4,4);	% Wavelet
XFM = 1;

% initialize Parameters for reconstruction
param = init;
param.FT = FT;
param.XFM = XFM;
param.TV = TVOP;
%param.data = data;
param.TVWeight =TVWeight;     % TV penalty 
param.xfmWeight = xfmWeight;  % L1 wavelet penalty
param.Itnlim = Itnlim;

for k4d = 1:n4d
    for k = 1:nx
        disp(int2str(k));
        data = squeeze(data3dft1(k,:,:,k4d));

        img  = FTall'*data;
        if display_image
            figure(1);
            imagesc(abs(img));
            title(['img', int2str(k), ': Direct FT']);
        end

        im_dc = FT'*(data.*mask./pdf); % img with undersampling data acquisition
        scale = max(abs(im_dc(:)));
        data = data/scale;
        im_dc = im_dc/scale;
        img = img/scale;

        param.data = data;

        if display_image
            figure(100), imshow(abs(im_dc),[]);drawnow;
        end
        res = XFM*im_dc;

        % do iterations
        tic
        for n=1:1
            res = fnlCg(res,param);
            im_res = XFM'*res;
            if display_image
                figure(100), imshow(abs(im_res),[]), colormap('default'), drawnow
            end
        end
        toc
        img3d(k,:,:,k4d) = im_res * scale;
    end
end
for k4d = 1:min(n4d,6)
    view3dgui(abs(img3d(:,:,:,k4d)));
end
