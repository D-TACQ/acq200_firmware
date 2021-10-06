/*****************************************************************************
 *
 * File: acq32_fungen.h
 *
 * $RCSfile: acq32_fungen.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32_fungen.h,v 1.3 2002/03/12 12:12:40 pgm Exp $
 * $Log: acq32_fungen.h,v $
 * Revision 1.3  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.2  2001/05/15 17:56:09  pgm
 * AO/DO fungen implemented
 *
 * Revision 1.1  2001/05/05 19:44:45  pgm
 * stub write(), fix bigdump dep, fault tolerant DUMDMA
 *
 *
 *
\*****************************************************************************/




extern ssize_t acq32_AO_immediate_write (
    struct file *, const char *, size_t, loff_t *);
extern ssize_t acq32_DO_immediate_write (
    struct file *, const char *, size_t, loff_t *);
extern ssize_t acq32_AO_function_write (
    struct file *, const char *, size_t, loff_t *);
extern ssize_t acq32_DO_function_write (
    struct file *, const char *, size_t, loff_t *);

extern int acq32_AO_fungen_open( struct inode* inode, struct file* filp );
extern int acq32_DO_fungen_open( struct inode* inode, struct file* filp );

extern int acq32_AO_fungen_release( struct inode* inode, struct file* filp );
extern int acq32_DO_fungen_release( struct inode* inode, struct file* filp );

