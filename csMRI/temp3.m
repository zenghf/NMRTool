path0 = '/Users/hzeng/Data/hzeng.sJ1/';expno = 8;procno = 1;

filename=fullfile(path0, num2str(expno), 'pdata', num2str(procno), '2dseq');
d3proc_file = fullfile(path0, num2str(expno), 'pdata', num2str(procno), 'd3proc');
d3proc = readnmrpar(d3proc_file);

datatype = 'int32';
% d3proc.DATATYPE = 'ip_int', 'ip_short'
if (strcmp(d3proc.DATTYPE, 'ip_short'))
    datatype = 'int16';
end
im_dim = [d3proc.IM_SIX, d3proc.IM_SIY, d3proc.IM_SIZ];
f=fopen(filename,'r','l');
data = fread(f, inf, datatype);
fclose(f);
return;
data = reshape(data, im_dim);
view3dgui(permute(data, [2 1 3]));