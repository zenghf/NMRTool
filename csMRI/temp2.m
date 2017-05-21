%% preparation and configuration
addpath(strcat(pwd,'/utils'));
%datapath = '/Users/hzeng/Data/hzeng.r52/'; conf = 'cs_32_16_1.0_8.txt';  expno = 37; ref_expno = 38;
%datapath = '/Users/hzeng/Data/hzeng.r52/'; conf = 'cs_64_64_0.5_8.txt';  expno = 34; ref_expno = 38;
%datapath = '/Users/hzeng/Data/hzeng.rA1/'; conf = 'cs_64_64_0.5_8.txt';  expno = 4; ref_expno = 3;
%datapath = '/Users/hzeng/Data/hzeng.rN1/'; conf = 'cs_64_64_0.5_8.txt';  expno = 3; ref_expno = 2;
%datapath = '/Users/hzeng/Data/hzeng.sF1/'; conf = 'cs_64_32_1.0_8b.txt';  expno = 4; ref_expno = 2;
%datapath = '/Users/hzeng/Data/hzeng.sF1/'; conf = 'cs_64_32_0.5_8.txt';  expno = 5; ref_expno = 2;
%datapath = '/Users/hzeng/Data/hzeng.sF1/'; conf = 'cs_64_32_0.25_8.txt';  expno = 11; ref_expno = 2;
%datapath = '/Users/hzeng/Data/hzeng.rN1/'; conf = 'cs_64_32_1.0_8b.txt';  expno = 8; ref_expno = 7;
%datapath = '/Users/hzeng/Data/hzeng.rN1/'; conf = 'cs_64_32_1.0_8_cenc.txt';  expno = 9; ref_expno = 7;
datapath = '/Users/hzeng/Data/hzeng.rN1/'; conf = 'cs_32_16_1.0_8_cenc.txt';  expno = 11; ref_expno = 10;

%% read data
fid = readRareFID3D(datapath, expno, 'NoSorting');


%% read compressed sensing configuration file
f = fopen(conf, 'r');
fgets(f);
line = fgets(f); 
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
data3d = zeros(nx, ny, nz);
data3dft1 = zeros(nx, ny, nz);
img3d = zeros(nx, ny, nz);
for k = 1:size(encInd, 1)
    k1 = mod(k - 1, CS_EncMatrix(1)) + 1;
    k2 = floor((k - 1) / CS_EncMatrix(1)) + 1;
    kk1 = encInd(k, 1) + CS_ACT_EncMatrix(1) / 2 + 1;
    kk2 = encInd(k, 2) + CS_ACT_EncMatrix(2) / 2 + 1;
    data3d(:, kk1, kk2) = fid(:, k1, k2);
    data3dft1(:, kk1, kk2) = fftshift(fft(fid(:, k1, k2)));
end

N = [ny, nz]; 	% image Size
DN = [ny, nz]; 	% data Size

%generate Fourier sampling operator
FTall = p2DFT(true(N), N, 1, 2);


for k = 1:nx
    %disp(int2str(k));
    data = squeeze(data3dft1(k,:,:));
    img  = FTall'*data;
 
    img3d(k,:,:) = img ;
end
view3dgui(abs(img3d));

