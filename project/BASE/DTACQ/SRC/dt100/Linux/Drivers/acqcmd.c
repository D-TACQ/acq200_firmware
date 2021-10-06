/*****************************************************************************
 *
 * File: acqcmd.c
 *
 * $RCSfile: acqcmd.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 * send command and get answer from same path - safe in multi task env
 *
 * $Id: acqcmd.c,v 1.12.4.3 2004/10/25 08:33:52 pgm Exp $
 * $Log: acqcmd.c,v $
 * Revision 1.12.4.3  2004/10/25 08:33:52  pgm
 * -s shell command
 *
 * Revision 1.12.4.2  2004/06/07 10:02:09  pgm
 * docs
 *
 * Revision 1.12.4.1  2004/06/07 09:57:36  pgm
 * verbose=lazypoll
 *
 * Revision 1.12  2002/03/18 07:35:33  pgm
 * silent polling
 *
 * Revision 1.11  2002/03/18 07:19:20  pgm
 * poll option
 *
 * Revision 1.10  2002/03/18 06:53:22  pgm
 * *** empty log message ***
 *
 * Revision 1.9  2001/10/07 10:04:08  pgm
 * blanks are coments too
 *
 * Revision 1.8  2001/10/03 06:01:46  pgm
 * reads commands from stdin
 *
 * Revision 1.7  2001/09/07 09:53:44  pgm
 * acqcmd-hold option
 *
 * Revision 1.6  2001/04/07 13:53:58  pgm
 * -b board opt
 *
 * Revision 1.5  2001/03/11 21:30:11  pgm
 * fixed indentation with snavigator
 *
 * Revision 1.4  2001/02/24 18:26:09  pgm
 * acqcmd works
 *
 * Revision 1.3  2001/02/24 18:18:36  pgm
 * test device for lltest, also acqcmd
 *
 *
\*****************************************************************************/

#include "local.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <popt.h>
#include <unistd.h>

#define HELP \
"acqcmd $Revision: 1.12.4.3 $\n"\
"acqcmd [-f device][-b board {1..7}] [command]\n"\
"acqcmd -s board [shell-command]\n"\
"or export ACQCMD_DEV=device\n"\
"if no command use stdin (tip:here docs are good)\n"\
"special polling opts:\n"\
"acqcmd --while state {ST_RUN|ST_STOP}\n"\
"acqcmd --until state\n"\
"acqcmd [-v verbose] (also slows while/until poll rate)\n"\
""

#define FNAME "/dev/acq32/acq32.1.m1"
#define ENV   "ACQCMD_DEV"             /* overrides default */

#define MFMT "/dev/acq32/acq32.%d.m%d"

#define SHELL_FMT "/dev/acq200/acq200.%d.rsh"

poptContext opt_context;
int verbose = 0;


#define PRINTF if ( verbose ) printf

static int process_command( FILE* fd );
static int process_stdin( FILE* fd );
static int do_poll( 
    FILE* fd, 
    const char* command,
    const char* do_while, 
    const char* do_until );

