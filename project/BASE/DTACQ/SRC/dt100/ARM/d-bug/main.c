/*****************************************************************************
 *
 * File: main.c
 *
 * $RCSfile: main.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: main.c,v 1.1.2.37 2009/09/02 20:31:46 pgm Exp $
 * $Log: main.c,v $
 * Revision 1.1.2.37  2009/09/02 20:31:46  pgm
 * B2217 supports ACQ164
 *
 * Revision 1.1.2.36  2009/03/30 07:35:20  pgm
 * B2212 - locks out commands in !=ST_STOP
 *
 * Revision 1.1.2.35  2008/03/25 15:52:00  pgm
 * acq132
 *
 * Revision 1.1.2.34  2007/05/07 14:19:10  pgm
 * ACQ200_SLAVEPRIO
 *
 * Revision 1.1.2.33  2007/04/18 19:44:20  pgm
 * handles AB buffers part 1
 *
 * Revision 1.1.2.32  2007/01/05 12:56:48  pgm
 * restore setDIO to old mbox, also abort
 *
 * Revision 1.1.2.31  2007/01/04 09:04:49  pgm
 * minimise printf output
 *
 * Revision 1.1.2.30  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.1.2.29  2007/01/03 12:31:38  pgm
 * MU interrupts rock! - .7s for 100 commands.
 *
 * Revision 1.1.2.28  2007/01/02 17:46:16  pgm
 * usc call acqcmd works @ 25 cps
 *
 * Revision 1.1.2.27  2007/01/01 14:39:35  pgm
 * mbox control works
 *
 * Revision 1.1.2.26  2006/12/24 15:03:25  pgm
 * NO_MBOX_THREAD made effective
 *
 * Revision 1.1.2.25  2006/08/19 15:31:29  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.24  2006/08/19 12:53:11  pgm
 * dual thread handles mbox calling interp
 *
 * Revision 1.1.2.23  2006/04/01 14:40:04  pgm
 * ensures minimum transient time 1s.
 * rarely seems to be needed :-).
 *
 * Revision 1.1.2.22  2006/01/28 16:19:28  pgm
 * dont die on command socket error
 *
 * Revision 1.1.2.21  2005/07/26 12:19:15  pgm
 * B2111
 *
 * Revision 1.1.2.20  2005/05/16 21:00:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.19  2005/05/16 08:49:58  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.18  2005/02/06 17:07:09  pgm
 * accepts N (N=4) localcommand streams
 *
 * Revision 1.1.2.17  2005/02/06 16:15:11  pgm
 * accepts 2 localslave clients
 *
 * Revision 1.1.2.16  2004/12/16 17:06:09  pgm
 * fix zero prelen kludges
 *
 * Revision 1.1.2.15  2004/11/01 12:22:27  pgm
 * 2100
 *
 * Revision 1.1.2.14  2004/10/25 09:19:18  pgm
 * dma experiments - no work, no crash
 *
 * Revision 1.1.2.13  2004/10/14 20:12:25  pgm
 * no warnings
 *
 * Revision 1.1.2.12  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.1.2.11  2004/07/25 15:40:26  pgm
 * ll hooks work
 *
 * Revision 1.1.2.10  2004/04/25 12:57:47  pgm
 * concurrent queries work
 *
 * Revision 1.1.2.9  2004/04/25 11:40:53  pgm
 * concurrency 1 with logging
 *
 * Revision 1.1.2.8  2004/03/23 22:45:45  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.7  2004/02/15 21:39:20  pgm
 * local slave command handling
 *
 * Revision 1.1.2.6  2004/02/15 14:40:27  pgm
 * local-slave stub
 *
 * Revision 1.1.2.5  2004/02/15 13:23:47  pgm
 * localSlave socket in place
 *
 * Revision 1.1.2.4  2004/02/14 23:20:44  pgm
 * usc separated
 *
 * Revision 1.1.2.3  2004/02/14 23:13:22  pgm
 * OO sockets are us (monitoring stdin bad idea in batch mode ie when its /dev/null
 *
 * Revision 1.1.2.2  2004/02/14 18:04:33  pgm
 * split out connector stuff from d-bug to main
 *
 * Revision 1.1.2.1  2004/02/10 07:35:17  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.11  2003/12/12 14:19:15  pgm
 */

