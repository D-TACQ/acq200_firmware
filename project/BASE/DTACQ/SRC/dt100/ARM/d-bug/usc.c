/*****************************************************************************
 *
 * File: usc.c
 *
 * $RCSfile: usc.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: impl of usc wrapper
 *
 * $Id: usc.c,v 1.1.2.5 2009/03/30 07:35:20 pgm Exp $
 * $Log: usc.c,v $
 * Revision 1.1.2.5  2009/03/30 07:35:20  pgm
 * B2212 - locks out commands in !=ST_STOP
 *
 * Revision 1.1.2.4  2007/01/02 17:46:16  pgm
 * usc call acqcmd works @ 25 cps
 *
 * Revision 1.1.2.3  2005/02/06 16:15:11  pgm
 * accepts 2 localslave clients
 *
 * Revision 1.1.2.2  2004/02/15 13:23:47  pgm
 * localSlave socket in place
 *
 * Revision 1.1.2.1  2004/02/14 23:20:44  pgm
 * usc separated
 *
 * Revision 1.1.2.3  2004/02/14 23:13:22  pgm
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>
#include <errno.h>

#include "local.h"

#define acq200_debug usc_debug

#include "acq200.h"
#include "usc.h"


int usc_debug;

int uscMake(struct UnixSocketConnection *cn)
{
	char cmd[80];

	if ((cn->fd_connector = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
		die("socket");
	}

	unlink(cn->connector.name);
	cn->connector.unx_addr.sun_family = AF_UNIX;
	strcpy(cn->connector.unx_addr.sun_path, cn->connector.name);
	cn->connector.addr_len = 
		sizeof(cn->connector.unx_addr.sun_family)+
		strlen(cn->connector.unx_addr.sun_path);
	cn->connector.address = (struct sockaddr *)&cn->connector.unx_addr;

	if (bind(cn->fd_connector, 
		 cn->connector.address,
		 cn->connector.addr_len)){
		die("bind");
	}

	if(listen(cn->fd_connector, 5)){
		die("listen");
	}
	sprintf(cmd, "chmod a+rw %s", cn->connector.name);	
	system(cmd);

	return cn->fd_connector;
}


int uscAccept(struct UnixSocketConnection *cn)
{
	if (cn->fd_active == 0){
		cn->fd_active = accept(cn->fd_connector,
				       cn->connector.address,
				       &cn->connector.addr_len);
		assert(cn->fd_active >= 0 );
		dbg(2, "accept fd:%d",  cn->fd_active);
		return cn->fd_active;
	}else{
		dbg(1, "fd_active %d - return -EBUSY", cn->fd_active);
		return -EBUSY;
	}
}

void uscClose(struct UnixSocketConnection *cn)
{
	close(cn->fd_active);
	cn->fd_active = 0;
}


struct IoBuf* iobCreate(int in_max, int out_max)
{
	struct IoBuf *iob = calloc(1, sizeof(struct IoBuf));

	iob->in.buf = calloc(iob->in.maxlen = in_max, 1);
	iob->out.buf = calloc(iob->out.maxlen = out_max, 1);
	
	assert(iob->in.buf);
	assert(iob->out.buf);

	return iob;
}

void iobDestroy(struct IoBuf* iob)
{
	free(iob->in.buf);
	free(iob->out.buf);
	free(iob);
}

int getUnixSocket(const char* _path) 
/** client function */
{
	struct sockaddr_un address;
	int sock;	

	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(-errno);
	}
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, _path);
		
	int addrLen = sizeof(address.sun_family) + strlen(address.sun_path);

	if (connect(sock, (struct sockaddr *)&address, addrLen)){
		perror( "connect failed SOCK" );
		exit(-errno);
	}	
	return sock;
}

