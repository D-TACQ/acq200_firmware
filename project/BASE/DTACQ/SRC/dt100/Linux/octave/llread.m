
# read llcontrol data from lldump
# llcontrol command line:
# llcontrol -v 0 --histogram 1 -b 2 -d 5 -A -n 4000 -o /tmp/lldump Bii 10
# or similar

fd = fopen( "/tmp/lldump","r" );
chXX = fread( fd, [32,Inf], "short" );
chXX = chXX';
fclose( fd );
fd = fopen( "/tmp/lldump", "r" );
stats = fread( fd, [16,Inf], "int" );
stats = stats';
fclose( fd );

whos 

# plot some data

gplot chXX using 1 with lines, chXX using 2 with lines

# look at the stats

tinst = stats(:,14);
tlatch= stats(:,15);
tproc=  stats(:,16);

tsample= tlatch(3:1000)-tlatch(2:999);

whos

