/*
 * is-ramp.c - incoming data must conform to a 0..7fff ramp
 * or it's an error
 */

#include <stdio.h>

short increment( short test )
{
	int itest = test;
	test = ++itest&0x7fff;
	return test;
}

int main()
{
	int isample;
	short last;
	short data;	
	int nerrors = 0;
	
	if ( fread( &data, 2, 1, stdin ) == 1 ){
		last = data;
	}else{
		return 1;
	}
	for ( isample = 1; fread( &data, 2, 1, stdin ) == 1; ++isample ){
		if ( data != increment( last ) ){
			printf( "ERROR at sample %d value 0x%04x expected 0x%04x\n",
					isample, data, increment( last ) );
			++nerrors;
		}
		last = data;		
	}
	
	return nerrors != 0;
}
