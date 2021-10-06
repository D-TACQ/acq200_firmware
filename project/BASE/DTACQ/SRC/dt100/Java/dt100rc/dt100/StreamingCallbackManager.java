/*
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 25, 2002
 * Time: 9:50:27 AM
 * To change template for new class use
 * Code Style | Class Templates options (Tools | IDE Options).
 */
package dt100;

import java.util.HashSet;
import java.util.Set;
import java.util.Iterator;

public abstract class StreamingCallbackManager {
// Singleton
    static StreamingCallbackManager _instance = new DefaultStreamingCallbackManager();

    public  static StreamingCallbackManager instance() {
        return _instance;
    }
    abstract public void register( StreamingAdapter client );
    abstract public void unRegister( StreamingAdapter client );

    abstract StreamingCallback createCallbackInstance(String frame_type);

    // package local
    abstract void onRun(StreamingCallback cb);
}


class DefaultStreamingCallbackManager extends StreamingCallbackManager {

    Set clients = new HashSet();
    StreamingCallback cb;

    public DefaultStreamingCallbackManager() {

    }

    public void register(StreamingAdapter client) {
        clients.add(client);
    }

    public void unRegister(StreamingAdapter client) {
        clients.remove(client);
    }

    StreamingCallback createCallbackInstance(String frame_type) {
        cb = StreamingCallback.createInstance(frame_type);
        cb.registerManager(this);
        return cb;
    }


    void onRun(StreamingCallback cb) {
        Set cc = new HashSet(clients);
        clients.clear();
        Iterator it = cc.iterator();

        debug.Debug.println("StreamingCallbackManager orig:" + this.cb.hashCode() + "cb: " + cb.hashCode());
        
        while (it.hasNext()) {
            StreamingAdapter sa = (StreamingAdapter) it.next();
            sa.onNewFrame(cb);
        }
    }
}
