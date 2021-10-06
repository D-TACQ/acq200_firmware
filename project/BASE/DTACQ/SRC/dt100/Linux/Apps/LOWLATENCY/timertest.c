/** @file timertest.c test harness for timer.
 */


#include <stdio.h>
#include <stdlib.h>
#include "x86timer.h"


int main(int argc, char* argv[])
{
	unsigned long long raw[10];
	unsigned long times[10];
	unsigned int delay = 1;
	int ii;

	if (argc > 1) delay = atoi(argv[1]);
	
	get_cpu_clock_speed();    /** inits library */

	printf("timertest get_cpu_clock_speed %d\n", get_cpu_clock_speed());

	printf("raw\n");
	for (ii = 0; ii != 10; ++ii){
		raw[ii] = get_cpu_ticks();
	}

	for (ii = 0; ii != 10; ++ii){
		printf("%2d ticks 0x%016Lx %16Lu usecs %10Lu\n", 
		       ii, raw[ii], raw[ii], raw[ii]/get_cpu_clock_speed());
	}	

	


	for (ii = 0; ii != 10; ++ii){
		times[ii] = get_elapsed_microseconds(ii == 0);
	}

	printf("No Delay Loop\n");

	for (ii = 0; ii != 10; ++ii){
		printf("%2d usecs %10lu\n", ii, times[ii]);
	}	

	printf("Delay Loop %d\n", delay);

	for (ii = 0; ii != 10; ++ii){
		times[ii] = get_elapsed_microseconds(ii == 0);
		cpu_delay_awhile(delay);
	}

	for (ii = 0; ii != 10; ++ii){
		printf("%2d usecs %10lu\n", ii, times[ii]);
	}	
	return 0;
}
