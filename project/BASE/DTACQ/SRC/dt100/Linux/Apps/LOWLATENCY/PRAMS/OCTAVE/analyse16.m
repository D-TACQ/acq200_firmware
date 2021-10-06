# Block routine to call the FFT Analysis and figure of merit calculator
# Copyright D-TACQ Solutions 2004
# 5th May 2004
# Written by John McLean
function analyse16(boardname, start)
printf("Hello World \n");
for x=start:start+15
	filename = sprintf("%s.1.%02d",boardname,x);
	printf("%s\n",filename);
	fftanalysis(4096,400,filename);
endfor
