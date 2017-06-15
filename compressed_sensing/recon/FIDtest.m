% clear all
% close all
% clc

%% read data
%path='/Users/jiadixu/MRIdata/pw072712da38.fa1';
path='~/Data/SIF.ql1/';
Nscan=1;
rotate=0;
[images k Nch NE NS NR]= bruker_RARE_FID(path,Nscan,rotate);

%display figure
for i=3
figure
img=images(:,:,:,i);

imagesc(img)
%imagesc(raw_real(:,:,1))

colormap (gray(256))
%axis image
axis off
end