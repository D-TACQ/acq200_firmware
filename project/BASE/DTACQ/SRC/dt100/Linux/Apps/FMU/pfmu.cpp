/*****************************************************************************
 *
 * File: pfmu.cpp: universal flash device and manager for ACQ32, EBSA. 
 *             extendible to other flash types
 *
 * $RCSfile: pfmu.cpp,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: pfmu.cpp,v 1.17 2002/07/30 02:28:00 pgm Exp $
 * $Log: pfmu.cpp,v $
 * Revision 1.17  2002/07/30 02:28:00  pgm
 * don't advertise first retry
 *
 * Revision 1.16  2002/07/30 02:12:34  pgm
 * more retries, extra check on write
 *
 * Revision 1.15  2002/07/30 11:00:52  pgm
 * with retry on write
 *
 * Revision 1.14  2001/05/06 18:47:20  pgm
 * cleanup
 *
 * Revision 1.13  2001/01/02 12:35:02  pgm
 * top-down Make, release targets
 *
 * Revision 1.12  2000/09/30 09:51:23  pgm
 * better banner
 *
 * Revision 1.11  2000/09/30 09:50:08  pgm
 * MakeArmSpin call added for boot66 op
 *
 * Revision 1.10  1999/11/18 21:15:37  pgm
 * dont delete sectors if already deleted
 *
 * Revision 1.9  1999/11/12 08:55:31  pgm
 * -b board opt added
 *
 * Revision 1.8  1999/10/31 21:02:49  pgm
 * command line args are go
 *
 * Revision 1.7  1999/10/31 20:22:43  pgm
 * help and ident opts added
 *
 * Revision 1.6  1999/10/31 19:41:11  pgm
 * works again with acq32 device
 *
 * Revision 1.5  1999/10/20 19:56:43  pgm
 * mod to suit acq32 driver (untested)
 *
 * Revision 1.4  1999/10/11 10:34:56  pgm
 * Works at DTACQ
 *
 * Revision 1.3  1999/10/01 21:47:05  pgm
 * WORD rwrite fixup/frigs for 21285
 *
 * Revision 1.2  1999/09/30 20:45:12  pgm
 * pfmu hacked to pgm archi satis
 *
 * Revision 1.1  1999/09/30 10:14:33  pgm
 * pfmu - enhanced fmu utility
 *
 *
\*****************************************************************************/



#include "local.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "FlashDevice.h"
#include "ebsa_app_layer.h"

#include <popt.h>

static FlashDevice* FD;



// Copyright (C) 1997 by
// Digital Equipment Corporation, Maynard, Massachusetts.
// All rights reserved.
//
// This software is furnished under a license and may be used and copied
// only  in  accordance  of  the  terms  of  such  license  and with the
// inclusion of the above copyright notice. This software or  any  other
// copies thereof may not be provided or otherwise made available to any
// other person.  No title to and  ownership of the  software is  hereby
// transferred.
//
// The information in this software is  subject to change without notice
// and  should  not  be  construed  as a commitment by Digital Equipment
// Corporation.
//
// Digital assumes no responsibility for the use  or  reliability of its
// software on equipment which is not supplied by Digital.
//
//
// Abstract:    
//
// Author:      
//
// Revision:    
//
// Modifications:
//
//     $History: $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#ifndef ANGEL
#include "cdef.h"
#endif

#include "platform.h"                   // FLASH_BASE

#include "fmu.h"

struct aif_header {
    unsigned int decompress ;
    unsigned int SelfReloc ;
    unsigned int DBGinit ;
    unsigned int EntryPoint ;
    unsigned int Exit ;
    unsigned int ROSize ;
    unsigned int RWSize ;
    unsigned int DebugSize ;
    unsigned int ZeroInitSize ;
    unsigned int ImageDebugType ;
    unsigned int ImageBase ;
} typedef aif_t ;

/* NOP instruction */
#define NOP      0xe1a00000 
/* bitmask to check for BL instruction */
#define BL       0x0a000000

/* Macro to create branch instructions */
#define BINSTR(offset) (0xeb000000+(((offset)-8)>>2))

#define NOBOOTMASK 1


/*
 * PGM Linux port here 
 */

#define REAL_DEVICE

#define FNAME "/dev/acq32/acq32.1.flash"


#define IMAGE_HEADER_SIZE 64 

#include "arm_modules.h"

static struct image_header *header = NULL ;

/*
 *  Global variables
 */
unsigned volatile char *flash_base = NULL ;

static int* S_blocks;

#define BLOCK_IN_USE(n) (S_blocks[n] != ((char) -1))

#ifndef ANGEL
void* dInfo ;
#endif


/*
 *  forward declarations.
 */
static void read_flash_header(int blockno) ;
static int write_flash_header(int offset, aif_t *aif, int unique, char *name, int map, int size, int file_checksum, int noboot);
static void print_flash_header(struct image_header *header) ;
static int valid_flash_header(struct image_header *header) ;

