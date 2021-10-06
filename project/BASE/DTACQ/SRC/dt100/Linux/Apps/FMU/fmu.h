//
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
//

/*
 *  Local definitions.
 */
#define FLASH_BYTE                      0x000000
#ifdef ANGEL
#define U32 unsigned int
#endif
#ifdef PGMCOMOUT
#define NUMBER_OF_BLOCKS                16
#endif
#ifndef TRUE 
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0 
#endif

#ifdef PGMCOMOUT
extern unsigned int flash_block_size ;
/*
 *  Externals. read_flash_dword
 */
extern unsigned int read_flash_dword(int offset) ;
extern int write_flash_dword(int offset, unsigned int data) ;
extern void delete_flash_block(int blockno) ;
extern int check_flash_id(volatile void *flash) ;

#endif



extern int G_debug;

#define PDEBUGL(level) if ( G_debug> (level)) printf









