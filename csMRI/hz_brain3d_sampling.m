% generate variable density random sampling
addpath(strcat(pwd,'/utils'));
P = 5; 
pctg = 0.25;
N = [80 30];
%rng(1000);
if 1
    pdf = genPDF(N, P, pctg, 2, 0.1, 1);	% generates the sampling PDF
    %[pdf,val] = genPDF(imSize,p,pctg [,distType,radius,disp])
    % pdf = (1-r).^p + c, pdf=1 if r<radius.
    %return;
    [mask,stat,actpctg] = genSampling(pdf,10000,0);		% generates a sampling pattern
    plt(stat);
else
    %load 128_8_0.375.mat
    load 128_0.25.mat
    minIntrVec = mask1d;
end

return;
