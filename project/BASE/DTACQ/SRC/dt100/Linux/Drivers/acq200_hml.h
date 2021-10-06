/*
 * acq200hml.h - hardware model layer
 */


#define HOT_FIFO_FULL_ENTRIES(fifcon) \
        (((fifcon)>>ACQ200_FIFCON_HOTP_SHIFT)&ACQ200_FIFCON_MASK)

#define COLD_FIFO_FULL_ENTRIES(fifcon) \
        (((fifcon)>>ACQ200_FIFCON_COLDP_SHIFT)&ACQ200_FIFCON_MASK)

#define HOT_FIFO_FREE_ENTRIES(fifcon) \
        (HOT_FIFO_ENTRIES-HOT_FIFO_FULL_BLOCKS(fifcon)-1)

#define COLD_FIFO_FREE_ENTRIES(fifcon) \
        (COLD_FIFO_ENTRIES-COLD_FIFO_FULL_BLOCKS(fifcon)-1)


