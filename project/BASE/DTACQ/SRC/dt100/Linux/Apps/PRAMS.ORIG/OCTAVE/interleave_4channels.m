# function processes llcontrol shotfile
# usage
# this is a script file

1;

function [volts, volts2] = \
  interleave(file, shots, samples, channels, calvin)

	if (nargin != 5)
		usage("interleave(yy, shots, samples, channels, calvin)");
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
	
	for shot = 1:shots	
		rawx = yy(1:4,:,shot);
		calix = mod(shot, settings) + 1

		for ch = 1:4
			V1 = calvin(calix,1,ch);
			V2 = calvin(calix,2,ch);
			volts(shot,ch,:) = V1 + (rawx(ch,:) + 32768)*(V2 - V1)/65535;
		endfor

		volts2(shot,1,:) = vec(volts(shot,1:2,:));
		volts2(shot,2,:) = vec(volts(shot,3:4,:));
	endfor;
endfunction

function calvin = load_calvin(calvin_file)
# load predefined octave file with cal consts
# input [shots][channels*2]
# output calvin[shots][2][channels]

	load(calvin_file);
	calvin = reshape(calvin, rows(calvin), 2, columns(calvin)/2);
endfunction

printf "functions load_calvin(), interleave(), defined\n";


# yy = read_llc_output ("/tmp/dds.ftw-100-2048.2.userbuf", 4, 2048, 100);
# load_calx1vin vin.octave
# cooked = interleave(file, shots, samples, channels, calvin);

