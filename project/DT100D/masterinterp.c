/*****************************************************************************
 *
 * File: masterinterp.c
 *
 * $RCSfile: masterinterp.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: master interpreter for dt100d.
 * 
 * Refs: MasterInterpreterFactory.java
 *
 * $Id: masterinterp.c,v 1.23 2008/08/27 19:10:07 pgm Exp $
 * $Log: masterinterp.c,v $
 * Revision 1.23  2008/08/27 19:10:07  pgm
 * word_size is a factor in read
 *
 * Revision 1.22  2006/10/02 16:14:06  pgm
 * shell supports longer lines, continuations and converts ';' to 'n' so that heredocs can work
 *
 * Revision 1.21  2006/05/06 12:40:29  pgm
 * implement environment controlled data buffer and timeout
 *
 * Revision 1.20  2006/04/25 14:00:38  pgm
 * Uses a 4MB data buffer. It works
 *
 * Revision 1.19  2006/04/21 09:27:36  pgm
 * first time data buf is 4Mb, the rest are 256K
 *
 * Revision 1.18  2005/11/17 08:59:29  pgm
 * restores shell exit/bye shutdown following long output mod
 *
 * Revision 1.17  2005/08/23 09:12:01  pgm
 * process_reply, unlimited length, no duplicate result
 *
 * Revision 1.16  2005/08/22 16:25:52  pgm
 * remove restriction on reply length
 *
 * Revision 1.15  2005/05/21 15:07:12  pgm
 * binhead for LABVIEW type clients
 *
 * Revision 1.14  2004/10/20 08:03:56  pgm
 * B1001 removes acqcmd -- marks and has close opt
 *
 * Revision 1.13  2004/10/12 19:39:26  pgm
 * chomp extra line in sys
 *
 * Revision 1.12  2004/10/12 19:36:45  pgm
 * add sys type, make shell generic
 *
 * Revision 1.11  2004/07/17 19:27:21  pgm
 * EOF only for shell
 *
 * Revision 1.10  2004/06/06 12:59:34  pgm
 * multi-frame streaming works layer1
 *
 * Revision 1.9  2004/05/30 20:30:08  pgm
 * 256K data buf
 *
 * Revision 1.8  2004/05/30 15:41:28  pgm
 * accumulating read to smooth driver buffer variations
 *
 * Revision 1.7  2004/05/08 14:08:34  pgm
 * live streaming ship it quick
 *
 * Revision 1.6  2004/05/01 11:36:27  pgm
 * init test data version
 *
 * Revision 1.5  2004/04/17 20:33:19  pgm
 * refine getBoards
 *
 * Revision 1.4  2004/04/17 16:10:02  pgm
 * removed acq2sh kludge
 *
 * Revision 1.3  2004/03/06 22:08:56  pgm
 * remote shell its a runner
 *
 * Revision 1.2  2004/03/06 18:35:58  pgm
 * One shot acq2sh works. But it's very crude.
 * There has to be a better way!
 *
 * Stahsing this for now, nextstop socketpair()
 *
 * Revision 1.1.1.1  2004/03/03 21:41:11  pgm
 *
 *
 * Revision 1.10  2004/02/28 20:36:54  pgm
 * data server with endpoint detect
 *
 * Revision 1.9  2004/02/24 15:55:13  pgm
 * no line mode for complete packets
 *
 * Revision 1.8  2004/02/24 14:52:32  pgm
 * remove line buffer - not wanted, use flush on lines
 *
 * Revision 1.7  2004/02/17 12:48:02  pgm
 * data upload multi thread good
 *
 * Revision 1.2.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/

char* MASTERINTERP_VERID = 
	"$Id: masterinterp.c,v 1.23 2008/08/27 19:10:07 pgm Exp $\n"
	"$Revision: 1.23 $ B1008";

/*
 * Masterinterp.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <netinet/in.h>

#include <fcntl.h>

#include "skel.h"


#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


#include "local.h"

#define STREQ( s1, s2 )        (strcmp( s1, s2 ) == 0)
#define STREQN( s1, s2, n )    (strncmp( s1, s2, n ) == 0) 
#define STREQNL( s1, s2 )      (strncmp( s1, s2, strlen(s2) ) == 0)
#define MAX(aa,bb)      ((aa)>=(bb)?(aa):(bb))
#define MIN(aa,bb)      ((aa)<=(bb)?(aa):(bb))


#define STREAMPROC  "dt100-stream"

#define ACQCMD_DEV "/tmp/acq200control_command"
#define DT100PROMPT "DT100:"

#define DT100PROMPTEND fprintf(crp->fd_net, "%s\n", DT100PROMPT)


#define CMD_BUFLEN 4096
#define _DAT_BUFLEN 0x40000


#define DAT_BUFLEN getDataBufLen()

struct command_resources {
#define CRP struct command_resources *
	SOCKET id_net;
	FILE *fd_net;           /* outside world connection */
	int id_acq;             /* connection to internal (low level) */
	FILE *fd_acq;           /* buffered connection to internal    */
	int acq_type;           /* ACQ_TYPE_MASTER, ACQ_TYPE_DATA       */
	int channel;            /* channel 1..N if DATA               */
	int buflen;
	char *buf;
	int (*split)(char *str, char*argv[], int maxargs);
	int (*decoder)(int argc, char* argv[], CRP crp);
	pid_t cpid;
	struct ReadMode {
		int mode;
		int value;
	} sample_read_start, sample_read_stride;
	int seqnum;
	char knob_file[80];
	int binhead;
};

