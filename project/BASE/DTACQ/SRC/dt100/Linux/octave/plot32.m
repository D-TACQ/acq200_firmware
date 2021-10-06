# read and plot 32 channels

fd = fopen( "data", "r" );
[chX, count] = fread( fd, [32,128], "short" )
fclose( fd );
chXX = fliplr( rot90( chX, -1 ))
t = (1:1:128)';
yy = [t,chXX(:,1),chXX(:,2),chXX(:,3),chXX(:,4),\
      chXX(:,5),chXX(:,6),chXX(:,7),chXX(:,8)]
gplot [1:5] \
    yy using 1:2, \
    yy using 1:3, \
    yy using 1:4, \
    yy using 1:5, \
    yy using 1:6, \
    yy using 1:7, \
    yy using 1:8


