/* ------------------------------------------------------------------------- */
/* mthread_test.cpp: test harness for multithread wrapper lib                */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2003 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License 
    as published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>


#include "mthread.h"

class MySlaveThread: public SlaveThread {

public:
	MySlaveThread(  const char* _name ) : 
		SlaveThread( _name )
	{}
protected:
	virtual void exec_payload() {
		printf ( "MySlaveThread exec_payload %s\n", getName() );
	}
};



int main( int argc, char* argv[] )
{
	MasterThread* master = new MasterThread( "Master" );

	for ( int it = 0; it != NTHREADS; ++it ) {
		char buf[80];
		sprintf( buf, "Number %d", it );
		
		master->addSlave( new MySlaveThread( buf ) );
	}

	master->create();

	master->join();

	printf( "All done, goodbye\n" );
}