static void listAction(void) ;
static void listBlocksAction(void) ;
static void testBlockAction(void) ;
static void eraseAction(void) ;
static void delete_blockAction(void) ;
static void delete_allAction(void) ;
static void programAction(void) ;
static void readAction(void) ;
static void quitAction(void) ;
static void helpAction(void) ;
static void mapBlocksAction();

static int find_room(int nlongs, int start, int * map, int* pblocks) ;
static int find_image(int imageno) ;
static void scan_blocks(void) ;

static void deleteBlock( int blockno )
{
    if ( FD->DeleteBlock( blockno ) ){
        S_blocks[blockno] = -1;
    }else{
        fprintf( stderr, "WARNING: Delete Block reports error\n" );
    }
}
long fsize(FILE *file)
{
    long offset ;

    fseek(file, 0, SEEK_END) ;
    offset = ftell(file) ;
    fseek(file, 0, SEEK_SET) ;

    return offset ;
}

/* ---------- M A K E     A N G E L   B A S E D  I M A G E   FA S T E R ------------*/
#ifdef ANGEL

static char *fileCache = NULL ;
static int fileCacheOffset = 0 ;

FILE *MyFopen(char *file, char *mode) 
{
    FILE *dataFile ;
    int size ;
    int i ;
  
    /* the the world */
    printf("Caching the file in memory\n") ;

    /* initialize */
    fileCacheOffset = 0 ;

    /* read the data file into a cached lump of memory */
    dataFile = fopen(file, mode) ;
    if (dataFile != NULL) {
	size = (int)fsize(dataFile) ;
	fileCache = (char *)malloc(size) ;
	if (fileCache == NULL) {
	    fclose(dataFile) ;
	    dataFile = NULL ;
	} else {
	    i = 0 ;
	    while (i < size) {
		int blockSize ;

		if (size - i < 512) 
		    blockSize = size - i ;
		else
		    blockSize = 512 ;

		if (fread(&fileCache[i], blockSize, 1, dataFile) == 0) {
		    printf("ERROR: reading data file\n") ;
		    fclose(dataFile) ;
		    dataFile = NULL ;
		    break ;
		} else {
		    i += blockSize ;
		}
	    }
	}
    }

    /* return the file descriptor */
    return dataFile ;
}

int MyFseek(FILE *file, int offset, int flags)
{
    fileCacheOffset = offset ;
    return 0 ;
}

int MyFgetc(FILE *file) 
{
    return (int)fileCache[fileCacheOffset++] ;
}

int MyFclose(FILE *file) 
{
    fclose(file) ;
    fileCacheOffset = 0 ;
    if (fileCache) free(fileCache) ;
    fileCache = NULL ;
    return 0 ;
}

#define __fopen(file, mode) MyFopen(file, mode) 
#define __fseek(file, offset, flags) MyFseek(file, offset, flags) 
#define __fgetc(file) MyFgetc(file) 
#define __fclose(file) MyFclose(file) 

#else

#define __fopen(file, flags) fopen(file, flags) 
#define __fseek(file, offset, flags) fseek(file, offset, flags) 
#define __fgetc(file) fgetc(file) 
#define __fclose(file) fclose(file) 

#endif
/* ---------- P A R S I N G    &    A C T I O N --- R O U T I N E S ----------------*/

static int yes_no(void)
{
    char c ;

    printf("Do you really want to do this (y/N)? ") ;
    c = getchar() ;
    /* Ignore the rest of the line */
    if(c != '\n') while(getchar() != '\n');
    if (tolower(c) == 'y') 
	return TRUE ;
    else
	return FALSE ;
}

struct command {
    char *name;
    char *alias;
    char *help_args;
    char *help_text;
    void (*action_rtn)(void);
};

static struct command commands[] = {
    /*
     *  Help
     */
    {"Help", "?",
     "",
     "Print this help text", 
     &helpAction},
    {"List", "Dir",
     "", 
     "List images in flash",
     &listAction},
    {"ListBlocks", "",
     "", 
     "List how each Flash block is being used",
     &listBlocksAction},
    { "MapBlocks", "",
      "",
      "Show address and length of each block",
      &mapBlocksAction },
    {"TestBlock", "",
     "<block-number>",
     "Write a test pattern to a particular flash block",
     &testBlockAction},
    {"Delete", "Erase",
     "<image-number>",
     "Delete an image in flash",
     &eraseAction},
    {"DeleteBlock", "",
     "<block-number>",
     "Deletes a block that appears not to be in an image",
     &delete_blockAction},
    {"DeleteAll", "",
     "",
     "Deletes all blocks except block 0",
     &delete_allAction},
    {"Program", "",
     "<image-number> <image-name> <file-name> [<block-number>] [noboot]",
     "Program the given image into flash",
     &programAction},
    {"Read", "",
     "<image-number> <file-name>",
     "Read the given image from flash into a file",
     &readAction},
    {"Quit", "Exit",
     "",
     "Quit",
     &quitAction},

};

#define NUM_FMU_COMMANDS (sizeof(commands) / sizeof(struct command))

#define MAX_TOKENS 10
char *tokens[MAX_TOKENS];
int ntokens = 0;

