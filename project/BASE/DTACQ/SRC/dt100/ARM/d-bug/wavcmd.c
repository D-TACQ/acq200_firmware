/*****************************************************************************
 *
 * File: wavcmd.c
 *
 * $RCSfile: wavcmd.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 * wav232icd implementation
 * $Id: wavcmd.c,v 1.1.2.2 2004/09/02 15:17:21 pgm Exp $
 * $Log: wavcmd.c,v $
 * Revision 1.1.2.2  2004/09/02 15:17:21  pgm
 * works with cgi
 *
 * Revision 1.1.2.1  2004/02/28 10:59:22  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "local.h"

#include <errno.h>
#include <unistd.h>
#include <stdarg.h>

#include "popt.h"



static int help(void) {
	printf( "wavcmd $Revision: 1.1.2.2 $ - conforms to wav32icd v2\n" );
	return 0;
}




/*
 * returns
 * 0: not me
 * 1: success
 * -n: error
 */



#define VSEL_INT10 0x0
#define VSEL_INT5  0x1
#define VSEL_EXT2  0x2
#define VSEL_EXT1  0x3

int bstrtou(const char* str, unsigned* value)
/* left to right conversion to binary from string */
{
	unsigned umask = 0;
	unsigned ucursor;
	int ichar = 0;

	for (ucursor = 0x1; str[ichar] && ucursor; ucursor <<= 1){
		umask |= str[ichar] =='1'? ucursor: 0;
	}

	return 0;
}

static unsigned S_ref;

unsigned i2cReadRef(void)
{
#ifndef TEST
	FILE *rp = popen( "i2c-r.w 0x20", "r" );
#else
	FILE *rp = fopen( "i2c-r.w-0x20", "r" );
#endif
	assert(rp);
	assert(fscanf(rp, "%x", &S_ref) == 1);

	fclose(rp);
	return S_ref;

}

void i2cUpdateRef(void)
{
	char buf[80];

	sprintf(buf, "i2c-w.w 0x20 0x%04x", S_ref);

#ifndef TEST
	system(buf);
#else
	FILE *fp = fopen( "i2c-r.w-0x20", "w" );
	fprintf(fp, "%x", S_ref);
#endif
}

void i2cWriteRef(unsigned refsel)
{
	S_ref = refsel;
	i2cUpdateRef();
}

#define ALL_CHANNELS 'X'

int doSetRef(int channel, unsigned vsel)
{
	if (channel == ALL_CHANNELS){
		int ch;
		for (ch = 1; ch <= 32; ++ch){
			doSetRef(ch, vsel);
		}
	}else{
		unsigned igroup = (32-channel) / 4;
		unsigned lshift = igroup * 2;

		S_ref &= ~(0x3 << lshift);
		S_ref |= vsel << lshift;
	}
	return 0;
}


int parseSetRef(const char *arg)
{
	int channel = ALL_CHANNELS;
	char vsel[10];

	if (sscanf(arg, "%d=%s", &channel, vsel) == 2    ||
            sscanf(arg, "%d%%3D%s", &channel, vsel) == 2 ||
	    sscanf(arg, "XX=%s", vsel) == 1              || 
	    sscanf(arg, "XX%%3D%s", vsel) == 1               ){
		if  (channel == ALL_CHANNELS || 
		     (channel >= 1 && channel <= 32)){
			if (strcmp(vsel, "INT10") == 0 ||
			    strcmp(vsel, "INT")   == 0   ){
				return doSetRef(channel, VSEL_INT10);
			}else if (strcmp(vsel, "INT5") == 0){
				return doSetRef(channel, VSEL_INT5);
			}else if (strcmp(vsel, "EXT1") == 0){
				return doSetRef(channel, VSEL_EXT1);
			}else if (strcmp(vsel, "EXT2") == 0){
				return doSetRef(channel, VSEL_EXT2);
			}else{
				printf("PARSE ERROR vsel \"%s\"\n", vsel);
			}
		}else{
			printf("PARSE ERROR channel OOR %d\n", channel);
		}
	}else{
		printf("PARSE ERROR on \"%s\"\n", arg);
	}
	return -1;
}
int setRef(poptContext context)
{
	int use_command_line = 0;
	const char *next;

	i2cReadRef();

	while((next = poptGetArg(context)) != 0){
		parseSetRef(next);
		use_command_line = 1;
	}
	
	if (!use_command_line){
		char aline[80];
		while(fgets(aline, 80, stdin) != 0){
			parseSetRef(aline);
		}
	}

	i2cUpdateRef();

	return 0;
}

int getRef(poptContext context)
{
	unsigned group = i2cReadRef();
	unsigned gcursor = 0xc000;
	int igroup;
	int ichannel = 0;


	for (igroup = 8; --igroup >= 0; gcursor >>= 2){
		char *vsel = 0;

		switch((group&gcursor)>>igroup*2){
		case VSEL_INT10:
			vsel = "INT10"; break;
		case VSEL_INT5:
			vsel = "INT5"; break;
		case VSEL_EXT1:
			vsel = "EXT1"; break;
		case VSEL_EXT2:
			vsel = "EXT2"; break;
		default:
			printf( "This cant happen %x & %x >> %d\n",
				group, gcursor, igroup*2 );
		}
		do {
			printf("%02d=%s\n", ++ichannel, vsel);
		} while((ichannel&3) != 0);
	}
	return 0;
}


int main(int argc, const char **argv)
{
	struct poptOption opt_table[] = {
		{ "help",       'h', POPT_ARG_NONE,   0,           'h' },
		{}
	};
	poptContext context = poptGetContext( 
		argv[0], argc, argv, opt_table, 0 );
	int rc;

	while((rc =poptGetNextOpt(context)) > 0){
		switch(rc) {
		case 'h':
			return help();
		}
	}

	if (strstr(argv[0], "setRef") != 0){
		return setRef(context);
	}else if (strstr(argv[0], "getRef") != 0){
		return getRef(context);
	}else{
		fprintf(stderr, "INTERNAL ERROR: do not run this command\n");
		return -1;
	}
}
