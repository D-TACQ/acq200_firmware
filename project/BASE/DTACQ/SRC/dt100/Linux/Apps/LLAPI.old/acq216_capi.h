/* ------------------------------------------------------------------------- */
/* ACQ216 cAPI  'C' language API for ACQ216                                  */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2005 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU Lesser Public License
    as published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */


/** @file acq216_capi.h defines 'C' language bindings for ACQ216. 
 *  To make use of these bindings, link to acq216capi.so, acq216api.so
 *  And install appropriate driver.
 *
 *  maps entire Low Latency API for ACQ216. 
 *   methods return 0 on SUCCESS, or negative number on error.
 */

#ifndef __ACQ216_CAPI_H__
#define __ACQ216_CAPI_H__

/** opaque structure holds driver state. */
typedef struct ACQ216_DEV_STRUCT ACQ216_DEV;              

/** nominal range controls. */
enum M5VRange {
	VR_ACQ216_M5_30,
	VR_ACQ216_M5_50,
	VR_ACQ216_M5_625,
	VR_ACQ216_M5_1,
	VR_ACQ216_M5_1p9,
	VR_ACQ216_M5_3
};

typedef enum M5VRange M5VRange;

/** actual calibrated range. */
struct VRange {
	double vmin;
	double vmax;
};

typedef struct VRange VRange;

/** defines DIO signaling line. */
enum SignalLine {
	DI0, DI1, DI2, DI3, DI4, DI5
};

typedef enum SignalLine SignalLine;

/** defines DIO signaling edge. */
enum SignalEdge {
	rising, falling
};

typedef enum SignalEdge SignalEdge;

typedef int boolean;

/** defines API for ACQ216 . 
 *  Applications will obtain ACQ216LL objects via the factory method.
*/
ACQ216_DEV* ACQ216_open(int board_id, int flags ) ;
/**< connects to the card, refreshes cache of calibration values. */

extern int ACQ216_close(ACQ216_DEV* dev);
/**< disconnect from the card. */

extern int ACQ216_setSampleClockDDS(ACQ216_DEV* dev, int enable);
/**< select DDS as the sample clock. */
	
extern int ACQ216_setDDS_ClockRate(ACQ216_DEV* dev, double rate);
/**< set the clock rate 1..160MHz. */

extern int ACQ216_getDDS_ClockRate(ACQ216_DEV* dev, double* rate);
/**< get the clock rate. */
	

extern int ACQ216_setVRange(ACQ216_DEV* dev, M5VRange range);
/**< set input voltage range all channels. */

extern int ACQ216_setVRange_channel(
	ACQ216_DEV* dev, M5VRange range, int channel);
/**< set input voltage range for single channel. */

extern int ACQ216_getVRange_nominal(
	ACQ216_DEV* dev, int channel, M5VRange* range);
/**< get nominal range for channel. */

extern int ACQ216_getVRange_actual(
	ACQ216_DEV* dev, int channel, VRange* range);
/**< get calibrated range for channel. */

extern int ACQ216_setVOffset(ACQ216_DEV* dev, double percent);
/**< set input voltage offset as percent full scale for all channels. */

extern int ACQ216_setVOffset_channel(
	ACQ216_DEV* dev, double percent, int channel);
/**< set input voltage offset as percent full scale for channel. */

extern int ACQ216_getVOffset(
	ACQ216_DEV* dev, int channel, double* percent);
/**< get input voltage offset as percent full scale for channel. */
	

/**< input impedance is NOT software controlled.
   extern int DT_set_impedance(Dev_t *dev, double impedance);
   extern int DT_get_impedance(Dev_t *dev, double *impedance);
*/

extern int ACQ216_setSoftTrigger(ACQ216_DEV* dev, boolean enable);
/**< enable/disable soft trigger. */

extern int ACQ216_getSoftTrigger(ACQ216_DEV* dev, boolean* enabled);
/**< return soft trigger enable state. */

extern int ACQ216_setHardTriggerSource(
	ACQ216_DEV* dev, SignalLine dio, SignalEdge edge);
/**< set Hard Trigger Source and edge. */
	
extern int ACQ216_getHardTriggerSource(
	ACQ216_DEV* dev, SignalLine* dio, SignalEdge* edge);
/**< get Hard Trigger Settings. */

extern int ACQ216_setChannels(ACQ216_DEV* dev, int nchan);
/**< select channels to capture {2/4/8/12/16}. */

extern int ACQ216_getChannels(ACQ216_DEV* dev, int* nchan);
/**< return number of channels current selected. */

extern int ACQ216_setCaptureLength(ACQ216_DEV* dev, int nsamples);
/**< set number of samples to capture. */

extern int ACQ216_getCaptureLength(ACQ216_DEV* dev, int* nsamples);
/**< get number of samples to capture. */

extern int ACQ216_capture(ACQ216_DEV* dev, int nshots);
/**< enable the card - flushes parameter cache to the card, 
 *  captures a data set and returns.
 */

extern int ACQ216_printCaptureStats(ACQ216_DEV* dev);
/**< prints a report detailing capture timings. */

extern int ACQ216_read(
	ACQ216_DEV* dev, int ch, double* buf, int max_elements);
/**< read at most max_elements calibrated data from host buffer.
 *  returns actual number elements read.
 */
	       

extern int ACQ216_read_raw(
	ACQ216_DEV* dev, int ch, short* buf, int max_elements);
/**< read at most max_elements raw data from host buffer,
 *   returns actual number of elements read.
 */


#endif /*  __ACQ216_CAPI_H__ */
