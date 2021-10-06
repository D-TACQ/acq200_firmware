/* ------------------------------------------------------------------------- */
/* mthread.h interface to pthread wrappers                                   */
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


#ifndef _MTHREAD_H_
#define _MTHREAD_H_

struct ThreadState;  



class Thread {
	static int complete;
	struct ThreadState* state;
protected:

	void setComplete() { complete = 1; }

	static void* start( void* arg );

	const char* getName();

	const ThreadState* getState() { return state; }
public:
	virtual void exec();
	Thread( const char* _name );


	virtual int create();

	void join();

	int isComplete() { return complete != 0; }


};


class Condition;
class SlaveCondition;
class MasterCondition;

class SlaveThread: public Thread {
	SlaveCondition* condition;

protected:
	virtual void exec_payload() {}

	void setCondition( SlaveCondition* condition );
	friend class MasterThread;  // calls setCondition
public:
	virtual void exec();

	SlaveThread( const char* _name );
};

#define NTHREADS 12

class MasterThread: public Thread {

   
	MasterCondition* condition;
	SlaveThread* slaves[NTHREADS];

protected:
	int nslaves;

	void createSlaves();
	void broadcast();
public:
	virtual void exec();
	MasterThread( const char* _name );
	virtual void addSlave( SlaveThread* slave );

	void cleanup();
	virtual int create();
};


#endif // _MTHREAD_H_
