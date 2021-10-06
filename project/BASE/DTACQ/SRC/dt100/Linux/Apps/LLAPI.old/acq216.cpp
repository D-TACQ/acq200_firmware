/* ACQ216 - basic test harness                                               */
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

#include "local.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#include "acq216.h"

#define MAXCHAN 16

class ACQ216TEST : public ACQ216 {
private:
	double sample_clock_dds;
	M5VRange ranges[MAXCHAN+1];   /** index from 1. */

public:
	ACQ216TEST() {

	}
	virtual ~ACQ216TEST() {

	}

	virtual int open(int board_id, int optional_flags = 0) {
	/**< connects to the card, refreshes cache of calibration values. */
		return 0;
	}

	virtual int close() {
	/**< disconnect from the card. */
		return 0;
	}

	virtual int setSampleClockDDS(int enable) {
	/**< select DDS as the sample clock. */
		return 0;
	}
	
	virtual int setDDS_ClockRate(double rate) {
	/**< set the clock rate 1..160MHz. */
		sample_clock_dds = rate;
		return 0;
	}
	virtual int getDDS_ClockRate(double& rate) {
	/**< get the clock rate. */
		rate = sample_clock_dds;
		return 0;
	}
	

	virtual int setVRange(M5VRange range) {
	/**< set input voltage range all channels. */
		for (int ii = 1; ii <= MAXCHAN; ++ii){
			ranges[ii] = range;
		}

		return 0;
	}

	virtual int setVRange(M5VRange range, int channel) {
	/**< set input voltage range for single channel. */
		assert(channel >= 1 && channel <= MAXCHAN);
		ranges[channel] = range;

		return 0;
	}

	virtual int getVRange(int channel, M5VRange& range) {
	/**< get nominal range for channel. */

		return 0;
	}

	virtual int getVRange(int channel, VRange& range) {
	/**< get calibrated range for channel. */

		return 0;
	}

	virtual	int setVOffset(double percent) {
	/**< set input voltage offset as percent full scale for all channels. */
		return 0;
	}


	virtual	int setVOffset(double percent, int channel) {
	/**< set input voltage offset as percent full scale for channel. */
		
		return 0;
	}
	virtual int getVOffset(int channel, double& percent) {
	/**< get input voltage offset as percent full scale for channel. */
		
		return 0;
	}


	virtual	int setSoftTrigger(boolean enable) {
	/**< enable/disable soft trigger. */
		return 0;
	}

	virtual	int getSoftTrigger(boolean& enabled) {
	/**< return soft trigger enable state. */
		return 0;
	}

	virtual	int setHardTriggerSource(SignalLine dio, SignalEdge edge) {
	/**< set Hard Trigger Source and edge. */
		return 0;
	}
	
	virtual	int getHardTriggerSource(SignalLine& dio, SignalEdge& edge) {
	/**< get Hard Trigger Settings. */
		return 0;
	}


	virtual	int setChannels(int nchan) {
	/**< select channels to capture {2/4/8/12}. */
		return 0;
	}

	virtual	int getChannels(int& nchan) {
	/**< return number of channels current selected. */
		return 0;
	}

	virtual	int setCaptureLength(int nsamples) {
	/**< set number of samples to capture. */
		return 0;
	}
	virtual int getCaptureLength(int& nsamples) {
	/**< get number of samples to capture. */
		return 0;
	}
	virtual	int capture(int nshots = 1) {
	/**< enable the card - flushes parameter cache to the card, 
	 *  captures a data set and returns.
	 */

		return 0;
	}

	virtual int printCaptureStats(void) {
	/**< prints a report detailing capture timings. */
		return 0;
	}

	virtual	int read(int ch, double* buf, int max_elements) {
	/**< read at most max_elements calibrated data from host buffer.
         *  returns actual number elements read.
	 */

		return 0;
	}
	       

	virtual	int read(int ch, short* buf, int max_elements) {
	/**< read at most max_elements raw data from host buffer,
	 *   returns actual number of elements read.
	 */
		return 0;
	}
};

ACQ216& ACQ216::create(int board_id, int flags)
{
	ACQ216& myboard =  *new ACQ216TEST();

	myboard.open(board_id, flags);

	return myboard;
}


int main(int argc, char* argv[])
{
	ACQ216& myboard = ACQ216::create(2);

/** typical "test sequence" .*/	
	myboard.setDDS_ClockRate(123456);
	double clock_rate;
	myboard.getDDS_ClockRate(clock_rate);
	printf("getDDS_ClockRate: %f\n", clock_rate);

	myboard.setVRange(VR_ACQ216_M5_625);
	M5VRange range;
	myboard.getVRange(4, range);
	printf("getVRange %d\n", range);

	myboard.setCaptureLength(1024);
	myboard.capture();
	
	double* data = new double[1024];
	myboard.read(1, data, 1024);

	myboard.close();
}
