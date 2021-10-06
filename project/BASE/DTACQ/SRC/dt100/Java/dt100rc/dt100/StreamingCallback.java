/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: StreamingCallback.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: StreamingCallback.java,v 1.6.4.7 2004/06/26 22:57:24 pgm Exp $
 * $Log: StreamingCallback.java,v $
 * Revision 1.6.4.7  2004/06/26 22:57:24  pgm
 * Build 5210 MF stats look good.
 * Teed up for previous second control output.
 * (see DtacqClient.connectRaw()).
 *
 * Revision 1.6.4.6  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.6.4.5  2004/06/22 20:46:17  pgm
 * only run StreamCallbacks on Frame Accept.
 *
 * Revision 1.6.4.4  2004/06/12 16:54:06  pgm
 * burst streaming plot buggy, but generally good enough
 *
 * Revision 1.6.4.3  2004/06/11 16:54:45  pgm
 * MF display works, but can core dump also
 *
 * Revision 1.6.4.2  2004/06/10 21:53:23  pgm
 * fix MF frame sync error
 *
 * Revision 1.6.4.1  2004/06/10 20:32:14  pgm
 * MF is go!
 *
 * Revision 1.6  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
 \*****************************************************************************/

package dt100;

import debug.*;

public abstract class StreamingCallback implements Runnable, StreamingInfo {
    final int frames_per_update;
    StreamFrame the_frames[];
    StreamFrame last_frame;
    StreamFrame prev_frames[];
    StreamFrame prev_last_frame;

    int iframe;
    int npasses;
    boolean update_queued;
    int nerrors;
    long nbytes;
    long prev_bytes;
    long prev_sample_number;
    long t_prev;
    long t_delta;   // time between frames
    float mbps;
    float fps;
    private int t_update;

    int frame_id;
    protected boolean force_update;
    protected int frame_count;

    protected static int droppedFrameCount;

    void init() {
        the_frames = new StreamFrame[frames_per_update];
        prev_frames = new StreamFrame[frames_per_update];
    }

    public int setTheFrame(StreamFrame aFrame) {
        the_frames[iframe] = last_frame = aFrame;
        frame_count = iframe + 1;
        nbytes += aFrame.length();
        aFrame.setFrame_accepted(true);
        return ++frame_id;
    }

    public StreamingCallback(int frames_per_update) {
        iframe = 0;
        this.frames_per_update = frames_per_update;
        mbps = fps = 0.0f;
        init();
        update_queued = false;
        setT_update(1000);

    }

    public StreamingCallback() {
        iframe = 0;
        frames_per_update = 1;
        init();
        setT_update(1000);
    }

    private synchronized void makeDownstreamclone() {
        System.arraycopy(the_frames, 0, prev_frames, 0, frames_per_update);
        prev_last_frame = last_frame;
    }

    static String frameWarnNull(StreamFrame frame, String warning){
        if (frame == null){
            return warning;
        }else{
            return String.valueOf(Dt100StreamFrame.FN(frame));
        }
    }
    void dumpMFNs() {
        for (int frame = 0; frame != frames_per_update; ++frame){
            System.err.println(frame + " " +
                        frameWarnNull(the_frames[frame], "orig null") + " " +
                        frameWarnNull(prev_frames[frame], "clone null") );
        }
    }
    synchronized boolean makeReadyToRun() {
        if (true /*iframe == frames_per_update*/) {
            if (!update_queued) {
                long t_now = System.currentTimeMillis();

                t_delta = t_now - t_prev;

                if (/* force_update || */ t_delta > getT_update()) {
                    t_prev = t_now;
                    long current_sample_number = last_frame.getSampleNumber();

                    mbps = (float) 8 * (nbytes - prev_bytes);
                    mbps /= t_delta * 1000;
                    fps = 1000 * (current_sample_number - prev_sample_number) / t_delta;
                    prev_bytes = nbytes;
                    prev_sample_number = current_sample_number;
                    //@@todo is this our memory leak?
                    makeDownstreamclone();

                    force_update = false;

                    return update_queued = true;
                }
            } else {
                init();
            }
        }
        return false;
    }

