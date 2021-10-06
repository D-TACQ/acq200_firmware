/* ------------------------------------------------------------------------- */
/* tblock-stream.c 							     */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2011 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    http://www.d-tacq.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License
    as published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

/** @file tblock-stream.c tblock server
 * Refs: 
 * runs under inetd. 
 * sends each tblock preceded by a header when available
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include "popt.h"

#include "local.h"                                
#include "acq200_root.h"

#define STDOUT		1
#define STATFILE	"/dev/acq200/data.dma/tbstat2"

FILE *fp_stat;

/* Header:
> Where binary header is - 4 byte marker (0xfeedc0de for example) -
> 4 byte block id (may be useful for checking) - 4 byte unsigned
> timestamp (should be monotonic) - 4 byte spare:
- 4 byte: length in bytes 
*/

struct HEADER {
	u32 marker;
	u32 block_id;
	u32 ts;
	u32 length;
} header = {
	0xfeedc0de,
};

int do_header = 1;
int use_sendfile = 1;


char *data_buffer;		/* only if use_sendfile==0 */

/* tbstat2: tblock, timestamp 
049 1638400
*/

void send_header(const char* tbn, unsigned ts)
{
	if (!do_header) return;

	header.block_id = strtoul(tbn, 0, 10);
	header.ts = ts;
	if (write(STDOUT, &header, sizeof(header)) != sizeof(header)){
		perror("write failed");
		exit(1);
	}
}

void send_tblock(const char* tbn)
{
	char tbname[80];
	int in;
	int rc;

	sprintf(tbname, "/dev/acq200/tblocks/%s", tbn);
	in = open(tbname, O_RDONLY);
	if (in < 0){
		perror(tbname);
		exit(1);
	}
	if (use_sendfile){
		rc = sendfile(STDOUT, in, NULL, header.length);
		if (rc != header.length){
			fprintf(stderr, "sendfile failed %d\n", rc);
			exit(1);
		}
	}else{
		rc = read(in, data_buffer, header.length);
		if (rc != header.length){
			fprintf(stderr, "failed to read full length %d\n",
				header.length);
			/* no point in multiple reads, it either works or not */
			exit(1);
		}
		write(STDOUT, data_buffer, header.length);
	}
	close(in);
}

int please_stop(void)
{
	FILE *fp = fopen("/tmp/tblock-stream-please-stop", "r");
	if (fp == 0){
		return 0;
	}else{
		unlink("/tmp/tblock-stream-please-stop");
		return 1;
	}
}

int tblock_stream(void)
{
	char status[80];
	char tbn[4];
	unsigned ts;

	if (fgets(status, 80, fp_stat) == 0){
		perror("fgets return 0");
		exit(-1);
	}
	if (sscanf(status, "%3s %u", tbn, &ts) == 2){
		send_header(tbn, ts);
		send_tblock(tbn);
	}

	if (please_stop()){
		exit(0);
	}
	return 0;
}


void init(int argc, char** argv) 
{
	int length = 0;
        struct poptOption opt_table[] = {
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 0   },
		{ "length",     'l', POPT_ARG_INT,    &length,       'l' },
		{ "header",     'h', POPT_ARG_INT,    &do_header,    0   },
		{ "sendfile",   's', POPT_ARG_INT,    &use_sendfile, 0   },
		POPT_AUTOHELP
		POPT_TABLEEND
	};
	

	poptContext context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0);
	int rc;

	if (getenv("USE_SENDFILE")){
		use_sendfile = atoi(getenv("USE_SENDFILE"));
	}
	acq200_getRoot("/dev/dtacq","tblock_len",1,"%u", &header.length);

        while((rc =poptGetNextOpt(context)) > 0){
                switch(rc) {
		case 'l':
			header.length = length;
			break;
                }
        }

	
	fp_stat = fopen(STATFILE, "r");
	if (fp_stat == 0){
		perror(STATFILE);
		exit(-1);
	}

	if (use_sendfile == 0 && header.length > 0){
		data_buffer = malloc(header.length);
	}
}
int main(int argc, char *argv[])
{
	init(argc, argv);

	while(1){
		tblock_stream();
	}
	return 0;
}
