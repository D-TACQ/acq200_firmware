/** @file x86timer.h API prototype for x86 timer funcs.
 */

unsigned long get_elapsed_microseconds(int reset);
void cpu_delay_awhile(unsigned int delay_microseconds);

unsigned int get_cpu_clock_speed(void);

unsigned long long get_cpu_ticks(void);
unsigned long long get_cpu_usecs(void);
