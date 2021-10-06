/*
 * hex.c
 *
 * usage:
 * hex <hexnum> : print decval
 * 2hex <decnum> : print hexval
 */


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "popt.h"

static int help(void)
{
	printf("hex $Revision: 1.1.2.7 $\n");
	printf("hex|hexout [--scan fmt|--print fmt] [n1..]\n");
	printf("--scan -s : fmt - regular sscanf chars, %%s means binary\n");
	printf("--print -p: fmt - regular printf chars, %%s means binary\n");
	printf("example: hex --p \"\\t%%-32s\\n\" 10\n");
	return 0;
}

typedef union {
	unsigned xx;
	char bin[33];
} UniversalArg;


/* LEFTMOST CHAR IS LSB */

int isbin(const char* fmt){
#define LOOK4_CONV    0
#define LOOK4_BIN     1
#define CONV_SPEC     '%'
#define BIN_SPEC      's'
#define MODIFIERS     "-0123456789."
	const char* pc;
	int state = LOOK4_CONV;

	for (pc = fmt; *pc; ++pc){
		switch(state){
		case LOOK4_CONV:
			if (*pc == CONV_SPEC){
				state = LOOK4_BIN;
			}
			break;
		case LOOK4_BIN:
			if (*pc == BIN_SPEC){
				return 1;
			}if (index(MODIFIERS, *pc) != 0){
				continue;
			}else{
				return 0;
			}
		}
	}

	return 0;
}

static unsigned abtoi(const char bin[])
/* Ascii Binary-string To Integer */
{
	int ichar;
	unsigned icursor = 0x00001;
	unsigned xx = 0;
	
	for (ichar = 0;; ++ichar, icursor <<= 1){
		switch(bin[ichar]){
		case '0':
			continue;
		case '1':
			xx |= icursor;
			continue;
		default:
			return xx;
		}
	}

	return xx;
}

static void itoba(char bin[], unsigned xx)
{
	int ichar;
	unsigned icursor = 0x00001;

	for (ichar = 0; icursor; icursor <<= 1, ichar++){
		bin[ichar] = (xx & icursor)? '1': '0';
	}
	bin[ichar] = '\0';
}

int usscanf(const char *str, const char *format, UniversalArg *ua)
{
	if (sscanf(str, format, ua) == 1){
		if (isbin(format)){
			ua->xx = abtoi(ua->bin);
		}
		return 1;
	}else{
		return 0;
	}
}


int uprintf(const char *format, UniversalArg *ua)
{
	if (isbin(format)){
		itoba(ua->bin, ua->xx);
		return printf(format, ua->bin);
	}else{
		return printf(format, ua->xx);
	}
}

static char *fix_escapes(char *fmt)
{
#define ESC '\\'
	if (index(fmt, ESC)){
		char *litstr = malloc(strlen(fmt));
		int isrc;
		int idest;
		int found_esc = 0;

		for (isrc = idest = 0; fmt[isrc]; ++isrc){
			char src_char = fmt[isrc];

			switch(src_char){
			case ESC:
				found_esc = 1;
				continue;
			default:
				if (found_esc){
					switch(src_char){
					case 'n':
						src_char = '\n';
						break;
					case 't':
						src_char = '\t';
						break;
					default:
						;
					}
					found_esc = 0;
				}
				litstr[idest++] = src_char;
			}
		}
		litstr[idest] = '\0';
		return litstr;
	}else{
		return fmt;
	}
}
int main(int argc, const char **argv)
{
	char *fmtscan;
	char *fmtprint;
	UniversalArg xx;
	int errors = 0;
	int rc;
	const char *arg;

	struct poptOption opt_table[] = {
		{ "help",       'h', POPT_ARG_NONE,   0,           'h' },
		{ "scan",       's', POPT_ARG_STRING, &fmtscan,     0 },
		{ "print",      'p', POPT_ARG_STRING, &fmtprint,    0 },
		{ }
	};


	if (strcmp(argv[0], "hexout") == 0){
		fmtscan = "%d";
		fmtprint = "%x";
	}else{
		fmtscan = "%x";
		fmtprint = "%d";
	}

	poptContext context = poptGetContext( 
		argv[0], argc, argv, opt_table, 0 );

	while((rc =poptGetNextOpt(context)) > 0){
		switch(rc) {
		case 'h':
			return help();
		}
	}

	fmtscan = fix_escapes(fmtscan);
	fmtprint = fix_escapes(fmtprint);

	while((arg = poptGetArg(context)) != 0){
		if (usscanf(arg, fmtscan, &xx) == 1){
			uprintf(fmtprint, &xx);
		}else{
			errors++;
		}
	}

	return errors? -1: 0;
}
