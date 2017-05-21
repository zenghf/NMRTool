n1 = 24;
n2 = 24;
rareFactor = 8;
K1 = -n1/2 : n1/2-1;
K2 = -n2/2 : n2/2-1;

[~,ind] = sort(abs(K1));
cK1 = reshape(K1(ind), n1/rareFactor, rareFactor);
cK1 = cK1';
cK1 = cK1(:)';
[~,ind] = sort(abs(K2));
cK2 = K2(ind);

[S2,S1] = meshgrid(cK2, cK1);
S1 = S1(:);
S2 = S2(:);
nSlice = n1 * n2 / rareFactor;
enc1 = reshape(S1, rareFactor, nSlice);
enc2 = reshape(S2, rareFactor, nSlice);

for k = 1:rareFactor
    ind = randperm(nSlice);
    ind = 1:nSlice;
    enc1(k,:) = enc1(k,ind);
    enc2(k,:) = enc2(k,ind);
end

enc = [enc1(:), enc2(:)];

dlmwrite('temp.txt', enc, ' ');