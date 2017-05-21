function res = zpad(x,sx,sy)
%res = zpad(x,sx,sy)
%
%  Zero pads a 2D matrix around its center.
%
% (c) Michael Lustig 2007

    [mx,my] = size(x);
    res = zeros(sx,sy);
    
    idxx = sx/2+1-mx/2 : sx/2+mx/2;
    idxy = sy/2+1-my/2 : sy/2+my/2;

    res(idxx,idxy) = x;

