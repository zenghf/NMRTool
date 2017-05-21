function [k,w] = loadks(filename)
% load a k-space coordinate, and density compensation file
%
% file is a binary file ordered as [kx1,ky1,w1,kx2,ky2,w2......,kxn,kyn,wn] and each element is a float.
%
%	inputs:
%			filename : guess what? it is the file name!
%
%	outputs:
%			k - k-space coordinate (complex - real=kx, imag=ky)
%			w - density compensation
%
%	(c) Michael Lustig 2007


fid = fopen(filename,'r');
ks_tmp = fread(fid, inf,'float');
kx = ks_tmp(1:3:end);
ky = ks_tmp(2:3:end);
w = ks_tmp(3:3:end);
fclose(fid);

k =+i*kx - ky;
%k
