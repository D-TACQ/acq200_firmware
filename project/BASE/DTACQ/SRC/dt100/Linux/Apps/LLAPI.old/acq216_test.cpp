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


int main(int argc, char* argv[])
{
	ACQ216& myboard = ACQ216::create(2);

/** typical "test sequence" .*/	
	myboard.setDDS_ClockRate(123456);
	double clock_rate;
	myboard.getDDS_ClockRate(clock_rate);
	printf("getDDS_ClockRate: %f\n", clock_rate);

	myboard.setVRange(VR_ACQ216_M5_625);
	VRange vrange;
	myboard.getVRange(4, vrange);
	printf("getVRange %f,%f\n", vrange.vmin, vrange.vmax);

	myboard.setCaptureLength(1024);
	myboard.capture();
	
	double* data = new double[1024];
	myboard.read(1, data, 1024);

	myboard.close();
}
