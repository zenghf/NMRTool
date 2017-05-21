% This script run the thresholding experiment described in the paper.
%
% the intent is to show the sparsity of images with different transforms.
%

if exist('FWT2_PO') <2
	error('must have Wavelab installed and in the path');
end



cd threshold
experiment
cd ..

