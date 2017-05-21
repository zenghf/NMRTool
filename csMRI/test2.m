addpath(strcat(pwd,'/utils'));
%datapath='/Users/hzeng/Projects/CSMRI/data/CSTEST.qw1/';
datapath='/Users/hzeng/Data/hzeng.r52/';
expno=19;
data0 = readRareFidScale(datapath, expno, 0);
%data0 = readRareFID(datapath, expno);
fid = data0(:,:,1,1);
fid = permute(fid, [2 1]);
ny = size(fid,1);
nx = size(fid,2);
data = zeros(128,128);
load 128_0.5.mat;
data(mask1d,:) = fid;
data = flipud(data);


img = fftshift(fftshift(fft(fft(data,[],1),[],2),1),2);
figure
imagesc(abs(img));