static char *__PULL()
{
    char *value = NULL;

    if (ntokens > 0) {
	int i;

	value = tokens[0];
	for (i = 1; i < ntokens; i++) tokens[i-1] = tokens[i];
	ntokens--;
    }
    return value;
}

#define __ADD(value) tokens[ntokens++] = value

static bool checkChecksum()
    // returns true if OK
{
    /* Check the checksum */
    int image_bytes = 0 ;
    int checksum = 0 ;

    for( int iblock = 0;  iblock < FD->GetNumBlocks(); iblock++ ) {

        if ( S_blocks[iblock] >= 0 ) {
	    int flash_offset = FD->GetBlockStart( iblock ); 

	    for( int nlongs = 0;  
                 nlongs < FD->GetBlockLen( iblock ); 
                 nlongs++                            ){
		if( image_bytes >= header->Length) {
		    break;
		}
		checksum += FD->read( flash_offset++ ) ;
		image_bytes += 4 ;
	    }

        }
    }

    if(checksum != 0) {
	printf( "Checksum error; image checksum is 0x%x, should be 0\n"
		,checksum);
    }

    return checksum == 0;
}

/*  List all of the images in flash in ascending image number order */
static void listAction(void)
{
    unsigned in_use_map = 0;

    printf("Listing images in Flash\n");

    checkChecksum();

    /* look for every possible image number */
    for (int image = 0 ; image < 31 ; image++) {

	int blockno = find_image( image ) ;

	if ( blockno >=  0)  {
	    read_flash_header(blockno) ;
	    print_flash_header(header) ;
	    /* Check for dual allocation of blocks */
	    if((in_use_map & header->Map) != 0) {
		printf( "ERROR: Some blocks in this image have are also "
                        "allocated to earlier images");
	    }
	    in_use_map |= header->Map;

	}
    }
}

/* list how each flash block is being used */
static void listBlocksAction(void) 
{
    /* look at every block in turn */
    for ( int blockno = 0; blockno < FD->GetNumBlocks(); blockno++) {
        int offset = FD->GetBlockStart( blockno );
        printf("%2d: ", blockno) ;
        if ( BLOCK_IN_USE(blockno)  ){
            printf("(Image %2d) ", S_blocks[blockno]) ;
        }else{
            printf("(Unused)   ") ;
	}

        for ( int i = 0; i < 5 ; i++) {
            printf("0x%08x ", FD->read(offset++) );
        }
        printf("\n") ;
    }
}

/* write and read a test pattern into block N */
static void testBlockAction(void) 
{
    int blockno ;
    char *token ;
    unsigned char data = 0 ;
    int i, j;
    unsigned w, rv;
    int offset ;
    int ecount = 0;

    /* did the user supply a block number? */
    token = __PULL() ;
    if (token == NULL) {
	printf("ERROR: you should supply a block number to test\n") ;
	return ;
    }

    /* ...and is it valid? */
    blockno = atoi(token) ;

    if ( !IN_RANGE( blockno, 0, FD->GetNumBlocks()-1 ) ){
	printf("ERROR: invalid block number\n") ;
	return ;
    }

    /* can it be deleted? */
    if (BLOCK_IN_USE(blockno))  {
	printf("ERROR: that block is not free (image # %d)\n",
	       S_blocks[blockno]) ;
	return ;
    }

    /* does the user really want to? */
    if (!yes_no()) return ;

    /* delete it before testing it */
    printf("deleting block %d\n", blockno) ;
    deleteBlock( blockno );

    /* write and read a test pattern to the block */
    printf("Writing test pattern to block %d\n", blockno) ;
    data = 0 ;
    offset = FD->GetBlockStart( blockno );
    for (i = 0 ; i < FD->GetBlockLen(blockno); i++ ){
	w = 0 ;
	for (j = 0 ; j < 4 ; j++) {
	    if (data == 0xff) data = 0 ;
	    w |= (data << (j * 8)) ;
	    data++ ;
	}
	FD->write( offset++, w) ;
    }

    printf("Reading test pattern from block %d\n", blockno) ;
    data = 0 ;
    offset = FD->GetBlockStart(blockno);
    for (i = 0 ; i < FD->GetBlockLen(blockno); i++ ) {
	w = 0 ;
	for (j = 0 ; j < 4 ; j++) {
	    if (data == 0xff) data = 0 ;
	    w |= (data << (j * 8)) ;
	    data++ ;
	}
	if (  (rv = FD->read(offset++)) != w) {
	    ecount++ ;
	    if (ecount < 5) {
		printf( "ERROR: test pattern does not match (offset = 0x%x)\n"
			"\tValue is 0x%08x, should be 0x%08x\n",
			offset, rv, w );
	    }
	}
    }

    printf("Flash test of block %d worked\n", blockno) ;
}

