/*****************************************************************************
 *
 * File: sink.h
 *
 * $RCSfile: sink.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: sink.h,v 1.3 2002/04/14 10:58:32 pgm Exp $
 * $Log: sink.h,v $
 * Revision 1.3  2002/04/14 10:58:32  pgm
 * this seems to work (don't use heap after fftw :-))
 *
 * Revision 1.2  2002/04/12 10:35:47  pgm
 * added fftw code. segfaults :-(
 *
 * Revision 1.1  2002/04/12 08:52:00  pgm
 * decoupled factory arch runs
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



class Sink {
 protected:
    int nchannels;
    int nactive;
    unsigned channel_mask;
    int active_channel;

    Sink();
    virtual ~Sink() {}
 public:
    virtual void init() = 0;
    virtual void init( int totlen ) { init(); }
    virtual void sink( unsigned pbuffer, unsigned nbuffer ) = 0;
    virtual void final() = 0;

    int getNumChannels() const { return nchannels; }
    int getNumActive()   const { return nactive; }
    int getActiveChannelMask() const { return channel_mask; } // iterator WBN
    int getActiveChannel()  const { return active_channel; } // first if > 1

    virtual void print() {}
};


class NullSink : public Sink {
    void init() {}
    void sink( unsigned pbuffer, unsigned nbuffer ) {

    }
    void final() {}
};

class SinkImpl : public Sink {
// producers of sinks may find it handy to subclass this one
protected:
    SinkImpl( int _nchannels = 12, unsigned _channel_mask = 0x1 );
    void print();
};
