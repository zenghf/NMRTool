function y2 = blochSolveSimple(pools, frequency, pulse, magneticField, crusher, gyro, y1)
% bloch simulation of the magnetization transfer 
% Author: Haifeng Zeng, hzeng6@jhmi.edu
% pool    = {name, T1, T2, lifetime, chemicalShift, concentration};
%    name: pool name
%    T1: T1 of the pool
%    T2: T2 of the pool
%    lifetime: lifetime of the exchangable proton in that pool, 
%              1/(exchange rate), 1 for free water 
%    chemicalShift: chemical shift the exchangable proton, in ppm
%    concentration: concentration of the exchangable proton, in mM
% pools = {pool1; pool2; pool3; pool4;...}; pool1 is always free water pool
% frequency: carrier frequency of the pulse, in ppm
% pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
%        strength: in Hz 
%        phase: (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
%        duration in s
% magneticField: magnetic field in Tesla
% crusher: true of false, whether to destroy the transverse magnetization
% gyro: relative gyromagnetic ratio to 1H
% y1: initial magnetization vector in the follwoing order
%     [x(freewater); x(exchangable1); x(exchangable2);...
%      y(freewater); y(exchangable1); y(exchangable2);...
%      z(freewater); z(exchangable1); z(exchangable2);...]
%      z(freewater) = 1;
%     y1 = 0 menas thermal equilibrium
% y2: magnetization vector after the pulse
%     The calculation is in a rotating frame with a frequency of the pulse

gyro1H = 42.576;  % gyromagnetic ratio of 1H, MHz/T
pools = pools(:);
freewater = pools{1};
otherPools = pools(2:end);
nPools = length(pools);


%  Calculate the initial pool concentrations
%  concList: array of concentrations of all pools
concList = cellfun(@(pool) pool{6},  pools);
% y0: array of initial magnetizations, 
%     ([x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...])
y0 = [zeros(nPools*2,1);concList(:)] / concList(1);
if (size(y1(:)) == size(y0))
    y1 = y1(:);
else
    y1 = y0;
end

% Calculate the exchange matrix
other2water = cellfun(@(pool) 1/pool{4},  otherPools);
water2other = cellfun(@(pool) pool{6}/pool{4},  otherPools) / freewater{6};
exchMat = zeros(nPools*3);
blockExchMat = zeros(nPools);
blockExchMat(1,2:end) = other2water(:)';
blockExchMat(2:end,1) = water2other(:);
for k = 0:2
    exchMat(k*nPools+1:(k+1)*nPools,k*nPools+1:(k+1)*nPools) = blockExchMat;
end
exchMat = exchMat - diag(sum(exchMat,1));

% Calculate the relaxation matrix
r1List = cellfun(@(pool) 1/pool{2},  pools);
r2List = cellfun(@(pool) 1/pool{3},  pools);
rMat = -diag([r2List;r2List;r1List]);

% Calculate the chemical shift matrix
% chemicalShiftList in ppm
chemicalShiftList = cellfun(@(pool) pool{5},  pools); 
% wList: chemical shift evolution frequency in radian
wList = 2*pi*magneticField*gyro1H*gyro*(chemicalShiftList-frequency);        
chemicalShiftMat = diag([-wList;zeros(nPools,1)],nPools) + ...
                   diag([wList;zeros(nPools,1)],-nPools);

% Calculate the evolution matrix for the pulse
% http://en.wikipedia.org/wiki/Rotation_matrix
% section "9.3 Exponential map"
pulseMatList = zeros(nPools*3, nPools*3, size(pulse,1));
for kPul = 1:size(pulse,1)
    xList = ones(nPools,1) * pulse(kPul,1) * cos(pulse(kPul,2)) * pi * 2;
    yList = ones(nPools,1) * pulse(kPul,1) * sin(pulse(kPul,2)) * pi * 2;
    zList = zeros(nPools,1);
    pulseMat = diag([-zList;-xList], nPools) + diag(yList, nPools*2) + ...
               diag([zList;xList], -nPools) + diag(-yList, -nPools*2);
    pulseMatList(:,:,kPul) = pulseMat;
end

% Calculate the evolution matrix
% dy/dt = (exchMat + chemicalShiftMat + pulseMat)y + rMat(y-y0)
% y is the vector of magnetization, y0 is the vector of magnetization at
%     thermal equilibrium
% change the equation into the following form
% d(y+yp)/dt = (exchMat + chemicalShiftMat + pulseMat + rMat)*(y+yp)
%         = evolMat*(y+yp)
%    evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
%    yp      = mldivide(evolMat, -rMat*y0);
% the solution for y is 
% y = expm(evolMat*t)*(y(0)+yp) - yp

% Calculae the evolution of the magnetization
y2 = y1(:);
for kPul = 1:size(pulse,1)
    t = pulse(kPul,3);
    if isinf(t)
        y2 = y0;
    else
        pulseMat = pulseMatList(:,:,kPul);
        evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
        yp = mldivide(evolMat, -rMat*y0);
        y2 = expm(evolMat*t) * (y2+yp) - yp;
    end
end

% Calculate the crusher matrix
crusherList = [ones(nPools*2,1)*(1-crusher);ones(nPools,1)];
y2 = crusherList .* y2;

end