#define SAMPLE_READ_MODE_NEVER 0    /* ignore start, stride */
#define SAMPLE_READ_MODE_ONCE  1    /* act on start , stride first time */
#define SAMPLE_READ_MODE_ALWAYS 2   /* update start, stride every time */

#define SAMPLE_READ_VALID 0x80000000

#define SAMPLE_READ_IS_VALID(sr) ((sr&SAMPLE_READ_VALID) != 0)
#define SAMPLE_READ_VALUE(sr) (sr&~SAMPLE_READ_VALID)


#define SAMPLE_READ_BINHEAD 1  /* binary header */

#define ACQ_TYPE_NONE   0
#define ACQ_TYPE_MASTER 1
#define ACQ_TYPE_DATA   2
#define ACQ_TYPE_SHELL  3
#define ACQ_TYPE_KNOB   4

typedef int (*CmdFn)( int argc, char* argv[], CRP crp);

#define CMDFN(name) \
static int name(int argc, char* argv[], CRP crp)

struct command_def {
    char* key;
    char *prams;
    char* help;
    CmdFn cmd;
};


#define NETPRINTF(fmt, args...) do { \
               fprintf(crp->fd_net, fmt, ## args);fflush(crp->fd_net);\
       } while(0)
 


static int split(char *str, char *argv[], int maxargs);
static int nosplit(char *str, char *argv[], int maxargs)
{
	argv[0] = str;
	return 1;
}

static int process_command(int argc, char* argv[],  CRP crp);
static void make_connection(const char *device, CRP crp);

static int sample_size = sizeof(short);

static int getDataBufLen(void)
{
	int len;

	if (getenv("DT100D_DATABUFLEN") && 
	    (len = strtol(getenv("DT100D_DATABUFLEN"),0,0)) > 0){
		return len;
	}else{
		return _DAT_BUFLEN;
	}
}

static void setSample(
	CRP crp,
	const char *func, int value, struct ReadMode *state)
{
	if (state->mode == SAMPLE_READ_MODE_NEVER){
		return;
	}else{
		char fname[80];
		FILE* fp;

		sprintf(fname, "/dev/dtacq/%s", func);
		fp = fopen(fname, "w");

		if (!fp){
			NETPRINTF( "ERROR: failed to open %s %d\n", 
				   fname, errno);		
			return;
		}
		fprintf(fp, "%d\n", state->value = value);
		fclose(fp);

		if (state->mode == SAMPLE_READ_MODE_ONCE){
			state->mode = SAMPLE_READ_MODE_NEVER;
		}
	}
}

static void setSampleReadStart(CRP crp, int start)
{
	setSample(crp, "sample_read_start", start, &crp->sample_read_start);
}

