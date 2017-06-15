load brain.mat
W = Wavelet; % Daubechies-4 wavelet operator
im_W = W*im; % Forward Wavelet transform
im_W2  = zeros(size(im_W));
n = 128;
im_W2(1:n,1:n) = im_W(1:n,1:n);
im_rec = W'*im_W; % Inverse Wavelet transform
im_rec2 = W'*im_W2;
figure;
imshow(abs(im_rec));
figure;
imshow(abs(im_rec2));