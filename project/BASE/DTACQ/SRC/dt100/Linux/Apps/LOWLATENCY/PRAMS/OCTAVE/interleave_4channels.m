# function processes llcontrol shotfile
# usage
# this is a script file

1;

ffta;

printf("interleave_4channels.m $Revision: 1.7 $ 1007\n");

function [v4, v2] = \
  interleave(file, shots, samples, channels, calvin)

# volts: all channels, regular
# volts2: interleaved, double length
# file: data file eg /tmp/dds.ftw-10-4-2048.3.userbuf
# shots: #shots eg 10
# samples: # samples eg 2048
# channels: #channels eg 4
# calvin: calibrated voltage input - see load_calvin below.

	if (nargin != 5)
		usage("[v4,v2] = interleave(file, shots, samples, channels, calvin)");
	endif
        if (channels != 4)
	   usage("channels must be 4 (2+2 bonded)");
        endif

	settings = rows(calvin);

	yy = read_llc_output(file, channels, samples, shots);
# yy [ch][samples][shots]

# cooked data: 2channels, all shots, double # samples

	printf("shots %d samples %d\n", shots, samples);

	volts = reshape(1:channels*shots*samples, shots, channels, samples);
	volts2 = reshape(volts, shots, 2, samples*2);
	
	for ch = 1:4
		# octave seems to need this printf to eval correctly
		printf("ch %d V1 %f V2 %f\n",
			ch, calvin(1,1,ch), calvin(1,1,ch));
	endfor

	for shot = 1:shots	
		rawx = yy(1:4,:,shot);
		calix = mod(shot, settings) + 1;

		for ch = 1:4
			V1 = calvin(calix,1,ch);
			V2 = calvin(calix,2,ch);
			gain = (V2 - V1)/65535;
			v4(shot,ch,:) = V1 + (rawx(ch,:) + 32768) * gain;
		endfor

		v2(shot,1,:) = vec(v4(shot,1:2,:));
		v2(shot,2,:) = vec(v4(shot,3:4,:));
	endfor;
endfunction

function calvin = load_calvin(calvin_file)
# load predefined octave file with cal consts
# input [shots][channels*2]
# output calvin[shots][2][channels]

	load(calvin_file);
	calvin = reshape(calvin, rows(calvin), 2, columns(calvin)/2);
endfunction

function save_interleave(file, v2)
# save v2 data to file name beginning base
	for shot = 1:rows(v2)
		for ch = 1:2
			fname = sprintf("%s.shot%02d.ch%02d.fbin", file, shot, ch);
			fp = fopen(fname, "w");
			fwrite(fp, v2(shot,ch,:), "float");
			fclose(fp);
		endfor
	endfor
endfunction


function [anti, freq, db] = anti_report_fft(fout, freqs, qdacs, shots, ch, stride )
	if (nargin < 5)
		usage( "fout[shots,channels,samples], freqs[shots], qdacs[shots], shots, ch [, stride]");		
	endif
	if (nargin < 6)
		stride = 1;
	endif

	anti = 1:shots; anti *= 0;

	for s = 1:shots
		shot = (s-1) * stride + 1;
		printf("SHOT %2d QDAC:%d ", shot, qdacs(shot));
		[freq, db, anti(s)] = analyse_fft(vec(fout(shot,ch,:)), freqs(shot), sprintf("shot.%02d.", shot),0,0);
	endfor
endfunction


function [anti] = anti_report(v2, freqs, qdacs, shots, ch, stride )
	if (nargin < 5)
		usage( "v2[shots,channels,samples], freqs[shots], qdacs[shots], shots, ch [, stride]");		
	endif
	if (nargin < 6)
		stride = 1;
	endif

	anti = 1:shots*stride; anti *= 0;
	whos anti

	for s = 1:shots
		shot = s * stride;
		printf("QDAC:%d ", qdacs(shot));
		[freq, db, anti(shot)] = fftanalysis_local(vec(v2(shot,ch,:)), freqs(shot), sprintf("shot.%02d.", shot),0,0);
	endfor
endfunction



function [fout] = interleave_mean_fft(filebase, shots, samples, channels, calvin, N)
	if (nargin < 6)
		usage("interleave_mean_fft(filebase, shots, samples, channels, calvin, N)\n");
	endif

	pairs = channels/2;
	psamples = samples*2;

	fout_all = 1:shots*psamples*pairs*N;
	fout_all = reshape(fout_all, shots, pairs, N, psamples);

	for mset = 1:N
		file = sprintf(filebase, mset)
		[v4, v2] = interleave(file, shots, samples, channels, calvin);
		for shot = 1:shots
			for pair = 1:pairs
				fout_all(shot,pair,mset,:) = w_fft(v2(shot, pair, :));
			endfor
		endfor
	endfor	

	fout = 1:shots*psamples*pairs;
	fout = reshape(fout, shots, pairs, psamples);

	tmp1 = vec(fout(1,1,:)); tmp1 *= 0;

	for shot = 1:shots
		for pair = 1: pairs
			tmp = tmp1;
			for mset = 1:N
				tmp += (vec(fout_all(shot, pair, mset, :)));
			endfor
			fout(shot,pair,:) = tmp;
		endfor
	endfor

	fout /= N;
endfunction



printf "functions load_calvin(), interleave(), anti_report() defined\n";
more off;

# load_calvin('/tmp/vin.octave');
# [v1,v2] = interleave(file, shots, samples, channels, calvin);

# v1: N channels, regular data (
# v2; [channels==4] : 2 channels, interleaved data
# plot(vec(v1(1,1:));



