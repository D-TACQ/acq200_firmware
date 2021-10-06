/** @file flatline.c ad-hoc test file looking for an error condition.
 * test files for flatline condition
 * input files are channelized shorts
 */


#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


#define NFLATS 32
#define MAXBUF 0x10000

unsigned short last_value;
int repeat_count;


int error_count;
short* buffer;
const char* fname;

void find_flatline(int fd)
{
#define FLATFMT "FLATLINE %s %5d 0x%04x %6s at %d\n"
#define HEXIBLE(v) ((v)&0x0000ffff)
	int totread = 0;
	int nread;
	int nflats;

	for( ; (nread = read(fd, buffer, MAXBUF)) > 0; totread += nread){
		int ibuf;

		for (ibuf = 0, nread /= 2; ibuf != nread; ++ibuf){
			switch(last_value){
			case 0xdead:
			case 0xbeef:
				nflats = 2; break;
			default:
				nflats = NFLATS;
			}
			if (buffer[ibuf] == last_value){
				if (++repeat_count == nflats){
					fprintf(stderr, FLATFMT,
						fname,
						last_value, 
						HEXIBLE(last_value),
						"starts",
						totread + ibuf - nflats);
				}
			}else{
				if (repeat_count >= nflats){
					fprintf(stderr, FLATFMT,
						fname,
						last_value, 
						HEXIBLE(last_value),
						"ends",
						totread + ibuf);
				}
				last_value = buffer[ibuf];
				repeat_count = 0;
			}
		}
	}
}

int main(int argc, const char* argv[])
{
	int iarg;

	buffer = malloc(MAXBUF);

	if (argc <= 1){
		fname = "stdin";
		find_flatline(0);
	}else{
		for (iarg = 1; iarg < argc; ++iarg){
			int fd = open(fname = argv[iarg], O_RDONLY);

			if (fd < 0){
				perror("failed to open file");
				exit(errno);
			}else{
				find_flatline(fd);
			}
		}
	}

	exit(error_count != 0);
}
