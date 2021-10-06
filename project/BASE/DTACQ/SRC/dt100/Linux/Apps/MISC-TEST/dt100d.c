/*****************************************************************************
 *
 * File: dt100d.c
 *
 * $RCSfile: dt100d.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: generic daemon.
 * 
 * First instance - clones and returns
 * Second instance - setsuid, pgid, spawns call program
 * On HUP, kills all procs in pgid
 *
 * $Id: dt100d.c,v 1.1 2002/05/31 08:31:17 pgm Exp $
 * $Log: dt100d.c,v $
 * Revision 1.1  2002/05/31 08:31:17  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>

#define UID 1000   /* dt100 std uid */
#define GID 1001

void hupHandler( int signum )
{
    ; /* no action required */
}

int main( int argc, char** argv )
{
    pid_t daemon;
    pid_t worker;

    if ( (daemon = fork()) == 0 ){
	if ( setuid( UID ) == 0 ){
	    if ( setpgid( 0, 0 ) == 0 ){
		if ( (worker = fork()) == 0 ){
		    if ( execvp( argv[1], &argv[1] ) == -1 ){
			perror( "execvp failed" );
			return 1;
		    }else{
			return 1;		    // else no return here
		    }
		}else{
		    struct sigaction sa;
		    int status;

		    memset( &sa, 0, sizeof(sa) );
		    sa.sa_handler = hupHandler;

		    if ( sigaction( SIGTERM, &sa, NULL ) ){
			perror( "dt100d sigaction() failed " );
			return 1;
		    }else{
			pause();
			kill( 0, SIGKILL );
		    }
		    wait( &status );
		    return 0;
		}		
	    }else{
		perror( "dt100d setgid() failed" );
		return 1;
	    }
	}else{
	    perror( "dt100d setuid() failed" );
	    return 1;
	}
    }else{
	/* WORKTO .. should linger until daemon is happy ... */
	return 0;
    }
}

