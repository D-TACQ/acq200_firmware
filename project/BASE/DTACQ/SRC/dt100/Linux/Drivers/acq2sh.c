/*****************************************************************************
 *
 * File: acq2sh.c
 *
 * $RCSfile: acq2sh.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 * execute sh command on acq2xx target
 *
 * $Id: acq2sh.c,v 1.1.2.5 2004/02/08 10:27:41 pgm Exp $
 * $Log: acq2sh.c,v $
 * Revision 1.1.2.5  2004/02/08 10:27:41  pgm
 * wavegen init files
 *
 * Revision 1.1.2.4  2004/02/04 15:07:10  pgm
 * ensure command sent before listen
 *
 * Revision 1.1.2.3  2004/02/04 07:39:30  pgm
 * now with stdin handling
 *
 * Revision 1.1.2.2  2004/02/01 13:06:30  pgm
 * works
 *
 * Revision 1.1.2.1  2004/02/01 12:20:18  pgm
 * *** empty log message ***
 *
 *
\*****************************************************************************/

#include "local.h"

#include <stdio.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <popt.h>
#include <unistd.h>

#define VERSION "acq2sh $Revision: 1.1.2.5 $\n"


#define FNAME "/dev/acq200/acq200.1"
#define ENV   "ACQCMD_DEV"             /* overrides default */

#define MFMT "/dev/acq200/acq200.%d"

poptContext opt_context;
int verbose = 0;


#define PRINTF if ( verbose ) printf

#define FD_TO    0    /* command path to target */
#define FD_OUT   1    /* output from target     */
#define FD_INPUT 2    /* input data for command */
#define FD_DATA  3    /* output path for data   */
#define FD_MAX 4

#define INPUT_NONE 0
#define INPUT_HERE 1
#define INPUT_FILE 2



static int send_command( FILE* fd[]);
static int process_stdin( FILE* fd[] );
static int print_reply( FILE* fd[] );
static void terminate_command(FILE *fd[]);

static FILE* open_path( 
	const char* device, const char* extn, const char* mode);
static void send_input_here(FILE *fd[]);
static void send_input_there(FILE *fd[]);


int main( int argc, const char* argv[] )
{
	char* device = getenv( ENV )? getenv( ENV ):FNAME;
	char devname_buffer[80];
	FILE* fd[2];          /* send commands here */

	int rc;
	int board = 0;
	char *infile;
	int input = INPUT_NONE;

	struct poptOption opt_table[] = {
		{ "device",     
		  'f', POPT_ARG_STRING, &device,     'f',
		  "enter full name of device (appends .rsh-{to|out})"
		},
		{ "board",      
		  'b', POPT_ARG_INT,    &board,      'b',
		  "board index"
		},
		{ "input",
		  'i', POPT_ARG_STRING|POPT_ARGFLAG_OPTIONAL, &infile, 'i',
		  "input file {- == stdin run as a here doc}"
		},
		{ "version",    'V', POPT_ARG_NONE,   0,           'V' },
		{ "verbose",    'v', POPT_ARG_INT,    &verbose,    0   },
		POPT_AUTOHELP
		{ }
	};



	opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

	while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
		switch( rc ){
		case 'b':
			device = devname_buffer;
			sprintf( device, MFMT, board);
			break;
		case 'i':
			PRINTF("infile %s\n", infile);

			if (infile == 0 || strcmp(infile, "-") == 0){
				fd[FD_INPUT] = stdin;
				input = INPUT_HERE;
			}else{
				if ((fd[FD_INPUT] = fopen(infile, "r")) == 0){
					perror( "ERROR failed to open infile");
					exit(1);
				}
				input = INPUT_FILE;
				fd[FD_DATA] = open_path(device, "01", "w");
			}
			break;
		case 'V':
			fprintf(stderr, VERSION);
			return 1;
		}
	}  // processes all other opts via arg pointers

	PRINTF("using device \"%s\"\n", device);

	fd[FD_TO]  = open_path( device, "rsh-to",  "w");

	if ( poptPeekArg( opt_context ) ){
		PRINTF( "poptPeekArg() returns %p \"%s\"\n",
			poptPeekArg( opt_context ),
			poptPeekArg( opt_context )? 
			poptPeekArg( opt_context ): "---");
	        send_command( fd );

		switch(input){
		case INPUT_HERE:
			send_input_here(fd);
			break;
		case INPUT_FILE:
			send_input_there(fd);
			break;
		default:
			terminate_command(fd);
		}
		fd[FD_OUT] = open_path( device, "rsh-out", "r");
		return print_reply(fd);
	}else{
		return process_stdin( fd );
	}
}


