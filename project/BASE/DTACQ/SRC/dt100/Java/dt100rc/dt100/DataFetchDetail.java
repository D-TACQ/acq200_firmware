
/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 30, 2004
 * Time: 10:39:40 PM
 * To change this template use File | Settings | File Templates.
 */

package dt100;

public class DataFetchDetail {
    public dt100.Channel channel;
    public int start;
    public int stop;
    public int stride;
    public String format;
    public Runnable callback;
    public boolean saves_to_file;
    public boolean held_in_ram;
}
