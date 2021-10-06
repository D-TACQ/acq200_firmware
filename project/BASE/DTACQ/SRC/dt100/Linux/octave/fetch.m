
function chx =  fetch ( channel, len, offset, stride )
	if ( nargin < 4 )
		stride = 1;
		if ( nargin < 3 )
			offset = 0;
			if ( nargin < 2 )
				len = 20000
				if ( nargin < 1 )
					channel = 1
				endif
			endif
		endif
	endif

	fmt = "acq32fetch -o - %d %d %d %d";
	command = sprintf( fmt, channel, len, offset, stride );
	fid = popen( command, "r" );
	chx = fread( fid, len, "short" );
	fclose( fid );
endfunction


