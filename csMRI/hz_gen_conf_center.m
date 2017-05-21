n1 = 24;
n2 = 24;
rareFactor = 8;
K1 = -n1/2 : n1/2-1;
K2 = -n2/2 : n2/2-1;

[S2,S1] = meshgrid(K2, K1);
S1 = S1(:);
S2 = S2(:);
R = S1.^2 + S2.^2;
[~,ind] = sort(R);
S1 = S1(ind);
S2 = S2(ind);
nSlice = n1 * n2 / rareFactor;
enc1 = reshape(S1, nSlice, rareFactor);
enc2 = reshape(S2, nSlice, rareFactor);

enc1 = enc1';
enc2 = enc2';

enc = [enc1(:), enc2(:)];

dlmwrite('temp.txt', enc, ' ');