/*
 * new main for arm app. 
 * we go to C++ for better command dispatcher, command parsing and
 * future better GPEM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/resource.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


#include <assert.h>
#include <errno.h>

#include "local.h"

#include "acq200.h"
#include "acq200_hal.h"
#include "usc.h"

#include "mbox_slave.h"

#include "processMessage.h"
#include "acq32_drv.h"

#define MBOX_POLL_PRIO	19	/* poll at v.low priority */

void init_stubs(void);
void pgm_monitor(void);

void pgm_init(void);
void pgmDemonOnce(char *buf);

struct IoBuf;

void localSlave(struct IoBuf *iob);


static void handleChld(int signum)
{
	int status;
	wait(&status);
}

int G_command_log = 0;

static void logCommand(struct IoBuf *the_iob)
{
	if (G_command_log){
		info("RX %s", the_iob->in.buf);
		info("TX %s", the_iob->out.buf);
	}
}


static void process_env()
{
	char *value;

	if ((value = getenv("ACQ200_DMAMODE"))){
		acq200_setTransferModeDma(atoi(value));
	}
	if ((value = getenv("ACQ200_CMDLOG"))){
		G_command_log = atoi(value);
	}
	if ((value = getenv("ACQ200_OIMR"))){
		init_OIMR(value);		
	}
	if ((value = getenv("ACQ200_INTACK"))){
		acq32_setCommandInterruptAck(strtol(value, 0, 0));
	}
}

static void init_signals(void)
{
	/* static because I think this may still be in use once out of scope */
	struct sigaction sa = {
		.sa_handler = handleChld
	};
	if (sigaction(SIGCHLD, &sa, NULL)){
		perror("sigaction");
	}

	info("installed handler for SIGCHLD");
}

void clear_signals(void)
{
	/* static because I think this may still be in use once out of scope */
	struct sigaction sa = {
		.sa_handler = SIG_DFL
	};
	if (sigaction(SIGCHLD, &sa, NULL)){
		perror("sigaction");
	}
}

extern const char* getBuildDate( char client_buf[], int maxbuf );

int main( int argc, char* argv[] )
/* a main() - YIPPEE! */
{
	int bt;

	if ( argc >= 2 ){
		if (STREQNL(argv[1], "wav232")){
			bt = BT_WAV232;
		}else if (STREQNL(argv[1], "acq216")){
			bt = BT_ACQ216|BT_ACQ;
		}else if (STREQNL(argv[1], "acq196")){
			bt = BT_ACQ196|BT_ACQ;
		}else if (STREQNL(argv[1], "acq232")){
			bt = BT_ACQ232|BT_ACQ;
		}else if (STREQNL(argv[1], "acq132")){
			bt = BT_ACQ132|BT_ACQ;
		}else if (STREQNL(argv[1], "acq164")){
			bt = BT_ACQ164|BT_ACQ;
		}else if (STREQNL(argv[1], "acq000")){
			bt = 0;
		}else{
			err("ERROR: Board Type \"%s\" Not Recognised\n",
			       argv[1] );
			goto zombie;
		}

		acq200_setBoardType(bt);
		info("acq200control |%d| $Revision: 1.1.2.37 $ Board Type %x\n",			getpid(), bt);
	}else{
		err("ERROR: Please spec Board Type\n");
		goto zombie;
	}

	process_env();
	init_signals();
	init_stubs();
	pgm_monitor();
	return -1;

 zombie:
	printf("Sleep forever (1hr)\n");
	puts(getBuildDate(0, 0));
	sleep(3600);         /* sleep forever to fool init */
	return -1;
}


enum TOP_STATE {
	TS_IDLE,
	TS_ABORT,
	TS_MBOX_SLAVE,
	TS_PGM_DEMON,
	TS_LOCAL_SLAVE,
	TS_INIT,
};



#define IBUF(state)  (((state)&0xf0)>>4)
#define SET_LOCAL_SLAVE(ibuf)  (((ibuf)<<4)|TS_LOCAL_SLAVE)
#define TS(state)   ((state)&0x0f)



#define iob_deb (iob[0])
#define iob_cmd (iob[1])


