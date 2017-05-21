% generate variable density random sampling
addpath(strcat(pwd,'/utils'));
P = 5; 
pctg = 0.25;
ny = 128;
%rng(1000);
if 0
    pdf1d = genPDF(ny,P,pctg , 1 ,0.1,0);	% generates the sampling PDF
    %[pdf,val] = genPDF(imSize,p,pctg [,distType,radius,disp])
    % pdf = (1-r).^p + c, pdf=1 if r<radius.
    %return;
    [minIntrVec,stat,actpctg] = genSampling(pdf1d,1000,0);		% generates a sampling pattern
    plt(stat);
else
    %load 128_8_0.375.mat
    load 128_0.25.mat
    minIntrVec = mask1d;
end

%% genList
rareFactor = 16;
nExcitation = ceil(ny / rareFactor);
mask = minIntrVec;
lin = -ny/2:(ny/2-1);
sel = lin(mask);
nsel = lin(~mask);
[~,indSel] = sort(abs(sel));
[~,indNsel] = sort(abs(nsel));
encSteps = [sel(indSel), nsel(indNsel)];

ord = 0:ny-1;
kExc = floor(ord/rareFactor);
kFid = mod(ord,rareFactor);
kOrd = kFid*nExcitation + kExc + (kFid >= rareFactor*pctg) * ny;

[~,ind]=sort(kOrd);
csEncSteps = zeros(ny,1);
csEncSteps(ind) = encSteps;
%dlmwrite('myFile.txt',csEncSteps, '\n');