static void setSampleReadStride(CRP crp, int stride)
{
	setSample(crp, "sample_read_stride", stride, &crp->sample_read_stride);
}
static void crpKill(CRP crp)
{
	if (crp->cpid){
		kill(crp->cpid, SIGKILL);
		wait(0);
		crp->cpid = 0;
	}
}
static CRP crpConstruct(
	SOCKET s, 
	int (*decoder)(int argc, char* argv[], CRP crp) )
{
	CRP crp = calloc(1, sizeof(struct command_resources));

	if (!crp) return 0;

	crp->id_net = s;
	crp->fd_net = fdopen(s, "r+");
	crp->split = split;
	crp->decoder = decoder;	
	return crp;
}

static void crpClose(CRP crp)
{
	if (crp->fd_acq){
		fclose(crp->fd_acq);
		crp->fd_acq = 0;
	}
	if (crp->id_acq){
		close(crp->id_acq);
		crp->id_acq = 0;
	}
	crp->acq_type = 0;
	if (crp->buf) free(crp->buf);
}

static void crpDestroy(CRP crp)
{
	crpClose(crp);
	fclose(crp->fd_net);
	crpKill(crp);
	free(crp);
}



static void alarm_handler(int signum) {
        err("TIMEOUT");
        exit( -ETIMEDOUT );
}



static void installAlarmHandler() {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = alarm_handler;
        if (sigaction(SIGALRM, &sa, NULL)) perror ("sigaction");
}


static void setInactivityAlarm(int enable)
{
	static int to;

	if (to == 0){
		if (getenv("DT100D_TIMEOUT")){
			to = atoi(getenv("DT100D_TIMEOUT"));
			if (to > 0){
				installAlarmHandler();
				dbg(1, "timeout set %d", to);
			}else{
				to = -1;
			}
		}
	}

	if (to > 0){
		dbg(1, "alarm(%d)", enable? to: 0);
		alarm(enable? to: 0);
	}
}


void interpreter(SOCKET s)
{
	static char inbuf[4096];
	CRP crp = crpConstruct(s, process_command);

	if (!crp) return;

	NETPRINTF("MasterInterpreter\n" );


	setInactivityAlarm(1);

	while(fgets(inbuf, sizeof(inbuf)-1, crp->fd_net)){
		setInactivityAlarm(0);
		dbg(1, "interpreter %d:\"%s\"\n", getpid(), inbuf);

		char *argv[20];
		int argc = crp->split(chomp(inbuf), argv, 20);
		
		if (crp->decoder(argc, argv, crp) < 0 ){
			break;
		}
		setInactivityAlarm(1);
	}

	crpDestroy(crp);
}



static void openMaster(CRP crp);
static void openData(CRP crp, char *file, int mode, int binhead);



static void process_reply(CRP crp, int include_eof)
/*
 * scan reply linewise for EOF mark.
 * but buffer it all up to maximise tx packet len
 */
{
	char* reply, *eob;
	int eof_sent = 0;

	dbg(1, "process_reply buflen %d fd_acq %p\n", 
	       crp->buflen, crp->fd_acq);
#define HEADROOM (crp->buflen - (eob - crp->buf))

	do {
		for ( reply = eob = crp->buf; HEADROOM > 256 && !eof_sent; 
		      reply = eob ){
			if (fgets(reply, HEADROOM, crp->fd_acq) == NULL){
				/* EOF from far end */
				sprintf(reply, "EOF %d 99", crp->seqnum);
				if (include_eof){
					eob = reply + strlen(reply);
				}
				dbg(1, "process_reply EOF");
				eof_sent = 1;
			}else if ( strncmp(reply, "EOF", 3) == 0){
				if (include_eof){
					eob = reply + strlen(reply);
				}
				dbg(1, "process_reply EOF");
				eof_sent = 1;
			}else{
				eob = reply + strlen(reply);
			}
			dbg(1, "process_reply %s", reply);
		}
		fwrite(crp->buf, eob-crp->buf, 1, crp->fd_net);
	} while(!eof_sent);

	fflush(crp->fd_net);

	dbg(1, "process_reply done\n");
#undef HEADROOM
}

