function [C,time,g,s,k, phi, sta, stb] = minTimeGradient(C,g0, gfin, gmax, smax,T, show)

% [C,time,g,s,k] = minTimeGradient(C, g0, gfin, gmax, smax,T,show)
%
% Given a k-space trajectory C(n), gradient and slew constraints. This
% function will return a new parametrization that will meet these
% constraint while getting from one point to the other in minimum time.
%  
%   C       -   The Curve in k-space given in any parametrization [1/cm]
%               Accepts complex value C for 2D trajectory, and a Nx3 real
%               Matrix for 3D trajectories.
%   g0      -   Initial gradient amplitude (leave empty for g0 = 0)
%   gfin    -   Gradient value at the end of the trajectory. If not possible, 
%               the result would be the largest possible ampltude.
%               (Leave empty if you don't care to get maximum gradient.)
%   gmax    -   Maximum gradient [G/cm] (3.9 default)
%   smax    -   Maximum slew [G/Cm/ms]  (14.5 default)
%   T       -   Sampling time interval [ms] (4e-3 default)
%   show    -   Show plots while optimizing (Warning: This will make the
%               process considerably slower!)
%   
% return values:
%   C       - reparametrized curve, sampled at T[ms]
%   time    - total time to get to the end
%   g       - gradiet waveform [G/cm]
%   s       - slew rate [G/cm/ms]
%   k       - exact k-space corresponding to gradient g (This function reparametrizes
%             C, then takes a derivative. Numerical errors in the derivative can lead to 
%             deviation.  
%   phi     - Geometry constraints on the amplitude vs. arclength
%   sta     - Solution for the forward ODE
%   stb     - Solution for the backward ODE
%
%
%   Examples:
%
%               t = linspace(-1,1, 500)';
%               C = (5+ t.*cos(5.5*pi*t)).*exp(i*pi*t);
%               [C,time,g,s,k] = minTimeGradient(C);
%               figure, 
%               subplot(311), plot(C,'.');
%               subplot(312), plot([real(g(:)), imag(g(:))]);, title('gradient');
%               subplot(313), plot([real(s(:)), imag(g(:))]);title('slew-rate');
%
%
%
%
%   (c) Michael Lustig 2005
%       modified 2006



if nargin<1
    error('You gotta give me a curve!');
end

if nargin<2
    g0 = [];
end

if nargin<3
    gfin = [];
end

if nargin<4
    gmax = 3.8;
end

if nargin<5
    smax = 14.5;
end

if nargin<6
    T = 4e-3;
end


if nargin<7
    show=0;
end

dt = T;




gamma = 4.257;



disp('Const arc-length parametrization');

% represent the curve using spline with parametrization p
Lp = length(C);
p = [0:Lp-1].';
PP = spline(p,C);


% interpolate curve for gradient accuracy
dp = 1e-2;
CC = ppval(PP, [0:dp:Lp-1].');


% find length of curve


Cp = (CC([2:end,end]) - CC)/dp;
Cp(end) = Cp(end-1);
Cp(1) = (CC(2)-CC(1))/dp;
%Cp = C([2:end,end])-C;
s_of_p = cumtrapz(abs(Cp))*dp;
L = s_of_p(end);


% decide ds and compute st for the first point
stt0 = (gamma*smax) ; % always assumes first point is max slew/5
st0 = stt0*dt;
s0 = st0*dt;
ds = s0/3;


s = [0 : ds : L].';
s_half = [0:ds/2:L].';
sta = s*0;

if isempty(g0)
    g0 = 0;
end
sta(1) = min(max(g0*gamma+st0), gamma*gmax);
p_of_s_half = interp1(s_of_p, [0:dp:Lp-1].', s_half, 'spline');
p_of_s = p_of_s_half(1:2:end);


disp('Compute geometry dependent constraints');
% compute constraints (forbidden line curve)
[phi,k] = sdotMax(PP,p_of_s_half,s_half, gmax, smax);
k = k([1:end,end,end]); % extend for the Runge-Kutte method

disp('Solve ODE forward');
% solve ODE forward
for n=2:length(s)
    %dstds = ODEFUN(s(n),ds,sta(n-1),k(n-1),smax,L);
    dstds = RungeKutte(s(n),ds,sta(n-1),k([(n-1)*2-1:(n-1)*2+1]),smax,L);
    tmpst = sta(n-1) + dstds;
    
    if isreal(tmpst) 
        sta(n) = min(tmpst,phi(n*2-1));
    else
        sta(n) = phi(n*2-1);
    end

    if mod(n,1000)==1 & show
       s(n)/L*100
       figure(100), plot(s,sta,linspace(s(1),s(end),length(phi)),phi); , axis([0,s(end),0,gmax*gamma*sqrt(2)]);, drawnow
   end
end


stb = 0*s;

if isempty(gfin)
    stb(end) = sta(end);
else
    stb(end) = min(max(gfin*gamma,st0), gamma*gmax);
end

% solve ODE backwards
disp('Solve ODE backwards');
for n=length(s)-1:-1:1
    %dstds = ODEFUN(s(n),ds,stb(n+1),k(n),smax,L);
    dstds = RungeKutte(s(n),ds,stb(n+1),k([(n+1)*2-1:-1:(n)*2-1]),smax,L);
    
    tmpst = stb(n+1) + dstds;
    
    if isreal(tmpst) 
        stb(n) = min(tmpst,phi(n*2-1));
    else
        stb(n) = phi(n*2-1);
    end

    if mod(n,1000)==1 & show
        s(n)/L*100
       figure(100), plot(s,stb,linspace(s(1),s(end), length(phi)),phi,s,sta);, axis([0,s(end),0,gmax*gamma*sqrt(2)]);,drawnow
   end
end

disp('Final Interpolations')
% take the minimum of the curves 
st_of_s = min([sta,stb],[],2);

% compute time
t_of_s = cumtrapz(1./st_of_s*ds);
t = 0:dt:t_of_s(end);

s_of_t = interp1(t_of_s, s, t,'spline');
p_of_t = interp1(s, p_of_s, s_of_t,'spline');

C = ppval(PP,p_of_t);
g = (C([2:end, end]) - C([1:end-1, end-1]))/gamma/dt;
k = cumtrapz(g)*dt*gamma;
s = (g([2:end, end]) - g([1:end-1, end-1]))/dt;
time = t(end);



disp('Done');


function [res] = ODEFUN(s,ds,st,k,smax,L)
% solves the ODE using Euler method
%waitbar(s/L);
gamma = 4.257;

res = ds*1/st*sqrt(gamma^2*smax^2 - abs(k)^2*st^4);

function [res] = RungeKutte(s,ds,st,k,smax,L)
gamma = 4.257;
k = abs(k);
k1 = ds * 1/st*sqrt(gamma^2*smax^2 - abs(k(1))^2*st^4);
k2 = ds * 1/(st+k1/2) * sqrt(gamma^2*smax^2 - abs(k(2))^2*(st+k1/2)^4) ;
k3 = ds * 1/(st+k2/2) * sqrt(gamma^2*smax^2 - abs(k(2))^2*(st+k2/2)^4) ;
k4 = ds * 1/(st+k3/2) * sqrt(gamma^2*smax^2 - abs(k(3))^2*(st+k3/2)^4) ;

res = k1/6 + k2/3 + k3/3 + k4/6;



function [sdot, k] = sdotMax(PP, p_of_s, s, gmax, smax)

% [sdot, k, ] = sdotMax(PP, p_of_s, s, gmax, smax)
%
% Given a k-space curve C (in [1/cm] units), maximum gradient amplitude
% (in G/cm) and maximum slew-rate (in G/(cm*ms)).
% This function calculates the upper bound for the time parametrization
% sdot (which is a non scaled max gradient constaint) as a function of s.
% 
%   PP       --  spline polynomial
%   p_of_s  --  parametrization vs arclength
%   s       --  arclength parametrization (0->1)
%   gmax    --  maximum gradient (G/cm)
%   smax    --  maximum slew rate (G/ cm*ms)
%
%   returns the maximum sdot (1st derivative of s) as a function of arclength s
%   Also, returns curvature as a function of s and length of curve (L)
%
%  (c) Michael Lustig 2005 
%  last modified 2006



gamma = 4.257;

s = s(:);
dp_p = p_of_s([2:end,end]) - p_of_s; , dp_p(end) = dp_p(end-1);
dp_m = p_of_s - p_of_s([1,1:end-1]);, dp_m(1) = dp_m(2);
ds_p = s([2:end,end]) - s; , ds_p(end) = ds_p(end-1);
ds_m = s - s([1,1:end-1]);, ds_m(1) = ds_m(2);


Cs_p = (ppval(PP,p_of_s + dp_p) - ppval(PP, p_of_s))./ds_p;
Cs_m = (ppval(PP,p_of_s) - ppval(PP, p_of_s-dp_m))./ds_m;
Cs = Cs_p/2 + Cs_m/2;
Css = (Cs_p - Cs_m)./(ds_m/2+ds_p/2);
k = abs(Css);
% fix edge numerical problems
k(end) = k(end-1);
k(1) = k(2);



% calc I constraint curve (maximum gradient)
sdot1 = gamma*gmax*ones(size(s));


% calc II constraint curve (curve curvature dependent)
sdot2 = sqrt(gamma*smax ./ (abs(k)+eps));

% calc total constraint
sdot = min([sdot1, sdot2],[],2);

