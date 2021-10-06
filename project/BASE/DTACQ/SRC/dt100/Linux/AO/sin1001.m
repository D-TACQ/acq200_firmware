# create sine and quadrature signals
t=1:1000;
z=[sin(2*pi*t/1000);0.5*sin(2*pi*t/1000)+0.3*sin(3*2*pi*t/1000)+0.2*sin(5*2*pi*t/1000)];
z=z+1;
z=z*32767;

myfile=fopen( "sin1001.dat", "w" );
fwrite( myfile, z, "ushort" );
fclose( myfile );


