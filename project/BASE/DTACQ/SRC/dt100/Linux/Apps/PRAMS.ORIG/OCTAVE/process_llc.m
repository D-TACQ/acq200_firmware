# function processes llcontrol shotfile
# usage
# [psx, chx, chx_plot] = process_llc(
#			file,  		data file name  
#			shots, 		number of shots (iterations) in file
#			samples,	samples per shot
#			channels,       channels in set
#			offset,		offset of channel in data set
#
#	offset is determined from /dev/dtacq/channel_mapping
#	eg: acq216, 16 channels, channel 1 is offset 13 (index from 1)
#
# example:
# ./llcontrol  -M ffff -b 2 -t --hbpoll --bigbuf \
# -n 100 -c 4 --samples 2048 \
# --prams PRAMS/ftw.bin -O -W -o /tmp/dds.ftw-100-2048 SCM
#
# [psx, chx, chx_plot] = process_llc(
#		'/tmp/dds.ftw-100-2048.S.userbuf',       S = slot #
#		100,                                     -n 100
#               2048,                                    --samples 2048
#               4,                                       -c 4
#               1);,                                     channel 1 in c4
#		
# Using the output data:
#
# plot(1:2048,chx_plot(1:0,:));                 # pretty plot of ch1, all shots
# plot(1:1024,psx);                             # spectral plot
# save as .ps file:
# saveplot(psx,'psx','1:1024,psx','llcontrol-1m-1m1-2048-spectrum.ps');


function [psx, chx, chx_plot] = \
	process_llc(file, shots, samples, channels, offset)

	if (nargin != 5)
		usage("process_llc(file, shots, samples, channels, offset)");
	endif
	yy = read_llc_output(file, channels, samples, shots);

	chx = reshape(1:samples*shots,[shots,samples]);
	chx_plot = chx;
	for ix = 1:shots
		chx(ix,:) = yy(offset,:,ix);
		if (ix == 1)
			range = max(chx(ix,:)) - min(chx(ix,:));
		endif
		chx_plot(ix,:) = range*ix + chx(ix,:);
	endfor

	samples2 = samples/2;
	psx = reshape(1:samples2*shots,[shots,samples2]);

	for ix = 1:shots
		psx(ix,:) = pspec(chx(ix,:));
	endfor
  endfunction
