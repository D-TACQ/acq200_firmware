/*****************************************************************************
 *
 * File: tcpserver.c
 *
 * $RCSfile: tcpserver.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: dt100d network socket handling.
 *
 * Refs: Copied from Snader (Effective TCP/IP Programming by Jon Snader)
 *
 * $Id: tcpserver.c,v 1.9 2008/08/27 19:10:07 pgm Exp $
 * $Log: tcpserver.c,v $
 * Revision 1.9  2008/08/27 19:10:07  pgm
 * word_size is a factor in read
 *
 * Revision 1.8  2006/05/06 12:40:29  pgm
 * implement environment controlled data buffer and timeout
 *
 * Revision 1.7  2006/04/21 09:27:36  pgm
 * first time data buf is 4Mb, the rest are 256K
 *
 * Revision 1.6  2004/10/20 08:03:56  pgm
 * B1001 removes acqcmd -- marks and has close opt
 *
 * Revision 1.5  2004/05/30 15:41:28  pgm
 * accumulating read to smooth driver buffer variations
 *
 * Revision 1.4  2004/05/08 14:08:34  pgm
 * live streaming ship it quick
 *
 * Revision 1.3  2004/05/01 11:36:27  pgm
 * init test data version
 *
 * Revision 1.2  2004/03/06 22:08:56  pgm
 * remote shell its a runner
 *
 * Revision 1.1.1.1  2004/03/03 21:41:11  pgm
 *
 *
 * Revision 1.9  2004/02/28 20:36:54  pgm
 * data server with endpoint detect
 *
 * Revision 1.8  2004/02/24 15:55:13  pgm
 * no line mode for complete packets
 *
 * Revision 1.7  2004/02/24 14:52:32  pgm
 * remove line buffer - not wanted, use flush on lines
 *
 * Revision 1.6  2004/02/17 12:48:02  pgm
 * data upload multi thread good
 *
 * Revision 1.2.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/




/* include includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "skel.h"
#include "popt.h"

#include "local.h"

#define NLISTEN 20   /* backlog: many data connections may Q */


#define VERID \
    "$Id: tcpserver.c,v 1.9 2008/08/27 19:10:07 pgm Exp $ $Revision: 1.9 $ "\
    "BUILD 1001\n"


unsigned S_BUFLEN = SKT_BUFLEN;


const char *program_name;

/* end includes */
/* error - print a diagnostic and optionally exit */
void error( int status, int err, char *fmt, ... )
{
	va_list ap;

	va_start( ap, fmt );
	fprintf( stderr, "%s: ", program_name );
	vfprintf( stderr, fmt, ap );
	va_end( ap );
	if ( err )
		fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
	if ( status )
		EXIT( status );
}

/* set_address - fill in a sockaddr_in structure */
static void set_address( 
	const char *hname, const char *sname,
	struct sockaddr_in *sap, char *protocol )
{
	struct servent *sp;
	struct hostent *hp;
	char *endptr;
	short port;

	bzero( sap, sizeof( *sap ) );
	sap->sin_family = AF_INET;
	if ( hname != NULL )
	{
		if ( !inet_aton( hname, &sap->sin_addr ) )
		{
			hp = gethostbyname( hname );
			if ( hp == NULL )
				error( 1, 0, "unknown host: %s\n", hname );
			sap->sin_addr = *( struct in_addr * )hp->h_addr;
		}
	}
	else
		sap->sin_addr.s_addr = htonl( INADDR_ANY );
	port = strtol( sname, &endptr, 0 );

	if ( *endptr == '\0' )
		sap->sin_port = htons( port );
	else
	{
		sp = getservbyname( sname, protocol );
		if ( sp == NULL )
			error( 1, 0, "unknown service: %s\n", sname );
		sap->sin_port = sp->s_port;
	}
}


extern void interpreter(SOCKET s);
extern void interp_init(void);

/* server - place holder for server */
static void server( SOCKET s, struct sockaddr_in *peerp )
{
	static int _id;
	int id;

	id = ++_id;

	if (fork()==0){
		dbg(1, "server %d %d HELLO\n", id, getpid());
		interpreter(s);
		shutdown(s, SHUT_RDWR);
		dbg(1, "server %d close %d\n", id, getpid());
		_exit(0);
	}else{
		CLOSE(s);  /* must shut our side */
	}
}


static void reaper(int sig)
{
	int ws, rc;

	while ((rc = waitpid(-1, &ws, WNOHANG)) > 0){
		dbg(1, "reaper pid:%d status:%d\n", rc, ws);
	}
}

static SOCKET build_socket(void)
{
	const int on = 1;
	SOCKET s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "socket call failed" );

	if ( setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &on,
		 sizeof( on ) ) )
		error( 1, errno, "setsockopt failed" );

	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF,
		       (char*)&S_BUFLEN, sizeof(S_BUFLEN)) ){
		error(1, errno, "setsockopt() SO_RCVBUF failed\n");
	}
	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF,
		       (char*)&S_BUFLEN, sizeof(S_BUFLEN)) ){
		error(1, errno, "setsockopt() SO_SNDBUF failed\n");
	}
	return s;
}

int G_verbose;
int G_use_seek;
int acq200_debug;

static int help(void)
{
	fprintf(stderr, "help\n" );
	return 1;
}

extern char* MASTERINTERP_VERID;

/* main - TCP setup, listen, and accept */
int main( int argc, const char **argv )
{
        struct poptOption opt_table[] = {
                { "help",       'h', POPT_ARG_NONE,   0,           'h' },
                { "verbose",    'v', POPT_ARG_INT,    &G_verbose,    0   },
		{ "use_seek",   'u', POPT_ARG_INT,    &G_use_seek,   0 },
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 0 },
		{}
	};
	

	struct sockaddr_in local;

	const char *hname = NULL;
	const char *sname = "0xd100";

	SOCKET s;
	int rc;
	const char *arg;

	poptContext context = poptGetContext(
                argv[0], argc, argv, opt_table, 0 );


	info(VERID);
	info("%s\n", MASTERINTERP_VERID);

	INIT();

        while((rc =poptGetNextOpt(context)) > 0){
                switch(rc) {
                case 'h':
                        return help();
                }
        }
                                                                                
	if ((arg = poptGetArg(context)) != 0){
		if (poptPeekArg(context)){
			hname = arg;
			sname = poptGetArg(context);
		}else{
			sname = arg;
		}
	}

	signal(SIGCHLD, reaper);

	set_address( hname, sname, &local, "tcp" );
	s = build_socket();

	if (bind(s, (struct sockaddr *)&local, sizeof(local))){
		error(1, errno, "bind failed");
	}

	if (listen( s, NLISTEN )){
		error( 1, errno, "listen failed" );
	}

	interp_init();

	do
	{
		struct sockaddr_in peer;
		int peerlen = sizeof( peer );

		SOCKET s1 = accept(s, (struct sockaddr *)&peer, &peerlen);

		if (!isvalidsock( s1 )){
			error( 1, errno, "accept failed" );
		}
		server( s1, &peer );
	} while ( 1 );

	EXIT( 0 );
}
