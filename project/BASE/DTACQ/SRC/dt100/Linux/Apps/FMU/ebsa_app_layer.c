/* 
 * ebsa_mmap.c
 * test harness for mmap function call
 * ebsa_map [-o offset] [-l length]  device - dumps device to stdout
 * 
 * most useful is ebsa_map /dev/ebsa/ebsa.1r | od -x | more
 *
 * with acknowledgements to Johnson and Troan p220
 */


#undef __KERNEL__

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>

#define FNAME (argc>1? argv[1]: "/dev/ebsa/ebsa.1r")

#define MB       0x100000

static int length = 4 * MB;

#include "ebsa.h"



extern void* ebsa_get_base( const char* fname, const char* prot )
/* returns mapping of base address for device
 *
 * inputs: fname - the device name
 *         prot  - r - readable, w - writeable 
 */
{    
    int fd;
    void* region;

    int can_read  = strchr( prot, 'r' ) != NULL;
    int can_write = strchr( prot, 'w' ) != NULL;

    mode_t mode = can_read&&can_write? O_RDWR:
                  can_write?           O_WRONLY:
                                       O_RDONLY;

    int iprot = (can_read? PROT_READ: 0) | (can_write? PROT_WRITE: 0);

    if ( (fd = open( fname, mode)) < 0 ){
        perror( "open()" );
        return NULL;
    }    

    region = mmap( NULL, length, iprot, MAP_SHARED, fd, 0 );

    if ( region == (caddr_t)-1 ){
        perror( "mmap" );
        return NULL;
    }

    return region;
}
