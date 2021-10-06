/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DtacqComboBoxModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqComboBoxModel.java,v 1.5.4.2 2004/06/26 23:23:05 pgm Exp $
 * $Log: DtacqComboBoxModel.java,v $
 * Revision 1.5.4.2  2004/06/26 23:23:05  pgm
 * Build 5211 - Externalizable Rocks - fixed dodgy saved state, now it is MUCH smaller,
 * [pgm@islay dt100rc]$ ls -l dt100.saved
 * -rw-rw-r--    1 pgm      pgm         11992 Jun 27 00:07 dt100.saved
 * [pgm@islay dt100rc]$ ls -l dt100.saved
 * -rw-rw-r--    1 pgm      pgm         13531 Jun 27 00:09 dt100.saved
 * all the DtacqCombos work
 * AND it works with JDK1.5 - Funky!
 *
 * Revision 1.5.4.1  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.5  2002/05/27 19:40:16  pgm
 * numeric combos R us
 *
 * Revision 1.4  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package resources;

import debug.Debug;

import javax.swing.*;
import java.io.*;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.Iterator;
import java.util.Comparator;

public class DtacqComboBoxModel extends DefaultComboBoxModel implements Externalizable {
    private SortedSet myset;
    protected Comparator comparator;            // not used

    private void init() {
        if ( comparator != null ){
            myset = new TreeSet( comparator );
        }else{
            myset = new TreeSet();
        }
    }
    public DtacqComboBoxModel(){
        super();
        init();
    }
    public DtacqComboBoxModel( Object[] items ){
        super();
        init();
        inhibit_events = true;
        setItems( items );
        setSelectedItem(items[0]);
        inhibit_events = false;
    }

    public void addElement( Object item ) {
        if ( !myset.contains( item ) ){
            myset.add( item );
        }

        removeAllElements();

        Iterator it = myset.iterator();
        for ( int index = 0; it.hasNext(); ++index ){
            insertElementAt( it.next(), index);
        }
    }

    public void addElement( Object item, boolean fireEvents ){
        boolean old_inhibit_events = inhibit_events;

        if ( !fireEvents ){
            inhibit_events = true;
        }
        addElement( item );
        inhibit_events = old_inhibit_events;

    }
    public void setItems( Object[] items ){
        // remove first ( user can do that )
        for ( int ii = 0; ii != items.length; ++ii ){
            addElement( items[ii] );
        }
    }
    private void writeObject(java.io.ObjectOutputStream out)
            throws IOException {
        //out.defaultWriteObject(); // Black Magic TIJ ed1 p 503

        // save no listeners (they may not be Serializable)
        // depends on client adding them again afterwards
        // ie restore FIRST
        int size = getSize();

        out.writeObject( new Integer( size )  );

        for ( int ii = 0; ii != size; ii++ ){
            Object object = getElementAt( ii );
            out.writeObject( object );
        }

        out.writeObject( getSelectedItem() );

//       Debug.println( "writeObject() selected item "+getSelectedItem().toString() );
    }
    boolean inhibit_events;

    protected void fireContentsChanged(Object source, int index0, int index1) {
        if ( !inhibit_events ){
            super.fireContentsChanged( source, index0, index1 );
        }
    }
    protected void fireIntervalRemoved(Object source, int index0, int index1) {
        if ( !inhibit_events ){
            super.fireIntervalRemoved( source, index0, index1 );
        }
    }
    protected void fireIntervalAdded(Object source, int index0, int index1)
    {
        if ( !inhibit_events ){
            super.fireIntervalAdded( source, index0, index1 );
        }
    }

    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        //To change body of implemented methods use File | Settings | File Templates.  #
        inhibit_events = true;
        init();                     // ???? doesn't no-arg constructor do this?
        removeAllElements();        // discard any existing construction

        // build the whole of myset before copyinjg it to the parent collection.

        int size = ((Integer)in.readObject()).intValue();

        for ( int ii = 0; ii != size; ++ii ){
            myset.add( in.readObject() );
        }

        Iterator it = myset.iterator();
        for ( int index = 0; it.hasNext(); ++index ){
            insertElementAt( it.next(), index);
        }
        setSelectedItem( in.readObject());

        inhibit_events = false;
    }

    public void writeExternal(ObjectOutput out) throws IOException {
        //To change body of implemented methods use File | Settings | File Templates.
        //out.defaultWriteObject(); // Black Magic TIJ ed1 p 503

          // save no listeners (they may not be Serializable)
          // depends on client adding them again afterwards
          // ie restore FIRST
          int size = getSize();

          out.writeObject( new Integer( size )  );

          for ( int ii = 0; ii != size; ii++ ){
              Object object = getElementAt( ii );
              out.writeObject( object );
          }

          out.writeObject( getSelectedItem() );

    }
}
