/** @file channelize.196.c takes a block of data and dechannelize to 
    logical channels.
 *
 * usage:
 * channelize.196 [nchan]
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SSZ (sizeof(short))

int fd_in;
FILE* fd_out;

#define INPUT fd_in
#define OUTPUT fd_out

int acq200_lookup_pchan(int lchannel)
/* lchannel {1:32} returns physical channel {1:32} */
{
	static	int plut[] = {
/* index on logical channel {1:32} , map to physical channel {1:32} */
		[ 1] =  1, 
		[ 2] = 17, 
		[ 3] =  2, 
		[ 4] = 18, 
		[ 5] =  9,	
		[ 6] = 25,
		[ 7] = 10,
		[ 8] = 26,

		[ 9] =  3,
		[10] = 19,
		[11] =  4,
		[12] = 20,
		[13] = 11,
		[14] = 27,
		[15] = 12,
		[16] = 28,

		[17] =  5,
		[18] = 21,
		[19] =  6,
		[20] = 22,
		[21] = 13,
		[22] = 29,
		[23] = 14,
		[24] = 30,

		[25] =  7,
		[26] = 23,
		[27] =  8,
		[28] = 24,
		[29] = 15,
		[30] = 31,
		[31] = 16,
		[32] = 32 
	};


	int block = ((lchannel-1)/32);
	int index = lchannel - block*32;

	return block*32 + plut[index];
}

int acq200_lookup_lchan(int pchan)
/* input pchan {1:32} returns logical channel {1:32} */
{
	static	int llut[] = {
/* index on physical channel {1:32} , map to logical channel {1:32}*/
		[ 1]		=  1, 
		[17]		=  2, 
		[ 2]		=  3, 
		[18]		=  4, 
		[ 9]		=  5, 
		[25]		=  6, 
		[10]		=  7, 
		[26]		=  8, 

		[ 3]		=  9, 
		[19]		= 10, 
		[ 4]		= 11, 
		[20]		= 12, 
		[11]		= 13, 
		[27]		= 14, 
		[12]		= 15, 
		[28]		= 16, 

		[ 5]		= 17, 
		[21]		= 18, 
		[ 6]		= 19, 
		[22]		= 20, 
		[13]		= 21, 
		[29]		= 22, 
		[14]		= 23, 
		[30]		= 24, 

		[ 7]		= 25, 
		[23]		= 26, 
		[ 8]		= 27, 
		[24]		= 28, 
		[15]		= 29, 
		[31]		= 30, 
		[16]		= 31, 
		[32]		= 32
	};

	int block = ((pchan-1)/32);
	int index = pchan - block*32;

	return block*32 + llut[index];
}


static void open_files(const char* fname_in)
{
	char fname_out[80];

	fd_in = open(fname_in, O_RDONLY);
	assert(fd_in != -1);

	sprintf(fname_out, "%s.ch", fname_in);

	fd_out = fopen(fname_out, "w");
}

static int load(short* input, int ndata)
{
	int rc;

	fprintf(stderr, "load call read(%d %p %lu)\n",
		INPUT, input, ndata*SSZ);


	rc = read(INPUT, input, ndata*SSZ);

	if (rc > 0){
		fprintf(stderr, "load returning %lu\n", rc/SSZ);
		return rc/SSZ;
	}else{
		perror( "ERROR on load");
		_exit(errno);
	}
}

static void channelize(short* output, short* input, int nchan, int nsamples)
{
	int isample;
	int ichan;

	fprintf(stderr, "channelize %d %d\n", nchan, nsamples);

	for (isample = 0; isample != nsamples; ++isample){
		for (ichan = 1; ichan <= nchan; ++ichan){
			output[(ichan-1)*nsamples + isample] = 
				input[acq200_lookup_lchan(ichan)-1];
		}
		input += nchan;
	}

	fprintf(stderr, "channelize %d %d done\n", nchan, nsamples);
}
static void store(short* output, int ndata)
{
	int rc = fwrite(output, SSZ, ndata, OUTPUT);

	assert(rc == ndata);
}

int main(int argc, char* argv[])
{
	int nchannels = 96;
	int nsamples = 1;
	short* input;
	short* output;
	int ncs_product;
	const char* raw_fn = 0;
	

	switch (argc){
	case 4:
		raw_fn = argv[3];
	case 3:
		nsamples = atoi(argv[2]);
	case 2:
		nchannels = atoi(argv[1]);
		break;
	default:
		fprintf(stderr,"usage channelize.196 <nchannels> <len> <raw>\n");
		_exit(-1);
	}	   

	open_files(raw_fn);
	
	ncs_product = nsamples * nchannels;

	input = malloc( ncs_product * SSZ );
	output = malloc( ncs_product * SSZ );

	assert(input);
	assert(output);
	
	ncs_product = load(input, nsamples*nchannels);
	nsamples = ncs_product / nchannels;

	channelize(output, input, nchannels, nsamples);
	store(output, nsamples*nchannels);

	return 0;
}
