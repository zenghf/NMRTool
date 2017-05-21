%function data = readRareFID3D(path, expno, varargin)
path0 = '/Users/hzeng/Data/hzeng.rA1/';
procno = 1;
expno = 3;
% rotation can read from RECO_rotate, which shift the image up

filename=fullfile(path0, num2str(expno), 'pdata', num2str(procno),'2dseq');
f=fopen(filename,'r','l');
data = fread(f, inf, 'uint16');
fclose(f);
return;
%% read paramters from method file
acqp = fullfile(path0, num2str(expno),'acqp');
P = readnmrpar(acqp);
FIDSIZE = P.ACQ_size;
NE = P.ACQ_rare_factor;
NS = P.NSLICES;
NR = P.NR;
PHASEENC1 = P.ACQ_spatial_phase_1;
PHASEENC2 = P.ACQ_spatial_phase_2;
method = fullfile(path0, num2str(expno),'method');
P_method = readnmrpar(method);
Nch = P_method.PVM_EncNReceivers;
nx = FIDSIZE(1)/2;
%when fid is not order of 2 Bruker will zero filling fid to order of 2
nx_fid = 2^nextpow2(nx);
ny = FIDSIZE(2);
nz = FIDSIZE(3);
% data in the following order
% [real, imag] x nx_fid x Nch x NE x NS x ny/NE x NR;
dim_acq = [2, nx_fid, Nch, NE, NS, ny/NE, nz, NR];
dim_fid = [2, nx_fid, ny, nz, Nch, NS, NR];
f=fopen(filename,'r','l');
data = fread(f, prod(dim_acq), 'int32');
%data = fread(f, inf, 'int32');
data = reshape(data, dim_acq);
data = permute(data, [1, 2, 4, 6, 7, 3, 5, 8]);
data = reshape(data, dim_fid);
data = complex(data(1,:,:,:,:,:,:), data(2,:,:,:,:,:,:));
data = shiftdim(data);

%resort the k base based on the phase encoding
[~, idx1] = sort(PHASEENC1,2,'descend');
[~, idx2] = sort(PHASEENC2,2,'descend');
%nargin
if (nargin < 3) 
    %disp('sort fid');
    data=data(:,idx1,:,:,:,:);
    data=data(:,:,idx2,:,:,:);
end
data = squeeze(data);

%end