static int doAcqCommand(int argc, char* argv[], CRP crp)
{
/* 1 remove popt style switch end marks */
	int iarg;

	crp->buf[0] = '\0';
	for (iarg = 1; iarg != argc; ++iarg){  /* no need to send "acqcmd" */
		if (iarg == 1 && strcmp(argv[iarg], "--") == 0){  /* 1 */
			continue;
		}else{
			strcat(crp->buf, argv[iarg]);
			strcat(crp->buf, " ");
		}
	}
	strcat(crp->buf, "\n");

	dbg(1, "doAcqCommand: %s", crp->buf);

	fputs(crp->buf, crp->fd_acq);

	process_reply(crp, 0);

	return 0;
}

static int dt100GetBoards(int argc, char* argv[], CRP crp);
static int dt100Open(int argc, char* argv[], CRP crp);
static int dt100Close(int argc, char* argv[], CRP crp);
static int dt100Read(int argc, char* argv[], CRP crp);
static int dt100Stream(int argc, char* argv[], CRP crp);

static int doDt100Command(int argc, char* argv[], CRP crp)
{
	static struct command_def command_def_tab[] = {
		{ .key = "getBoards", .cmd = dt100GetBoards },
		{ .key = "open",      .cmd = dt100Open },
		{ .key = "close",      .cmd = dt100Close },
		{ .key = "read",      .cmd = dt100Read },
		{ .key = "stream",    .cmd = dt100Stream },
		{ 0 }
	};
	struct command_def *pcom;

	argv += 1; argc -= 1;

	dbg(1, "doDt100Command %s\n", argv[0]);

	for (pcom = command_def_tab; pcom->key != 0; ++pcom){
		if (STREQ( argv[0], pcom->key )){
			return pcom->cmd(argc, argv, crp);
		}
	}

	NETPRINTF("%sERROR command not found %s\n", DT100PROMPT, argv[0]);
      
	return -1;
}


static int doBye(int argc, char* argv[], CRP crp)
{
	return -1;
}


static int doPut(int argc, char* argv[], CRP crp)
{
	FILE* fp = fopen(argv[1], "w");

	if (fp){
		char *my_buf = malloc(4096);		
		int nread;

		while ((nread = fread(my_buf, 1, 4096, crp->fd_net)) > 0){
			fwrite(my_buf, 1, nread, fp);
		}
		free(my_buf);
		fclose(fp);
	}else{
		NETPRINTF("ERROR: failed to open %s\n", argv[1]);
	}

	return -1;
}


static int doGet(int argc, char* argv[], CRP crp)
{
	FILE* fp = fopen(argv[1], "r");

	if (fp){
		char *my_buf = malloc(4096);		
		struct stat statbuf;
		int nread;
		int len;
		
		if (fstat(fileno(fp), &statbuf) == 0){
			len = statbuf.st_size;
		}else{
			len = -1;
		}
		
		fprintf(crp->fd_net, "get %s %d bytes\n", argv[1], len);

		while ((nread = fread(my_buf, 1, 4096, fp)) > 0){
			fwrite(my_buf, 1, nread, crp->fd_net);
		}
		free(my_buf);
		fclose(fp);
	}else{
		NETPRINTF("ERROR: failed to open %s\n", argv[1]);

	}

	return -1;
}


static int process_command(int argc, char* argv[], CRP crp)
{
	static struct command_def command_def_tab[] = {
		{ .key = "dt100", .cmd = doDt100Command },
		{ .key = "acqcmd", .cmd = doAcqCommand },
		{ .key = "get",    .cmd = doGet },
		{ .key = "put",    .cmd = doPut },
		{ .key = "bye",    .cmd = doBye },
		{ 0 }
	};
	struct command_def *pcom;

	for (pcom = command_def_tab; pcom->key != 0; ++pcom){
		if (STREQ( argv[0], pcom->key )){
			return pcom->cmd(argc, argv, crp);
		}
	}

	for (pcom = command_def_tab; pcom->key != 0; ++pcom){
		if (STREQNL( argv[0], pcom->key )){
			return pcom->cmd(argc, argv, crp);
		}
	}

	NETPRINTF("%sERROR command not found %s\n", DT100PROMPT, argv[0]);
	return 0;
}