/* delete an image in flash */
static void eraseAction(void) 
{
    char *token ;
    int imageno ;
    int blockno ;

    /* did the user supply an image number? */
    token = __PULL() ;
    if (token == NULL) {
	printf("ERROR: you should supply an image number to delete\n") ;
	return ;
    }

    /* ...and is it valid? */
    imageno = atoi(token) ;
    if ((imageno > 31) || (imageno < 0)) {
	printf("ERROR: invalid image number\n") ;
	return ;
    }

    /* can I find it? */
    blockno = find_image(imageno) ;
    if (blockno < 0) {
	printf("ERROR: no such image\n") ;
	return ;
    }

#ifdef ANGEL
    /* cannot delete image starting in flash block 0 */
    if (blockno == 0) { 
	printf("ERROR: cannot delete image in flash block 0\n") ;
	return ;
    }
#endif

    /* does the user really want to? */
    if (!yes_no()) return ;

    if ( !checkChecksum() ){
	/* Bad checksum, delete only the first block to avoid corruption */
        printf( "WARNING: Bad checksum; "
                "only deleting first block (block no %d)\n", 
                blockno);
        deleteBlock( blockno );
    }else{
	/* delete it */
	printf("Deleting flash blocks: ") ;
	while ( S_blocks[blockno]==imageno && blockno< FD->GetNumBlocks() ){
	    printf("%d ", blockno) ;
	    deleteBlock( blockno++ ) ;
	}
    }
    printf("\n") ;
    scan_blocks();

}

/* delete a block in flash, so long as it is not in an image */
static void delete_blockAction(void) 
{
    char *token ;
    int doneone = 0;

    for ( ; (token = __PULL()) != NULL; doneone = 1 ){
        /* did the user supply a block number? */

	/* ...and is it valid? */
        int blockno = atoi(token) ;
 
        if ( !IN_RANGE( blockno, 0, FD->GetNumBlocks()-1 ) ){
            printf("ERROR: invalid block number\n") ;
            return ;
        }

	/* can it be deleted? */
        if (BLOCK_IN_USE(blockno))  {
	    printf("ERROR: that block is not free (image # %d)\n",
		   S_blocks[blockno]) ;
	    return ;
        }

	/* does the user really want to? */
        if (!yes_no()) return ;

        printf("Delete flash block %d\n", blockno) ;

        deleteBlock( blockno );
        scan_blocks();
    }

    if ( doneone == 0 ) {
        printf("ERROR: you should supply a block number to delete\n") ;
        return ;
    }
}

/* delete all blocks except block 0 */
static void delete_allAction(void) 
{
    /* does the user really want to do this? */
    if (!yes_no()) return ;

    printf( "do you want to use chip erase function ?\n" );
    if ( yes_no() ){
	FD->ChipErase();
	printf( "Sleeping for 60 secs. Take five\n"
		"<ctrl-C> aborts\n" );

	{
	    for ( int i = 1; i <= 60; ++i ){
		sleep( 1 );
		putchar( '0' + i%10 ); 
		fflush( stdout );
	    }
	    putchar( '\n' );
	}
	return;
    }
    printf("Deleting flash blocks: ") ;
    for ( int block = 1 ; block < FD->GetNumBlocks(); block++ )  {
	printf("%d ", block) ;
	deleteBlock( block );
    }
    printf("\n") ;
}

/* read an AIF header and check it */
static int read_aif_header(FILE *file, aif_t *aif) 
{
    int data ;

    /* Read the AIF image header */
    for (unsigned i = 0 ; i < sizeof(aif_t) ; i++) {
	data = fgetc(file) ;
	if (data == EOF) {
	    printf("ERROR: reading file\n") ;
	    __fclose(file) ;
	    return -1 ;
	}
	*((char *)(aif) + i) = data ;
    }
  
    /* reset the file to offset 0 */
    __fseek(file, 0, SEEK_SET) ;

    /* return success */
    return 0 ;
}

