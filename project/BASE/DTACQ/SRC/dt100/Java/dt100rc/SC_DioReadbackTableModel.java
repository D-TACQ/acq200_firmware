/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: SC_DioReadbackTableModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: SC_DioReadbackTableModel.java,v 1.4 2002/05/27 10:25:10 pgm Exp $
 * $Log: SC_DioReadbackTableModel.java,v $
 * Revision 1.4  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import java.awt.*;
import java.io.Serializable;
import javax.swing.*;
import javax.swing.table.*;




class SC_DioReadbackTableModel extends AbstractTableModel {
 
    public Object bits[] = new Object[16];

    public SC_DioReadbackTableModel() {
        for ( int ib = 0; ib != 16; ++ib ){
            bits[ib] = new JLabel( "-" );   
        }
    }
    
    public int getRowCount() {
        return 1;
    }
    public int getColumnCount() {
        return 16;
    }
    public Class getColumnClass( int column_index ) {
        return JLabel.class;   
    }
    public Object getValueAt( int row, int col ) {
        return bits[col];
    }
    public void setValueAt(Object aValue,  int row, int col ){
        bits[col] = aValue;
    }

    public String getColumnName( int c ) {
        return String.valueOf( c+1 );
    }
   
    public boolean isCellEditable(int rowIndex, int columnIndex){
        return false;
    }
        
    public void setTable( JTable table ) {
        table.setModel( this );
        table.setRowSelectionAllowed( false );
        table.setCellSelectionEnabled( false );
        
        TableColumnModel columnModel = table.getColumnModel();
        
        for ( int icol = 0; icol != 16; ++icol ){
            TableColumn column = columnModel.getColumn( icol );
            
            column.setMinWidth( 32 );
            column.setMaxWidth( 32 );
        }
    }
}

