//
// Copyright (C) 1995
// Digital Equipment Corporation, Maynard, Massachusetts.
// All Rights Reserved
//
// Permission to  use, copy,  modify, and distribute  this software
// and its documentation for any  purpose and without fee is hereby 
// granted, provided that the  copyright notice and this permission 
// notice   appear  in  all   copies  of  software  and  supporting  
// documentation,  and  that  the  name  of Digital  not be used in 
// advertising or  publicity  pertaining to   distribution  of  the 
// software  without  specific,  written prior  permission. Digital  
// grants  this  permission  provided  that you  prominently  mark, 
// as  not part  of the original,  any modifications that  is  made  
// to this software or documentation.
//
// Digital Equipment Corporation  disclaims all  warranties  and/or 
// guarantees with  regard to  this software, including all implied 
// warranties   of  fitness  for   a   particular    purpose    and   
// merchantibility, and makes no representations regarding  use of,  
// or  the  results of  the use of, the  software and documentation 
// in terms  of correctness, accuracy, reliability, currentness, or 
// otherwise;  and   you rely  on  the software,  documentation and 
// results solely at your own risk.
//
// ======================================================================
//
// Abstract:          some C declarations and definitions
//
// Author:            cjm (Digital Semiconductor Engineering) 
//
// Hardware Platform: 90 Mhz Pentium PCI Based PC 
//
// Operating System:  MSDOS V6.22
//
// Development Env:   Watcom C/C++ V10.0 Professional Development System
//
// Modifications:
//
//    $History: CDEF.H $
// 
// *****************  Version 2  *****************
// User: Carey        Date: 6/08/95    Time: 1:24p
// Updated in $/PCI Library
// Cleaned up comments et al.
// 
// *****************  Version 1  *****************
// User: Carey        Date: 6/08/95    Time: 12:00p
// Created in $/PCI Library
// Portable C definitions header file
// 
//


//
// type definitions
//
typedef unsigned char   U8,  *PU8;
typedef unsigned short  U16, *PU16;
typedef unsigned long   U32, *PU32;
typedef char            S8,  *PS8;
typedef short           S16, *PS16;
typedef long            S32, *PS32;

#define MSG_FAILURE 0
#define MSG_SUCCESS 1
#define PPB_VERBOSE 1                   // this determines printing

/*
#define INT_REGS U16 bp,U16 di,U16 si,U16 ds,U16 es,U16 dx,U16 cx, \
                                    U16 bx,U16 ax,U16 ip,U16 cs,U16 flags
*/

