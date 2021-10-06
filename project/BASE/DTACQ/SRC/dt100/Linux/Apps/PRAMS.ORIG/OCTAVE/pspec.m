function power = pspec(xx)
	nsam = columns(xx);
	nsam2 = nsam/2;
	ww=hanning(nsam);
	xw=xx;
	for ix = 1:nsam
		xw(ix) *= ww(ix);	
	endfor
	zz = real(fft(xw));
	power = 1:nsam2;
	for ix = 1:nsam2
#		power(ix) = 20*log(zz(ix) * zz(ix)) - 220;
		power(ix) = zz(ix) * zz(ix);
	endfor
endfunction
