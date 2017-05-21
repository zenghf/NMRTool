%% preparation and configuration
%WavePath;
addpath(strcat(pwd,'/utils'));
%datapath = '/Users/hzeng/Data/hzeng.r52/'; expno = 45;
%datapath = '/Users/hzeng/Data/haifeng.so1/';expno = 2;procno = 1;
datapath = '/Users/hzeng/Data/hzeng.rN1/';expno = 10;procno = 1;

method_file = fullfile(datapath, num2str(expno), 'method');
method = readnmrpar(method_file);

data0 = readRareFID3D(datapath, expno);
fid = data0;
dim = length(size(fid));
img = zeros(size(fid));
n4 = 1;
if dim == 4
    n4 = size(fid, 4);
end
for k4 = 1:n4
    data = fid(:,:,:,k4);
    for k = 1:3
        for k1 = 1:size(data,2)
            for k2 = 1:size(data,3)
                data(:,k1,k2) = fftshift(fft(data(:,k1,k2)));
                %data(:,k1,k2) = fft(data(:,k1,k2));
            end
        end
        data = permute(data, [2 3 1]);
    end
    img(:,:,:,k4) = data;
end

%% for view 

imga = zeros(size(img(:,:,:,1)));
for k = 1:size(img,4)
    imga = imga + img(:,:,:,k) * method.PVM_EncChanScaling(k);
end

%imga = sum(img,4);

imgabs = abs(img(:,:,:,1));
view3dgui(imgabs);