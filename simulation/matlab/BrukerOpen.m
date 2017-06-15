function  [MI] = BrukerOpen(directory)
% Read in acqusition paramters
%  directory='/Users/guanshuliu/Desktop/20110609.glu&lipsomes/gl110609.8t1/42'
	
    Matrix_size=BrukerPar(directory, 'method', 'PVM_Matrix');
    M=Matrix_size(1); N=Matrix_size(2); 
    NR =BrukerPar(directory, 'method', 'PVM_NRepetitions');
%     NS =BrukerPar(directory, 'method', 'PVM_SPackArrNSlices');
    NS =BrukerPar(directory, 'acqp', '$NSLICES');
    try 
        NE =BrukerPar(directory, 'method', 'PVM_NEchoImages');
    catch
        NE = 1;
    end
   
    
    
 
	% Read in recostruction parameters
    subdir=[ directory '/pdata/1/'];
	bb = BrukerPar(subdir, 'reco', 'RECO_map_slope');
    Reco_trans= BrukerPar(subdir, 'reco', 'RECO_transposition');
	Doffset = BrukerPar(subdir, 'reco', 'RECO_map_offset');
 	if BrukerParStr(subdir, 'reco', 'RECO_wordtype')=='_32BIT_SGN_INT'  %% to be changed how to extract 16 or 32
    wordtype='32';
    elseif BrukerParStr(subdir, 'reco', 'RECO_wordtype')=='_16BIT_SGN_INT'  wordtype='16';
    else error ('not 16 bit or 32 bit data'); 
    end
    
   if Reco_trans==0
d = read_2dseq(directory,  [ M, N, NE, NS, NR], wordtype) ;
   else 
       d = read_2dseq(directory,  [ N, M, NE, NS, NR], wordtype) ;
   end 
d=squeeze(d); 
    
MI=d;
for k=1:NR*NS*NE
                MI(:,:,k) = d(:,:,k) ./ bb(k) - Doffset(k);
end



function NUM= BrukerPar(loc, file, parameter)

filename=fullfile(loc, file);
fid = fopen(filename); 
while 1 
     tline = fgetl(fid);
     
     if ~ischar(tline), break, end
     
     if ~isempty (strfind(tline, parameter))     
    
         if (isempty (strfind(tline, '(')))
             ind1=strfind(tline, '=');
             NUM=tline (ind1+1:end);
         else 
             ind1=strfind(tline, '('); 
             ind2=strfind(tline, ')'); 
             num_element=str2num(tline(ind1+1: ind2-1));
             tline = fgetl(fid);
             NUM=tline;

                             while length (str2num(NUM))<num_element
%                                  disp('loop2')
                                        tline = fgetl(fid);
                                        NUM=[NUM tline];
                             end
            end
          
   break,   end

end 
% disp('NUM:');disp(NUM);if (length(NUM)==0) NUM = '1'; end
NUM=str2num(NUM);
fclose(fid);    

function NUM= BrukerParStr(loc, file, parameter)

filename=fullfile(loc, file);
fid = fopen(filename); 
while 1 
     tline = fgetl(fid);
     
     if ~ischar(tline), break, end
     
     if ~isempty (strfind(tline, parameter))     
    
         if (isempty (strfind(tline, '(')))
             ind1=strfind(tline, '=');
             NUM=tline (ind1+1:end);
         else 
             ind1=strfind(tline, '('); 
             ind2=strfind(tline, ')'); 
             num_element=str2num(tline(ind1+1: ind2-1));
             tline = fgetl(fid);
             NUM=tline;

                             while length (str2num(NUM))<num_element
%                                  disp('loop2')
                                        tline = fgetl(fid);
                                        NUM=[NUM tline];
                             end
            end
          
   break,   end

end 


fclose(fid);    

function image = read_2dseq(fname, ds, Bit )

filename=[ fname '/pdata/1/2dseq'];
recon=fopen(filename,'r+','l');
 if strcmp(Bit, '16')
       image_1D= fread(recon, prod(ds), 'int16');;   % RECO_image_type=COMPLEX_IMAGE
      else
         image_1D= fread(recon, prod(ds), 'int32');  % RECO_image_type=COMPLEX_IMAGE
 end
      
fclose(recon);
image = reshape(image_1D, ds);

   
                        
 % read a 2dseq file and display in matlab
