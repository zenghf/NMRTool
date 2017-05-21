function mask = genLRSampling_pctg(imSize,pctg,distType,disp)
% mask = genLRSampling_pctg(imSize,pctg,distType,disp)
%
% Creates a circular mask around the center
% user enters percentage of pixels out of the total number.
%
% (c) Michael Lustig 2007


sx = imSize(1);
sy = imSize(2);


if sum(imSize==1)==0  % 2D	
	[x,y] = meshgrid(linspace(-1,1,sy),linspace(-1,1,sx));
	switch distType
		case 1
			r = max(abs(x),abs(y));
		otherwise
			r = sqrt(x.^2+y.^2);
			r = r/max(abs(r(:)));			
	end
else %1d
	r = abs(linspace(-1,1,max(sx,sy)));
end

[nothing, circOrder] = sort(r(:));

mask = zeros(imSize);
mask(circOrder(1:floor(pctg*sx*sy))) = 1;

if disp
	figure, 
	subplot(211), imshow(mask)
	if sum(imSize==1)==0
		subplot(212), plot(mask(end/2+1,:));
	else
		subplot(212), plot(mask);
	end
end

