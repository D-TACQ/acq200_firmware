/* ACQ216capi_test.c - basic test harness                                    */
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



/**
 * @file ACQ216capi_test.c - basic test harness for C API. 
 */
#include <stdio.h>
#include <stdlib.h>

#include "acq216_capi.h"

#define BOARD 1
#define FLAGS 0

#define CAPLEN 1024
#define TEST_CHAN 4
#define NSHOTS 1

int main(int argc, char* argv[])
{
	double clock_rate;
	VRange vrange;
	double* data = malloc(CAPLEN * sizeof(double));
	ACQ216_DEV* dev = ACQ216_open(BOARD, FLAGS);

	ACQ216_setDDS_ClockRate(dev, 123456);
	ACQ216_getDDS_ClockRate(dev, &clock_rate);

	printf("getDDS_ClockRate: %f\n", clock_rate);

	ACQ216_setVRange(dev, VR_ACQ216_M5_625);

	ACQ216_getVRange_actual(dev, TEST_CHAN, &vrange);
	printf("getVRange %f,%f\n", vrange.vmin, vrange.vmax);

	ACQ216_setCaptureLength(dev, CAPLEN);
	ACQ216_capture(dev, NSHOTS);

	ACQ216_read(dev, TEST_CHAN, data, CAPLEN);

	ACQ216_close(dev);
	return 0;
}
