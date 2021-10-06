# Deate Merging for Anti-phase testing
# Copyright D-TACQ Solutions 2005
# Written by John McLean
function convvolts( nsamples, filename1  )
#nsamples=4096;
#frequency=400;

#printf("\n%s\n\n",filename);
# Open the file and read in the data
myfile=fopen(filename1, "r" );
#z1=fread( myfile, Inf,"short" );
z1=fread( myfile, Inf,"float" );
fclose( myfile );


for x=1:nsamples
z1(x) = round(z1(x)/3.1 * 32700);
endfor;

# save off the merged output to the data file.
datafile = strcat(filename1,"conv");
myfile=fopen( datafile, "w" );
for x=1:nsamples
out=fwrite( myfile, z1(x), "short" );
endfor
fclose( myfile );



endfunction
