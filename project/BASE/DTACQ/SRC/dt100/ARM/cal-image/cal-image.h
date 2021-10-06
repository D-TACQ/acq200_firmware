/*
 * cal-image.h
 */

struct CAL_IMAGE {
    char board[32];
    char date[32];
    unsigned data[32];
};


#define G_CAL_IMAGE	((struct CAL_IMAGE*)0x410f00c0)
