/*
 * cal-image.h
 */

struct CAL_IMAGE {
    char board[32];
    char date[32];
    union {
	unsigned data[32];    
	char text[4096-(32+32)];
    } _d;    
};


#define G_CAL_IMAGE	((struct CAL_IMAGE*)0x410f00c0)