/* program an image into flash */
static void program_flash(int imageno, 
			  char *name, 
			  int blockno, 
			  char *filename, 
			  int noboot) 
{
    FILE *file ;
    int blocks ;
    aif_t aif ;
    int i, j, offset ;
    int map ;
    int size ;
    unsigned int data ;
    int checksum;

    /* open the file */
    file = __fopen(filename, "rb") ;
    if (file == NULL) {
	printf("ERROR: failed to open file\n") ;
	return ;
    }

    /* is there room? */
    size = (int)fsize(file) ;

    int nlongs =  (size + IMAGE_HEADER_SIZE)/sizeof(int);
    blocks = nlongs/FD->GetBlockLen(blockno) ;

    if ( (nlongs % FD->GetBlockLen(blockno) ) != 0) blocks++ ;

    blockno = find_room(nlongs, blockno, &map, &blocks) ;
    if (blockno < 0) {
	printf("ERROR: no room for the image in flash\n") ;
	return ;
    }

    /* read the aif header into memory (to play with) */
    if (read_aif_header(file, &aif) < 0)  {
	__fclose(file) ;
	return ; 
    }

    /* tell the world what is going on */
    if (blocks == 1) 
	printf("Writing %s into flash block %d",
	       filename, blockno) ;
    else {
	printf("Writing %s into flash: blocks ",
	       filename) ;
	for (i = blockno ; i < FD->GetNumBlocks(); i++) {
	    if(map & (1<<i)) { 
		printf("%d ", i) ;
	    }
	}
    }

    /* delete the flash blocks that this image will take up */
    printf("\nDeleting blocks ready to program:\n") ;
    for (i = blockno ; i < FD->GetNumBlocks(); i++) {
	if ( (map & (1<<i)) && BLOCK_IN_USE(i) ){ 
	    printf("Deleting block %d \n", i) ;
	    deleteBlock( i );
	}
    }

    /* Calculate the file checksum. The checksum is simply the (little
       endian) 32 bit sum of the 32 bit words of the file. 
       Overflow is ignored, and excess bytes in the last word are treated 
       as 0xff (since this is the value of unprogrammed bytes of a flash).
       The checksum of the header will added in in write_flash_header */
    printf("Calculating checksum\n");
    checksum=0;
    __fseek(file, 0, SEEK_SET) ;
    for(i=0;i<size;i++) {
	checksum += __fgetc(file) << ((i%4)*8);
    }
    for(i=((size-1)%4)+1;i<4;i++) {
	checksum += 0xff << i*8;
    }

    /* build an image header and write it to flash */
    offset = FD->GetBlockStart( blockno );
    offset = write_flash_header(offset, &aif, imageno, name, map,
				size, checksum, noboot) ;
    if  (offset < 0) {
	__fclose(file) ;
	return ;
    }

    /* write the whole file into flash */
    __fseek(file, 0, SEEK_SET) ;
    printf("Writing image file\n") ;
    putchar('#') ;
    for (i = 0; i < size; i += 4) {
	if ((((unsigned int)offset) % 512) == 0) putchar('#') ;
	if ((((unsigned int)offset) % FD->GetBlockLen(blockno) ) == 0) {
	    blockno++;
	    while(!(map & (1 << blockno))) {
		blockno++;
	    }
	    offset = FD->GetBlockStart( blockno );
	}
	data = 0 ;
	for (j = 0 ; j < 4 ; j++) {
	    data |= (__fgetc(file) << (j * 8)) ;
	}
	FD->write( offset++, data );
    }
    putchar('\n') ;

    /* close the file */
    __fclose(file) ;
    printf( "image write done\n" );
    /* 
     * finally, re-scan the flash blocks 
     */
    scan_blocks() ;
}

/* program an image into flash */
static void programAction(void) 
{
    char *token ;
    int imageno ;
    int blockno = -1 ;
    char *imagename ;
    char *filename ;
    int noboot = FALSE;

    /*
     *  Get all the arguments.
     */
    /* image number supplied? */
    token = __PULL() ;
    if (token == NULL) {
	printf("ERROR: you should supply an image number\n") ;
	return ;
    }
    imageno = atoi(token) ;

    /* is it a valid (unused) image number? */
    if (find_image(imageno) >= 0) {
	printf("ERROR: image number already in use\n") ;
	return ;
    }

    /* image name supplied? */
    token = __PULL() ;
    if (token == NULL) {
	printf("ERROR: you should supply an image name\n") ;
	return ;
    }
    imagename = token ;
  
    /* file name supplied? */
    token = __PULL() ;
    if (token == NULL) {
	printf("ERROR: you should supply a file name\n") ;
	return ;
    }
    filename = token ;

    /* block number supplied */
    token = __PULL() ;
    if (token != NULL) {
	if(isdigit(*token)) {
	    blockno = atoi(token) ;
	    if ((blockno < 0) || (blockno > (FD->GetNumBlocks() -1))) {
		printf("ERROR: invalid block number\n") ;
		return ;
	    }
	    if (BLOCK_IN_USE(blockno)) {
		printf("ERROR: block in use by image %d\n", S_blocks[blockno]) ;
		return ;
	    }
	    token = __PULL() ;
	}
    }
    if(token != NULL) {
	char *tp=token;
	while(*tp!=0) {
	    *tp=tolower(*tp);
	    tp++;
	}
	if(strcmp(token,"noboot") == 0) {
	    noboot=TRUE;
	} else {
	    printf("ERROR: bad option %s\n", token);
	    return;
	}
    }

    /*
     * program the image into flash.
     */ 
    program_flash(imageno, imagename, blockno, filename, noboot) ;
}

