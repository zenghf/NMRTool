%% generate variable density random sampling
addpath(strcat(pwd,'/utils'));
P = 5; 
pctg = 0.25;
n1 = 64;
n2 = 32;
rareFactor = 8;
if 0
    %pdf1d = genPDF(ny,P,pctg , 1 ,0.1,0);	% generates the sampling PDF
    [pdf,val] = genPDF([n1, n2], P, pctg, 2, 0.1, 1);
    %[pdf,val] = genPDF([nx, ny], P, pctg, distType,radius,disp])
    % pdf = (1-r).^p + c, pdf=1 if r<radius.
    %return;
    [mask,stat,actpctg] = genSampling(pdf,10000,0);		% generates a sampling pattern
    plt(stat);
    save('a.mat', 'pdf', 'mask');
else
    %load 128_8_0.375.mat
    load 64_32_0.25.mat
end

%% genList

[ind1, ind2] = find(mask); % row and column index of mask, 1...m, 1...n
enc1 = ind1 - size(mask,1)/2 - 1; % row index, -m/2 ... m/2-1
enc2 = ind2 - size(mask,2)/2 - 1; % column index, -n/2 ... n/2-1
indr = enc1.^2 + enc2.^2;
[~, sort_ind] = sort(indr);
enc1 = enc1(sort_ind);
enc2 = enc2(sort_ind);

nExcitation = ceil(length(ind1) / rareFactor);
csEncSteps1 = zeros(rareFactor * nExcitation, 1);
csEncSteps2 = zeros(rareFactor * nExcitation, 1);
csEncSteps1(1:length(enc1)) = enc1;
csEncSteps2(1:length(enc2)) = enc2;
csEncSteps1 = reshape(csEncSteps1, [nExcitation, rareFactor]);
csEncSteps2 = reshape(csEncSteps2, [nExcitation, rareFactor]);
csEncSteps1 = csEncSteps1';
csEncSteps2 = csEncSteps2';

%return;
%reshape(csEncSteps, [rareFactor, length(csEncSteps) / rareFactor])
encMat = [csEncSteps1(:), csEncSteps2(:)];
f = fopen('myFile.txt','w');
fprintf(f, '# First column: index of dimension 2; Second column: index of dimension 3\n');
fprintf(f, 'CS_EncMatrix %d %d\n', n1, n2 * pctg);
fprintf(f, 'CS_ACT_EncMatrix %d %d\n', n1, n2);
fprintf(f, 'PVM_RareFactor %d\n', rareFactor);
fprintf(f, '%4d %4d\n', encMat');
fclose(f);
%dlmwrite('myFile.txt', [csEncStepsy(:), csEncStepsx(:)], ' ');