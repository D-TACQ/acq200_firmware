# FFT Analysis and figure of merit calculator
# Copyright D-TACQ Solutions 2004
# Update 5th August for ploting in new octave
# Written by John McLean
function antiphanalysis( nsamples, frequency, filename )
#nsamples=4096;
#frequency=400;
binsize=0.0;
resfile="fftout";
shortfile="shortfftout";
datafile="fftdat";
num_of_harmonics = 10;
fund_leakage=30;
leakage=6;
dc_leakage = 30;
is_spur = 1;
is_fundamental = 2;
is_harmonic = 3;
is_dc = 4;
is_noise = 5;
fund_amp = 0;
fund_amp = 0;
total_harm_amp = 0;
total_noise = 0;
largest_spur_val = 0;
largest_spur_pos = 0;
largest_spur_amp = 0;
number_of_noise_bins = 0;
zerodb_ref = nsamples/3.6248;
t=1:nsamples;
shortnumber=1000;

#printf("\n%s\n\n",filename);
# Open the file and read in the data
myfile=fopen(filename, "r" );
z=fread( myfile, Inf,"short" );
fclose( myfile );
# scale z to the a sinwave of amplitude 1
z = z/32768;

binsize = frequency/nsamples;
nyquist = frequency/2;
for x=1:nsamples/2
binfrequency(x) = binsize * x;
endfor

for x=1:shortnumber
shortbin(x) = binfrequency(x);
endfor

shortfreq = shortnumber * binsize;


# Perform the window function
for x=1:nsamples
cut(x)=z(x);
endfor
#gplot cut;
#w=hanning(nsamples);
#w=hamming(nsamples);
w=blackman(nsamples);
for x=1:nsamples
	p(x)=cut(x)*w(x);
endfor
#w=hanning(nsamples);
#w=hamming(nsamples);
w=blackman(nsamples);
for x=1:nsamples
	p(x)=p(x)*w(x);
endfor



# Perform the FFT
fout=fft(p);
#fout=fft(cut);
#foutr = real(fout);
#Rather than just take the real part lets get a magnitude
foutr = abs(fout);
# Cut off the negative frequency elements
for x=1:nsamples/2
amplitude(x)=foutr(x);
endfor
# calculate the amplitude spectrum and reference it to the peak
logamplitude=20*real(log10(amplitude/zerodb_ref));
# mark the DC and end bins as 0 for the amplitude calculation
logamplitudeb = logamplitude;
for x=1:dc_leakage
    logamplitudeb(x) = -140;
    logamplitudeb(nsamples/2-(x-1)) = -140;
endfor

for x=1:shortnumber
shortlog(x) = logamplitude(x);
endfor

# save off the fft output to the data file.
datafile = strcat(filename,datafile);
myfile=fopen( datafile, "w" );
nyquist=fwrite( myfile, logamplitude, "float" );
fclose( myfile );

nyquist = frequency/2;

# fill the bins with initial signatures
for x=1:nsamples/2
bin_type(x) = is_noise;
endfor

# find the fundamental
[peak,fundamental]=max(logamplitudeb);
printf("fundamental at %d %3.9f\n",fundamental,peak);

for x=fundamental-fund_leakage:fundamental+fund_leakage
   fund_amp = fund_amp + (amplitude(x) * amplitude(x)) ;
  #printf("amplitude is = %f \n",logamplitudeb(x));
   bin_type(x) = is_fundamental;
endfor
fund_amp = sqrt(fund_amp);
#logfund_amp = 20*real(log10(fund_amp/1130));
logfund_amp = 20*real(log10(fund_amp/zerodb_ref));
fund_frequency = fundamental * binsize;
printf("Fundamental is closest to bin %d of frequency %f and amplitude %fdB %f \n",fundamental,fund_frequency,logfund_amp,fund_amp);


#Come up with and estimate for where the fundamental actually is buy
#looking at the amplitudes in the adjacent bins
#this is an empirical estimate not based on any math
#this is good for up to 10 harmonics
interbin = 0;
if amplitude(fundamental+1) > amplitude (fundamental-1)
     interbin = amplitude(fundamental+1)-amplitude(fundamental-1);
#     printf("%f \n",interbin);
     interbin = interbin/(amplitude(fundamental+1)+amplitude(fundamental));
else
     interbin = amplitude(fundamental-1)-amplitude(fundamental+1);
     interbin = interbin/(amplitude(fundamental-1)+amplitude(fundamental));
endif
     
printf("Fundamental is %f bins away from bin centre \n",interbin);


# Find the nyquist - fundamental antiphase signal
antispur_amp = 0;
printf("Antiphase Spur at bin %d \n",(nsamples/2-fundamental));
for i=((nsamples/2-fundamental)-leakage):((nsamples/2-fundamental)+leakage)
	antispur_amp = antispur_amp + (amplitude(i) * amplitude(i));
endfor
antispur_amp = sqrt(antispur_amp);

# Find the harmonics
# Now with the interbin adjustment
# Each harmonic amplitude is defined as the RMS off the bins around the nominal
# bin in which each harmonic is calculated at being.
# The total of all the harmonics is the RMS sum of each of these harmonics.
for x=1:num_of_harmonics
   harmonic_bins(x) = fix((fundamental-1+interbin) * (x+1));
      reflections = fix(harmonic_bins(x)/(nsamples/2));
   harmonic_bins(x) = harmonic_bins(x) - reflections * (nsamples/2);
   if rem(reflections,2) == 1
      harmonic_bins(x) = (nsamples/2) - harmonic_bins(x);
