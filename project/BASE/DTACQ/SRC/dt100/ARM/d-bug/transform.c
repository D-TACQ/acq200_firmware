/*
 * transform.c - channel de-interleave of bigbuf
 */

#include "local.h"
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>


#define TFILE "/dev/dtacq_drv/transformer"

int acq200_debug;

static void report_error(const char *mode) {
	err("ERROR: failed to open \"%s\", \"%s\"\n", TFILE, mode);
	perror("");
	_exit(-errno);
}

static int send_command() {
	FILE *fpout = fopen(TFILE, "w");

	if (!fpout){
		report_error("w");
	}

	fputs("xxx\n", fpout);
	fclose(fpout);
	return 0;
}

static int check_completion(char old_block[], char new_block[])
{
	FILE *fpin  = fopen(TFILE, "r");
	int rc;

	if (!fpin){
		report_error("r");
	}

	strcpy(old_block, new_block);

	if ((rc = (fgets(new_block, sizeof(new_block), fpin) == 0))){
		dbg(1, "fgets returns 0\n");
	}else{
		dbg(1, "fgets ret %s old %s\n", new_block, old_block );
	}

	fclose(fpin);
	return rc;
}
static int transform(char old_block[], char new_block[] )
{
	send_command();
	check_completion(old_block, new_block);
	return 0;
}
int main(int argc, char* argv[])
{
	char old_block[80] = {"-2" };
	char new_block[80] = {"-1" };

	if (argc > 1 && strcmp(argv[1], "-d") == 0){
		acq200_debug = 1;
	}

	dbg(1, "transform $Revision: 1.1.2.8 $\n");

	setpriority(PRIO_PROCESS, getpid(), -15);

	while(strcmp(old_block, new_block) != 0){
		transform(old_block, new_block);
	}

	return 0;
}
