function [minIntrVec,stat,actpctg] = genSampling(pdf,iter,tol)

%[mask,stat,N] = genSampling(pdf,iter,tol)
%
% a monte-carlo algorithm to generate a sampling pattern with 
% minimum peak interference. The number of samples will be
% sum(pdf) +- tol
%
%	pdf - probability density function to choose samples from
%	iter - number of tries
%	tol  - the deviation from the desired number of samples in samples
%
% returns:
%	mask - sampling pattern
%	stat - vector of min interferences measured each try
%	actpctg    - actual undersampling factor
%
%	(c) Michael Lustig 2007

h = waitbar(0);

pdf(find(pdf>1)) = 1;
K = sum(pdf(:));
%%%%%%% add by H. Zeng %%%%%%
K = floor(sum(pdf(:)));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

minIntr = 1e99;
minIntrVec = zeros(size(pdf));

for n=1:iter
	tmp = zeros(size(pdf));
	while abs(sum(tmp(:)) - K) > tol
		tmp = rand(size(pdf))<pdf;
	end
	
	TMP = ifft2(tmp./pdf);
	if max(abs(TMP(2:end))) < minIntr
		minIntr = max(abs(TMP(2:end)));
		minIntrVec = tmp;
	end
	stat(n) = max(abs(TMP(2:end)));
	waitbar(n/iter,h);
end

actpctg = sum(minIntrVec(:))/prod(size(minIntrVec));

close(h);


