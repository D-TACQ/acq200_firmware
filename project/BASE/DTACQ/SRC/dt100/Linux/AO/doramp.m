# DO ramp
t=1:256;
z=t*512+t;

myfile=fopen( "doramp.dat", "w" );
fwrite( myfile, z, "ulong" );
fclose( myfile );


