/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: AbstractChannelCallback.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: AbstractChannelCallback.java,v 1.4 2002/05/27 10:25:11 pgm Exp $
 * $Log: AbstractChannelCallback.java,v $
 * Revision 1.4  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;




abstract public class AbstractChannelCallback implements Runnable {
    
    protected Channel channel;
    
    public AbstractChannelCallback( Channel channel ) {
        this.channel = channel;
    }
    
    public void setNumSamples( int nsamples ) {
        channel.setNumSamples( nsamples );   
    }
    
    void clearUploading() {
        channel.clearUploading();   
    }
    //public abstract void run();
}