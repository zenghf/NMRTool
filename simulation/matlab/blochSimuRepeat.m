% m0 = [0; 0; 1];
% frequencies = [0, 1000];
% pulse = [100, 0 ,2.5e-3];
% T1 = inf;
% T2 = inf;

function [ mMat ] = blochSimuRepeat(m0, pulse, nPulse, frequencies, T1, T2)

% function [ mMat ] = blochSimu(m, pulse, frequencies, T1, T2)
%Bloch simulation of magnetization evolution ver 1.0
%Input parameters
%  m: initial magnetization in 3x1 vector
%  pulse: [strength1,phase1,duration1;strength2,phase2,duration2;...]
%        strength: in Hz 
%        phase: (0:x, pi/2:y, pi:-x, 3*pi/2:-y) 
%        duration: in s
%  frequencies: offest list in Hz
%  T1: in s, default value is Inf
%  T2: in s, default value is Inf
%Output parameters
%  mMat: 3 dimensional matrix 
%    dim 1: [x y z] magnetization;
%    dim 2: frequencies
%    dim 3: elements of pulses, initial magnetization, then magnetization
%           after each pulse element, total (nPulses+1) elements

%{
m = [0; 0; 1];
pulse = [10000, 0, 0.25e-4;
         10000, pi, 0.25e-4];
frequencies = linspace(-1000,1000);
T1 = Inf;
T2 = Inf;
mMat =  blochSimu(m, pulse, frequencies, T1, T2);
figure;
plot1 = plot(frequencies, mMat(:,:,end));
set(plot1(1),'DisplayName','x');
set(plot1(2),'DisplayName','y');
set(plot1(3),'DisplayName','z');
legend(gca,'show');
%}
     
%minf = [0; 0; 1];    % magnetization at thermal equalibrium 
     
rMat = -diag([1/T2;1/T2;1/T1]);

% Calculate the evolution matrix
% dm/dt = (pulseMat)m + rMat(m-minf)
% m is the vector of magnetization, minf is the vector of magnetization at
%     thermal equilibrium
% change the equation into the following form
% d(m+mp)/dt = (pulseMat + rMat)*(m+mp)
%    mp      = mldivide(pulseMat+rMat, -rMat*minf);
% the solution for m is 
% m = expm((pulseMat+rMat)*t)*(m+mp) - mp;
% Calculate pulseMat, the evolution of the magnetization 
%   http://en.wikipedia.org/wiki/Rotation_matrix
%   section "9.3 Exponential map"



% mp = [0;0;0];
% for kFreq = 1:length(frequencies)
%     for kPul = 1:size(pulse,1)
%         t = pulse(kPul,3);
%         x = pulse(kPul,1) * cos(pulse(kPul,2)) * pi * 2;
%         y = pulse(kPul,1) * sin(pulse(kPul,2)) * pi * 2;
%         z = frequencies(kFreq) * pi * 2;
%         pulseMat = [   0,  -z,  y;
%                     z,      0, -x;
%                    -y,   x,     0];
%         if isfinite(T1)
%             mp = mldivide(pulseMat+rMat, [0;0;1/T1]);
%         end
%         mMat(:,kFreq,kPul+1) = expm((rMat+pulseMat)*t)*...
%                                (mMat(:,kFreq,kPul)+mp) - mp;
%     end
% end

nFreq = length(frequencies);
nPulseShape = size(pulse, 1);
mpMatrix = zeros(3, nPulseShape, nFreq);
expmMatrix = zeros(3, 3, nPulseShape, nFreq);

for kFreq = 1:nFreq
    for kPul = 1:nPulseShape
        t = pulse(kPul,3);
        x = pulse(kPul,1) * cos(pulse(kPul,2)) * pi * 2;
        y = pulse(kPul,1) * sin(pulse(kPul,2)) * pi * 2;
        z = frequencies(kFreq) * pi * 2;
        pulseMat = [   0,  -z,  y;
                    z,      0, -x;
                   -y,   x,     0];
        if isfinite(T1)
            mp = mldivide(pulseMat+rMat, [0;0;1/T1]);
            mpMatrix(:, kPul, kFreq) = mp;
        end
        
        expmMatrix(:, :, kPul, kFreq) = expm((rMat+pulseMat)*t);
        
    end
end


mMat = zeros(3, nFreq);
for kFreq = 1:nFreq
    m = m0;
    for k = 1:nPulse   
        for kPul = 1:nPulseShape
            mp = mpMatrix(:, kPul, kFreq);
            evolMat = expmMatrix(:, :, kPul, kFreq);
            m = evolMat * (m + mp) - mp;
        end
    end
    mMat(:, kFreq) = m;
end