static void flush(struct UnixSocketConnection *conn, struct IoBuf *iob)
{
	int rc = 0;

	rc = write(conn->fd_active, iob->out.buf, iob->out.len);
	if (rc <= 0) err("write failed %d", __LINE__);
		
	rc = write(conn->fd_active, "EOF\n", 4);
	if (rc <= 0) err("write failed %d", __LINE__);

	rc = fsync(conn->fd_active);     /* willit work ?? */
#ifdef PGMCOMOUT
	if (rc < 0) dbg(1, "WARNING:fsync() failed %d", __LINE__);
#endif
}

int new_state_debug;

#define DBG	if (new_state_debug) dbg

enum TOP_STATE new_state(
	enum TOP_STATE state, 
	struct timeval *on_timeout,
	int niob,
	struct IoBuf *iob[]
	)
{

	static int first_time = 1;
	static struct FdPool fdpool;
	static struct UnixSocketConnection **conns;

	static fd_set watchset;
	static sigset_t blocker;  /* block SIGPIPE */

	struct timeval timeout;
	fd_set inset;
	fd_set exceptset;

	static struct UnixSocketConnection debug = {
		.connector.name = DEBUG_SOCK
	};
	static struct UnixSocketConnection command = {
		.connector.name = COMMAND_SOCK
	};
	
	int rc;

	if (first_time){
		addFd(&fdpool, uscMake(&debug));
		addFd(&fdpool, uscMake(&command));

		conns = malloc(sizeof(struct UnixSocketConnection*) * niob);

		conns[0] = &debug;
		conns[1] = &command;

		int ic;

		for (ic = 2; ic < niob; ++ic){
			struct UnixSocketConnection *con = 
				malloc(sizeof(struct UnixSocketConnection));
			memcpy(con, &command, 
			       sizeof(struct UnixSocketConnection));
			conns[ic] = con;
		}
		FD_ZERO(&watchset);
		FD_SET(debug.fd_connector, &watchset);
		FD_SET(command.fd_connector, &watchset);

		sigemptyset(&blocker);
		sigaddset(&blocker, SIGPIPE);
		sigprocmask(SIG_BLOCK, &blocker, 0);

		first_time = 0;
	}

	switch(TS(state)){
	case TS_LOCAL_SLAVE:
		flush(conns[IBUF(state)], iob[IBUF(state)]);
		state = TS_MBOX_SLAVE;
		break;
	default:
		;
	}
	
	inset = watchset;
	exceptset = watchset;
	memcpy(&timeout, on_timeout, sizeof(struct timeval));

	rc = select(maxFd(&fdpool), &inset, 0, &exceptset, &timeout);
	if ( rc < 0 ){
		if (errno == EINTR){
			DBG(1, "select was interrupted");
		}else{		
			err("select return error %d", errno);
			_exit(-errno);
		}
	}else if (rc == 0){
		return TS_MBOX_SLAVE;
	}else{
		int ic;
/*
 * handle all incoming data - debug is lowest priority 
 */
		for (ic = niob - 1; ic >= 0; --ic){
			struct UnixSocketConnection *con = conns[ic];

			if (uscDataPending(con, &exceptset)){
				DBG(2, "exception cleanup %d", ic);
				FD_CLR(con->fd_active, &watchset);
				removeFd(&fdpool, con->fd_active);
				uscClose(con);
			}
		}

		for (ic = niob - 1; ic >= 0; --ic){
			struct UnixSocketConnection *con = conns[ic];
			
			if (uscDataPending(con, &inset)){
				rc = read(con->fd_active, 
					  iob[ic]->in.buf, iob[ic]->in.maxlen);
				if (rc <= 0){
					if (rc < 0){
						err("error on read %d",
						    con->fd_active);

					}else{
						DBG(2,"read(0) cleanup %d %d",
							    ic, con->fd_active);
					}

					FD_CLR(con->fd_active, &watchset);
					removeFd(&fdpool, con->fd_active);
					uscClose(con);
				}else{
					DBG(2, "ic %d fd %d data %d", 
							ic, con->fd_active, rc);

					iob[ic]->in.len = rc;
					if (ic == 0){
						return TS_PGM_DEMON;
					}else{
						return SET_LOCAL_SLAVE(ic);
					}
				}
			}		
		}
		if (uscConnectionPending(&command, &inset)){
			DBG(2, "command connect");
			for (ic = niob - 1; ic > 0; --ic){
				struct UnixSocketConnection *con = conns[ic];

				if (uscAccept(con) >= 0){

					

					FD_SET(con->fd_active, &watchset);
					addFd(&fdpool, con->fd_active);
					break;
				}
			}

			if (ic == 0){
				DBG(2, "command limit: BLOCK");
			}
		}
		if (uscConnectionPending(&debug, &inset)){
			if (uscAccept(&debug) >= 0 ){
				FD_SET(debug.fd_active, &watchset);
				addFd(&fdpool, debug.fd_active);

				/* redirect printf */
				close(1); dup(debug.fd_active);
				close(2); dup(debug.fd_active);
				/* lets hope stdout still assoc with fd=1 */
				/* force line buf */
				setvbuf(stdout, NULL, _IOLBF, 0);
			}
		}

		return TS_MBOX_SLAVE;
	}
	return state;
}

