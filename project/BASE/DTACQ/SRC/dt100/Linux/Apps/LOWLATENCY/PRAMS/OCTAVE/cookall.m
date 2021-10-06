# cookall - process a series of shots, extract anti


function [antiset] = cookall(
	fileroot, freqs, qdacs, shotlen, nmean, calvin)
# fileroot: eg './TEST/qdac161_300.bin-140-4-4096.3.%dM
# freqs; eg [60,65,70,75,80]
# qdacs: eg [161:300]
# N : runs to average over
	if (nargin < 6)
		usage "[antiset] = cookall(fileroot, freqs, qdacs, \
shotlen, nmean, calvin)\n";
		return;
	endif


	t1 = time;
	
	shots = rows(vec(qdacs));
	runs = rows(vec(freqs));
	antiset = 1:2 * runs * shots; 	antiset = reshape(antiset, 2, runs, shots);

	printf("Kickoff runs:%d N:%d shots:%d\n",
		runs, nmean, shots);


	for fix = 1:runs
		freq = freqs(fix)
		_freqs=1:shots;_freqs *= 0; _freqs += 1; _freqs *= freq * 1000000;
		fname = sprintf(fileroot, freq);
		fname = strcat(fname, ".%d");


		printf("Processing %s %d Hz\n", fname, _freqs(1));

		out = interleave_mean_fft(fname,shots,shotlen, 4,calvin, nmean);
		
		for ch = 1:2
			antiset(ch, fix, :) = anti_report_fft(out, _freqs, qdacs, shots, ch, 1);		
		endfor
	endfor

	printf("All done. Elapsed time %d\n", time-t1);

endfunction
