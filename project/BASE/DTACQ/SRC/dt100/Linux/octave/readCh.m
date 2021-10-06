function ch = readCh ( device )
     if (nargin < 1)
         usage ("readCh ( device )");
     endif

     fd = fopen( device, "r" );
     ch = fread( fd, 2048, "short" );
     fclose( fd );
endfunction