extern void init_stubs(void);

extern void mboxSlaveOnce();

static void mboxSetSlavePriority(void)
{
	const char* pv = getenv("ACQ200_SLAVEPRIO");
	 
	if (pv){
		int pid = getpid();
		int priority;
		int rc;

		if (strcmp(pv, "-") == 0){
			priority = MBOX_POLL_PRIO;
		}else{
			priority = atoi(pv);
		}
		rc = setpriority(PRIO_PROCESS, pid, priority);
		if (rc != 0){
			perror("setpriority failed");
		}
		info("setpriority pid:%d prio:%d rc:%d", pid, priority, rc);
	}
}


static void* mboxSlaveLoop(void* nothing) 
{
	mboxSetSlavePriority();

	while(1){
		mboxSlaveOnce(MBOX_SLAVE_ONCE_BLOCK);
	}
}

#define MAX_COMMAND_STREAMS 4
#define NIOB (MAX_COMMAND_STREAMS+1)




#include <pthread.h>


void pgm_monitor()
{
	struct IoBuf *iob[NIOB];
	enum TOP_STATE state = TS_MBOX_SLAVE;
	struct timeval timeout = {
		.tv_sec = 0,
		.tv_usec = 10000
	};	
	iob_deb = iobCreate( 1024, 1024 );
	
	int ic;


	int use_mbox_thread = getenv("NO_MBOX_THREAD")  == 0;

	for (ic = 1; ic <= MAX_COMMAND_STREAMS; ++ic){
		iob[ic] = iobCreate( 1024, 4096 );
	}

	pgm_init();

	if (use_mbox_thread){
		int rc;
		pthread_t mbox_thread;
		pthread_attr_t mbox_attr;


		pthread_attr_init(&mbox_attr);
		pthread_attr_setdetachstate(
			&mbox_attr, PTHREAD_CREATE_DETACHED);
		rc = pthread_create(
			&mbox_thread, &mbox_attr, mboxSlaveLoop, 0);
		if (rc != 0){
			err("ERROR:failed to create mbox thread %d", rc);
		}
		info("thread created");
	}


	for (;;){
		switch(TS(state = new_state(state, &timeout, NIOB, iob))){
		case TS_MBOX_SLAVE:
			if (!use_mbox_thread){
				mboxSlaveOnce(0);
			}
			break;
		case TS_PGM_DEMON:
			iob_deb->in.buf[iob_deb->in.len] = '\0';
			pgmDemonOnce(iob_deb->in.buf);
			break;
		case TS_INIT:
			pgm_init();
			state = TS_IDLE;
			break;
		case TS_ABORT:
			return;
		case TS_IDLE:
			state = TS_MBOX_SLAVE;
			break;
		case TS_LOCAL_SLAVE: {
			struct IoBuf *the_iob = iob[IBUF(state)];
			the_iob->in.buf[the_iob->in.len] = '\0';
			localSlave(the_iob);
			logCommand(the_iob);
			break;
		}
		}

		if (!use_mbox_thread){
			mbox_getState();
		}
	}

	for (ic = 0; ic <= MAX_COMMAND_STREAMS; ++ic){
		iobDestroy(iob[ic]);
	}
}