#define isspace(c) ((c)==' '||(c)=='\t'||(c)=='\n')

static int split(char *str, char *argv[], int maxargs)
{
#define INCR(argc) (argc<maxargs? argc++: argc)
#define DQUOTE '"'

	int argc = 0;
	char *ptok;
	char *pend;
	enum { IN_ARG, IN_QUOTED_ARG, IN_SPACE } state = IN_SPACE;

	for (ptok = pend = str; *pend; ++pend){
		switch(state){
		case IN_ARG:
			if (isspace(*pend)){
				*pend = '\0';
				argv[INCR(argc)] = ptok;
				ptok = pend + 1;
				state = IN_SPACE;
			}
			break;
		case IN_SPACE:
			if (*pend == DQUOTE){
				state = IN_QUOTED_ARG;
				ptok = pend+1;
			}else if (!isspace(*pend)){
				state = IN_ARG;
				ptok = pend;
			}/* else do nothing */
			break;
		case IN_QUOTED_ARG:
			if (*pend == DQUOTE){
				*pend = '\0';
				argv[INCR(argc)] = ptok;
				ptok = pend + 1;
				state = IN_SPACE;
			}
			break;
		default:
			assert(0);
		}
	}

	switch(state){
	case IN_ARG:
	case IN_QUOTED_ARG:
		argv[INCR(argc)] = ptok;
	default:
		;
	}
	argv[argc+1] = NULL;
	return argc;
}


static void openMaster(CRP crp)
{
	dbg(1,  "openMaster()\n" );
	switch(crp->acq_type){
	case ACQ_TYPE_MASTER:
		return;
	case ACQ_TYPE_DATA:
		crpClose(crp);        /* fall thru */
	default:
		make_connection(ACQCMD_DEV, crp);
		crp->buf = malloc(crp->buflen = CMD_BUFLEN);
		assert(crp->buf);
		crp->acq_type = ACQ_TYPE_MASTER;
	}
}


static int decodeKnobCommand(int argc, char* argv[], CRP crp)
{
	int ii;

	if (argc >= 1 && STREQ(argv[0], "bye")){
		fprintf(crp->fd_net, "goodbye\n");
		return -1;
	}
	crp->fd_acq = fopen(crp->knob_file, "w");

	if (!crp->fd_acq){
		fprintf(crp->fd_net, 
			"ERROR: failed top open %s\n",
			crp->knob_file);
		return -1;
	}


	for (ii = 0; ii != argc; ++ii){
		dbg(1,"%s ", argv[ii]);
		fputs(argv[ii], crp->fd_acq);
		fputs(" ", crp->fd_acq);
	}
	fclose(crp->fd_acq);

	crp->fd_acq = fopen(crp->knob_file, "r");
	if (!crp->fd_acq){
		fprintf(crp->fd_net, 
			"ERROR: failed top open %s\n",
			crp->knob_file);
		return -1;
	}
	fgets(crp->buf, crp->buflen, crp->fd_acq);
	fprintf(crp->fd_net, "%s\nEOF\n", chomp(crp->buf));
	fclose(crp->fd_acq);
	return 0;
}
static void openKnob(CRP crp, char* file)
{
	crpClose(crp);
	crp->acq_type = ACQ_TYPE_KNOB;
	strncpy(crp->knob_file, file, 79);
	crp->buf = malloc(crp->buflen = CMD_BUFLEN);
	assert(crp->buf);
	crp->decoder = decodeKnobCommand;
}


