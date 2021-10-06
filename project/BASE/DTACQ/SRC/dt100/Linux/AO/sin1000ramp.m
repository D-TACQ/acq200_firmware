
# create sine and quadrature signals
t=1:20000;
z=[sin(2*pi*t/1000);rem(t,1000)/1000];
z=z+1;
z=z*32767;

myfile=fopen( "sin1000ramp.dat", "w" );
fwrite( myfile, z, "ushort" );
fclose( myfile );