/* Read an image from flash */
static void readAction(void) 
{
    FILE	*file ;
    char	*token ;
    char	*filename ;
    int	 imageno ;
    int	 blockno = -1 ;
    int	 flash, checksum ;
    int	 image_offset, flash_offset ;

    /*
     *  Get all the arguments.
     */
    /* image number supplied? */
    token = __PULL() ;
    if ((token == NULL) || (isdigit(*token) == 0)) {
	printf("ERROR: you must supply an image number\n") ;
	return ;
    }
    imageno = atoi(token) ;

    /* is it a valid (used) image number? */
    blockno = find_image(imageno) ;
    if (blockno < 0) {
	printf("ERROR: image %d is empty\n", imageno) ;
	return ;
    }

    /* file name supplied? */
    token = __PULL() ;
    if (token == NULL) {
        printf("ERROR: you must supply a file name\n") ;
	return ;
    }
    filename = token ;

    token = __PULL() ;

    /* Any stray paramters? */
    if(token != NULL) {
	printf("ERROR: bad option %s\n", token);
	return;
    }

    /* open the file */
    file = fopen(filename, "rb") ;
    if (file != NULL) {
	printf("WARNING: this will overwrite the file %s\n", filename) ;
	if (!yes_no())
	    return ;
    }

    file = fopen(filename, "wb") ;
    if (file == NULL) {
	printf("ERROR: failed to open file\n") ;
	return ;
    }

    image_offset = 0 ;
    checksum = 0 ;
    read_flash_header(blockno) ;

    if (!valid_flash_header(header)) {
	printf("ERROR: image header is corrupt\n") ;
	return ;
    }

    for ( int blockno = 0; blockno < FD->GetNumBlocks(); blockno++) {
        if (S_blocks[blockno] == header->Number) {
	    flash_offset = FD->GetBlockStart( blockno  ); 

	    for (int nlongs = 0; 
                 nlongs < FD->GetBlockLen(blockno); nlongs++ ) {
		if (image_offset >= header->Length) {
  		    break;
		}else{
		    flash = FD->read( flash_offset++ );
		    checksum += flash ;

  		    if ( image_offset >= IMAGE_HEADER_SIZE) {
			fputc( flash & 0xFF,         file);
			fputc( (flash >> 8) &  0xFF, file);
			fputc( (flash >> 16) & 0xFF, file);
			fputc( (flash >> 24) & 0xFF, file);
		    }
 		    image_offset += 4 ;
		}
	    }
	}
    }

    __fclose(file) ;

    if(checksum != 0) {
	printf("Checksum error; image checksum is 0x%x, should be 0\n",
	       checksum);
    }
}

/* let's get outa here! */
static void quitAction(void) 
{
    exit(0);
}

static void helpAction(void) 
{
#define COMMAND_SIZE 20
    unsigned i, count;

    printf("FMU command summary:\n\n");

    for (i = 0; i < NUM_FMU_COMMANDS; i++) {
	printf("%s ", commands[i].name);
	count = strlen(commands[i].name);
	count++;
	printf("%s ", commands[i].help_args);
	count += strlen(commands[i].help_args);
	count++;
	if (count < COMMAND_SIZE) {
	    count = COMMAND_SIZE - count;
	} else {
	    printf("\n");
	    count = COMMAND_SIZE;
	}
	while(count--) printf(" ");
	printf("- %s\n", commands[i].help_text); 
    }
}

static void mapBlocksAction()
{
    for( int ii = 0; ii != FD->GetNumBlocks(); ++ii ){
        printf( "[%2d] 0x%08x 0x%08x 0x%06x\n",
		ii, FD->GetBlockStart( ii )*4, 
		FD->GetBlockStart(ii)*4+FD->GetBlockLen(ii)*4,
		FD->GetBlockLen(ii)*4
		);
    } 
}
static int partial_compare(char *token, char *command) 
{
    int len ;
    int i ;

    if (strlen(command) < strlen(token)) return FALSE ;
    len = strlen(token) ;

    for (i = 0 ; i < len ; i++)  
	if (tolower(*token++) != tolower(*command++)) return FALSE ;

    return TRUE ;
}

static int compare(char *token, char *command) 
{
    int len ;
    int i ;

    if (strlen(command) != strlen(token)) return FALSE ;
    len = strlen(token) ;

    for (i = 0 ; i < len ; i++)  
	if (tolower(*token++) != tolower(*command++)) return FALSE ;

    return TRUE ;
}

static void parse_command(void)
{
    char *token;
    unsigned i;
    int found = 0;
    int index = -1 ;

    /* now work out what the command was from the first token */
    token = __PULL();
    if (token != NULL) {
	/* first, look for an absolute match */
	for (i = 0; i < NUM_FMU_COMMANDS; i++) {
	    if (compare(token, commands[i].name) == TRUE ||
		compare(token, commands[i].alias) == TRUE) {
		(commands[i].action_rtn)();
		return ;
	    }
	}
	/* now, look for a partial absolute match */
	for (i = 0; i < NUM_FMU_COMMANDS; i++) {
	    if (partial_compare(token, commands[i].name) == TRUE ||
		partial_compare(token, commands[i].alias) == TRUE) {
		found++ ;
		index = i ;
	    }
	}
	if (found == 0) 
	    printf("FMU: unknown command, try typing Help\n");
	else {
	    if (found > 1) 
		printf("ERROR: more than one command matches, try typing Help\n") ;
	    else 
		(commands[index].action_rtn)();
	}
    }
}

static void parse_command_string( const char *_string)
{
    char string[180];
    char *token;
    
    strcpy( string, _string );

    /* Grab all of the command tokens */
    ntokens = 0;
    token = strtok(string, " \t");
    while(token != NULL) {
	/* treat ';' as end of line, new command */
	if (strcmp(token, ";") == 0) {
	    parse_command();
	    ntokens = 0;
	} else {
	    __ADD(token);
	}
	token = strtok(NULL, " \t");
    }

    /* scan the flash blocks for images */
    scan_blocks() ;

    /* now that we've found it, parse it. */
    parse_command();

}

/* ---------- I M A G E --- S U P P O R T --- R O U T I N E S ----------------*/