static void openData(CRP crp, char *file, int mode, int binhead)
/* /dev/acq32/acq32.1.01: by luck or design, this name is preserved! */
/* WORKTODO ... in the future, might be on board, off board.
 * for now, assume one board only */
{
/* WORKTODO -- open data - where to hide the fp ? - in crp I guess */
	dbg(1, "openData() file %s\n", file);

	crpClose(crp);
	crp->id_acq = open(file, O_RDONLY);
	if (crp->id_acq < 0){
		NETPRINTF( "ERROR: failed to open %s %d\n", file, errno);
	}else{
		crp->sample_read_start.mode = mode;
		crp->sample_read_stride.mode = mode;
		crp->acq_type = ACQ_TYPE_DATA;
		crp->channel = strtoul(rindex(file,'.')+1, 0, 10);
		crp->buf = malloc(crp->buflen = DAT_BUFLEN);
		assert(crp->buf);
		crp->binhead = binhead;

		dbg(2, "data buffer allocated: %d", DAT_BUFLEN);
	}
}

static int decodeShellCommand(int argc, char* argv[], CRP crp)
{
	int rc = 0;
	int len = strlen(argv[0]);
	int append_command_terminator = 0;
	int ii;

	assert(argc == 1);

	dbg(1, "%s %d last char[%c]", argv[0], len, argv[0][len-1]);

/* jump thru fiery hoops to try to add additional "shell" functionality
 * to the existing line-oriented interface.
 * 
 * trailing ';' - substitute a \n (so "here" docs can work on one line)
 * trailing '\' - shell line continuation - let it thrue
 * replace ';' with newlines generally. Multiline commands passed with ';' sep.
 */
	switch(argv[0][len-1]){
	case ';':
		argv[0][len-1] = '\n';
		break;
	case '\\':
		break;
	default:
		append_command_terminator = 1;
	}
	
	for (ii = 0; ii != len; ++ii){
		if (argv[0][ii] == ';'){
			argv[0][ii] = '\n';
		}
	}

	fprintf(crp->fd_acq, "%s\n", argv[0]);

	if (append_command_terminator){
		fprintf(crp->fd_acq, "echo EOF %d $?\n", ++crp->seqnum);
		rc = fflush(crp->fd_acq);      
		dbg(1, ";echo EOF\n");
		process_reply(crp, 1);
	}
	return rc;                /** @todo detect dead shell ? */
}
static void spawnShell(CRP crp, char* file)
{
	int socks[2];
	char *exec_file = "/bin/sh";

	if (file != 0 && access(file, F_OK|X_OK) == 0){
		exec_file = file;
	}
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, socks)){
		assert(0);
	}
	if ((crp->cpid = fork()) != 0){
		dbg(1, "parent %d child %d\n", getpid(), crp->cpid);
		dbg(1, "close socks[1] %d use socks[0] %d\n", 
			socks[1],socks[0]);
		close(socks[1]);
		crp->id_acq = socks[0];
		crp->fd_acq = fdopen(crp->id_acq, "r+");
		crp->split = nosplit;
		crp->decoder = decodeShellCommand;
		crp->buf = malloc(crp->buflen = CMD_BUFLEN);
		assert(crp->buf);
		crp->acq_type = ACQ_TYPE_SHELL;	
	}else{
		static char* argv[2];
		
		argv[0] = exec_file; argv[1] = NULL;
		dbg(1, "child %d close socks[0] %d use socks[1] %d\n", 
			getpid(), socks[0], socks[1]);

		close(socks[0]);
		dup2(socks[1], 0);
		dup2(socks[1], 1);
		dup2(socks[1], 2);
		execv(argv[0], argv);
		assert(0);
	}
}
static void openShell(CRP crp, char *file)
{
	dbg(1, "openShell()\n" );
	switch(crp->acq_type){
	case ACQ_TYPE_SHELL:
		break;
	case ACQ_TYPE_MASTER:
	case ACQ_TYPE_DATA:
		crpClose(crp);        /* fall thru */
	default:
		spawnShell(crp, file);
	}
}


static int is_socket(const char *file)
{
	struct stat buf;

	stat(file, &buf);
	return (buf.st_mode&S_IFMT) == S_IFSOCK;
}


