/*
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 25, 2002
 * Time: 9:43:47 AM
 * To change template for new interface use 
 * Code Style | Class Templates options (Tools | IDE Options).
 */
package dt100;

public interface StreamingInfo {
    StreamFrame getLastFrame();
    public StreamFrame[] getFrameSet();

    int getFrameCount();

    int getErrors();

    int getPasses();

    long getBytes();

    float getMbps();

    float getSampleRate();

    int getUpdateNumber();
}
