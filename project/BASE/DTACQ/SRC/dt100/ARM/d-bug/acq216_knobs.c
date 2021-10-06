/*
 * acq216_knobs.c : multi-call binary
 *
 * set.route <diX> in <source> out [dest] [dest]
 * get.route <diX>
 *
 * set.vin <Vrange> <channel-mask>
 * get.vin <Vrange>
 * 
 * set.event <eventX> <sense>
 * get.event <eventX>
 */


#include "local.h"

#define ROOT "/acq200.core/"


int verbose = 1;

typedef int (*action)(int argc, char **argv);

struct LUT {
	char *key;
	action action;
};

static int set_route(int argc, char **argv)
{
#define BAD "bad arg %d:%s wanted %s\n"
	int iarg = 0;
	char *diX = 0;
	char *in;
	char *out;
	int input = 0;
	int outputs = 0;
	char fname[80];
	FILE *fp;

	while (++iarg != argc){
		switch(iarg){
		case 1:
			diX = argv[iarg];
			if (strlen(diX) == 2 &&
			    (diX[0] == 'd' || diX[0] == 'D') &&
			    (diX[1] >= '0' || diX[1] <= '5')    ){
				diX[0] = 'd';
				break;
			}else{
				err(BAD, iarg, diX, "dX" );
				return -1;
			}
		case 2:
			if (strcmp(argv[iarg], "in") == 0){
				break;
			}else{
				err(BAD, iarg, argv[iarg], "in" );
				return -2;
			}
		case 3:
			in = argv[iarg];
			if (strcmp(in, "fpga") == 0){
				input = 0;
			}else if (strcmp(in, "mezz") == 0){
				input = 1;
			}else if (strcmp(in, "rio") == 0){
				input = 2;
			}else if (strcmp(in, "pxi") == 0){
				input = 3;
			}else{
				err(BAD, iarg, argv[iarg], "mezz|fpga|pxi");
				return -3;
			}
			break;
		case 4:
			if (strcmp(argv[iarg], "out") == 0){
				break;
			}else{
				printf( BAD, iarg, argv[iarg], "out" );
				return -4;
			}
		default:
			out = argv[iarg];
			if (strcmp(out, "fpga") == 0){
				if (input != 0){
					outputs |= 1;
				}
			}else if (strcmp(out, "mezz") == 0){
				if (input != 1){
					outputs |= 2;
				}
			}else if (strcmp(out, "rio") == 0){
				if (input != 2){
					outputs |= 4;
				}
			}else if (strcmp(out, "pxi") ==0){
				if (input |= 3){
					outputs |= 8;
				}
			}else{
				err( BAD, iarg, argv[iarg], 
					"fpga|mezz|rio|pxi");
				return -5;
			}
		}
	}

	if (diX){
		sprintf(fname, "%s/dio_route_%s", ROOT, diX );
		fp = fopen(fname, "w");
		fprintf(fp, "%x%x\n", input, outputs);
		if (verbose){
			printf( "%x%x > %s\n", input, outputs, fname);
		}
		fclose(fp);
		return 0;
	}else{
		return -6;
	}
}

static int get_route(int argc, char **argv)
{
	printf( "get_route\n" );
	return 1;
}

static struct LUT lut[] = {
	{ "set.route", set_route },
	{ "get.route", get_route },
};
#define NLUT (sizeof(lut)/sizeof(struct LUT))

int main(int argc, char** argv)
{
	int ilut;

	for (ilut = 0; ilut != NLUT; ++ilut){
		if (strcmp(lut[ilut].key, argv[0]) == 0){
			return lut[ilut].action(argc, argv);
		}
	}

	printf( "ERROR: command not found \"%s\"\n", argv[0] );
	return 1;
}

