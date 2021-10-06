# create sine and quadrature signals
t=1:1000;
z=[sin(2*pi*t/1000);sin(2*pi*t/1000+pi/2)];
z=z+1;
z=z*32767;

myfile=fopen( "sin1000q.dat", "w" );
fwrite( myfile, z, "ushort" );
fclose( myfile );