int main( int argc, const char* argv[] )
{
    char* device = getenv( ENV )? getenv( ENV ):FNAME;
    char devname_buffer[80];
    char* poll_while = 0;
    char* poll_until = 0;
    FILE* fd;

    int rc;
    int board;
    int poll = 0;

    struct poptOption opt_table[] = {
        { "help",       'h', POPT_ARG_NONE,   0,           'h' },
        { "device",     'f', POPT_ARG_STRING, &device,     'f' },
        { "board",      'b', POPT_ARG_INT,    &board,      'b' },
	{ "shell",      's', POPT_ARG_INT,    &board,      's' },
        { "verbose",    'v', POPT_ARG_INT,    &verbose,    0   },
        { "while",      'w', POPT_ARG_STRING, &poll_while, 'p' },
        { "until",      'u', POPT_ARG_STRING, &poll_until, 'p' },
        { }
    };

   

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 'b':
		 if (strstr(argv[0], "acq2sh") == 0){
			 device = devname_buffer;
			 sprintf( device, MFMT, board, board );
			 break;
		 }
		 /* else fall thru */
	case 's':
		device = devname_buffer;
		sprintf(device, SHELL_FMT, board);
		break;
        case 'p':
            poll = 1;
            break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        }
    }  // processes all other opts via arg pointers

    if ( verbose > 1 ){
        fprintf( stderr, "using device \"%s\"\n", device );
    }
    
    fd = fopen( device, "r+" );
    if ( fd == NULL ){
        fprintf( stderr, "ERROR: failed to open device \"%s\"\n", device );
        return 1;
    }


    if ( STREQ( argv[0], "acqcmd-hold" ) ){
        pause();                                // hold path until killed
        return -1;
    }else if ( poll ) {
        return do_poll( fd, poptGetArg( opt_context ), 
			poll_while, poll_until );
    }else if ( poptPeekArg( opt_context ) ){
        PRINTF( "poptPeekArg() returns %p \"%s\"\n",
                poptPeekArg( opt_context ),
                poptPeekArg( opt_context )? poptPeekArg( opt_context ): "---");
        return process_command( fd );
    }else{
        return process_stdin( fd );
    }
}

static int process_command( FILE* fd )
{       
    char reply[80];
    const char* next_word;
    int nout;

    while( (next_word = poptGetArg( opt_context )) ){
        fputs( next_word, fd );
        fputs( " ", fd );
    }

    fputs( "\n", fd );

    for ( nout = 0; fgets( reply, sizeof(reply), fd ) != NULL; ++nout ){
        fputs( reply, stdout );
    }

    if ( nout == 0){
        perror( "fgets() ret 0" );
        return 1;
    }else{
        return 0;
    }
}

static int do_poll( 
    FILE* fd, 
    const char* command, 
    const char* do_while, 
    const char* do_until )
{       
    char reply[80];
    int nout;

    if( command == 0 ){
        command = "getState";
    }

    PRINTF( "command \"%s\" while \"%s\" until \"%s\"\n",
            command, 
            do_while!=0? do_while: "any",
            do_until!=0? do_until: "none" );

    while (1) {
        int found_while = 0;
        int found_until = 0;

        PRINTF( "%s\n", command );

        fputs( command, fd );
        fputs( "\n", fd );

	if ( verbose==1 ) fputs( ".", stdout );

        for ( nout = 0; fgets( reply, sizeof(reply), fd ) != NULL; ++nout ){
	    if ( verbose > 0 ){
		PRINTF( reply );
	    }
            if ( do_while && strstr( reply, do_while ) ){
                found_while = 1;
            }
            if ( do_until && strstr( reply, do_until ) ){
                found_until = 1;
            }
        }

        if ( nout == 0){
            perror( "fgets() ret 0" );
            return 1;
        }else{

	    if ( verbose==1 ) fputs( "+", stdout );
	    
            if ( found_until ){
                PRINTF( "SUCCESS - found until\n" );
                return 0;
            }else if ( do_until==0 && !found_while ){
		    PRINTF( "SUCCESS2 - !while\n" );
		    return 0;
            }else if ( (do_while!=0 && found_while) || do_while==0 ){
		    if (verbose){
			    sleep(1);
		    }
		    continue;
            }else{
		    perror( "while or until not found" );
		    return 1;
            }
        }
    }
}

#define ISCOMMENT( cl ) (cl[0]=='#')
#define ISBLANK(cl)     (strlen(cl)<=1)

static int process_stdin( FILE* fd )
{
    char command_line[128];
    char reply[80];
    int nout;

    while ( fgets( command_line, sizeof(command_line), stdin ) ){
        PRINTF( command_line );
        if ( !ISCOMMENT( command_line ) && !ISBLANK( command_line ) ){
            fputs( command_line, fd );

            for ( nout = 0; fgets( reply, sizeof(reply), fd )!=NULL; ++nout ){
                fputs( reply, stdout );
            }

            if ( nout == 0){

                perror( "fgets() ret 0" );
                return 1;
            }
        }
    }

    return 0;
}