/* given the size of the image (in bytes), find room for it */
static int find_room(int nlongs, int first, int * map, int* pblocks_used) 
{
    int last ;
    int nfree = 0;
    int header_block = 0;

    *map=0;

    /* find a sequence of N free blocks */
    if (first >= 0) {
        header_block = first;
	/* The user has specified a block number so they must be sequential
	   from that block */
	for(last = first; last < FD->GetNumBlocks(); last++) {

            
	    if(BLOCK_IN_USE(last)) {
		/* Can't put it where the user specified, give up */
		return -1;
	    } else {
		*map |= (1<<last);
	    }
            nfree += FD->GetBlockLen(last);
            if ( nfree >= nlongs ){
                break;
            }
	}
    } else {
        
	for( last=0; last < FD->GetNumBlocks(); last++) {
	    if(!BLOCK_IN_USE(last)) {
		*map  |= (1<<last);
		if( nfree ==0 ) {
		    header_block=last;
		}
		nfree += FD->GetBlockLen(last);
		if( nfree >= nlongs ) {
                    goto found_ok;
		}
	    }
	}
	
	/* no room */
	return -1 ; 
    }

 found_ok:
    *pblocks_used = last - first + 1;
    return header_block;    
}

static int find_image(int imageno)
{
    int i ;

    for (i = 0 ; i < FD->GetNumBlocks() ; i++) {
	if (S_blocks[i] == imageno) return i ;
    }
  
    return -1 ;
}

static int valid_flash_header(struct image_header *header) 
{
    /* perform a series of tests to validate the header */
    /* valid signature (0x55, 0xaa, 0x00)? */
    if (header->Sig[0] != 0x55) return FALSE ;
    if (header->Sig[1] != 0xaa) return FALSE ;
    if (header->Sig[2] != 0x00) return FALSE ;

    /* return happily */
    return TRUE ;
}

static void read_flash_header(int blockno)
{
    unsigned	 i;
    int		 offset = FD->GetBlockStart( blockno ) ;
    unsigned	 *buffer = (unsigned int *)header ;

    for (i = 0 ; i < sizeof(image_header_t) ; i += 4 ) {
	*buffer++ = FD->read( offset++ ) ;
    }
}

static void print_flash_header(struct image_header *header)
{
    unsigned i ;

    printf("Image %d \"", header->Number) ;
    for (i = 0; i < sizeof(header->Name); i++) {
        printf("%c", header->Name[i]) ;
    }
    printf("\" Length %d bytes", header->Length) ;
    printf(", Map 0x%08x", header->Map) ;
    if((header->Bootflags) & NOBOOTMASK) {
        printf(" Noboot");
    }
    printf("\n");
}

static void scan_blocks(void) 
{
    int j ;
    int blockno ;

    printf("Scanning Flash blocks for usage\n") ;

    /* assume the worst */
    for (blockno = 0 ; blockno < FD->GetNumBlocks() ; blockno++)  
        S_blocks[blockno] = (char) -1 ;

    /* look at every block in turn, trying to find a header. */
    for (blockno = 0 ; blockno < FD->GetNumBlocks() ; blockno++)  {
	read_flash_header(blockno) ;
	if (valid_flash_header(header) && (S_blocks[blockno] == (char) -1)) {
	    for (j = 0 ; j < FD->GetNumBlocks() ; j++) {
		if (((header->Map) & (1 << j)) != 0) {
		    if(S_blocks[j] == (char) -1){
			S_blocks[j] = header->Number ;
		    } else {
			printf("ERROR: Two images own block %d\n",j);
		    }
		}
	    }
	}
    }
}