#      printf("%d \n",reflections);
   endif
#   printf("Harmonic at %d \n",harmonic_bins(x));
   harmonic_amp(x) = 0;
   if ((harmonic_bins(x) < (nsamples/2-dc_leakage)) & (harmonic_bins(x) > dc_leakage)) 
       for i=(harmonic_bins(x)-leakage):(harmonic_bins(x)+leakage)
         bin_type(i) = is_harmonic;
         harmonic_amp(x) = harmonic_amp(x) + (amplitude(i)* amplitude(i));
       endfor
       harmonic_amp(x) = sqrt(harmonic_amp(x));
#      printf("%d, %f \n",harmonic_bins(x),harmonic_amp(x));
  endif
  total_harm_amp =  total_harm_amp + harmonic_amp(x)*harmonic_amp(x);
  endfor
  total_harm_amp =  sqrt(total_harm_amp);
printf("Total Harmonic amplitude = %f \n",total_harm_amp);

# Next mark the DC bins at 0 and nyquist to avoid DC componnents
for x=1:10
    bin_type(x) = is_dc;
    bin_type(nsamples/2-(x-1)) = is_dc;
endfor

# SFDR is defined as the ratio of the fundamental to the next non-harmonic bin
# This routine goes through the spectrum adding all of the noise for the RMS sum
# and tagging the largest signal for the SFDR calculation.
for x=1:nsamples/2
if bin_type(x) == is_noise
   number_of_noise_bins = number_of_noise_bins +1;
   total_noise = total_noise + (amplitude(x) * amplitude(x));
   if amplitude(x) > largest_spur_val
      largest_spur_val = amplitude(x);
      largest_spur_pos = x;
   #printf("big spur found at %f \n",x);
   endif
endif
endfor
# Remember the original total noise for later
orig_total_noise = sqrt(total_noise);

#since we have only marked some of the bins as noise assume
# that the noise enery in the other bins would be the same
# therefore multiply by the number of bins nit included.
total_noise = total_noise * ((nsamples/2)/number_of_noise_bins);
total_noise = sqrt(total_noise);
printf("Total Noise is %0.8f \n",total_noise);
printf("Noise Calculated using %d bins \n",number_of_noise_bins);

# Based on the detected largest spur calculate it's amplitude using the
# same RMS calculation as the other signals 
for x=largest_spur_pos-leakage:largest_spur_pos+leakage
   largest_spur_amp = largest_spur_amp + (amplitude(x) * amplitude(x));
endfor
largest_spur_amp = sqrt(largest_spur_amp);
# Check to see if the largest spur is in the skirt of the fundamental
# Generate a warning if found too close to the fundamental
if abs(largest_spur_pos - fundamental) < (fund_leakage+5)
printf("WARNING !!! Window function is leaking past the compensation limit \n");
endif
printf("Largest Spur at bin %d Amplitude is %f \n",largest_spur_pos,largest_spur_amp);

#Calculate total value of all components - useful when testing 0 input quality
total_energy = sqrt(orig_total_noise^2 + total_harm_amp^2 + fund_amp^2);
printf("\nTotal energy = %f \n\n",total_energy);

# Calculate THD
THD = 20 * log10((total_harm_amp)/fund_amp);
#printf("THD = %f dB \n",THD);
# Calculate SFDR
SFDR = 20 * log10((fund_amp/largest_spur_amp));
#printf("SFDR = %f dBFS \n",SFDR);
# Calculate SNR
SNR = 20 * log10((fund_amp/total_noise));
#printf("SNR = %f dB \n",SNR);
# Calculate the SINAD
SINAD = 20 * log10((fund_amp/(sqrt(total_noise*total_noise +total_harm_amp*total_harm_amp))));
# AntiPhase Component
ANTI = 20 * log10(fund_amp/antispur_amp);
printf("SNR = %3.2fdB SINAD = %3.2fdB THD = %3.2fdB SFDR = %3.2fdBFS ANTI = %3.2fdBFS \n",SNR,SINAD,THD,SFDR,ANTI);

#now draw the graph
titletext1 = sprintf("%s, Fundamental at %.3fkHz, Amplitude %.3fdB, using a %d point FFT \n",filename,fund_frequency,logfund_amp,nsamples);
titletext2 = sprintf("SNR=%3.2fdB SINAD=%3.2fdB THD=%3.2fdB SFDR=%3.2fdB ANTI = %3.2fdBFS\n",SNR,SINAD,THD,SFDR,ANTI);
titletext3 = sprintf("Board Test Data, Not To Be Used Without Permission, Copyright D-TACQ Solutions 2004");
thetitle = strcat(titletext1,titletext2,titletext3);
#printf("%s \n",thetitle);
title(thetitle);
grid("on");
gset timestamp;
gset term postscript color;
resfile = strcat(filename,resfile);
printf ("%s\n",resfile);
outfilecommand = sprintf("gset output \"%s\"",resfile);
eval(outfilecommand);
axis ([1,nyquist,-160,0]);
ylabel ("Amplitude (dB)");
xlabel ("Frequency (kHz)");
plot (binfrequency ,logamplitude);
closeplot;
resfile = strcat(filename,shortfile);
printf ("%s\n",resfile);
outfilecommand = sprintf("gset output \"%s\"",resfile);
eval(outfilecommand);
axis ([1,shortfreq,-140,0]);
plot (shortbin ,shortlog);
closeplot;
#gset output 
endfunction