static FILE* open_path( 
	const char* device, const char* extn, const char* mode)
{
	char fullname[80];
	FILE* fd;

	sprintf(fullname, "%s.%s", device, extn);

	if ((fd = fopen(fullname, mode)) == 0){
		fprintf(stderr, "ERROR: failed to open file \"%s\"\n", 
			fullname);
		_exit(2);
	}

	return fd;
}



static int print_reply(FILE* fd[])
{
	char reply[80];
	int nout = 0;

	while(1) {
		char* aline = fgets(reply, sizeof(reply), fd[FD_OUT]);

		if (aline == 0 && nout != 0){
			break;
		}else{
			nout += 1;
			if (aline){
				fputs( reply, stdout );
			}else{
				PRINTF("waiting ...\n");
				usleep(10000);
			}
		}
	}
/*
 * catch dodgy reply truncation
 */
	if (reply[strlen(reply)-1] != '\n'){
		fputs("\n", stdout);
	}

	if ( nout == 0){
		perror( "fgets() ret 0" );
		return 1;
	}else{
		return 0;
	}
}


static void terminate_command(FILE *fd[])
{
	fputs( "\n", fd[FD_TO] );

	PRINTF("ftell() %ld\n", ftell(fd[FD_TO]));

	if (ftell(fd[FD_TO])&1){
		fputs("\n", fd[FD_TO]);
	}
	fclose(fd[FD_TO]);
}

static int send_command( FILE* fd[] )
{       
	const char* next_word;
	int nput = 0;

	while( (next_word = poptGetArg( opt_context )) ){
		nput += fputs( next_word, fd[FD_TO] );
		nput += fputs( " ", fd[FD_TO] );
	}
	return 0;
}

static void send_input_here(FILE *fd[])
{
	char my_line[81];

	fputs(" <<EOF\n", fd[FD_TO]);

	while(fgets(my_line, 80, fd[FD_INPUT])){
		fputs(my_line, fd[FD_TO]);
	}
	fputs("EOF\n", fd[FD_TO]);
	terminate_command(fd);
}

static void send_input_there(FILE *fd[])
{
	char* buf = malloc(4096);
	int nread;
	int nwrite;
	int totwrite = 0;
	assert(buf);

	fputs(" </tmp/acq200tmp_01\n", fd[FD_TO]);

	while((nread = fread(buf, 1, 4096, fd[FD_INPUT])) > 0){
		nwrite = fwrite(buf, 1, nread, fd[FD_DATA]);
		if (nwrite < 0){
			perror( "ERROR DATA WRITE FAILED" );
			exit(1);
		}else{
			totwrite += nwrite;
		}
	}
	if (totwrite&1){
		fwrite("", 1, 1, fd[FD_DATA]);
	}
	fclose(fd[FD_DATA]);
	free(buf);
	terminate_command(fd);
}


#define ISCOMMENT( cl ) (cl[0]=='#')
#define ISBLANK(cl)     (strlen(cl)<=1)

static int process_stdin( FILE* fd[] )
{
	char command_line[128];

	while ( fgets( command_line, sizeof(command_line), stdin ) ){
		fputs(command_line, fd[FD_TO]);
	}
	fputs( "\n", fd[FD_TO] );
	fclose(fd[FD_TO]);

	return print_reply(fd);
}




