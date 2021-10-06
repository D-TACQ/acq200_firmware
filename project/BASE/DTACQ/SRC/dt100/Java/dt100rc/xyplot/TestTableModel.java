/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: TestTableModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: TestTableModel.java,v 1.1 2002/05/27 10:27:23 pgm Exp $
 * $Log: TestTableModel.java,v $
 * Revision 1.1  2002/05/27 10:27:23  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/

package xyplot;

import javax.swing.table.*;


public class TestTableModel extends AbstractTableModel {
    public int getRowCount() { return 64; }
    public int getColumnCount() { return 1; }
    public Object getValueAt( int row, int column ) {
	return new Double( (double)(column*100+row) );
    }
    public TestTableModel() {
	System.out.println( "TestTableModel()" );
    }
}
