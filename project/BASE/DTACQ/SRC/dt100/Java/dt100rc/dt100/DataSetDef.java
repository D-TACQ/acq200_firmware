/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 31, 2004
 * Time: 5:42:32 PM
 * To change this template use File | Settings | File Templates.
 */
package dt100;

import java.io.Serializable;

public class DataSetDef implements Serializable{

    private void _dataSetDef(int start, int len, int stride) {
        this.start = start;
        this.len = len;
        this.stride = stride;
    }
    public DataSetDef(int start, int len, int stride) {
        _dataSetDef(start,  len, stride);
    }

    public DataSetDef() {
        _dataSetDef(0,0,0);
    }

    public DataSetDef(int start, int len) {
        _dataSetDef(start, len, 1);
    }

    public DataSetDef(int len) {
        _dataSetDef(1, len, 1);
    }

    public int getStart() {
        return start;
    }

    public int getStop() {
        return (getStart()-1) + getLen() * getStride();        
    }

    public void setStart(int start) {
        this.start = start;
    }

    public int getLen() {
        return len;
    }

    public void setLen(int len) {
        this.len = len;
    }

    public int getStride() {
        return stride;
    }

    public void setStride(int stride) {
        this.stride = stride;
    }

    private int start;
    private int len;
    private int stride;

}
