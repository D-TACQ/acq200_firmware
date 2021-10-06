
function data = fetch8x( first, channel, len, offset, stride )
        if ( nargin < 5 )
                stride = 1;
                if ( nargin < 4 )
                        offset = 0;
                        if ( nargin < 3 )
                                len = 20000
                                if ( nargin < 2 )
                                        channel = 1
					if ( nargin < 1 )
						first = 1;
					endif
                                endif
                        endif
                endif
        endif

	data=[  fetch(first+0),fetch(first+1),\
		fetch(first+2),fetch(first+3),\
		fetch(first+4),fetch(first+5),\
		fetch(first+6),fetch(first+7)];
endfunction
