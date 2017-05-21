function mask = genLRSampling_rad(imSize,radius,distType,disp)

% mask = genLRSampling_rad(imSize,radius,distType,disp)
%
% creates a mask of a circle around the center. 
% used enters radius as input
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

mask = r<radius;

if disp
	figure, 
	subplot(211), imshow(mask)
	if sum(imSize==1)==0
		subplot(212), plot(mask(end/2+1,:));
	else
		subplot(212), plot(mask);
	end
end

