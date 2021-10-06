/*****************************************************************************
 *
 * File:  dt100api.h: API for dt100 system
 *
 * $RCSfile: dt100api.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: dt100api.h,v 1.6.4.1 2006/01/28 18:18:55 pgm Exp $
 * $Log: dt100api.h,v $
 * Revision 1.6.4.1  2006/01/28 18:18:55  pgm
 * ST_CAPDONE
 *
 * Revision 1.6  2002/11/03 16:11:36  pgm
 * ST_POSTPROCESS
 *
 * Revision 1.5  2000/10/07 16:02:34  pgm
 * TC mode
 *
 * Revision 1.4  2000/10/01 21:32:26  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.2  2000/06/25 08:43:36  pgm
 * SOFT_CONTINUOUS
 *
 * Revision 1.1  1999/10/16 07:16:15  pgm
 * *** empty log message ***
 *
 *
\*****************************************************************************/


#ifndef _DT100API_H_
#define _DT100API_H_


enum STATE { 
	ST_STOP, ST_ARM, ST_RUN, ST_TRIGGER, ST_POSTPROCESS, 
	ST_CAPDONE, ST_UNKNOWN = -1
};

enum MODE { 			
    M_GATED_TRANSIENT, 
    M_GATED_CONTINUOUS,  
    M_SOFT_TRANSIENT,
    M_SOFT_CONTINUOUS,

	M_TRIGGERED_CONTINUOUS
};

#ifndef __KERNEL__
#ifndef __arm


int dt100_GetNumChannels();
// returns number of channels available in rack {32,64,96}



enum STATE dt100_GetState();
// returns current state

int dt100_SetChannelMask( short* channels, int maxchannels );
// <channels>: a user supplied array indicating channels to use
// <channels>[0] == 1 means "enable channel 0
// <channels>[0] == 0 means "disable channel 0
// <maxchannels>: number of elements { 32, 64, 96 }
// returns 0 or negative error code

int dt100_GetMemoryDepth( int channel );
// <channel> selects channel in range { 0..GetNumChannels()-1}
// returns depth of memory in samples for <channel>,
// Function of available memory depth on the board where
// <channel> is located and the current channel mask
// Eg - first board has 64MB memory:
// <channel>,    enabled channels,    dt100_GetMemoryDepth (0)
// 0         32, 0 is enabled,		  1M
// 0		  8, 0 is enabled, 	  4M
// 0         32, 0 NOT enabled,       0




int dt100_SetMode( enum MODE mode, int samples );
// <mode>               <samples>
// M_GATED_TRANSIENT  - NUM-SAMPLES
// M_GATED_CONTINUOUS - POST-SAMPLES
// M_SOFT_TRANSIENT   - NUM-SAMPLES { 0=>fill memory }

int dt100_SetModeGatedContinuous( 
    enum MODE mode, 
    int pre_samples,
    int post_samples
    );
     // set GATED_CONTINUOUS with control of pre-, post- samples



int dt100_SetArmed();	// request transition to ST_ARM

int dt100_SetAbort();	// request transition to ST_STOP

int dt100_GetNumSamples();
// returns number of samples available
// this is the number of clocks that have been captured.
// the total amount of data available is 
// GetNumSamples * enabled channels

int dt100_GetChannelData(
    unsigned short* buf,	// caller's buffer
    int chan,      		// channel number 
    int n0,			// the first sample to transfer
    int ninc,			// increment (stride)
    int nmax			// the max # samples to copy
);
// returns # transferred samples or negative error
// in GATED_CONTINUOUS mode, 
// n0 == 0 represents the point of trigger (GATE going OFF)
// examples
//  <n0>, <ninc>,	 <nmax>
//     0,      1,  100   get first 100 samples from trigger
//  -100,      1,  200   get 200 samples spanning trigger
//  -100,      1,  100   get 100 samples before trigger
//     0,     -1,  100   get 100 samples before trigger, 
//                       in reverse order ??!!



#endif                // __arm
#endif                // __KERNEL__







#endif             // _DT100API.H
