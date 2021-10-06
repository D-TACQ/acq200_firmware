/*****************************************************************************
 *
 * File: set.vin.acq216.c
 *
 * $RCSfile: set.vin.acq216.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 * wav232icd implementation
 * $Id: set.vin.acq216.c,v 1.1.2.2 2004/09/02 15:17:21 pgm Exp $
 * $Log: set.vin.acq216.c,v $
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

static int verbose;


static int help(void) {
	printf( "set.vin.acq216 $Revision: 1.1.2.2 $\n" );
	printf( "set.vin V - set all channels\n"
		"set.vin ch=V - set channel ch\n"
		"V = {2.5, 4, 6, 10}V\n" );
	return 0;
}




/*
 * returns
 * 0: not me
 * 1: success
 * -n: error
 */


#define NCHAN 16

#define VSEL_2V5   1
#define VSEL_4V0   2
#define VSEL_6V0   3
#define VSEL_10V0  4

static const struct RangeSelector {
	unsigned s24;
	unsigned s20;
} range_selector[] = {
	{ 0 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0xffff },
	{ 0xffff, 0x0000 },
	{ 0xffff, 0xffff }
};

static const unsigned channel_masks[] = {
	0,
	0x0001, 0x0002, 0x0004,	0x0008,
	0x0010, 0x0020, 0x0040, 0x0080,
	0x0100, 0x0200, 0x0400, 0x0800,	
	0x1000, 0x2000, 0x4000, 0x8000,	
};


unsigned _i2cReadSwitch(unsigned addr)	
{
	char buf[80];
	unsigned sval;
	FILE* rp;

	sprintf(buf, "i2c-w.w 0x%02x", addr);

	rp = popen(buf, "r" );

	assert(rp);
	assert(fscanf(rp, "%x", &sval) == 1);
	fclose(rp);

	return sval;
}
void i2cReadRange(struct RangeSelector *rs)
{
	rs->s20 = _i2cReadSwitch(0x20);
	rs->s24 = _i2cReadSwitch(0x24);
}


void _i2cUpdateSwitch(unsigned addr, unsigned value)	
{
	char buf[80];
	sprintf(buf, "i2c-w.w 0x%02x 0x%04x", addr, value);
	system(buf);

	if(verbose){
		printf("%s\n", buf);
	}
}

void i2cWriteRange(struct RangeSelector *rs)
{
	_i2cUpdateSwitch(0x20, rs->s20);
	_i2cUpdateSwitch(0x24, rs->s24);
}



#define ALL_CHANNELS 'X'


int doSetRange(struct RangeSelector* rs, int channel, unsigned vsel)
{
	if (channel == ALL_CHANNELS){
		int ch;
		for (ch = 1; ch <= NCHAN; ++ch){
			doSetRange(rs, ch, vsel);
		}
	}else{
		unsigned cursor = channel_masks[channel];

		if (cursor & range_selector[vsel].s24){
			rs->s24 |= cursor;
		}else{
			rs->s24 &= ~cursor;
		}
		if (cursor & range_selector[vsel].s20){
			rs->s20 |= cursor;
		}else{
			rs->s20 &= ~cursor;
		}
	}
	return 0;
}


int parseSetRange(struct RangeSelector* rs, const char *arg)
{
	int channel = ALL_CHANNELS;
	char vsel[10];

	if (sscanf(arg, "%d=%s", &channel, vsel) == 2    ||
            sscanf(arg, "%d%%3D%s", &channel, vsel) == 2 ||
	    sscanf(arg, "XX=%s", vsel) == 1              || 
	    sscanf(arg, "XX%%3D%s", vsel) == 1               ){
		if  (channel == ALL_CHANNELS || 
		     (channel >= 1 && channel <= NCHAN)){
			if (strcmp(vsel, "2.5V") == 0 ){
				return doSetRange(rs, channel, VSEL_2V5);
			}else if (strcmp(vsel, "4V") == 0){
				return doSetRange(rs, channel, VSEL_4V0);
			}else if (strcmp(vsel, "6V") == 0){
				return doSetRange(rs, channel, VSEL_6V0);
			}else if (strcmp(vsel, "10V") == 0){
				return doSetRange(rs, channel, VSEL_10V0);
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


int setRange(poptContext context)
{
	struct RangeSelector rs;
	int use_command_line = 0;
	const char *next;

	i2cReadRange(&rs);

	while((next = poptGetArg(context)) != 0){
		parseSetRange(&rs, next);
		use_command_line = 1;
	}
	
	if (!use_command_line){
		char aline[80];
		while(fgets(aline, 80, stdin) != 0){
			parseSetRange(&rs, aline);
		}
	}

	i2cWriteRange(&rs);

	return 0;
}


static const char* toString(struct RangeSelector* rs)
{
	static const char* strings[] = {
		[VSEL_2V5] = "2.5",
		[VSEL_4V0] = "4.0",
		[VSEL_6V0] = "6.0",
		[VSEL_10V0] = "10.0"
	};
	int vsel;

	for (vsel = VSEL_2V5; vsel <= VSEL_10V0; ++vsel){
		if ((rs->s24 != 0) == (range_selector[vsel].s24 != 0) &&
                    (rs->s20 != 0) == (range_selector[vsel].s20 != 0)  ){
			return strings[vsel];
		}
	}

	return "OOR";
}


#define ALL_THE_SAME(rs) ((rs.s20 == 0xffff || rs.s20 == 0x0000) && \
                          (rs.s24 == 0xffff || rs.s24 == 0x0000) )

static inline void copyRangeMask(
	struct RangeSelector *dest, 
	const struct RangeSelector *src, unsigned cursor)
{
	dest->s24 = src->s24 & cursor;
	dest->s20 = src->s20 & cursor;
}

int getRange(poptContext context)
{
	struct RangeSelector rs;
	int ichannel = 0;

	i2cReadRange(&rs);

	if (ALL_THE_SAME(rs)){		
		printf("-%sV,%sV\n", toString(&rs), toString(&rs));
	}else{
		printf("-10V,10V\n");
	}
	for (ichannel = 1; ++ichannel <= 16; ){
		struct RangeSelector rs2;

		copyRangeMask(&rs2, &rs, channel_masks[ichannel]);

		printf("%2d=-%sV,%sV\n", ichannel, 
		       toString(&rs2), toString(&rs2));
	}
	return 0;
}



int main(int argc, const char **argv)
{
	struct poptOption opt_table[] = {
		{ "help",       'h', POPT_ARG_NONE,   0,           'h' },
		{ "verbose",    'v', POPT_ARG_INT,    &verbose,    0   },
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

	if (strstr(argv[0], "set.vin") != 0){
		return setRange(context);
	}else if (strstr(argv[0], "get.vin") != 0){
		return getRange(context);
	}else{
		fprintf(stderr, "INTERNAL ERROR: do not run this command\n");
		return -1;
	}
}
