/** @file: x86timer.c  API for x86 time funcs.
 * thanks to Ben Penaflor 

First run

. ./setup.clocks2 3 0 64 2
init_clocks_mbcb

    telnet 192.5.166.179
    /tmp/trigger_toggler

******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "x86timer.h"

static unsigned int CPU_CLOCK_SPEED = -1;

#define rdtscll(val) \
     __asm__ __volatile__ ("rdtsc" : "=A" (val))


unsigned long long get_cpu_ticks(void)
{
	unsigned long long ticks;
	
	rdtscll(ticks);
	return ticks;
}
unsigned long long get_cpu_usecs(void)
{
	unsigned long long ticks;
	
	rdtscll(ticks);
	return ticks/CPU_CLOCK_SPEED;
}


/*
******************************************************************************
SUBROUTINE: get_cpu_clock_speed
******************************************************************************
*/

unsigned int get_cpu_clock_speed(void)
{
	char line[256];
	FILE *inputfile;
	float fspeed = 0;
	unsigned int speed;

	if(CPU_CLOCK_SPEED == -1)
	{
		speed = 0;
		/*
		  Get the cpu speed.
		  Open the file.
		*/
		inputfile = fopen("/proc/cpuinfo", "r");
		if(inputfile != NULL)
		{
			/*
			  Read the data from the file.
			*/
			while(fgets(line, 256, inputfile) != NULL)
			{
				if(strstr(line,"cycle frequency [Hz]") != NULL)
				{
					fspeed = (float)atoi(strchr(line,':')+1)/1000000.0;
					break;
				}
				if(strstr(line,"cpu MHz") != NULL)
				{
					fspeed = (float)atoi(strchr(line,':')+1);
					break;
				}
			}
			fclose(inputfile);
			speed = (int)(fspeed);
		}
		else
			fprintf(stderr,"Cannot get cpu speed from /proc/cpuinfo\n");

		fprintf(stderr,"speed=%d ticks per microsecond\n", speed);
		CPU_CLOCK_SPEED=speed;
	}

	return(CPU_CLOCK_SPEED);
}



/*
******************************************************************************
SUBROUTINE: cpu_delay_awhile

Delay the specified number of input microseconds.

{// for testing, this delays for 10 seconds
int ii;
printf("delaying\n");
for(ii=0;ii<1000*10;ii++)
    cpu_delay_awhile(1000);
printf("done\n");
}
******************************************************************************
*/


static unsigned long
_get_elapsed_microseconds(int reset, unsigned long long* start)
{
	unsigned long long now;

	if(reset==1){
		*start = get_cpu_usecs();
		return 0;
	}
	now = get_cpu_usecs();

	if (now >= *start){
		return now - *start;
	}else{
		*start = 0;
		return 0xffffffffffffffffULL - *start + now;
                /**    0x1234567890abcdefULL */
	}
}


unsigned long
get_elapsed_microseconds(int reset)
{
	static unsigned long long start;

	return _get_elapsed_microseconds(reset, &start);
}


void cpu_delay_awhile(unsigned int delay_microseconds)
{
	unsigned long long _start;
	unsigned long start = _get_elapsed_microseconds(1, &_start);
	unsigned long now;

	do {
		now = _get_elapsed_microseconds(0, &_start);
	} while( now < start + delay_microseconds);
}


