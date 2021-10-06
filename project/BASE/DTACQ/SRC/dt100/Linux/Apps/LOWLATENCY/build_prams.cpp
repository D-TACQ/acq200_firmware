/*****************************************************************************
 *
 * File: build_prams.c
 *
 * $RCSfile: build_prams.cpp,v $
 *
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 * ACQ216 precooking file. Build a struct LLC200_INIT[] parameter array
 * for use with ACQ216
 * Use with ascii definition file
 *
 * $Id: build_prams.cpp,v 1.1.2.7 2009/03/26 15:52:42 pgm Exp $
 * $Log: build_prams.cpp,v $
 * Revision 1.1.2.7  2009/03/26 15:52:42  pgm
 * x64 syntax errors fixed
 *
 * Revision 1.1.2.6  2005/12/01 21:48:47  pgm
 * qdac
 *
 * Revision 1.1.2.5  2005/11/30 10:52:53  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.4  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.3  2005/10/22 12:18:56  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.2  2005/10/20 20:51:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2005/10/20 17:00:29  pgm
 * *** empty log message ***
 *
 * Revision 1.2.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/

/** @file build_prams.cpp ACQ216 precooking file. 
 - Build a struct LLC200_INIT[] parameter array
 * for use with ACQ216
 - This is an example application, backing test definitions of parameter
 * settings into a binary array. llcontrol loads this binary array,
 * one record per shot.
 - This application is not rocket science, it simply interprets the definitions  * and packs them.
 - RESTRICTION: only supports changing one parameter type at a time. It should 
 * be possible to OR together records. llcontrol supports this.
 * 
 */
#include "local.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <ctype.h>

#include <popt.h>

typedef unsigned u32;        // define a 32 bit quantity

#include "acq32busprot.h"
#include "llcontrol.h"


struct LLC200_INIT *llc200_init;
int llc200_init_count;

struct ULL_LUT {
	const char* key;
	unsigned long long mask;
};

template<class T> class InputLineHandler {
protected:
	T *array;
	int maxcount;
	int ix;
  

public:
	InputLineHandler () :
		array(0), maxcount(0) {
		ix = 0;
	}
	virtual ~InputLineHandler() {}

	virtual void handleLine(char myline[]) {
    
	}

	void setMaxCount(int _maxcount) { maxcount = _maxcount; }
	void setArray(T* _array) { array = _array; }
	int getCount(void) { return ix; }
};


class TextFileReader {
	FILE *fp;
public:
	TextFileReader(const char* fname) {
		fp = fopen(fname, "r");
		assert(fp);
	}
	~TextFileReader() { fclose(fp); }

	char* getLine(char* myline, int maxline) {
		return fgets(myline, maxline, fp);
	}    
};

#define MAXLINE 256

template<class T> class DefinitionFileHandler {
protected:
	const char* def_file;
  
	int isComment(const char* my_line) {
		if (strlen(my_line) == 1){
			return 1;
		}else if (my_line[0] == '#'){
			return 1;
		}else if (strspn(my_line, " \n") == strlen(my_line)){
			return 1;
		}else{
			return 0;
		}
	}
public:
	DefinitionFileHandler(const char* _def_file) : def_file(_def_file) {}

		int getMaxLines(void) {
			char myline[MAXLINE];
			int line_count = 0;

			TextFileReader fileReader(def_file);

			while(fileReader.getLine(myline, MAXLINE) != 0){
				++line_count;
			}

			return line_count;
		}

		void processFile(InputLineHandler<T> *handler) {
			char myline[MAXLINE];
			TextFileReader fileReader(def_file);
    
			while(fileReader.getLine(myline, MAXLINE) != 0){
				if (!isComment(myline)){
					handler->handleLine(myline);
				}
			}
		}
};

static unsigned char hexval(char hexchar) 
/** ASCII coding assumed. */
{
	if (hexchar >= '0' && hexchar <= '9'){
		return hexchar - '0';
	}else if (hexchar >= 'A' && hexchar <= 'F'){
		return hexchar - 'A' + 10;
	}else if (hexchar >= 'a' && hexchar <= 'f'){
		return hexchar - 'a' + 10;
	}else{
		assert(0);
	}
}


static void initBuf(DefinitionFileHandler<struct LLC200_INIT>* fileHandler) {
	if (!llc200_init_count){
		llc200_init_count = fileHandler->getMaxLines();
		llc200_init = (LLC200_INIT *)
			calloc(llc200_init_count, LLC200_INIT_SZ);

		assert(llc200_init);
	}
}



/** processes DDS Frequency Tuning Word. */
class FtwHandler : public InputLineHandler<struct LLC200_INIT> {
public:
	FtwHandler() : 
		InputLineHandler<struct LLC200_INIT>() {}

		virtual void handleLine(char myline[]){
			if (ix == maxcount){
				return;
			}
			unsigned char ftw1[6];

			/** myline contains a sequence of 12 hex digits. 
			 *  output as 6 uchars
			 */
			for (int ic = 0, iw = 0; iw < 6; ++ic){
				if (isxdigit(myline[ic])){
					if ((ic&1) == 0){
						ftw1[iw] = hexval(myline[ic]) << 4;
					}else{
						ftw1[iw++] |= hexval(myline[ic]);
					}
				}else{
					return;
				}
			}

			memcpy(array[ix].dds_ftw, ftw1, sizeof(ftw1));

			array[ix].mask |= LLC200_INIT_MASK_DDS_FTW;
			array[ix].marker = LLC200_INIT_MAGIC_MARKER;
			ix++;
		}
};


/** processes DDS Frequency Tuning Word. */
class QdacHandler : public InputLineHandler<struct LLC200_INIT> {
public:
	QdacHandler() : 
		InputLineHandler<struct LLC200_INIT>() {}

