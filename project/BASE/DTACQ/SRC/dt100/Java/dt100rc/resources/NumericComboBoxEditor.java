/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 6:36:41 PM
 *
 * $RCSfile: NumericComboBoxEditor.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: NumericComboBoxEditor.java,v 1.1 2002/05/27 19:41:06 pgm Exp $
 * $Log: NumericComboBoxEditor.java,v $
 * Revision 1.1  2002/05/27 19:41:06  pgm
 * numeric combos R us
 *
 *
 *
\*****************************************************************************/
package resources;

import debug.Debug;

import javax.swing.plaf.basic.BasicComboBoxEditor;
import java.io.Serializable;


// Why does it have to be Serializable (we only save models,not combo boxes

public class NumericComboBoxEditor extends javax.swing.plaf.metal.MetalComboBoxEditor implements Serializable {
    Range range;

    public NumericComboBoxEditor( Range r ) {
        this.range = r;
    }
    public void setItem(Object anObject) {
        if ( anObject != null ){
            editor.setText( anObject.toString() );
        }
    }

    public void setRange( FloatRange range ){
        this.range = range;
    }
    public Object getItem() {
        return UnitFactory.newUnit( range, editor.getText() );
    }
}