static void make_connection(const char *device, CRP crp)
{
	FILE *fd = 0;
	int sock = 0;

	if (is_socket(device)){
		struct sockaddr_un addr;
		size_t addrLength;

		if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
			fprintf( stderr, "ERROR:failed to open socket\n");
			_exit(errno);
		}
		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, device);
		addrLength = sizeof(addr.sun_family) + strlen(addr.sun_path);

		if (connect(sock, (struct sockaddr*)&addr, addrLength)){
			fprintf( stderr, "ERROR connect failed \"%s\"\n",
				 device );
		}
		fd = fdopen(sock, "r+");
		if ( fd == NULL ){
			fprintf( stderr, 
				 "ERROR: failed hook socket \"%s\"\n", 
				 device );
			_exit(errno);
		}
	}else{
		fd = fopen( device, "r+" );
		if ( fd == NULL ){
			fprintf( stderr, 
				 "ERROR: failed to open device \"%s\"\n", 
				 device );
			_exit(errno);
		}
	}
	crp->id_acq = sock;
	crp->fd_acq = fd;
}



#define ID_CALDATE     "/tmp/caldate"


static const char *getNumChannels(void)
{
	const char *nchan = getenv("AICHAN");

	return nchan? nchan: "16";
}

static const char *contentsOf(char *fname, char *buf, int maxbuf)
{
	FILE *fp = fopen(fname, "r");
	
	if (!fp){
		return "-";
	}else{
		fgets(buf, maxbuf, fp);
		if (index(buf, '\n')){
			*index(buf, '\n') = '\0';
		}
		fclose(fp);
		return buf;
	}
}

static const char *result(char *command, char *buf, int maxbuf)
{
	FILE *fp = popen(command, "r");

	if (!fp){
		return "-";
	}else{
		fgets(buf, maxbuf, fp);
		if (index(buf, '\n')){
			*index(buf, '\n') = '\0';
		}
		fclose(fp);
		return buf;		
	}
}


#define B1 \
"model n chn      s/n       arm-fw       lca-fw     cal date cycle heartbeat"
#define B2 \
"ACQ200 0  16   D223737     arm-linux      acq-lca       251203     0  10"
#define B3 \
"physical=0 logical=1 model=ACQ200"

static int dt100GetBoards(int argc, char* argv[], CRP crp)
{
	char caldate[20];
	char armfw[20];
	char fpga[20];

	NETPRINTF( "%slines=%d boards=%d\n", DT100PROMPT, 3, 1 );
	NETPRINTF( "%s\n", B1 );
	NETPRINTF( "ACQ200 0 %s %s %s %s %s %02d %02d\n",
		   getNumChannels(),
		   getenv("serialnum"),
		   result("uname -r", armfw, sizeof(armfw)),
		   result("md5sum /tmp/fpga_header | cut -c -6", 
			  fpga, sizeof(fpga)),
		   contentsOf(ID_CALDATE, caldate, sizeof(caldate)),
		   0,
		   100 );
	NETPRINTF( "%s\n", B3 );
	return 0;
}


static int dt100Open(int argc, char* argv[], CRP crp)
{
	char * file = "/dev/acq32/acq32.1.m1";
	char *type = "master";

	switch(argc){
	case 3:
		file = argv[2];
	case 2:
		type = argv[1];
		if (STREQ(type, "master")){
			openMaster(crp);
			DT100PROMPTEND;
			return 0;
		}else if (STREQ(type, "data") || 
			  STREQ(type, "data1")){
			openData(crp, file, 
				 STREQ(type, "data1")?
				 SAMPLE_READ_MODE_ONCE:
				 SAMPLE_READ_MODE_NEVER, 
				 !SAMPLE_READ_BINHEAD);
			DT100PROMPTEND;
			return 0;
		}else if (STREQ(type, "data2")){
			openData(crp, file, 
				 SAMPLE_READ_MODE_ONCE,
				 SAMPLE_READ_BINHEAD);
			return 0;
		}else if (STREQ(type, "shell")){
			openShell(crp, file);
			DT100PROMPTEND;
			return 0;
		}else if (STREQ(type, "knob") || STREQ(type, "sys")){
			openKnob(crp, file);
			return 0;
		}
		/* fall thru */
	default:
		NETPRINTF( "%sERROR wrong #args\n", DT100PROMPT);
		return -1;
	}
}

