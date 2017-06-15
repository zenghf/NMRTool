function [imgs k Nch NE NS NR]= bruker_RARE_FID(path,expno, rotation)

%path 
% experiment number
% rotation can read from RECO_rotate, which shift the image up


antialis=0;      % anti-alising filtering
Datatype=2;   % 1 for complex 2 for magitude 3 for phase


filename=fullfile(path, num2str(expno),'fid');
method_file = fullfile(path, num2str(expno),'acqp');;
%% read paramters from method file

para_list = fopen(method_file);
if para_list == -1;
    error('Could not open method file');
end
tline = fgetl(para_list);
while ischar(tline)
    
   matches = strfind(tline, '##$ACQ_size=');  % Read direction
   num = length(matches);
   if num > 0    
      FIDSIZE = str2num(fgetl(para_list));;
   end
   matches = strfind(tline, '##$ACQ_rare_factor=');                     % NI: Nslice*Nechoes
    num = length(matches);
    if num > 0
      NE = str2num(tline(strfind(tline, '=')+1:end));
    end
       matches = strfind(tline, '##$NSLICES=');                     % NI: Nslice*Nechoes
    num = length(matches);
    if num > 0
      NS = str2num(tline(strfind(tline, '=')+1:end));
    end
       matches = strfind(tline, '##$NR=');                     % NI: Nslice*Nechoes
    num = length(matches);
    if num > 0
      NR = str2num(tline(strfind(tline, '=')+1:end));
    end
       matches = strfind(tline, '##$ACQ_spatial_phase_1=');  % Read direction
   num = length(matches);
   if num > 0 
       idx=0;
       tmp=fgetl(para_list);
       PHASEENC=[];
       while (tmp(1)~='#')
           idx=idx+1;
            newdata= str2num(tmp);
            PHASEENC=[PHASEENC newdata];
        tmp=fgetl(para_list);
       end
      
   end
    
    
   tline = fgetl(para_list);
end
fclose(para_list);

method_file = fullfile(path, num2str(expno),'method');
para_list = fopen(method_file);
if para_list == -1;
    error('Could not open method file');
end
tline = fgetl(para_list);
while ischar(tline)
    
   matches = strfind(tline, '##$PVM_EncNReceivers=');  % Read direction
   num = length(matches);
   if num > 0    
     Nch = str2num(tline(strfind(tline, '=')+1:end));
   end
   
   tline = fgetl(para_list);
end
fclose(para_list);


nx= FIDSIZE(1)/2;
%when fid is not order of 2 Bruker will zero filling fid to order of 2
nx_fid=2^nextpow2(nx);

ny= FIDSIZE(2);




fid=fopen(filename,'r','l');



k_real=zeros(nx,ny,Nch,NS,NR);
k_imag=k_real;

%%% reshape the matrix 
for idx_nr=1:NR
    for nne=1:ny/NE
        for idx_ns=1:NS
            %notice the rare k space is interleave for each shot
            for ne=1:NE
              for idx_ch=1:Nch
                for idx_x=1:nx_fid
                  %idx_y=(nne-1)+(ne-1)*ny/NE +1;
                  idx_y=(ne-1)+(nne-1)*NE +1;
                 
                 k_real(idx_x,idx_y, idx_ch, idx_ns, idx_nr)=fread(fid,1,'int32');
                 k_imag(idx_x,idx_y, idx_ch, idx_ns, idx_nr)=fread(fid,1,'int32');

                end
              end
            end
        end
    end
end

fclose(fid);

j=sqrt(-1);
%resize the k space to original size from nx_fid to nx
k_real=k_real(1:nx,:,:,:,:);
k_imag=k_imag(1:nx,:,:,:,:);


k=k_real+k_imag.*j;



%%% flip over the k-space from bottom to top because the phase encoding
%%%- is starting form bottom
%k_temp=k;%temporary keep old k-space data
%for m=1:nx
%    for n=1:ny
%        k(m, n, :, :)=k_temp(nx-m+1, ny-n+1, :, :);
%    end
%end

%resort the k base based on the phase encoding
[newphaseenc, idx] = sort(PHASEENC,2,'descend');
k=k(:,idx,:,:,:);

%%% FT portion




F=fftshift(fft2(k),1);
F=fftshift(F,2);

% F=squeeze(fftshift(F,2))

if (antialis) 
    F=alisingfilter(F);
end

if Datatype==1
imgs=F;
elseif Datatype==2
imgs=abs(F);
 elseif Datatype==3
 imgs=angle(F);
end



%permute the final image
imgs = permute(imgs, [2, 1, 3:length(size(imgs))]);

 % rotate the image i.e. shift the images 
imgs=circshift(imgs,-round(rotation*ny));
    
end




% save (strcat(Matname),  'MI', 'tr')
