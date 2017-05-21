function res = crop(x,sx,sy)
%res = crop(x,sx,sy)
%
% crops matrix a to size (sx,sy) around the center
% only works for even size matrixes
%
%	(c) Michael Lustig 2007

[mx,my] = size(x);
    idxx = mx/2+1 - sx/2 : mx/2 + sx/2;
    idxy = my/2+1 - sy/2 : my/2 + sy/2;

    res = x(idxx,idxy);