    synchronized private void clearUpdate_queued(){
        update_queued = false;
    }
    public void run() {
        update_number++;
        onRun();
        manager.onRun(this);
        clearUpdate_queued();
        System.gc();   // @@todo - beware java.lan.OutOfMemoryError - will this fix it?
    }

    abstract protected void onRun();

    public StreamFrame getLastFrame() {
        return last_frame;
    }

    public StreamFrame[] getFrameSet() {
        return prev_frames;
    }

    public int getFrameCount() {
        return frame_count;  //To change body of implemented methods use File | Settings | File Templates.
    }

    public void incrementErrors(int nerrors) {
        this.nerrors += nerrors;
    }

    public void incrementPasses(int npasses) {
        this.npasses += npasses;
    }

    public int getErrors() {
        return nerrors;
    }

    public int getPasses() {
        return npasses;
    }

    public long getBytes() {
        return nbytes;
    }

    public float getMbps() {
        return mbps;
    }

    public float getSampleRate() {
        return fps;
    }

    StreamingCallbackManager manager;

    void registerManager(StreamingCallbackManager manager) {
        this.manager = manager;
    }

    int update_number = 0;

    public int getUpdateNumber() {
        return update_number;
    }

    /**
     * @param frame_type
     * @return Factory method for streaming callback.
     */

    static StreamingCallback createInstance(String frame_type) {
        if (frame_type.compareTo("mf") == 0) {
            return new MultiframeStreamingCallback();
        } else {
            return new StreamingCallback() {
                protected void onRun() {
                }
            };

        }
    }

    public int getT_update() {
        return t_update;
    }

    public void setT_update(int t_update) {
        this.t_update = t_update;
    }

    public static int getDroppedFrameCount() {
        return droppedFrameCount;
    }
}

class MultiframeStreamingCallback extends StreamingCallback {
    static final int MAXFRAMES = 64;

    MultiframeStreamingCallback() {
        super(MAXFRAMES);
    }

    boolean mf_complete = false;

    int last_error_report_frame_id;

    synchronized public int setTheFrame(StreamFrame aFrame) {
        if (Dt100StreamFrame.isMF(aFrame)) {
            int fn = Dt100StreamFrame.FN(aFrame);
            if (fn == iframe) {
                int blen =  Dt100StreamFrame.BLEN(aFrame);
                nbytes += aFrame.length();
                the_frames[iframe++] = last_frame = aFrame;
                if (iframe == blen) {
                    frame_count = iframe;
                    iframe = 0;
                    ++frame_id;
                    mf_complete = true;
                    aFrame.setFrame_accepted(true);
                    Debug.println(2, "MF_COMPLETE: Accepted iframe:" + iframe + " frame_id:" + frame_id + " blen:"+blen);
                } else {
                    Debug.println(2, "!!_COMPLETE: Accepted iframe:" + iframe + " frame_id:" + frame_id + " blen:"+blen);
                    mf_complete = false;
                }
            } else {
                if (iframe != 0) {
                    System.err.println(System.currentTimeMillis() / 100 + " " +
                            (fn > iframe ? "MISSED SUBFRAME" : "LOST SUBFRAME") + " " +
                            "fn:" + fn + " iframe:" + iframe + " terminate MF:" + frame_id);
                    iframe = 0;
                    ++frame_id;
                } else {
                    if (last_error_report_frame_id != frame_id){
                        System.err.println(System.currentTimeMillis() / 100 +
                            " Waiting for MF wanted:" + iframe + " got:" + fn + " blen:" +
                            Dt100StreamFrame.BLEN(aFrame) + " frame_id:" + frame_id);
                        last_error_report_frame_id = frame_id;
                    }
                }
                droppedFrameCount++;
            }
        } else {
            the_frames[iframe] = last_frame = aFrame;
            aFrame.setFrame_accepted(true);
            ++frame_id;
        }
        return frame_id;
    }

    boolean makeReadyToRun()
            // Hold off until we have a full MF available
    {
        force_update = true;
        return super.makeReadyToRun();
    }

    public void onRun() {
    }
}