static int write_flash_header(int offset, aif_t *aif, int unique, char *name, int map, int size, int file_checksum, int noboot) 
{
    int count;
    unsigned int *data = NULL ;
    int pad = 0 ;
    int checksum;
    int * header_p;
  

    /* tell the world */
    printf("Writing flash image header\n") ;

    /* zero out the header */
    memset((char *)header, 0, sizeof(image_header_t))  ;

    /* work out what sort of image it is */
    if ((aif->EntryPoint & BL) == BL) {
	printf("Image is Executable AIF file\n") ;
	printf("Image will execute from Flash\n") ;
	/* Create a BL to the start of the aif header (The aif header itself is executable) */
	header->Type = BINSTR(0x40)  ; 
    } else {
	printf("Image is non-executable AIF file\n") ;
	pad = IMAGE_HEADER_SIZE - sizeof(image_header_t) ;
	if((aif->ImageBase > FLASH_BASE) && (aif->ImageBase < FLASH_END)) {
	    /* The base address is within the EPROM or FLASH area. This
	     * only makes sense if the image is to be executed from FLASH.
	     */
	  
	    /* Check that the bottom bits of the base address are correct */
	    if((aif->ImageBase & 0x00ffffff) != (unsigned)offset+0xc0) {
		printf("ERROR: image base address is in ROM but does not match address of flash block\n") ;
		printf("       Image base address = 0x%x\n",aif->ImageBase);
		printf("       Expected start of image = 0x??%06x\n",offset+0xc0);
		return -1;
	    }
	    printf("Image will execute from Flash\n") ;
	    /* Put in a branch to the entry point */
	    header->Type= BINSTR((0xc0 + aif->EntryPoint));
	} else {
	    printf("The bootloader will copy this image to %x before executing it\n",aif->ImageBase);
	    printf("       Eprom is from 0x%x to 0x%x\n", FLASH_BASE, FLASH_END) ;
	    /* The bootloader will relocate the image */
	    header->Type = NOP ;
	}
    }

    header->Number = unique ;

    header->Sig[0] = 0x55 ;
    header->Sig[1] = 0xaa ;
    header->Sig[2] = 0x00 ;

    header->Map = map ;
    header->Bootflags = noboot & NOBOOTMASK;

    /* it has a name */
    for (unsigned i = 0 ; i < sizeof(header->Name); i++) header->Name[i] = ' ';
    for (unsigned i = 0 ; i < strlen(name) ; i++) header->Name[i] = name[i] ;

    /* it's size */
    header->Length = size + IMAGE_HEADER_SIZE ;

    /* work out the checksum */
    header->Checksum = 0 ;

    checksum=file_checksum;
    header_p = (int *)header;
    for( unsigned i=0; i<sizeof(image_header_t); i+=4) {
	checksum += *(header_p++);
    }
    /* Negate the checksum so that the final checksum of the whole image is 0 */
    header->Checksum = -checksum;
  
    /* write it out */
    count = sizeof(image_header_t) ;
    data = (unsigned int *)header ;
    do {
	if (!FD->write( offset++, *data++)) {
	    printf("ERROR: writing flash header at offset 0x%x (%d)\n",
		   offset, offset) ;
	    return -1 ;
	}
    } while ( (count -= 4) > 0) ;

    printf("padding is %d bytes\n", pad) ;
    /* pad it out */
    while (pad) {
	if (!FD->write(offset++, 0)) {
	    printf("ERROR: writing flash header at offset 0x%x (%d)\n",
		   offset, offset) ;
	    return -1 ;
	}
	pad -= 4 ;
    }
     
    /* return successfully */
    return offset ;
}


/* ------------------------ M A I N ---------------------------- */
#define fmuBanner \
    "D-TACQ solutions ACQ32 Flash Management Utility $Revision: 1.17 $\n"\
    "Digital EBSA-285 Flash Management Utility [v1.4]\n" \
    "Build Date: " __DATE__ "\n"

int G_debug;

#define HELP \
"pfmu [opts] [commands]\n"\
"-b : board {1..4}\n"\
"-d : devname (default:" FNAME ")\n"\
"-I : flashid\n"\
"-D <n> : set debug level\n"\
"-l : loop on ident phase\n"\
"-h : help\n"

int main( int argc, const char* argv[] )
{  
    char devname_buf[80];
    char* devname = FNAME;
    char* ids     = "0xdeadbeef";
    int iboard;
    int loop = 0;

    struct poptOption opt_table[] = {
	{ "board",   'b', POPT_ARG_INT,    &iboard,    'b' },
	{ "device",  'd', POPT_ARG_STRING, &devname,   0   },
	{ "flashid", 'I', POPT_ARG_STRING,     &ids,   'I' },
	{ "debug",   'D', POPT_ARG_INT,    &G_debug,   0   },
	{ "help",    'h', POPT_ARG_NONE,          0,   'h' },
	{ "loop",    'l', POPT_ARG_NONE,          0,   'l' },
	{}
    };

    poptContext opt_context =
        poptGetContext( argv[0],argc, argv, opt_table, 0 );
    int rc;

    /* tell the world just who we are */
    printf(fmuBanner);
    
    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 'b':
            sprintf( devname_buf, "/dev/acq32/acq32.%d.flash", iboard );
            devname = devname_buf;
            break;
        case 'I':
	    FlashDevice::SetAlternateId( strtoul( ids, 0, 0 ) );
            break;
	case 'l':
	    loop = 1;
	    break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        default:
            fprintf( stderr, "option not recognised\n" );
            return 1;
	}
    }

    printf( "Using device %s\n", devname );

    Board board( devname );

    PDEBUGL(1)( "Board instantiated\n" );

    do {
	FD = board.GetFlashDevice();
    }
    while( loop );

    PDEBUGL(1)( "GetFlashDevice() returned\n" );

    S_blocks = (int*)calloc( FD->GetNumBlocks(), sizeof(int) );
    ASSERT( S_blocks );

    header = (struct image_header *)malloc(IMAGE_HEADER_SIZE) ;

    ASSERT( header );

    /* now just get strings from the user and parse them */

    const char* nextarg;
    bool interactive = true;

    while ( (nextarg = poptGetArg( opt_context )) != NULL ){
        parse_command_string( nextarg );
        interactive = false;
    }

    if ( !interactive ){
        return 0;
    }else{
 
        char command[80] ;

        printf("FMU> ");
        while (fgets( command, 79, stdin) != 0) {
            command[strlen(command)-1] = '\0';
            parse_command_string(command) ;
            printf("FMU> ") ;
        }

        return 0;
    }
}

