/*
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 25, 2002
 * Time: 2:54:21 PM
 * To change template for new class use 
 * Code Style | Class Templates options (Tools | IDE Options).
 */

import dt100.StreamingCallbackManager;
import dt100.StreamingAdapter;

import java.awt.event.ComponentAdapter;


public abstract class MonitorFrame extends FrameWithModel {
     abstract protected StreamingAdapter getStreamingAdapter();
}
