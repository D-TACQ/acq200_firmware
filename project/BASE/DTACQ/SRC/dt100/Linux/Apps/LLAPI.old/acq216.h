/* ------------------------------------------------------------------------- */
/* ACQ216 API                                                                */
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


/** class maps entire Low Latency API for ACQ216. 
 *   methods return 0 on SUCCESS, or negative number on error.
 */


#ifndef __ACQ216_CAPI_H__
/** nominal range controls. */
enum M5VRange {
	VR_ACQ216_M5_30,
	VR_ACQ216_M5_50,
	VR_ACQ216_M5_625,
	VR_ACQ216_M5_1,
	VR_ACQ216_M5_1p9,
	VR_ACQ216_M5_3
};



/** actual calibrated range. */
struct VRange {
	double vmin;
	double vmax;
};

enum SignalLine {
	DI0, DI1, DI2, DI3, DI4, DI5
};
/** defines DIO signaling edge. */
enum SignalEdge {
	rising, falling
};

#endif


typedef int boolean;

/** defines API for ACQ216 . 
 *  Applications will obtain ACQ216LL objects via the factory method.
*/
class ACQ216 {

protected:
	ACQ216() {}
	virtual ~ACQ216() {}

public:
	static ACQ216& create(int board_id, int flags = 0);
	/**< factory method. instantiates object and calls open(). */

	virtual	int open(int board_id, int flags = 0) = 0;
	/**< connects to the card, refreshes cache of calibration values. */

	virtual	int close()= 0;
	/**< disconnect from the card. */

	virtual	int setSampleClockDDS(int enable)= 0;
	/**< select DDS as the sample clock. */
	
	virtual	int setDDS_ClockRate(double rate)= 0;
	/**< set the clock rate 1..160MHz. */

	virtual	int getDDS_ClockRate(double& rate)= 0;
	/**< get the clock rate. */
	

	virtual	int setVRange(M5VRange range)= 0;
	/**< set input voltage range all channels. */

	virtual	int setVRange(M5VRange range, int channel)= 0;
	/**< set input voltage range for single channel. */

	virtual	int getVRange(int channel, M5VRange& range)= 0;
	/**< get nominal range for channel. */

	virtual	int getVRange(int channel, VRange& range)= 0;
	/**< get calibrated range for channel. */

	virtual	int setVOffset(double percent)= 0;
	/**< set input voltage offset as percent full scale for all channels. */
	virtual	int setVOffset(double percent, int channel)= 0;
	/**< set input voltage offset as percent full scale for channel. */

	virtual	int getVOffset(int channel, double& percent)= 0;
	/**< get input voltage offset as percent full scale for channel. */
	

/**< input impedance is NOT software controlled.
extern int DT_set_impedance(Dev_t *dev, double impedance)= 0;
extern int DT_get_impedance(Dev_t *dev, double *impedance)= 0;
*/
	/** defines DIO signaling line. */

	virtual	int setSoftTrigger(boolean enable)= 0;
	/**< enable/disable soft trigger. */

	virtual	int getSoftTrigger(boolean& enabled)= 0;
	/**< return soft trigger enable state. */

	virtual	int setHardTriggerSource(SignalLine dio, SignalEdge edge)= 0;
	/**< set Hard Trigger Source and edge. */
	
	virtual	int getHardTriggerSource(SignalLine& dio, SignalEdge& edge)= 0;
	/**< get Hard Trigger Settings. */

	virtual	int setChannels(int nchan)= 0;
	/**< select channels to capture {2/4/8/12}. */

	virtual	int getChannels(int& nchan)= 0;
	/**< return number of channels current selected. */

	virtual	int setCaptureLength(int nsamples)= 0;
	/**< set number of samples to capture. */

	virtual	int getCaptureLength(int& nsamples)= 0;
	/**< get number of samples to capture. */

	virtual	int capture(int nshots = 1)= 0;
	/**< enable the card - flushes parameter cache to the card, 
	 *  captures a data set and returns.
	 */

	virtual	int printCaptureStats(void)= 0;
	/**< prints a report detailing capture timings. */

	virtual	int read(int ch, double* buf, int max_elements)= 0;
	/**< read at most max_elements calibrated data from host buffer.
         *  returns actual number elements read.
	 */
	       

	virtual	int read(int ch, short* buf, int max_elements)= 0;
	/**< read at most max_elements raw data from host buffer,
	 *   returns actual number of elements read.
	 */
};
