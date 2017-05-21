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
    load 128_0.375.mat
    minIntrVec = mask1d;
end

%% genList
rareFactor = 4;
nExcitation = ceil(ny / rareFactor);
mask = minIntrVec;
lin = -ny/2:(ny/2-1);
sel = lin(mask);
[~,indSel] = sort(abs(sel));
encSteps = sel(indSel);
nExcitation = ceil(length(encSteps) / rareFactor);
csEncSteps = zeros(rareFactor * nExcitation, 1);
csEncSteps(1:length(encSteps)) = encSteps;
csEncSteps = reshape(csEncSteps, [nExcitation, rareFactor]);
csEncSteps = csEncSteps'
%reshape(csEncSteps, [rareFactor, length(csEncSteps) / rareFactor])
dlmwrite('myFile.txt',csEncSteps(:), '\n');