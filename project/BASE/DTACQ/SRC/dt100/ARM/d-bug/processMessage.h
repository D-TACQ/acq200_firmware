/*****************************************************************************
 *
 * File: processMessage.h
 *
 * $RCSfile: processMessage.c,v $
 *
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: processMessage.c,v 1.1.2.12 2004/10/23 14:17:45 pgm Exp $
 * $Log: processMessage.c,v $
 */


#ifndef __PROCESS_MESSAGE_H__
#define __PROCESS_MESSAGE_H__

struct Message;

void acq200_setTransferModeDma(int dma);
/*
void acq200_processHostbound(struct Message *message);
void acq200_processAcqbound(struct Message *message);
*/
#endif /* __PROCESS_MESSAGE_H__ */
