/*
 * gash1 - test data.dma read strategy
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>

char buf[256];




int main()
{
	int nread;

	memset(buf, 'A', 256);
//	while((nread = read(0, buf, 1024000)) > 0){
	while((nread = fread(buf, 1, 1024000, stdin)) > 0){
		write(1, buf, 256);
	}
	return 0;
}
