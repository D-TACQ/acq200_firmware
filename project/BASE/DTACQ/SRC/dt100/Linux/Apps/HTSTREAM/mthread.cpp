/* ------------------------------------------------------------------------- */
/* mthread.cpp : pthread wrapper library implementation                      */
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


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>


#include "mthread.h"

struct ThreadState {
	char name[80];
	pthread_t* thread;
	pthread_attr_t attr;
};

void* Thread::start( void* arg ) {
	Thread* theThread = (Thread*)arg;

	theThread->exec();
	return 0;
}

int Thread::create() {
	state->thread = new pthread_t;

	int rc = pthread_create( state->thread, &state->attr, start, this );

	if ( rc != 0 ){
		perror( "pthread_create() failed" );
		_exit( 1 );
	}
	return rc;
}

const char* Thread::getName() {
	return getState()->name;
}

void Thread::join() {
	pthread_join( *getState()->thread, NULL );
}

void Thread::exec() {
	printf( "Thread:%s\n", getName() );
	usleep( 10000 );
	printf( "Thread:%s done\n", getName() );
}

Thread::Thread( const char* _name ) {
	state = new ThreadState;
	strncpy( state->name, _name, 80 );
	pthread_attr_init( &state->attr );

	if ( geteuid == 0 ){
		pthread_attr_setschedpolicy( &state->attr, SCHED_FIFO );
	}
}

int Thread::complete;


class Condition {
protected:
	pthread_mutex_t* mutex;
	pthread_cond_t* cond;
public:
	Condition();
};

class SlaveCondition : public Condition {
public:
	int wait();
};

class MasterCondition : public SlaveCondition {
public:
	int broadcast();
};

Condition::Condition() {
	mutex = new pthread_mutex_t;
	cond = new pthread_cond_t;
	pthread_mutex_init( mutex, NULL );
	pthread_cond_init( cond, NULL );
}

int SlaveCondition::wait() {
	return pthread_cond_wait( cond, mutex );
}


int MasterCondition::broadcast() {
	return pthread_cond_broadcast( cond );
}






void SlaveThread:: exec() {
	while ( !complete ) {
		condition->wait();
		if ( !complete ){
			exec_payload();
		}else{
			printf( "%s complete\n", getName() );
		}
	}
}

void SlaveThread::setCondition( SlaveCondition* _condition ) {
	condition = _condition;
}
SlaveThread::SlaveThread( const char* _name ) :
		Thread( _name )
{
}




void MasterThread::createSlaves() {
	for ( int it = 0; it != nslaves; ++it ){
		slaves[it]->create();
	}
}

void MasterThread::broadcast() {
	condition->broadcast();
}
void MasterThread::exec() {
	for ( int iloop = 1; iloop <= 10; ++iloop ) {
		Thread::exec();
		broadcast();
		usleep( 100000 );
	}
	sleep( 1 );
	//cleanup(); // DOESN'T WORK :-(( PGM
}
MasterThread::MasterThread( const char* _name ) :
	Thread( _name ) {
	nslaves = 0;
	condition = new MasterCondition;
}

void MasterThread::addSlave( SlaveThread* slave ) {
	assert( nslaves < NTHREADS );

	slaves[nslaves++] = slave;
	slave->setCondition( condition );
}


void MasterThread::cleanup() {
	setComplete();		
	condition->broadcast();

	for ( int it = 0; it != nslaves; ++it ){
		printf( "join %d\n", it );
		pthread_join( *slaves[it]->getState()->thread, NULL );
	}
}
int MasterThread::create() {
	createSlaves();
	return Thread::create();
}


