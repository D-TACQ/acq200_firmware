
import dt100.*;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.*;

import debug.Debug;

/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: Jun 11, 2004
 * Time: 12:03:11 PM
 * To change this template use File | Settings | File Templates.
 */


class StreamingFrameAdapter implements StreamingAdapter {
        FrameWithModel frame;
        String title;
        boolean first_time;
        protected int sflags;
        boolean auto_update;
        private int maxchannels;
        private int first_channel;
        private boolean view_volts;
        boolean is_busy;

        private void register() {
            StreamingCallbackManager.instance().register( this );
        }
        private void unRegister() {
            StreamingCallbackManager.instance().unRegister( this );
        }

        class WindowClosedListener extends WindowAdapter {
            // if the window is closed explicitly, don't let it come back!!
                public void windowClosing(WindowEvent e) {
                    frame.clearParent();
                    unRegister();
                }
        }
        public FrameWithModel getFrame() {
            return frame;
        }
        public void freeze() {
            unRegister();
        }
        public StreamingFrameAdapter( FrameWithModel frame, Component parent, boolean auto_update ) {
            this.frame = frame;
            this.title = frame.getTitle();
            first_time = true;
            frame.setParent( parent );
            this.auto_update = auto_update;
            this.setFirst_channel(0);

            register();

            frame.addWindowListener( new WindowClosedListener() );
        }
        public void setTitle( String title ) {
            frame.setTitle( title );
        }
        public void repeat() {
            register();
        }
        public void setMaxchannels( int maxchannels ){
            this.maxchannels = maxchannels;
        }
        public void viewVolts(boolean view_volts ) {
            this.view_volts = view_volts;
        }

        public void onNewFrame(StreamingInfo info) {
            if (!is_busy) {
                is_busy = true;
                StreamFrame[] streamFrames;

                frame.setTitle(title + info.getUpdateNumber());
                if ((sflags & StreamDataModel.IS_MULTIFRAME) != 0) {
                    streamFrames = info.getFrameSet();
                } else {
                    streamFrames = new StreamFrame[1];
                    streamFrames[0] = info.getLastFrame();
                }
                frame.setModel(StreamDataModel.instantiate(
                            streamFrames, info.getFrameCount(),
                            maxchannels, sflags, view_volts, getFirst_channel()));
                if (first_time) {
                    frame.setVisible(true);
                    first_time = false;
                }

                if (auto_update) {
                    StreamingCallbackManager.instance().register(this);
                }
                is_busy = false;
            } else {
                Debug.println(1, "onNewFrame, is_busy, discard " + info.getUpdateNumber());
            }
        }

    public int getFirst_channel() {
        return first_channel;
    }

    public void setFirst_channel(int first_channel) {
        this.first_channel = first_channel;
    }

}