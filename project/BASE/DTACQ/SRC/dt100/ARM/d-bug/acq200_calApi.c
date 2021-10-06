/*****************************************************************************
 *
 * File: acq200_calApi.c - api for ACQ200 cal data
 *
 * $RCSfile: acq200_calApi.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq200_calApi.c,v 1.1.2.3 2007/01/03 23:16:16 pgm Exp $
 * $Log: acq200_calApi.c,v $
 * Revision 1.1.2.3  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.1.2.2  2004/10/14 20:12:45  pgm
 * no warnings
 *
 * Revision 1.1.2.1  2004/08/29 11:54:19  pgm
 * *** empty log message ***
 *
 * Revision 1.5  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.4  2002/04/26 10:51:59  pgm
 * B1638 finally handles flash num channels correctly
 *
 * Revision 1.3  2002/04/24 12:03:03  pgm
 * B1632 - qualifies queries at run, rounds clock down, selects #channels on Cal Title
 *
 * Revision 1.2  2001/05/29 19:51:24  pgm
 * fixed cal, DACpot
 *
 * Revision 1.1  2001/04/18 20:39:09  pgm
 * built in multi cal facility - needs busprot
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#include <stdio.h>
#include <string.h>

#include <assert.h>

#include <stdlib.h>


static char boot_env[4096];

const char* getBootEnv(const char* key, char vbuf[])
{
	char *pkey = strstr(boot_env, key);
	static char value[32];
	int ii = 0;
	int iv;

	if (pkey && pkey[strlen(key)] == '=' ){
		iv = strlen(key) + 1;

		for (; ii != 32 && pkey[iv] != '\0' && pkey[iv] != ' ';){
			value[ii++] = pkey[iv++];
		}
		value[ii] = '\0';
		
		strcpy(vbuf, value);
		return vbuf;
	}

	strcpy(vbuf, "key not found");
	return vbuf;
}
const char* calGetName( void )
{
	static char vbuf[32];

	getBootEnv("hostname", vbuf);
	return vbuf;
}
const char *calGetBoard( void )
{
	static char vbuf[32];

	getBootEnv("serialnum", vbuf);
	return vbuf;
}
const char *calGetDate( void )
{
	return __DATE__;
}
const unsigned* calGetImage( int ical )
{
	return (unsigned *)0;
}

int calGetNumChannels( void )
{
	char aichan[80];

	getBootEnv("AICHAN", aichan);
	if (atoi(aichan)){
		return atoi(aichan);
	}else{
		return 16;
	}
}

void cal_init_statics( void )
{
	FILE *fp = fopen("/proc/cmdline", "r");

	assert(fp);

	fread(boot_env, 1, sizeof(boot_env), fp);
	
	fclose(fp);
}


const char* envGetName( void )
{
	return "stub";
}
const char* envGetDate( void )
{
	return "20031031";
}
void envPrintEnv( void )
{

}
const char* envGetEnv( const char* key )
{
	return "stub";
}
int envGetEnvUInt( const char* key, unsigned* pvalue )
{
	return -37;
}