static int dt100Close(int argc, char* argv[], CRP crp)
{
	crpClose(crp);
	return 0;
}

static int dt100Read(int argc, char* argv[], CRP crp)
{
#define SAMPLESZ sizeof(short)
	unsigned start = argc>1? atoi(argv[1]): 0;
	unsigned stop  = argc>2? atoi(argv[2]): 100000000;
	unsigned stride = argc>3? atoi(argv[3]): 1;

	
	if (stride < 1) stride = 1;

	/* WORKTODO: set start, stop */

	int maxread = (stop-start)/stride;
	int nread_req;
	int nwrite;
	char header[80];
	struct iovec iovec[2];
	int totread;

	if (G_use_seek){
		lseek(crp->id_acq, start, SEEK_SET);
	}else{
		setSampleReadStart(crp, start);
		setSampleReadStride(crp, stride);
	}
	nread_req = MIN(maxread*sample_size, crp->buflen);

/*
 * run an accumulating read. Driver cannot guarantee full size return
 * bufs, and may hiccup at tblock ends
 */
	for (totread = 0; totread < nread_req; ){
		int nread;

		dbg(1, "dt100Read %d %d %d %d maxread %d w nread %d b\n", 
		       crp->channel, start, stop, stride, maxread, nread_req);

		nread = read(crp->id_acq, crp->buf+totread, nread_req-totread);

		if (nread > 0){
			totread += nread;
		}else{
			if (nread == 0){
				dbg(1, "read returned zero\n");
			}else if (nread < 0){
				perror("channel read error");
			}
			break;
		}
	}
	
	if (crp->binhead){
		unsigned message_length = htonl(totread);
		iovec[0].iov_base = &message_length;
		iovec[0].iov_len = 4;

		u16* pbuf = (u16*)crp->buf;
		int nwords = totread/sizeof(short);
		while(nwords--){
			*pbuf++ = htons(*pbuf);
		}
	}else{
		sprintf(header, "%s%d bytes\n", DT100PROMPT, totread);
		iovec[0].iov_base = header;
		iovec[0].iov_len  = strlen(header);
	}

	iovec[1].iov_base = crp->buf;
	iovec[1].iov_len  = totread;

	nwrite = writev(crp->id_net, iovec, totread>0? 2: 1);

	dbg(1, "dt100Read %d in:%d out :%d\n", crp->channel, totread, nwrite);

	return totread < nread_req? -1: 0;
}

static CRP _crp;

static void reaper(int sig)
{
	int ws, rc;

	while ((rc = waitpid(-1, &ws, WNOHANG)) > 0){
		err( "stream exit pid:%d status:%d\n", rc, ws);
	}
	/* owner likely stuck in fgets() */
	kill(getpid(), SIGHUP);
}

static int dt100Stream(int argc, char* argv[], CRP crp)
{
	int stride = argc>1? atoi(argv[1]): 1;
	int mean   = argc>2? atoi(argv[2]): 0;
	int npairs = argc>3? atoi(argv[3]): 16;
	int burst  = argc>4? atoi(argv[4]): 0;

	dbg(1, "dt100Stream %d %d %d %d  cpid %d\n", 
	       stride, mean, npairs, burst, crp->cpid);

	if (stride == 0){
		mean = npairs = burst = 0;  /* pacify gcc */
		return -1;   /* close down, destroying child proc */
	}else{
		signal(SIGCHLD, reaper);

		if ((crp->cpid = fork()) == 0){
			close(0);
			dup2(crp->id_net,1);
			execvp(STREAMPROC, argv);
			return -1; /* should NOT happen */
		}else{
			_crp = crp;
			dbg(1, "after fork() cpid %d\n", crp->cpid);
		}
	}
	return 0;
}

void interp_init(void)
{
	FILE *fp = fopen("dev/dtacq/word_size", "r");
	if (fp != 0){
		int success = fscanf(fp, "%d", &sample_size);
		if (!success){
			err("failed to scan sample_size");
		}else{
			dbg(1, "sample_size set %d", sample_size);
		}
		fclose(fp);
	}
}