		virtual void handleLine(char myline[]){
			if (ix == maxcount){
				return;
			}
			short qvc = atoi(myline);

			memcpy(&array[ix].dds_ftw[6], &qvc, sizeof(short));

			array[ix].mask |= LLC200_INIT_MASK_DDS_QDAC;
			array[ix].marker = LLC200_INIT_MAGIC_MARKER;
			ix++;
		}
};




static void load(InputLineHandler<struct LLC200_INIT>* lineHandler, const char* def_file)
{
	DefinitionFileHandler<struct LLC200_INIT>* fileHandler = 
		new DefinitionFileHandler<struct LLC200_INIT> (def_file);

	initBuf(fileHandler);

	lineHandler->setMaxCount(llc200_init_count);
	lineHandler->setArray(llc200_init);

	fileHandler->processFile(lineHandler);

	llc200_init_count = lineHandler->getCount();
	delete lineHandler;
}


/** processes Internal Clock settings.*/
class IntclkHandler : public InputLineHandler<struct LLC200_INIT> {
public:
	IntclkHandler() {}

	virtual void handleLine(char myline[]) {
		if (ix == maxcount){
			return;
		}
		unsigned int_clk;

		if (sscanf(myline, "%u", &int_clk) == 1){
			array[ix].int_clk = int_clk;
			array[ix].mask |= LLC200_INIT_MASK_INTCLK;
			array[ix].marker = LLC200_INIT_MAGIC_MARKER;
			ix++;
		}
	}
};


int strsplit(char *str, char *argv[], int maxargs, const char* delim)
/** strsplit() splits str into args, returns #args. */
{
        int iarg;
        char *s1, *s2;

        for (iarg = 0, s1 = str; iarg < maxargs && (s2 = strstr(s1, delim)); ){
                *s2 = '\0';
                argv[iarg++] = s1;
                s1 = s2 + strlen(delim);
        }
        if (s1 != 0){
                argv[iarg++] = s1;
        }
        return iarg;
}

/** Input Offsets are value,value,value 16 per line */
class InputOffsetHandler : public InputLineHandler<struct LLC200_INIT> {
public:
	virtual void handleLine(char myline[]) {
		if (ix == maxcount){
			return;
		}
		char* argv[16];
		int nargs = strsplit(myline, argv, 16, ",");

		for (int ic = 0; ic != nargs; ++ic){
			array[ix].offsets[ic] = strtol(argv[ic], 0, 0);
		}

		array[ix].mask |= LLC200_INIT_MASK_OFFSETS;
		array[ix].marker = LLC200_INIT_MAGIC_MARKER;
		ix++;
	}
};

#include "CONSTS/M2-consts.h"
#include "CONSTS/M5-consts.h"
#include "CONSTS/M2-consts.c"
#include "CONSTS/M5-consts.c"  

static char *chomp(char *str) {
	if (str[strlen(str)-1] == '\n'){
		str[strlen(str)-1] = '\0';
	}
	return str;
}

/* process input Voltage Range settings. */
class VRangeHandler : public InputLineHandler<struct LLC200_INIT> {
	const struct ULL_LUT* lut;

	unsigned long long lookup(const char* key){
		const struct ULL_LUT* cursor = lut;

		while(cursor->key != 0){
			if (strcmp(cursor->key, key) == 0){
				return cursor->mask;
			}
			cursor++;
		}
		fprintf(stderr, "WARNING: key \"%s\" NOT FOUND\n", key);
		return 0;
	}
public:
	VRangeHandler(const struct ULL_LUT* _lut) :  lut(_lut) {}

		virtual void handleLine(char myline[]) {
			if (ix == maxcount){
				return;
			}
			unsigned long long vrange = 0ULL;


			for (char* ps = myline; (ps = strtok(ps, " ")) != 0; ps = 0){
				chomp(ps);
				vrange |= lookup(ps);
			}

			array[ix].vranges.ull = vrange;
			array[ix].mask |= LLC200_INIT_MASK_RANGE;
			array[ix].marker = LLC200_INIT_MAGIC_MARKER;
			ix++;
		}
};



static void writeOutput(const char* outfile)
{
	FILE *fp = fopen(outfile, "w");
	assert(fp);

	fwrite(llc200_init, LLC200_INIT_SZ, llc200_init_count, fp);
	fclose(fp);
}




int main( int argc, const char* argv[] )
{
	char *def_file;
	const char *outfile = "build_prams_outfile";
	int rc;

	struct poptOption opt_table[] = {
		{ "dds_ftw", 0, POPT_ARG_STRING, &def_file, 'd'  },
		{ "dds_qdac",0, POPT_ARG_STRING, &def_file, 'q' },
		{ "M5range", 0, POPT_ARG_STRING, &def_file,  '5' },
		{ "M2range", 0, POPT_ARG_STRING, &def_file,  '2' },
		{ "M5offset", 0, POPT_ARG_STRING, &def_file, 'O' },
		{ "intclk",  0, POPT_ARG_STRING, &def_file, 'c'       },
		{ "output",  'o', POPT_ARG_STRING, &outfile,  0   },
		POPT_AUTOHELP
		POPT_TABLEEND
	};

	poptContext opt_context =
		poptGetContext(argv[0], argc, argv, opt_table, 0);


	while((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		case 'd':
		        load(new FtwHandler(), def_file);
			break;
		case 'q':
			load(new QdacHandler(), def_file);
		case 'c':
			load(new IntclkHandler(), def_file);
			break;
		case '2':
			load(new VRangeHandler(M2_consts), def_file);
			break;
		case '5':
			load(new VRangeHandler(M5_consts), def_file);
			break;
		case 'O':
			load(new InputOffsetHandler(), def_file);
			break;
		}
	}

	writeOutput(outfile);
	return 0;
}

