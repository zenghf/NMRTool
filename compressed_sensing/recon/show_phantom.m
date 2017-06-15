path='/Users/hzeng/Data/SIF.ql1/';
expno=1;
data = readRareFID(path, expno);
fid = data(:,:,1,1);
fid = permute(fid, [2 1]);
ny = size(fid,1);
nx = size(fid,2);

mask_vardens = true(ny,nx);
pdf_vardens = ones(ny, nx);
DATA = fid .* mask_vardens;
img = fftshift(fft2(DATA ./ pdf_vardens));
m = max(abs(img(:)));
fid2img = @(x) fftshift(fft2(x));
img2fid = @(x) ifft2(ifftshift(x));

W = Wavelet;

figure;
for iter = 1:5
    img = W'*(SoftThresh(W*img, 0.025*m));
    pause;
    img = fid2img(img2fid(img) * (1-mask_vardens) + DATA);
    imshow(abs(img),[]),drawnow;
end

return;

F=fftshift(fft2(fid));

fid2 = ifft2(ifftshift(F));

imgs = abs(F);

figure;
imagesc(imgs(:, :, 1, 1));
colormap (gray(256));