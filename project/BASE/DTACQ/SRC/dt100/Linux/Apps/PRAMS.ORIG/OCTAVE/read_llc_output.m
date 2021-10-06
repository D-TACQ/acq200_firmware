function yy = read_llc_output(file, channels, samples, shots)
	fd = fopen(file, "r");
	xx = fread(fd, Inf, "short");
	fclose(fd);
	yy = reshape(xx, channels, samples, shots);
endfunction
