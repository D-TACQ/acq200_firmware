/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: AnalogPrams.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: AnalogPrams.java,v 1.3 2002/11/09 21:27:50 pgm Exp $
 * $Log: AnalogPrams.java,v $
 * Revision 1.3  2002/11/09 21:27:50  pgm
 * fs range plot opt
 *
 * Revision 1.2  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

import java.io.Serializable;

public class AnalogPrams implements Serializable
// well no it doesn't. not yet. maybe we don't want to save this??
{
    float vmax;
    float vmin;
    int nbits;
    float fmax;
    int available_channels;
/*    private void writeObject(java.io.ObjectOutputStream out)
     throws IOException {}
 private void readObject(java.io.ObjectInputStream in)
     throws IOException, ClassNotFoundException; {}
*/
    public float getVmax() { return vmax; }
    public float getVmin() { return vmin; }
    
    public int getRawMax() { return  32767; } // WORKTODO
    public int getRawMin() { return -32768; }
}
