/*****************************************************************************
 *
 * File: getBuildDate.c
 *
 * $RCSfile: getBuildDate.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: getBuildDate.c,v 1.227.2.102 2011/11/11 19:13:16 pgm Exp $
 * $Log: getBuildDate.c,v $
 * Revision 1.227.2.102  2011/11/11 19:13:16  pgm
 * B2229 remvoe bogus sleep from acq200_setArm and runs FAST
 *
 * Revision 1.227.2.101  2011/11/11 15:25:30  pgm
 * attempt to speed ST_CAPDONE with raised priority for MGR process
 *
 * Revision 1.227.2.100  2010/04/15 19:13:28  pgm
 * channelMask for ACQ164 is same as ACQ196
 *
 * Revision 1.227.2.99  2010/02/14 13:34:40  pgm
 * set acq132 decimate 1 on acqcmd setExternalClock NOICS
 *
 * Revision 1.227.2.98  2010/02/12 14:57:46  pgm
 * B2224 fixes bug in clksel. ACQ132 selectable clocks
 *
 * Revision 1.227.2.97  2010/02/12 13:55:04  pgm
 * new clksel for ACQ132
 *
 * Revision 1.227.2.96  2009/09/08 06:32:43  pgm
 * B2223 really handle defaults
 *
 * Revision 1.227.2.95  2009/09/08 05:57:28  pgm
 * B2222 provides ExternalClock defaults for ob_clock cards
 *
 * Revision 1.227.2.94  2009/09/07 22:06:30  pgm
 * solves the ocs527 external clock conundrum
 *
 * Revision 1.227.2.93  2009/09/06 19:01:15  pgm
 * acq200_isAcq164, fix channelMask
 *
 * Revision 1.227.2.92  2009/09/02 20:31:46  pgm
 * B2217 supports ACQ164
 *
 * Revision 1.227.2.91  2009/06/10 20:10:20  pgm
 * reduces setArm/setAbort race, still needs set.Arm to be perfect
 *
 * Revision 1.227.2.90  2009/06/05 20:07:25  pgm
 * B2215
 *
 * Revision 1.227.2.89  2009/03/30 07:35:19  pgm
 * B2212 - locks out commands in !=ST_STOP
 *
 * Revision 1.227.2.88  2008/07/02 11:31:39  pgm
 * drop a lot of acq32 crud
 *
 * Revision 1.227.2.87  2008/05/09 09:45:12  pgm
 * cap_status monitor
 *
 * Revision 1.227.2.86  2008/05/07 09:56:51  pgm
 * B2208 fixes mas_clk output
 *
 * Revision 1.227.2.85  2008/03/25 15:52:00  pgm
 * acq132
 *
 * Revision 1.227.2.84  2007/12/08 18:14:35  pgm
 * tighten up error exit codes
 *
 * Revision 1.227.2.83  2007/11/14 12:26:49  pgm
 * remove setLength - setMode does it now.
 *
 * Revision 1.227.2.82  2007/05/10 21:04:25  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.81  2007/05/09 15:43:54  pgm
 * read_with_timeout
 *
 * Revision 1.227.2.80  2007/05/07 14:29:26  pgm
 * DMA_XXP, DMA_TBLOCK
 *
 * Revision 1.227.2.79  2007/04/18 19:44:20  pgm
 * handles AB buffers part 1
 *
 * Revision 1.227.2.78  2007/04/15 21:02:32  pgm
 * processFileDMA works again
 *
 * Revision 1.227.2.77  2007/04/13 17:13:20  pgm
 * B2184 hopefully fix hostwin init - both for usual [init] case and for reruns
 *
 * Revision 1.227.2.76  2007/04/10 17:28:36  pgm
 * B2180 allows restart on PCI
 *
 * Revision 1.227.2.75  2007/04/03 21:58:34  pgm
 * handles mbox interrupts
 *
 * Revision 1.227.2.74  2007/04/02 18:00:41  pgm
 * B2174 window leng
 *
 * Revision 1.227.2.73  2007/01/05 12:56:48  pgm
 * restore setDIO to old mbox, also abort
 *
 * Revision 1.227.2.72  2007/01/04 09:04:49  pgm
 * minimise printf output
 
 * Revision 1.227.2.71  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.227.2.70  2007/01/03 12:37:42  pgm
 * acq200_muThrottle suppress ints during capture/postprocess to eliminate priority inversion
 *
 * Revision 1.227.2.69  2007/01/03 12:31:37  pgm
 * MU interrupts rock! - .7s for 100 commands.
 *
 * Revision 1.227.2.68  2007/01/02 18:18:10  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.67  2007/01/02 17:46:16  pgm
 * usc call acqcmd works @ 25 cps
 *
 * Revision 1.227.2.66  2007/01/01 20:55:05  pgm
 * text acqcmd works - use minor 137
 *
 * Revision 1.227.2.65  2007/01/01 18:15:23  pgm
 * ST_STOP filter works
 *
 * Revision 1.227.2.64  2007/01/01 14:40:37  pgm
 * mbox control works
 *
 * Revision 1.227.2.63  2007/01/01 14:39:35  pgm
 * mbox control works
 *
 * Revision 1.227.2.62  2006/09/13 15:23:57  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.61  2006/08/19 14:45:32  pgm
 * B2126
 *
 * Revision 1.227.2.60  2006/08/19 12:53:11  pgm
 * dual t>hread handles mbox calling interp
 *
 * Revision 1.227.2.59  2006/06/25 11:58:40  pgm
 * works with new style dynamic hostbufs
 *
 * Revision 1.227.2.58  2006/06/16 11:46:35  pgm
 * int + rounded value output from getInternalClock
 *
 * Revision 1.227.2.57  2006/04/01 14:40:04  pgm
 * ensures minimum transient time 1s.
 * rarely seems to be needed :-).
 *
 * Revision 1.227.2.56  2006/01/28 18:18:55  pgm
 * ST_CAPDONE
 *
 * Revision 1.227.2.55  2005/10/20 20:49:50  pgm
 * avoid ST_STOP race
 *
 * Revision 1.227.2.54  2005/09/17 18:31:06  pgm
 * computes elapsed samples correctly
 *
 * Revision 1.227.2.53  2005/08/09 19:45:50  pgm
 * wildcard A4 at llc start
 *
 * Revision 1.227.2.52  2005/07/26 12:19:15  pgm
 * B2111
 *
 * Revision 1.227.2.51  2005/05/16 21:00:00  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.50  2005/05/16 08:49:58  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.49  2005/03/05 16:11:11  pgm
 * new get.vin
 *
 * Revision 1.227.2.48  2005/02/06 17:45:42  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.47  2005/02/06 17:07:09  pgm
 * accepts N (N=4) localcommand streams
 *
 * Revision 1.227.2.46  2005/02/06 16:15:11  pgm
 * accepts 2 localslave clients
 *
 * Revision 1.227.2.45  2004/12/17 09:26:20  pgm
 * pci getNumSamples uses A3=post, NOT A3=total
 *
 * Revision 1.227.2.44  2004/12/16 17:06:09  pgm
 * fix zero prelen kludges
 *
 * Revision 1.227.2.43  2004/11/01 12:22:27  pgm
 * 2100
 *
 * Revision 1.227.2.42  2004/10/26 11:23:54  pgm
 * aggregating DMA Direct 50M bytes/sec
 *
 * Revision 1.227.2.41  2004/10/25 09:19:18  pgm
 * dma experiments - no work, no crash
 *
 * Revision 1.227.2.40  2004/10/23 14:17:45  pgm
 * rsh single path style
 *
 * Revision 1.227.2.39  2004/10/21 09:29:46  pgm
 * various fixes to sync post process
 *
 * Revision 1.227.2.38  2004/10/21 09:29:16  pgm
 * various fixes to sync post process
 *
 * Revision 1.227.2.37  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.227.2.36  2004/09/04 22:48:40  pgm
 * B2073 - event0
 *
 * Revision 1.227.2.35  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.227.2.34  2004/08/14 14:04:16  pgm
 * really get the forking subprocess right
 *
 * Revision 1.227.2.33  2004/08/14 13:34:45  pgm
 * B2068
 *
 * Revision 1.227.2.32  2004/07/25 15:40:26  pgm
 * ll hooks work
 *
 * Revision 1.227.2.31  2004/07/21 08:30:10  pgm
 * 2G Low Latency driver hooks
 *
 * Revision 1.227.2.30  2004/06/12 16:56:11  pgm
 * fixes setChannelMask 196 bug
 *
 * Revision 1.227.2.29  2004/06/05 21:41:15  pgm
 * works with pre=0, trigger=event frig
 *
 * Revision 1.227.2.28  2004/05/25 14:06:12  pgm
 * plumb TriggeredContinuous
 *
 * Revision 1.227.2.27  2004/05/18 20:12:07  pgm
 * B2060 setChannelMask ok ACQ196
 *
 * Revision 1.227.2.26  2004/05/16 07:20:35  pgm
 * B1059
 *
 * Revision 1.227.2.25  2004/04/25 12:57:47  pgm
 * concurrent queries work
 *
 * Revision 1.227.2.24  2004/04/25 11:40:53  pgm
 * concurrency 1 with logging
 *
 * Revision 1.227.2.23  2004/04/13 09:35:46  pgm
 * no works with acq196
 *
 * Revision 1.227.2.22  2004/04/03 19:19:52  pgm
 * TRADITIONAL getStatus, getMode
 *
 * Revision 1.227.2.21  2004/03/26 11:11:48  pgm
 * acq196 mu
 *
 * Revision 1.227.2.20  2004/02/17 10:14:40  pgm
 * setSampleTagging stub required
 *
 * Revision 1.227.2.19  2004/02/15 22:04:57  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.18  2004/02/15 13:23:47  pgm
 * localSlave socket in place
 *
 * Revision 1.227.2.17  2004/02/14 23:13:22  pgm
 * OO sockets are us (monitoring stdin bad idea in batch mode ie when its /dev/null
 *
 * Revision 1.227.2.16  2004/01/17 10:12:11  pgm
 * dbg skt vbuf good, zombie mode operates
 *
 * Revision 1.227.2.15  2003/12/17 13:31:36  pgm
 * B2029
 *
 * Revision 1.227.2.14  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.13  2003/11/08 14:48:16  pgm
 * channel mask, pre post sorted
 *
 * Revision 1.227.2.12  2003/11/07 09:06:11  pgm
 * pre A4, post A3
 *
 * Revision 1.227.2.11  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.227.2.10  2003/11/03 11:30:29  pgm
 * chan def is 0... on wire, 1.. logical
 *
 * Revision 1.227.2.9  2003/10/30 14:12:36  pgm
 * B2017
 *
 * Revision 1.227.2.8  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.227.2.7  2003/10/26 21:19:16  pgm
 * DUMDMA works 1MS
 *
 * Revision 1.227.2.6  2003/09/13 13:52:39  pgm
 * DMA IN, PIO OUT works well enough
 *
 * Revision 1.227.2.5  2003/09/11 23:10:38  pgm
 * 2010
 *
 * Revision 1.227.2.4  2003/09/10 20:33:18  pgm
 * split File IO prep for DMA, PIO still works
 *
 * Revision 1.227.2.3  2003/09/08 21:09:09  pgm
 * *** empty log message ***
 *
 * Revision 1.227.2.2  2003/09/07 16:30:39  pgm
 * basic i2o
 *
 * Revision 1.227.2.1  2003/09/05 22:59:14  pgm
 * WAV232 first build
 *
 * Revision 1.227  2003/07/03 11:30:43  pgm
 * *** empty log message ***
 *
 * Revision 1.226  2003/07/03 10:08:40  pgm
 * internal loopback becomes hook on DO3
 *
 * Revision 1.225  2003/06/03 10:10:02  pgm
 * B1725 pci high address swithc for HTM loc status
 *
 * Revision 1.224  2003/06/02 21:18:28  pgm
 * B1724 htstream V2
 *
 * Revision 1.223  2003/05/31 11:39:01  pgm
 * B1720 fix extra samples acq32 post
 *
 * Revision 1.222  2003/03/05 08:26:39  pgm
 * B1719 reliable acq16 offset adjust
 *
 * Revision 1.221  2003/02/07 21:34:05  pgm
 * B1714 zeros pre buffer after filter
 *
 * Revision 1.220  2003/02/07 09:50:51  pgm
 * B1713 BoxCar
 *
 * Revision 1.219  2002/12/10 14:13:57  pgm
 * B1707 fixes DAC pot tweak
 *
 * Revision 1.218  2002/11/23 20:37:01  pgm
 * nodos
 *
 * Revision 1.217  2002/11/23 20:35:44  pgm
 * B1704 - fixes ext trig pol
 *
 * Revision 1.216  2002/11/03 19:20:27  pgm
 * harden adj
 *
 * Revision 1.215  2002/11/03 16:10:24  pgm
 * acq16 offset adj uses cmask, allows status check
 *
 * Revision 1.214  2002/11/02 21:39:10  pgm
 * B1700 ACQ16 cal adj works (I think)
 *
 * Revision 1.213  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.211  2002/10/31 10:42:38  pgm
 * fixes getEvent, attempts fix null P1
 *
 * Revision 1.210  2002/08/31 20:05:43  pgm
 * fix pair 0 starts fixup
 *
 * Revision 1.209  2002/08/23 18:27:30  pgm
 * B1691 - runners, err status
 *
 * Revision 1.208  2002/08/21 19:49:20  pgm
 * B1689 - auto trig, sample set adjust OK??
 *
 * Revision 1.207  2002/08/21 12:06:12  pgm
 * this is as fast as it gets
 *
 * Revision 1.206  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.205  2002/08/20 14:48:53  pgm
 * fix buffer blat with deadband
 *
 * Revision 1.204  2002/08/20 13:19:41  pgm
 * back out some failed acq16 speed ups
 *
 * Revision 1.203  2002/08/12 19:35:17  pgm
 * subrate streaming at 150kHz
 *
 * Revision 1.202  2002/08/06 19:41:38  pgm
 * lower ICACHE load, add mbox boost
 *
 * Revision 1.201  2002/08/05 19:49:34  pgm
 * status change int only on request
 *
 * Revision 1.200  2002/07/29 11:21:42  pgm
 * B1689 minimal acq16 AO reserve
 *
 * Revision 1.199  2002/07/29 11:03:49  pgm
 * B1688 remove acq16 AO reserve
 *
 * Revision 1.198  2002/07/25 17:31:48  pgm
 * B1687 - pipeline back to 2
 *
 * Revision 1.197  2002/07/25 15:13:12  pgm
 * acq16 trig comp now automatic
 *
 * Revision 1.196  2002/07/24 18:07:41  pgm
 * acq16 trigger offset comp - use 1st TW
 *
 * Revision 1.195  2002/07/22 20:01:34  pgm
 * fixes gap in pre gpem P1
 *
 * Revision 1.194  2002/07/18 18:43:05  pgm
 * 44M stub, 64b copy test
 *
 * Revision 1.193  2002/06/13 19:38:22  pgm
 * fix int clk rate bug, then stub 44M as Xilinx broken
 *
 * Revision 1.192  2002/06/11 20:06:46  pgm
 * handles subchannel streaming
 *
 * Revision 1.191  2002/06/10 20:02:40  pgm
 * B1666 multi samples per pci packet
 *
 * Revision 1.190  2002/06/10 13:38:07  pgm
 * streaming with correct data works Ship It
 *
 * Revision 1.189  2002/06/10 11:53:50  pgm
 * streaming runs, but the data is crap
 *
 * Revision 1.188  2002/06/09 15:57:57  pgm
 * from handa
 *
 * Revision 1.187  2002/04/26 10:51:59  pgm
 * B1638 finally handles flash num channels correctly
 *
 * Revision 1.186  2002/04/25 10:09:30  pgm
 * B1634 fixes master trig sync
 *
 * Revision 1.185  2002/04/24 12:51:28  pgm
 * B1633 TRMAS for acq16
 *
 * Revision 1.184  2002/04/24 12:03:03  pgm
 * B1632 - qualifies queries at run, rounds clock down, selects #channels on Cal Title
 *
 * Revision 1.183  2002/04/11 20:51:19  pgm
 * now with latched overflow check, recycle
 *
 * Revision 1.182  2002/04/10 20:21:47  pgm
 * htstream runs
 *
 * Revision 1.181  2002/03/31 09:12:19  pgm
 * *** empty log message ***
 *
 * Revision 1.180  2002/03/30 14:38:38  pgm
 * lowest poss latency ext trig
 *
 * Revision 1.179  2002/03/29 00:09:21  pgm
 * B1620 reports cycle, state
 *
 * Revision 1.178  2002/03/28 15:29:42  pgm
 * cycle reset, debounce opts
 *
 * Revision 1.177  2002/03/25 08:56:05  pgm
 * cycle count
 *
 * Revision 1.176  2002/03/24 11:54:54  pgm
 * B1617 - works (sometimes)
 *
 * Revision 1.175  2002/03/22 20:58:41  pgm
 * B1612 resets MULTI_TRIG_BIT correctly
 *
 * Revision 1.174  2002/03/21 15:07:11  pgm
 * merge from 1608 - multi trig safe, edge det etc
 *
 * Revision 1.173  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.172  2002/03/14 12:30:44  pgm
 * B1610
 *
 * Revision 1.171  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.170  2002/03/10 12:17:19  pgm
 * B1601 - tweak postlen to avoid buffer overrun
 *
 * Revision 1.169  2002/03/09 14:04:25  pgm
 * B1600 fixes bug in rouond up
 *
 * Revision 1.168  2002/03/09 13:50:25  pgm
 * force prelen to be a multiple of block length
 *
 * Revision 1.167  2002/03/09 12:20:40  pgm
 * fix FS44 sink, ensure clean hw ch change
 *
 * Revision 1.166  2002/03/09 11:07:03  pgm
 * make acq16 hw channel select more complete
 *
 * Revision 1.165  2002/03/08 19:32:15  pgm
 * B1591 - default no mask - its fast
 *
 * Revision 1.164  2002/03/07 21:22:09  pgm
 * B1587 - LEAN_AND_MEAN trigger callback
 *
 * Revision 1.163  2002/03/07 08:29:31  pgm
 * FET runs to end of nsamples on atrig
 *
 * Revision 1.162  2002/03/06 09:48:57  pgm
 * SMALL_DIG_TRIG now default
 *
 * Revision 1.161  2002/02/28 18:03:03  pgm
 * switches RED_LED OFF aftger capture
 *
 * Revision 1.160  2002/02/20 21:41:42  pgm
 * fix overzealous ASSERT in storeman.c
 *
 * Revision 1.159  2002/02/19 19:32:10  pgm
 * clean up llcontrol and make it go forever
 *
 * Revision 1.158  2002/02/16 17:53:34  pgm
 * for release, FET Multitrig nearly works
 *
 * Revision 1.157  2002/02/16 07:40:49  pgm
 * attempting to make edge trig faster
 *
 * Revision 1.156  2002/02/14 19:28:15  pgm
 * acq16 multitrig
 *
 * Revision 1.155  2002/02/11 22:06:08  pgm
 * DO is GO
 *
 * Revision 1.154  2002/02/10 17:56:51  pgm
 * usecs timing reliable over time
 *
 * Revision 1.153  2002/02/09 21:32:12  pgm
 * work on acq16 2/4/8/16 (boots)
 *
 * Revision 1.152  2002/02/09 10:16:49  pgm
 * more moving static inits till after lca load
 *
 * Revision 1.151  2002/02/09 09:31:05  pgm
 * war on warnings
 *
 * Revision 1.150  2002/02/08 22:41:24  pgm
 * multi num channel configs for acq16
 *
 * Revision 1.149  2002/02/07 16:56:36  pgm
 * update to new lca
 *
 * Revision 1.148  2002/02/02 15:13:41  pgm
 * usec timing changes from EPFL visit 01/2002
 *
 * Revision 1.147  2002/01/25 21:55:09  pgm
 * edge trigger - this one worked, both edges even channels
 *
 * Revision 1.146  2002/01/20 20:54:00  pgm
 * threshtrig in debug
 *
 * Revision 1.145  2002/01/19 18:26:09  pgm
 * fast th almost
 *
 * Revision 1.144  2002/01/18 22:47:13  pgm
 * *** empty log message ***
 *
 * Revision 1.143  2002/01/18 18:33:10  pgm
 * *** empty log message ***
 *
 * Revision 1.142  2002/01/18 10:05:41  pgm
 * onwards and upwards
 *
 * Revision 1.141  2002/01/16 10:00:59  pgm
 * docs
 *
 * Revision 1.140  2002/01/13 22:21:58  pgm
 * acq32 fast threshtrig kludge coded
 *
 * Revision 1.139  2002/01/08 22:01:51  pgm
 * ease ASSERT
 *
 * Revision 1.138  2002/01/07 14:14:35  pgm
 * Build 1452 - works with P1==0, disID works with GPEM
 *
 * Revision 1.137  2002/01/05 21:48:26  pgm
 * fixes DUMDMA stride probs
 *
 * Revision 1.135  2001/12/08 14:37:56  pgm
 * acq16 runs at 2.52MSPS - 2*6=4*3 rools OK
 *
 * Revision 1.134  2001/12/04 21:28:53  pgm
 * full rate threshtrig in switchable sink
 *
 * Revision 1.133  2001/12/02 15:44:28  pgm
 * lower AO threshodl 0x8 means NO overruns at lo speed
 *
 * Revision 1.132  2001/11/23 22:47:19  pgm
 * dumdma speed demo with correct data
 *
 * Revision 1.131  2001/11/23 22:28:07  pgm
 * fixes AO ST, direct dumdma speed demo
 *
 * Revision 1.130  2001/11/20 22:23:53  pgm
 * better dregs, i2o in capture, fix AO soft trig
 *
 * Revision 1.129  2001/11/19 14:28:58  pgm
 * Build 1420 - report tlatch to user before calc tproc
 *
 * Revision 1.128  2001/11/19 13:35:47  pgm
 * Build 1419, better regs use
 *
 * Revision 1.127  2001/11/19 12:46:15  pgm
 * built 1418 - LLC uses 16 bit counts
 *
 * Revision 1.126  2001/11/18 10:06:00  pgm
 * leaner llcontrol - has llcount probs
 *
 * Revision 1.125  2001/11/12 12:35:03  pgm
 * Build 1412, llcontrol tries for best latency
 *
 * Revision 1.124  2001/11/10 17:27:27  pgm
 * reworked llcontrol, still fails decimation
 *
 * Revision 1.123  2001/11/07 22:14:54  pgm
 * move div clock to DO3 for LLC
 *
 * Revision 1.122  2001/11/05 22:28:40  pgm
 * TEST messages, fix gut buffer overrun bug
 *
 * Revision 1.121  2001/11/04 23:35:10  pgm
 * host side fw debugs
 *
 * Revision 1.120  2001/11/04 20:56:50  pgm
 * Build 1405
 *
 * Revision 1.119  2001/10/28 21:49:32  pgm
 * llcontrol 24 bit rolloever fixed
 *
 * Revision 1.118  2001/10/22 20:00:17  pgm
 * Build 1400 edge trig works rel to Scalar
 *
 * Revision 1.117  2001/10/20 19:42:59  pgm
 * intclock master/slave WORKS
 *
 * Revision 1.116  2001/10/07 10:19:59  pgm
 * GPME looking good build 1370 checkin
 *
 * Revision 1.115  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.114  2001/10/01 21:19:58  pgm
 * fix incoming error message
 *
 * Revision 1.113  2001/10/01 21:13:43  pgm
 * hard trigger works for AO
 *
 * Revision 1.112  2001/09/30 18:38:01  pgm
 * i2o two commands still
 *
 * Revision 1.111  2001/09/30 14:08:20  pgm
 * i2o boot time - two commands
 *
 * Revision 1.110  2001/09/24 20:25:21  pgm
 * soft OK, hard not yet
 *
 * Revision 1.109  2001/09/24 12:54:31  pgm
 * GPEM AO works
 *
 * Revision 1.108  2001/09/08 13:54:36  pgm
 * Build 1333 - fixes E1 overrun, opt i2o incoming
 *
 * Revision 1.107  2001/09/07 11:31:59  pgm
 * Build 1332 release candidate
 *
 * Revision 1.106  2001/09/03 19:22:52  pgm
 * thresh trig multi ouptut 0 on trig
 *
 * Revision 1.105  2001/09/03 15:52:45  pgm
 * threshold triggering should now work
 *
 * Revision 1.104  2001/09/01 17:01:23  pgm
 * Build 1321, release candidate
 *
 * Revision 1.103  2001/09/01 16:57:51  pgm
 * rationalise calls to distribute, saves <if>
 *
 * Revision 1.102  2001/08/31 13:29:38  pgm
 * E1 handling OK, first pass
 *
 * Revision 1.101  2001/08/31 09:10:21  pgm
 * acq16 has channel mask, works on rearm
 *
 * Revision 1.100  2001/08/27 12:55:18  pgm
 * acq16 runs again - needs LCA bit change, caveat embedded trigger
 *
 * Revision 1.99  2001/08/24 07:56:11  pgm
 * new protocol llc seems reliable
 *
 * Revision 1.98  2001/08/19 13:25:41  pgm
 * fireEvent works
 *
 * Revision 1.97  2001/08/18 20:00:52  pgm
 * updates route correctly, we get data
 *
 * Revision 1.96  2001/08/17 20:46:55  pgm
 * GUT in debug
 *
 * Revision 1.95  2001/08/17 15:20:11  pgm
 * fail gracefully with duff LCA
 *
 * Revision 1.94  2001/08/13 16:43:23  pgm
 * ll counter extend works
 *
 * Revision 1.93  2001/08/13 10:47:41  pgm
 * LL timer update now works correctly, unprompted
 *
 * Revision 1.92  2001/08/10 20:47:58  pgm
 * fix lll counter overflow, add USER LEDS
 *
 * Revision 1.91  2001/08/03 19:35:59  pgm
 * remove second event to eliminate bad sample problem
 *
 * Revision 1.90  2001/07/14 11:10:30  pgm
 * use fillIcache techs for acq16 too
 *
 * Revision 1.89  2001/07/13 20:48:47  pgm
 * Icache preload reduces init max bin to 18 - ship it quick
 *
 * Revision 1.88  2001/07/13 16:56:26  pgm
 * AO+DO works at last
 *
 * Revision 1.87  2001/07/12 21:29:56  pgm
 * DO, AO now identical - gotta work please
 *
 * Revision 1.86  2001/07/12 10:16:49  pgm
 * take more care over lca init order
 *
 * Revision 1.85  2001/07/12 07:45:54  pgm
 * finally fixes the AO load problems
 *
 * Revision 1.84  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.83  2001/07/11 16:07:11  pgm
 * fix diags
 *
 * Revision 1.82  2001/07/11 15:07:26  pgm
 * remove stray printf
 *
 * Revision 1.81  2001/07/11 14:52:35  pgm
 * build 1265 in cvs, ++TLB flush done
 *
 * Revision 1.80  2001/06/28 21:11:28  pgm
 * fix phys channe mixup on acq16 flavour fetch, also trheshtrig experiments
 *
 * Revision 1.79  2001/06/27 22:49:42  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.78  2001/06/23 20:21:07  pgm
 * data fills to end
 *
 * Revision 1.77  2001/06/23 19:03:37  pgm
 * acq16 part 1 captures runs
 *
 * Revision 1.76  2001/06/23 09:22:50  pgm
 * *** empty log message ***
 *
 * Revision 1.75  2001/06/23 08:32:16  pgm
 * remove extra fixups, printf from acq32_run()
 *
 * Revision 1.74  2001/06/11 19:19:52  pgm
 * better
 *
 * Revision 1.73  2001/06/11 14:47:25  pgm
 * GUT ran thru
 *
 * Revision 1.72  2001/06/10 20:25:48  pgm
 * event handling in
 *
 * Revision 1.71  2001/06/09 11:57:16  pgm
 * *** empty log message ***
 *
 * Revision 1.70  2001/06/02 08:43:55  pgm
 * reserve AO, GUT STUBS
 *
 * Revision 1.69  2001/05/29 19:51:25  pgm
 * fixed cal, DACpot
 *
 * Revision 1.68  2001/05/25 12:04:47  pgm
 * command race eliminated -ll runs
 *
 * Revision 1.67  2001/05/23 20:42:27  pgm
 * *** empty log message ***
 *
 * Revision 1.66  2001/05/21 20:07:53  pgm
 * llc - WIP
 *
 * Revision 1.65  2001/05/20 21:22:50  pgm
 * ll WIP - clocks, no samples
 *
 * Revision 1.64  2001/05/19 19:44:40  pgm
 * LL work in progress
 *
 * Revision 1.63  2001/05/15 17:51:01  pgm
 * new fifo mapping scheme uses wider VM, avoids AI/AO overlap
 *
 * Revision 1.62  2001/05/15 08:50:33  pgm
 * even task handling, slicker status, err detect
 *
 * Revision 1.61  2001/05/15 06:50:29  pgm
 * 32 sample load avoids glitch
 *
 * Revision 1.60  2001/05/14 17:49:50  pgm
 * AO works well - shippit quick
 *
 * Revision 1.59  2001/05/14 16:24:25  pgm
 * fifo_source, increments, AO fifo 3/4 250ksps is go
 *
 * Revision 1.58  2001/05/14 11:36:13  pgm
 * AO waveform - histos look OK
 *
 * Revision 1.57  2001/05/13 20:00:46  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.56  2001/05/07 19:49:11  pgm
 * loads waveforms, uses storeman OK
 *
 * Revision 1.55  2001/05/07 14:23:01  pgm
 * storeman in
 *
 * Revision 1.54  2001/05/07 11:38:37  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.53  2001/05/06 18:49:53  pgm
 * *** empty log message ***
 *
 * Revision 1.52  2001/05/05 06:33:10  pgm
 * magic number removal - this works
 *
 * Revision 1.51  2001/05/04 20:37:08  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.50  2001/05/04 09:32:41  pgm
 * consistent pollcat now
 *
 * Revision 1.49  2001/05/01 20:59:23  pgm
 * first pass em trig runs - runs away in fact
 *
 * Revision 1.48  2001/04/18 20:39:09  pgm
 * built in multi cal facility - needs busprot
 *
 * Revision 1.47  2001/04/15 09:52:26  pgm
 * SyncSetRoute works
 *
 * Revision 1.46  2001/04/09 09:58:49  pgm
 * deep grief, distributor bug fixed (testing..)
 *
 * Revision 1.45  2001/04/02 21:43:07  pgm
 * channelized upload will finally be right
 *
 * Revision 1.44  2001/04/02 09:10:07  pgm
 * fix wrapped buffer fetch bug
 *
 * Revision 1.43  2001/04/02 06:08:09  pgm
 * transient DUMDMA is good
 *
 * Revision 1.42  2001/04/01 12:31:17  pgm
 * get channels right, test dist get s frame str
 *
 * Revision 1.41  2001/04/01 08:25:55  pgm
 * DUMDMA works - synch on post fixes
 *
 * Revision 1.40  2001/03/31 20:45:13  pgm
 * DUMDMA works with debugs on ...
 *
 * Revision 1.39  2001/03/30 20:20:13  pgm
 * UDMA first cut working
 *
 * Revision 1.38  2001/03/25 19:20:28  pgm
 * first cut i2o command handling
 *
 * Revision 1.37  2001/03/24 20:47:17  pgm
 * rejig I2O, add incoming hooks (outgoing still works)
 *
 * Revision 1.36  2001/03/20 21:51:04  pgm
 * added headers
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#include "string.h"

// build this file every time

// 1004 milestone DMA works!
// 1005 milestone DMA works without overwriting streaming data
// 1011 milestone DMA becomes default for streaming with message length 450
// 1012 SOFT_CONTINUOUS with trigger hack, dma test opt
// 1013
// 1014 
// 1015 has big buffer DMA features built in.
// 1016 changes inner loop for more reliable abort
// 1017 reworked ... willit work??
// 1018 nfg
// 1019 rewind
// 1020 improved acq32_run()
// 1021 send message at least *5/sec
// 1022 striding MUST work!
// 1030 stride==1, stride>1 BOTH work
// 1031 looking at gated continuous
// 1032 gated continuous with reduced sample count, spin for pfmu, zero sample_in_frame
// 1033 first streaming sample is correct
// 1034 trigger holdoff in GATED_CONTINUOUS
// 1035 merged big dma tests
// 1036 add BIGDUM, FWREV mbox options
// 1037 ditto now work!!
// 1038 triggered continuous
// 1039 bigdump should deliver only valid samples, linear fashion
// 1040 release candidate for above
// 1041 allows internal clock with gated modes
// 1042 triggered continuous starts on ARM
// 1043 add histogram fifo state log
// 1044 acq32_run() - multi samples per loop, + streaming orta work
// 1045 fix bug in recording num samples at end of GATED_TRANSIENT
// 1046 fix bug in recording pre samples TRIGGERED continuous
// 1047 improved InternalClock (off by one), clamp pre-samples
// 1048 fix bug limiting #samples in CONTINUOUS modes
// 1049 use SDRAM for storage where possible
// 1050 fixes bug in case SOFT_CONTINUOUS 0 (compiler bug)
// 1051 fix bug in bigdump -overwrite last .5M
// 1054 Continuous with pre=0 doesn't stop
// 1055 Use SDRAM for pre samples whereever possible
// 1056 fix assertion in CAL
// 1057 fix InternalClock, diags, first cut opt pollcat==2.2
// 1058 optimise acq32Run, distributor #2 - pollcat 5
// 1059 includes dac_sim hack
// 1060 clean id - matches sample #
// 1061 pre buffer count bug corrected
// 1062  how fast can it go ??
// 1063 lltest kludge part1 included
// 1064 data dependent trigger kludge
// 1065 ... not such a kludge with full mbox control
// 1066 - this has to be a goodie - distributor returns to multi sample op
// 1067 - ARM2.51, using cvs correctly
// 1068 - experiment with i2o incoming
// 1069 - i2o incoming works .. trashes i2o out :-(
// 1070 - dynamic application mapped push DAMP
// 1071 - Dynamic User Mapped DMA - DUMDMA - handshake i2o message send - willit fix garbled i2o during dma?
// 1072 - add mega debugs to fetch - to slow down
// 1073 - sdram race bug worked around, now get the channels right
// 1074 - 2nd instance sdram race, fix multi sample distribute() bug
// 1075 - fix bug in single channel fetch - been with us all the time!
// 1076 - DUMDMA fetch gotta work with pre, post
// 1077 - fix loop bug present in some distributors
// 1078 - heavy debug
// 1100 - fix BUG in default distributor - gotta work
// 1200 - init mods for new LCA, implemented SyncRoute func
// 1201 - fifo_sink mod - NB data now non functional and wrong order +
// 1202 - embedded trigger passing
// 1203 - remove initial clock set
// 1204 - more internal clock tweaking
// 1205 - try debug cal, sampling (set sample trigger 0)
// 1210 - RTFM, let's try again
// 1211 - move LCA SDRAM setup from d-bug.c to target.s
// 1212 - embedded trigger monitoring in
// 1213 - set DAC's remote func included
// 1214 - improved TW decode
// 1215 - STOREMAN - allows DAC upload
// 1216 - STOREMAN++ flags
// 1217 - add ao hist report
// 1218 - AO waveform hacked into acq32_go - SOFT_TRANSIENT only
// 1219 - Waveform output - now activated, may limp along
// 1220 - this one seems to work, OK to 200ksps
// 1221 - fifo_source - this one should fly
// 1222 - reduce AO fifo load length - try to real in second bump - was bin11
// 1223 - works well, but drop AO th by one to avoid top bin entry in AO
// 1224 - new LCA with 1K AO/DO boundary
// 1225 - load 32 words at a time to avoid write buffer probs
// 1226 - short write is good - remove marker
// 1227 - try iron out bumps in acq32_run() task dispatch
// 1228 - AO threshold at <=0xb0
// 1229 - better AO stats
// 1230 - AI, AO addressing separate
// 1231 - Split AI, AO ranges over full 32MB
// 1232 - Low Latency included + DO waveform output
// 1233 - Low Latency testing
// 1234 - Low Latency testing II
// 1235 - fix ext clock in LL
// 1236 - remove DMA done race in acq32busprot signalling
// 1237 - remove race in command receiver
// 1238 - fix defcal, DACPOT FREEZE
// 1239 - tested
// 1240 - mbox loadCal, reserveAO, stubs for GUT
// 1241 - start testing GUT
// 1242 - GUT includes event handling
// 1243 - GUT runs through (simplest possible case)
// 1244 - GUT even better
// 1245 - first build on LEWIS
// 1246 - try to remove extra work from acq32Run, + stray printf()
// 1247 - add pre, postAmble() funcs to standardize the go()'s
// 1250 - acq16 hacked in and hooked up.
// 1251 - acq16 - looking good!
// 1252 - acq16 - try different fifo depth formula
// 1260 - buffer management is good - runs to end
// 1261 - getCaptureStats etc implemented
// 1262 - acq16 fifo doubles
// 1263 - acq16 better reporting - long fifos are GO
// 1264 - fix acq16 data retrieve bug
// 1265 - fix physchan fetch bug intorduced to acq32 by acq16
// 1266 - remove getFlavor printf
// 1267 - fix histograms (again) 
// 1270 - AI fifo length extended to 32
// 1271 - fix SM reserve function
// 1272 - fix AO load funcs
// 1273 - fix AO load funcs (the debugs caused a bug)
// 1274 - rework static init/ lca load order - should force AO enable correctly
// 1275 - remember to tee up DO waveforms as well
// 1276 - remove additional DO setup at ARM time
// 1277 - fix interactions between AO, DO soft triggers
// 1278 - remove DO load from mbox - only loads at ARM
// 1279 - and restore WAVEFORM mode
// 1280 - selective relad - but only when data is present
// 1281 - XOfifoEnable() obsoleted - try slow the soft trig for DO
// 1282 - avoid error on AO end of sequence
// 1300 - sets DIO outputs correctly again - works AO+DO - shippit quick
// 1301 - hacks to try speed init cache load
// 1302 - bin 18 - ship it quick! - propagate into acq12 code
// 1303 - catch bad last sample problem
// 1304 - fix u32 counter for llmode, add User LED functionality
// 1305 - oops ll change was defective, fix?
// 1306 - try to eliminate init target addr  race
// 1307 - fix random start clock count
// 1308 - try to make failover with bad lca more graceful\n"
// 1309 - GUT debug - clean up endgame, arm
// 1310 - this runs thru, fast! but fails to trigger - work on trig
// 1311 - yes it runs, but data is NFG - investigate
// 1312 - data is good, make fireEvent work
// 1313 - work on racefreee LLC
// 1314 - debug acq16
// 1315 - acq16 uses old AIFIFOFF, add trigger detect
// 1316 - acq16 uses channel mask also - runs OK
// 1317 - change AIFIFOFF to suit new style lca
// 1318 - pratting about with acq16
// 1319 - fix acq16 setMode clock and trig restart
// 1320 - debug E1 handling
// 1321 - clean build, release candidate
// 1322 - fix thresh trigger stuff
// 1330 - thresh trig works, release candidate 2
// 1331 - reduce acq16 mbox poll rate, add info to getConfig
// 1332 - release candidate
// 1333 - remove E1 debug, enable i2o incoming only on driver request
// 1334 - further protection against rogue i2o
// 1335 - add output funcs to gut
// 1336 - more debugs
// 1342 - AO func is a runner
// 1343 - try AO with hard triggers
// 1350 - bootstrap i2o diags
// 1351 - fix CSR space to include SDRAM space
// 1360 - GPEM AO with trigger
// 1361 - fix i2o incoming error decode
// 1362 - add pci dregs
// 1363 - better LEDS and DIO diags
// 1364 - donacqcmd getFwrev
// 1365 - fill cache first on GPEM arm
// 1366 - delay AI fifo enable - AI convert enable is on all the time
// 1367 - kludge SynCRoute DI1, DI2, make AO edge triggered
// 1370 - release candidate
// 1380 - edge trigger
// 1381 - clock mastering
// 1382 - and resample the same input
// 1383 - make sure the clock is actually an output
// 1384 - take care to ensure DIO set correctly
// 1385 - speedier fifosink
// 1386 - debug edge detector
// 1387 - fix bug in edge detect
// 1388 - speed thresh dets by elim mask test, reduce edge channel count to half
// 1389 - debugs back - why didn't 1388 work ??
// 1390 - unroll edge loop
// 1391 - set var for max trigger pair setting
// 1392 - try to speed sink processing, no channel store
// 1393 - as before, but with masking code back in
// 1394 - user adjustable wimpout
// 1395 - default wimpout set 40
// 1396 - remove edge trigger deadband
// 1400 - and set the wimpfactor back to 40
// 1401 - looking at llcontrol count values
// 1402 - try get usecs timing
// 1403 - fix 24 bits rollover in llcontrol
// 1404 - HTRIGEN explicit enable of AO, DO HW trigger
// 1405 - fix mem alloc bug for AI P2, fix setUserLed st 3 is indep of 4
// 1406 - add SIOBUF for pci debug output
// 1407 - use i2o to take incoming
// 1408 - debug above
// 1409 - should be fixed (sample bytes/u16/u32 muddle)
// 1410 - use GASFTCLOCK, output GA div clock on DO3
// 1411 - avoid fifo reset when flushing data in LLC
// 1412 - only transfer data when timer running
// 1413 - test distributor didn't work with GPEM - fix it
// 1414 - improved 24 bit rollover, optimised C for LLC
// 1415 - use ~&, not ^ !!
// 1416 - try 16 bit counters instead
// 1417 - use GA formula with 16 bits counters
// 1418 - use TIMER_RUNNING value to avoid detecting prev count as overflow
// 1419 - alternate timer updates, use reg var
// 1420 - report tlatch to host before measuring latency
// 1421 - suspect HWTRIGEN cancels AO, DO soft trigger - work around
// 1422 - poll debug i2o (debug terminal) during gut
// 1423 - ensure AO is appropriately enabled, use HWTRIGEN correctly
// 1424 - add primitive MTC_HOST_REQUEST_X_DATA handling
// 1425 - fix primitive REQUEST_X_DATA - data is garbage, but its fast!!
// 1426 - maybe the data will be better this time
// 1427 - lowered threshold fix for AO, regular and GPEM
// 1428 - fifo_12_sink is a virtual call, multi possibilities for edge det
// 1429 - fix race in llcontrol timer start
// 1430 - multi-sink experiment in place
// 1440 - use ID dist with gut, improved d-bug trad funcs
// 1441 - modify striding code
// 1451 - attempt to debug discard overrun
// 1452 - fix bug for P1 len==0 breaks P2 buffer
// 1454 - working on decimation bug in llcontrol
// 1455 - debugging DO waveform output
// 1456 - debug llcontrol - monitor the clkdiv
// 1457 - llcontrol with clkdiv debug, min latency
// 1458 - try new pci reset time in target.s
// 1460 - improved target.c, minimum delay to pci ready
// 1492 - clean edge trig release
// 1493 - post Columbus, add trigger timing
// 1500 - mod trig detect for new acq16
// 1502 - extend to 16ch
// 1503 - and extend to 2/4/8/12/16
// 1514 - this one boots!
// 1516 - this one works
// 1517 - LEAN_AND_MEAN, add boost opt to lut, wire null distributor
// 1518 - work on the usec counter
// 1519 - revised 12ch sink - this is the fastest possible!!
// 1520 - work on DO waveform - fix bug in gut
// 1522 - lowered threshold fix for DO same as AO ??
// 1527 - fix DIx selection for external clock
// 1528 - setModeTriggeredContinuous pre- post- [DIx]
// 1529 - and get it right!!
// 1530 - fillIcache uses route->sink() to avoid init overruns 
// 1531 - fix setTreiggeredContinuous DIx to use 1<<0 <= DIx  <= 1<<3
// 1532 - post outgoing mfa in mbox2 as well as i2o
// 1533 - prototype relocateDistributor()
// 1534 - take it AWAY!
// 1535 - route->teeup - distributor MUST be faster !!
// 1536 - didn't work, debug teeup
// 1539 - try relocate again, but only for sink, coded in asm
// 1551 - this one will work!!
// 1560 - fix timer run BUG in llcontrol
// 1561 - that wasn't THE bug - add heavy debugging
// 1564 - reload DO fifo when it goes down - keep those timers running
// 1565 - use smMarkDO and fix bug in smMarkDO()
// 1566 - and reload less frequently
// 1567 - move DO refill to back of capture
// 1568 - fix over aggressive assert in storeman.c
// 1569 - fix read counter TINST
// 1570 - fix load XO bug - round store up to nearest %16 - how did it ever work ??
// 1571 - remove asserts in storeman, stub bad values instead
// 1572 - add comfort zone to AO reserve
// 1580 - small dig trig std of acq16 FET and norm, fix CAS2 indicator, add setDIO in ARM mode
// 1584 - fix FET - willit work ???
// 1585 - double-tap changes
// 1587 - make acq16triggeredContinuousCallBack LEAN_AND_MEAN
// 1588 - defaults to non masked
// 1589 - mask selection also selections #hw channels
// 1590 - make 12 ch special case - don't do hw chan reduction to save analog trig
// 1592 - looking at channel mask
// 1594 - try to fix manual channel setting
// 1595 - fix FS44 - don't corrupt put pointer
// 1596 - force prelen to be a multiple of block size
// 1600 - fix bug in above
// 1601 - tweak postlen to remove sample buffer overrun
// 1610 - Benbecula/HTM#1
// 1611 - merge from multi trig debug on 1601-1608 W7
// 1612 - correct cancel for multitrig output
// 1613 - fix acq16config overwrite, setDIO from mbox restored
// 1618 - cycle count
// 1619 - cycle count clears on setMode, debounce opt
// 1620 - remove AI overflow printf (is it causing a secondary overflow?)
// 1621 - work on atrig latency
// 1626 - HTstream debugging
// 1629 - HTsteam reports fifo overrun
// 1632 - derives acq16 set channels from cal info, rounds down int clock
// 1633 - TRMAS for acq16
// 1634 - correct use SRC not bit pattern
// 1639 - resurrect streaming
// 1651 - add new style int clock to acq16. assume acq32 is unchanged
// 1652 - streaming with logical channels. willit work ??
// 1664 - streaming data is good
// 1665 - try restrucured streaming (ch count prep for multi samples per DMA)
// 1667 - subrate by channel count works (but it's no faster :-(
// 1668 - work on internal clock for best setting
// 1671 - stub 44M clock (XILINX BUG)
// 1675 - work on variable dumdma
// 1677 - adj endbuf calc for gut little gap willit work ?
// 1678 - fix bug in fifo_sink() => ret code faulty on trigger (was that it??)
// 1679 - frig actuals at end of gut
// 1680 - try trace it, + offset monitor
// 1681 - always round upstream count UP
// 1682 - ensure sink picks off FIRST TW
// 1683 - attempt auto buffer adjust for acq16 dtrig pipeline
// 1686 - auto buffer adjust works - wire it in and hope for the best
// 1687 - pipeline delay back to 2
// 1688 - remove acq16 AO reserve
// 1689 - hmm, keep a small reserve for buffer overrun reasons (why??)
// 1690 - explicit status change interrupt enable
// 1691 - fix internal clock select FU, try debug end of capture OOPS
// 1692 - fix again, choose first smallest neg error, else smalles pos error
// 1695 - try to reduce ICACHE loading on abort
// 1696 - hook BOOST on mbox_setMask()
// 1697 - trim some bits from dh_acq12_run
// 1698 - only do odd jobs on fifo half (willit work?)
// 1670 - fix bug in mbox setting boost
// 1671 - back out reduced mbox handling in dh_acq12run()
// 1672 - route diags - trouble 't pre buffer again
// 1673 - add dead band to post buffer (overlaps pre??)
// 1674 - rework dh_acqXX_run() to try optimise. 12x2.5 here we come!
// 1681 - peamble with live FIFO - willit do the trick ??
// 1682 - back out 1681 - bad idea
// 1683 - fix post buffer after acq16 fixup. 
// 1684 - attempt length adjust on acq16 data
// 1685 - fix bug in above
// 1689 - looks good
// 1690 - add status err bit, acq16 Runner dist/!dist varients, split getCap
// 1692 - pair0 starts need update too
// 1693 - update GPEM for null P1, getEvent
// 1694 - fix getEvent (off by one error)
// 1695 - add env handling, DMAC expts
// 1699 - ACQ16 offset adjust
// 1700 - ACQ16 offset adj will work wont it
// 1701 - Offset adj with cahnnel mask, responsive, ST_POSTPROCESS
// 1703 - protect offset adj from rogue values on abort ??
// 1704 - negate ext clock divider, remove extra dual defined globs
// 1706 - new dac pot tweak, drop inc before exit
// 1707 - even newer dac pot tweak
// 1708 - PostProcessCommand - BoxCar
// 1713 - finally, BoxCar should work
// 1714 - clean filter buffer first for clear display
// 1716 - finally, diy acq16 zero offset must work
// 1717 - no this time, was only working for nchan==16
// 1718 - and finally
// 1720 - fixes extra post len bug in acq32
// 1721 - HTSTREAM V2
// 1722 - option to incr status word or not
// 1723 - debug HTSTREAM V2
// 1724 - HTSTREAM V2 with repeatable LEDS, loc stat complete
// 1725 - HTSTREAM status high pci addr switching
// 1726 - and interrupts should work too
// 1727 - llcontrol has LLC_CSR_M_SET_DAC, BP_MB_LLC_DAC options
// 1728 - change llcontrol internal loopback to sample DO3 output
// 1729 - fix llc setDAC oops

// 2000 - port to ACQ200
// 2003 - i2o file download works
// 2005 - first hack DMA on pci
// 2011 - fix HOST_BOUND DMA RACE
// 2012 - direction DMA mapping as que to mu_rma memory buf directions
// 2013 - DUMDMA emulation must work!
// 2017 - bigbuf size calc
// 2018 - fix chan def off by 1
// 2019 - fix data file read error (nil) FILE
// 2020 - link ChannelMask to driver.
// 2021 - bus prot check - pre in A4, post in A3 (for historical reasons)
// 2022 - second try, get it right everywhere
// 2023 - restore pci file transfer, minor snafu on channel off by 1 fixed
// 2026 - rsh commands on pci
// 2027 - wavegen data handling
// 2029 - ACQ200_PPJOB etc
// 2030 - setvbuf on debug socket
// 2040 - rework debug socket to prep for more ...
// 2045 - most local_slave command handling in
// 2046 - correct getVoltsRange etc
// 2050 - work on acq100 hooks
// 2051 - map mu_hostbuffer, not raw acq200.pcim
// 2052 - getState, getMode back to trad
// 2053 - adjust for acq196 non-pci device
// 2054 - attempt concurrent setArm
// 2055 - concurrent version takes shape - signal handling sussed
// 2056 - and post process too
// 2057 - continuous mode support (SOFT)
// 2059 - local setDIO, getDIO
// 2060 - fake setChannelMask for ACQ196
// 2061 - prelen post len handling for 2G
// 2062 - plumb TriggeredContinuous
// 2063 - force triggeredContinous pre==0 to be GATED_TRANSIENT
// 2064 - and forc trig = event0 (client sets events)
// 2065 - fix setChannelMask bug - forever
// 2066 - 2G low latency hook to driver
// 2068 - make it work with WAV232
// 2070 - intclk - clkmas plumbed for 2G
// 2071 - remove 2070 - hook to set_ext_clk
// 2072 - improve numberOfEnabledChannels() impl.
// 2073 - event0 not ev0
// 2074 - set default trigger event if ev0 0.
// 2075 - event0
// 2080 - stub pci abort, unify pci/user aborts, get dev state
// 2081 - dev state is good
// 2090 - MARK
// 2091 - rsh single path style
// 2092 - direct dma
// 2093 - direct dma with local aggregation
// escape read loop on zero
// 2100 - release ready?!!
// 2101 - remove zero prelen frig
// 2102 - getNumSamples on pci reports post, pre NOT TOTAL
// 2103 - setUserLed from local-slave
// 2104 - command2 - accepts 2 localcommand streams
// 2105 - accepts N (N=4) localcommand streams
// 2106 - getNumSamples elapsed reporting now correct
// 2107 - uses new-style get.vin
// 2108 - command logging via clog
// 2111 - fixes getNumChannels bug, tolerates acq196m and variants
// 2112 - wild card A4 in llc start
// 2113 - ull elapsed count
// 2115 - fix race condition on reading ST_IDLE
// 2116 - use ST_CAPDONE to really fix "
// 2121 - limit min run time to MINTIME
// 2122 - reduce debugs
// 2124 - host window accomodates big buf < 16MB ?.
// 2125 - split local interp and mailbox interp into two threads
// 2126 - optimise for short shell commands 
// 2127 - acq200_getAOcount() checks device files on ACQ196 
// 2128 - mailbox thread runs at low priority
// 2129 - fix core sys file error
// 2130 - poll priority debug
// 2131 - NO_MBOX_THREAD env control is effective.
// 2132	- use fork() not pthread_create
// 2135 - use fork() not thread.
// 2136 - filter ST_STATE
// 2137 - text acqcmd handling.
// 2140 - text acqcmd speed up using direct usc link
// 2156 - text acqcmd works at last (host ran out of buffers)
// 2157 - strip gut, storeman, reduce from 106 to 82K
// 2158 - attempt select() on MU IPQ.
// 2159 - change HRD_SPCLID_ACQCMD=109 to avoid channel 10 alias clash
// 2160 - throttle MU ints during capture to avoid priority inversion
// 2161 - ensure throttle only re-enabled on start.
// 2162 - reduce hits to BP_MB_STATUS
// 2165 - attempt to eliminate state report race
// 2171 now uses fork/fork/wait rather than SIGCHLD for callbacks
// 2172 - OIMR setting
// 2173 - allow setDIO from trad MBOX control
// 2174 setDownstreamWindow sets length as well
// 2180 - init_host_window_mapping by default (attempt restart pci).
// 2184 - yeah, but then first time, the mapping _may_ not be correct
// 2193	- slave priority adjust
// 2194 - DMATBLOCK
// 2202 - modify HAL to remove setLength (not needed)
// 2203 - set state ST_ARM, NOT RUN!!
// 2204 - search for mysterious exit(0)
// 2205 - ditto
// 2206 - ACQ200_SYSLOG - enables caller script to get exit status
// 2207 - ACQ132
// 2208 fix bug in InternalClock MASTER_CLK
// 2209 detect state at end of capture to avoid spurious POSTPROCESS
// 2210 general code cleanup, clog command should work
// 2210 improved debugs
// 2212 lock out most local commands when state != ST_STOP
// 2215 don't allow multiple fd_dma paths 
// 2216 fix setArm/setAbort/setArm race
// 2217 acq164
// 2219 ics_527 aware
// 2220 and ics limits aware, setting decimation if required.
// 2222 provide acq132, acq164 defaults to ExternalClock
// 2223 acq132 INCLK_NOICS
// 2226 set decimation=1 on acq132 setExternaClock NOICS
// 2227 setChannelMask for ACQ164 as well
// 2228 set manager to hi priority to try limit ST_CAPDONE time.

#define BUILD	"2229"

#include <stdio.h>


const char* GBD = 
	"ACQ216CPCI, WAV232CPCI copyright (c) D-TACQ Solutions Ltd 2003-2008\n"
        "ACQ200 image built at " 
        __DATE__ ":" __TIME__ " Build " BUILD "\n"
        "$Id: getBuildDate.c,v 1.227.2.102 2011/11/11 19:13:16 pgm Exp $\n"
        "$Revision: 1.227.2.102 $\n";

static const char* _getBuildDate(char client_buf[], int maxbuf)
{
	FILE* fp = popen("echo linux.initrd.extra "
			 "`/bin/get.release.versions`", "r");
	if (fp){
		fgets(client_buf, maxbuf, fp);
		pclose(fp);
	}else{
		sprintf(client_buf, "system version not available\n");
	}
	strncat(client_buf, GBD, maxbuf-strlen(client_buf));
	return client_buf;
}

const char* getBuildDate( char client_buf[], int maxbuf )
{
    if ( client_buf ) {
	    return _getBuildDate(client_buf, maxbuf);
    }else{
	    static char* cb;
	    if (!cb){
		    cb = malloc(1024);
	    }
	    return _getBuildDate(cb, 1024);
    }
}

