/*
 * 2hex.c - convert numbers on stdin, one per line to 32 bit binary words and output
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
    char aline[80];
    unsigned bval;
    
    FILE* fin = fdopen( 0, "r" );
    
    while( fgets( aline, 80, fin ) ){
        if ( aline[0] == '#' ){
	        continue;
		}else{ 
		    bval = strtoul( aline, 0, 16 );
		    write( 1, &bval, sizeof(unsigned) );
		}
    }
    
    return 0;
}
