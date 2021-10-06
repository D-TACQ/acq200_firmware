
function chx = fetchbs(path, c1, c2, len)
	 fd = fopen(path, "r");
	 chx = zeros(len, c2-c1);
	 i = 0;while (i++ < c1-1)
	     fread(fd, len, "short"); # discard
	 endwhile
	 for i = c1:c2
	     ch = fread(fd, len, "short");
	     chx(:, i) = ch;
	 endfor
	 fclose(fd);
endfunction
