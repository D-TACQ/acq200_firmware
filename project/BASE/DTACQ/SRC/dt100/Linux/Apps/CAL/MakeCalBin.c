/*****************************************************************************
 *
 * File:MakeCalBin.c
 *
 * $RCSfile: MakeCalBin.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: builds raw cal data into AIF file
 *
 * $Id: MakeCalBin.c,v 1.5 2002/11/14 12:08:59 pgm Exp $
 * $Log: MakeCalBin.c,v $
 * Revision 1.5  2002/11/14 12:08:59  pgm
 * text block
 *
 * Revision 1.4  2002/04/20 13:55:08  pgm
 * print file name on fail to open
 *
 * Revision 1.3  2001/04/17 20:37:15  pgm
 * multiple cals possible
 *
 * Revision 1.2  2001/04/17 20:24:07  pgm
 * multiple cals possible
 *
 * Revision 1.1  2001/04/09 19:37:10  pgm
 * i think we need these
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/




const char* DOC = 
"MakeCalBin -s <serial-number> <calfile1> ... <calfileN>\n"
"if calfile1 is '-', read from stdin and gen text block\n"
"or use --test\n";



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>
#include <time.h>

#include "local.h"

#include "cal-image.h"        // WARNING: copy of orig from ARM dir

#define NCHANNELS 32

#define FPIN stdin
#define FPOUT stdout

int G_debug;

#define PRINTF    if ( G_debug ) printf

static const char* home()
{
    char* home = getenv( "HOME" );
    ASSERT( home );

    return home;
}

static void getDacVals( const char* calfile, unsigned* dacvals )
{
    char line[80];
    int idac = 0;
    FILE* fp = fopen( calfile, "r" );

    if ( !fp ) {
        fprintf( stderr, "ERROR: failed to open calfile %s\n", calfile );
        exit( 1 );
    }

    while( fgets( line, 80, fp ) ){
        if ( line[0] != '#' && line[0] != ' ' ){
            dacvals[idac++] = strtoul( line, 0, 0 );

            PRINTF( "dacvals[%2d] = 0x%08x\n", idac-1, dacvals[idac-1] );
        }
    }

    ASSERT( idac == NCHANNELS );
}

#define REFIMAGE "%s/ArmImages/def-cal.axf"


static char* readRefImage( int* image_size )
// reads entire ref image into memory. client must free memory
// exit()s on error
{
    struct stat statbuf;
    char refname[80];
    FILE* fp;
    char* image;
    
    sprintf( refname, REFIMAGE, home() );

    PRINTF( "refname is %s\n", refname );

    if ( lstat( refname, &statbuf ) ){
        fprintf( stderr, "ERROR: failed to stat %s\n", refname );
        exit( 1 );
    }
    PRINTF( "Image %s size %d\n", refname, (int)statbuf.st_size );

    image = malloc( statbuf.st_size );
    ASSERT( image );

    fp = fopen( refname, "r" );
    if ( fp == 0 ) { 
	fprintf( stderr, "ERROR:failed to open file:%s\n", refname );
	perror( "" );
	ASSERT(fp);
    }

    fread( image, 1, statbuf.st_size, fp );
    fclose( fp );

    *image_size = statbuf.st_size;
    return image;
}


static void space_fill( char buf[], int nbuf )
{
    memset( buf, ' ', nbuf );
    buf[nbuf-1] = '\0';
}

#define IMAGE_OFFSET 0x34         // offset of our stuff in AIF

#define CALIMAGE "%s/ArmImages/%s.axf"

static void makeCalBin( 
    char* serialn, char* date, 
    unsigned* dacvals, int nsets )
{
    char fn_out[80];
    FILE* fp_out;
    int idac;
    char snline[80];
  /*
   * read the whole ref file
   */
    int image_size;

    char* bin_image = readRefImage( &image_size ); 

  /*
   * update the parts
   */
    struct CAL_IMAGE* cal_image = (struct CAL_IMAGE*)&bin_image[IMAGE_OFFSET];

    space_fill( cal_image->board, sizeof(cal_image->board) );
    space_fill( cal_image->date,  sizeof(cal_image->date) );

    sprintf( snline, "%s,datasets=%d", serialn, nsets );

    strncpy( cal_image->board, snline,  sizeof(cal_image->board) );
    strncpy( cal_image->date,  date,    sizeof(cal_image->date)  );

    for ( idac = 0; idac != NCHANNELS*nsets; ++idac ){
	ASSERT( (char*)&cal_image->_d.data[idac] - bin_image < image_size );

        cal_image->_d.data[idac] = dacvals[idac];
    }
  /*
   * write it out to serialn.axf
   */
 
   sprintf( fn_out, CALIMAGE, home(), serialn );

    fp_out = fopen( fn_out, "w" );
    ASSERT( fp_out );
    
    fwrite( bin_image, 1, image_size, fp_out );
    fclose( fp_out );

    free( bin_image );
}

