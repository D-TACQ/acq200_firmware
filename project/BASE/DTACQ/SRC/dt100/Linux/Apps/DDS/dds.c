/* ------------------------------------------------------------------------- */
/* dds.c compute dds FTW                                                     */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2003 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License
    as published by the Free Software Foundation;
                                                                               
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                              
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */


#define VERID "$Revision: 1.3 $ B1010"
#define USAGE "dds <fout> [ext-clk] [ext-clk-mult]"


#include "local.h"
#include "popt.h"

#include <assert.h>
#include <errno.h>

#undef SIGN
#include "tommath.h"

int acq200_debug = 0;



static void usage(void)
{
	printf("%s\n%s\n", VERID, USAGE);
	exit(1);
}


static int mp_sprint(mp_int *a, int radix, char* ubuf, int field_len)
{
   char *buf;
   int err, len, x;
   char *ubp = ubuf;
   
   if ((err = mp_radix_size(a, radix, &len)) != MP_OKAY) {
      return err;
   }

   buf = OPT_CAST(char) XMALLOC (len);
   if (buf == NULL) {
      return MP_MEM;
   }
   
   if ((err = mp_toradix(a, buf, radix)) != MP_OKAY) {
      XFREE (buf);
      return err;
   }
   
   for (x = len; x <= field_len; ++x){
	   *ubp++ = '0';
   }
   for (x = 0; x < len; x++) {
	   *ubp++ = buf[x];
   }
   
   XFREE (buf);
   return MP_OKAY;
}


static int calcFTW(int Fout, int ext_clk, int ref_clk, char* ubuf)
/** output FTW, return Fact */
{

	static int initialised;
	static mp_int divisor;
	static mp_int refclk;
	static mp_int remainder;
	static mp_int refclk2;
	static mp_int ONE;
	static mp_int TWO;
	static mp_int fout;
	static mp_int ftw;
	
	if (!initialised){
		mp_int TWO;

		mp_init(&divisor);

		mp_init(&refclk);
		mp_init(&refclk2);
		mp_init(&remainder);
		mp_init(&ONE);
		mp_init(&TWO);
		mp_init(&fout);
		mp_init(&ftw);

		mp_set_int(&ONE, 1);
		mp_set_int(&TWO, 2);
		mp_2expt(&divisor, 48);
		initialised = 1;
	}

	mp_set_int(&fout, Fout);
	mp_set_int(&refclk, ext_clk*ref_clk);
	mp_div(&refclk, &TWO, &refclk2, 0);
  
	mp_mul(&divisor, &fout, &ftw);
	mp_div(&ftw, &refclk, &ftw, &remainder);
	if (mp_cmp(&remainder, &refclk2) > 0){
		mp_add(&ftw, &ONE, &ftw);
	}

	mp_sprint(&ftw, 16, ubuf, 12);
//	strcat(ubuf, "\n");

	return Fout;   /* @@todo */
}


static void output(char* ubuf, char* fname)
{
	if (strcmp(fname, "-") == 0){
		puts(ubuf);
	}else{
		FILE* FOUT = fopen(fname, "w");
		if (!FOUT){
			printf("ERROR:failed to open %s\n", fname);
			exit(-errno);
		}
		fputs(ubuf, FOUT);
		fclose(FOUT);

		if (acq200_debug){
			fputs(ubuf, stderr);
			fputs("\n", stderr);
		}
	}
}

int main(int argc, const char* argv[])
{
	static char *fout = "/dev/rtmdds/ftw1";
	static char *fin = 0;
	static int ext_clk = 66000000;
	static int refclk_mult = 4;
	static struct poptOption opt_table[] = {
		{ "extclk", 'e', POPT_ARG_INT, &ext_clk, 0 },
		{ "refclk_mult", 'm', POPT_ARG_INT, &refclk_mult, 'r'},
		{ "debug",  'd', POPT_ARG_INT, &acq200_debug, 0 },
		{ "output", 'o', POPT_ARG_STRING, &fout, 'o' },
                { "input", 'i', POPT_ARG_STRING,    &fin,  'i' },
                { "usage", 'u', POPT_ARG_NONE, 0, 'u' },
                POPT_AUTOHELP
                POPT_TABLEEND
        };
	int rc;
	poptContext opt_context =
                poptGetContext(argv[0], argc, argv, opt_table, 0);
	FILE* FIN = 0;

	char ubuf[20];

        while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		case 'r':
			if (refclk_mult < 4) refclk_mult = 4;
			if (refclk_mult >20) refclk_mult = 20;
			break;
		case 'u':
			usage();
		}
	}
	
	if (poptPeekArg(opt_context) == 0){
		FIN = stdin;
	}else if (fin){
		if (strcmp(fin, "-") == 0){
			FIN = stdin;
		}else{
			FIN = fopen(fin, "r");
			if (!FIN){
				printf("ERROR:failed to open %s\n", fin);
				exit(-errno);
			}
		}
	}
	


	if (FIN){
		char buf[80];
		while (fgets(buf, 80, FIN)){
			int Fout = atoi(buf);
			int Fact = calcFTW(Fout, ext_clk, refclk_mult, ubuf);

			output(ubuf, fout);			
		}
	}else{
		const char* pram;
		int iarg = 0;
		int Fout = 0;

		while((pram = poptGetArg(opt_context)) != NULL){
			switch(++iarg){
			case 1:
				Fout = atoi(pram);
				break;
			case 2:
				ext_clk = atoi(pram);
				break;
			case 3:
				refclk_mult = atoi(pram);
				break;
			default:
				fprintf(stderr, "ERROR: %s\n", USAGE);
				exit(-1);
			}
		}

		assert(iarg != 0);

		int Fact = calcFTW(Fout, ext_clk, refclk_mult, ubuf);
		output(ubuf, fout);
	}

	return 0;
}
