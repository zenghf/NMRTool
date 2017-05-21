%% generate variable density random sampling
addpath(strcat(pwd,'/utils'));
P = 5; 
pctg = 1;
n1 = 8;
n2 = 8;
rareFactor = 8;

%% genList
ind1 = (1:n1) - n1/2 - 1;
ind2 = (1:n2) - n2/2 - 1;
[~, i1] = sort(abs(ind1));
ind1 = ind1(i1);
[~, i2] = sort(abs(ind2));
ind2 = ind2(i2);
ind1 = reshape(ind1, [n1/rareFactor, rareFactor]);
ind1 = ind1';
ind1 = ind1(:)';

[enc2, enc1] = meshgrid(ind2, ind1);
enc1 = enc1(:);
enc2 = enc2(:);
% [enc1,enc2]
%return;
%reshape(csEncSteps, [rareFactor, length(csEncSteps) / rareFactor])
encMat = [enc1,enc2];
f = fopen('myFile.txt','w');
fprintf(f, '# First column: index of dimension 2; Second column: index of dimension 3\n');
fprintf(f, 'CS_EncMatrix %d %d\n', n1, n2 * pctg);
fprintf(f, 'CS_ACT_EncMatrix %d %d\n', n1, n2);
fprintf(f, 'PVM_RareFactor %d\n', rareFactor);
fprintf(f, '%4d %4d\n', encMat');
fclose(f);
%dlmwrite('myFile.txt', [csEncStepsy(:), csEncStepsx(:)], ' ');