static void makeTextBlock( 
    char* serialn, char* date )
{
    char fn_out[80];
    FILE* fp_out;
  /*
   * read the whole ref file
   */
    int image_size;

    char* bin_image = readRefImage( &image_size ); 
    int nbytes, ibyte;

  /*
   * update the parts
   */
    struct CAL_IMAGE* cal_image = (struct CAL_IMAGE*)&bin_image[IMAGE_OFFSET];

    space_fill( cal_image->board, sizeof(cal_image->board) );
    space_fill( cal_image->date,  sizeof(cal_image->date) );

    strncpy( cal_image->board, serialn,  sizeof(cal_image->board) );
    strncpy( cal_image->date,  date,    sizeof(cal_image->date)  );

    /* grab text data */
    nbytes = read( 0, &cal_image->_d, sizeof(cal_image->_d) );

    for ( ibyte = 0; ibyte != nbytes; ++ibyte ){
	if ( cal_image->_d.text[ibyte] == '\n' ){
	    cal_image->_d.text[ibyte] = '\0';
	}
    }
    strcpy( &cal_image->_d.text[ibyte], "EOF" );
  /*
   * write it out to serialn.axf
   */
 
   sprintf( fn_out, CALIMAGE, home(), serialn );

    fp_out = fopen( fn_out, "w" );
    ASSERT( fp_out );
    
    fwrite( bin_image, 1, image_size, fp_out );
    fclose( fp_out );

    free( bin_image );
}

#define MAXCALFILES 8

int main( int argc, const char* argv[] )
{
   const char* calfile;
   char* serialn = 0;
   char* date;
   unsigned dacvals[MAXCALFILES][NCHANNELS];
   time_t now;   
   
  /* 
   * instantiate main data structures 
   */

    struct poptOption opt_table[] = {
        { "help",        'h', POPT_ARG_NONE,        0,   'h' },
        { "debug",       'd', POPT_ARG_INT,  &G_debug,     0 },
	{ "serial",      's', POPT_ARG_STRING, &serialn, 0 },
	{ "text",        't', POPT_ARG_NONE, 0, 't' },
        { }
    };

    poptContext opt_context =
        poptGetContext( argv[0], argc, argv, opt_table, 0 );
    int rc;
    int ical;
    int make_text_block = 0;

    while( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
	case 't':
	    make_text_block = 1;
	    break;
        case 'h':
            fprintf( stderr, DOC );
            return 1;
        }
    }

    if ( !serialn ){
	fprintf( stderr, "please specify serial number -s <sn>\n" );
	return 1;
    }

    for ( ical = 0; !make_text_block && ical != MAXCALFILES; ++ical ){
	if ( (calfile = poptGetArg( opt_context )) != NULL ){
	    if ( strcmp( calfile, "-" ) == 0 ){
		make_text_block = 1;
	    }else{
		getDacVals( calfile, dacvals[ical] );
	    }
	}else{
	    break;
	}
    }

    if ( !make_text_block && ical == 0 ){
	fprintf( stderr, "please specify at least one calfile\n" );
	return 1;
    }

    now = time( NULL );
    date = ctime( &now );    

    if ( make_text_block ){
	makeTextBlock( serialn, date );
    }else{
	makeCalBin( serialn, date, (unsigned*)dacvals, ical );
    }
    return 0;
}









