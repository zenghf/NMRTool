function mMat = blochSolveExch(pools, frequency, pulseCell, magneticField, nPulseRepeat, gyro, m0, soluteExchMat)
% bloch simulation of the magnetization transfer, ver1.1
% Author: Haifeng Zeng, hzeng6@jhmi.edu
% pool    = {name, T1, T2, lifetime, chemicalShift, concentration};
%    name: pool name
%    T1: T1 of the pool
%    T2: T2 of the pool
%    lifetime: lifetime of the exchangable proton in that pool, 
%              1/(exchange rate), 1 for free water 
%    chemicalShift: chemical shift of the exchangable proton, in ppm
%    concentration: concentration of the exchangable proton, in mM
% pools = {pool1; pool2; pool3; pool4;...}; pool1 is always free water pool
% frequency: carrier frequency of the pulse, in ppm
% pulseCell: {pulse1, pulse2, pulse3,...}
%     pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
%         strength: in Hz 
%             0: delay, no pulse
%         phase: (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
%         duration: in s 
%             -inf: crush transverse magnetization
%             inf:  return to thermal equlibrium magnetization
% magneticField: magnetic field in Tesla
% nPulseRepeat: number of repeat of the pulses in pulseCell
% gyro: relative gyromagnetic ratio to 1H
% m0: initial magnetization vector in the follwoing order
%     [x(freewater); x(exchangable1); x(exchangable2);...
%      y(freewater); y(exchangable1); y(exchangable2);...
%      z(freewater); z(exchangable1); z(exchangable2);...]
%      z(freewater) = 1;
%     0: thermal equilibrium
% soluteExchMat: exchange matrix of among the other pools, 
%     size: [nPools-1, nPools-1]
% mMat: three dimensional matrix of magnetization vector after the pulse
%     dim 1: [x0; x1; x2; ...y0; y1; y2;...z0; z1; z2; ...]
%     dim 2: [after pulse 1; after pulse 2; after pulse 3;...]
%     dim 3: [after repeat 1; after repeat 2; after repeat 3;...]
% sequence: y1 -> [pulse1, pulse2,...] -> [pulse1, pulse2,...] -> ...
%                 \_________________________________________________/
%                              repeat pulseRepeat times
%     The calculation is in a rotating frame with a frequency of the pulse

gyro1H = 42.576;  % gyromagnetic ratio of 1H, MHz/T
pools = pools(:);
freewater = pools{1};
otherPools = pools(2:end);
nPools = length(pools);
nPulseCell = length(pulseCell);

% Calculate the initial pool concentrations
%   concList: array of concentrations of all pools
concList = cellfun(@(pool) pool{6},  pools);
%   minf: vector of magnetizations at thermal equalibrium
%     ([x_water; x1; x2; ...y_water; y1; y2;...z_water; z1; z2; ...])
minf = [zeros(nPools*2,1);concList(:)] / concList(1);
if (length(m0) == 1) && (m0==0)
    m0 = minf;
    disp('Set the initial magnetization at thermal equalibrium');
elseif (any(size(m0) ~= size(minf)))
    error('Wrong initial magnetization m0, m0 is a 3*nPools by 1 vector, m0=0 means magnetization at thermal equalibrium');
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
tempExchMat = blkdiag(0, soluteExchMat);
exchMat = exchMat + blkdiag(tempExchMat, tempExchMat, tempExchMat); 

% Calculate the relaxation matrix
r1List = cellfun(@(pool) 1/pool{2},  pools);
r2List = cellfun(@(pool) 1/pool{3},  pools);
rMat = -diag([r2List;r2List;r1List]);

% Calculate the chemical shift matrix
%   chemicalShiftList: in ppm
chemicalShiftList = cellfun(@(pool) pool{5},  pools); 
%   wList: chemical shift evolution frequency in radian
wList = 2*pi*magneticField*gyro1H*gyro*(chemicalShiftList-frequency);        
chemicalShiftMat = diag([-wList;zeros(nPools,1)],nPools) + ...
                   diag([wList;zeros(nPools,1)],-nPools);               
               
% Calculate the evolution matrix for the pulse
% http://en.wikipedia.org/wiki/Rotation_matrix
% section "9.3 Exponential map"
mpListCell = cell(1,nPulseCell);
transMatListCell = cell(1,nPulseCell);
crusherMat = diag([zeros(nPools*2,1);ones(nPools,1)]);
for kPulseCell = 1:nPulseCell
    pulse = pulseCell{kPulseCell};
    nPulse = size(pulse,1);
    mpListCell{kPulseCell} = zeros(3*nPools,nPulse);
    transMatListCell{kPulseCell} = zeros(3*nPools,3*nPools,nPulse);
    for kPulse = 1:nPulse
        xList = ones(nPools,1) * pulse(kPulse,1) * cos(pulse(kPulse,2)) * pi * 2;
        yList = ones(nPools,1) * pulse(kPulse,1) * sin(pulse(kPulse,2)) * pi * 2;
        zList = zeros(nPools,1);
        pulseMat = diag([-zList;-xList], nPools) + diag(yList, nPools*2) + ...
                   diag([zList;xList], -nPools) + diag(-yList, -nPools*2);
        evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
        t = pulse(kPulse,3);
        if (~isinf(t))
            mp = mldivide(evolMat, -rMat*minf);
            transMat = expm(evolMat*t);
            mpListCell{kPulseCell}(:,kPulse) = mp;
            transMatListCell{kPulseCell}(:,:,kPulse) = transMat;
            if (t<0)
                warnmsg=sprintf('Pulse cell %i pulse element %i has a negative pulse time %5.4e',kPulseCell,kPulse,t);
                warning(warnmsg);
            end
        else
            if (t>0)
                warnmsg=sprintf('Pulse cell %i pulse element %i :Thermal equalibrium is reached when pulse time t = Inf',kPulseCell,kPulse);
                warning(warnmsg);
            else
                warnmsg=sprintf('Pulse cell %i pulse element %i :Transverse magnetization is crushed when pulse time t = -Inf',kPulseCell,kPulse);
                warning(warnmsg);
            end
        end

    end
end

% Calculate the evolution matrix
% dy/dt = (exchMat + chemicalShiftMat + pulseMat)y + rMat(y-y0)
% y is the vector of magnetization, y0 is the vector of magnetization at
%     thermal equilibrium
% change the equation into the following form
% d(y+yp) = (exchMat + chemicalShiftMat + pulseMat + rMat)*(y+yp)
%         = evolMat*(y+yp)
%    evolMat = exchMat + chemicalShiftMat + pulseMat + rMat;
%    yp      = mldivide(evolMat, -rMat*y0);
% the solution for y is 
% y = expm(evolMat*t)*(y+yp) - yp

% Calculae the evolution of the magnetization
mMat = zeros(nPools*3, nPulseCell, nPulseRepeat);
m = m0(:);
for kRepeat = 1:nPulseRepeat
    for kPulseCell = 1:nPulseCell
        pulse = pulseCell{kPulseCell};
        nPulse = size(pulse,1);
        for kPulse = 1:nPulse
            t = pulseCell{kPulseCell}(kPulse,3);
            if ~isinf(t)
                mp = mpListCell{kPulseCell}(:,kPulse);
                transMat = transMatListCell{kPulseCell}(:,:,kPulse);
                m = transMat*(m+mp) - mp;
            else
                if (t>0)
                    %Thermal equalibrium is reached when pulse time t = Inf
                    m = minf;
                else
                    %Transverse magnetization is crushed when pulse time t = -Inf
                    m = crusherMat * m;  
                end
            end
        end
        mMat(:,kPulseCell,kRepeat) = m;
    end
end

end