% generate variable density random sampling
addpath(strcat(pwd,'/utils'));
P = 5; 
pctg = 1;
nx = 32;
ny = 16;
rareFactor = 8;
if 0
    %pdf1d = genPDF(ny,P,pctg , 1 ,0.1,0);	% generates the sampling PDF
    [pdf,val] = genPDF([nx, ny], P, pctg, 2, 0.1, 1);
    %[pdf,val] = genPDF([nx, ny], P, pctg, distType,radius,disp])
    % pdf = (1-r).^p + c, pdf=1 if r<radius.
    %return;
    [mask,stat,actpctg] = genSampling(pdf,2,0);		% generates a sampling pattern
    plt(stat);
    save('a.mat', 'pdf', 'mask');
else
    %load 128_8_0.375.mat
    load 32_16_1.0.mat
end
%return;
%% genList

linx = -nx/2:(nx/2-1);
liny = -ny/2:(ny/2-1);
[X, Y] = meshgrid(linx, liny); 
% [X, Y] = meshgrid(x,y); 
% X = [x(:)';x(:)';x(:)';...]
% Y = [y(:), y(:), y(:), ...]
selx = X(mask);
sely = Y(mask);
selr = selx.^2 + sely.^2;
[~,indSel] = sort(selr);
encStepsx = selx(indSel);
encStepsy = sely(indSel);
nExcitation = ceil(length(encStepsx) / rareFactor);
csEncStepsx = zeros(rareFactor * nExcitation, 1);
csEncStepsy = zeros(rareFactor * nExcitation, 1);
csEncStepsx(1:length(encStepsx)) = encStepsx;
csEncStepsy(1:length(encStepsy)) = encStepsy;
csEncStepsx = reshape(csEncStepsx, [nExcitation, rareFactor]);
csEncStepsy = reshape(csEncStepsy, [nExcitation, rareFactor]);
csEncStepsx = csEncStepsx';
csEncStepsy = csEncStepsy';
%reshape(csEncSteps, [rareFactor, length(csEncSteps) / rareFactor])
encMat = [csEncStepsy(:), csEncStepsx(:)];
f = fopen('myFile.txt','w');
fprintf(f, '# First row: index of dimension 2; Second row: index of dimension 3\n');
fprintf(f, 'CS_EncMatrix %d %d\n', nx, ny * pctg);
fprintf(f, 'CS_ACT_EncMatrix %d %d\n', nx, ny);
fprintf(f, 'PVM_RareFactor %d\n', rareFactor);
fprintf(f, '%4d %4d\n', encMat');
fclose(f);
%dlmwrite('myFile.txt', [csEncStepsy(:), csEncStepsx(:)], ' ');