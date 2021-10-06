/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: SC_DioTableModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: SC_DioTableModel.java,v 1.6 2002/05/27 10:25:10 pgm Exp $
 * $Log: SC_DioTableModel.java,v $
 * Revision 1.6  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import java.awt.*;
import java.io.Serializable;
import javax.swing.*;
import javax.swing.table.*;




class SC_DioTableModel extends AbstractTableModel {
 
    public String bits[] = new String[16];

    public SC_DioTableModel() {
        for ( int ibit = 0; ibit != 16; ++ibit ){
            bits[ibit] = "-";
        }
    }
    
    public int getRowCount() {
        return 1;
    }
    public int getColumnCount() {
        return 16;
    }
    public Object getValueAt( int row, int col ) {
        return bits[col];
    }
    public void setValueAt(Object aValue,  int row, int col ){
        bits[col] = aValue.toString();
    }

    public String getColumnName( int c ) {
        return String.valueOf( c+1 );
    }
   
    public boolean isCellEditable(int rowIndex, int columnIndex){
        return true;
    }
    
    void setColumnModel( TableColumnModel columnModel ) {
        JComboBox dio_combo = new JComboBox();
        dio_combo.addItem( new String( "1" ) );
        dio_combo.addItem( new String( "0" ) );
        dio_combo.addItem( new String( "-" ) );
        
        for ( int icol = 0; icol != 16; ++icol ){
            columnModel.getColumn( icol ).setCellEditor( 
                new DefaultCellEditor( dio_combo ) 
            );
        }
    } 
    
    public void setTable( JTable table ) {
        table.setModel( this );
        table.setRowSelectionAllowed( false );
        table.setCellSelectionEnabled( true );
        setColumnModel( table.getColumnModel() );

        TableColumnModel columnModel = table.getColumnModel();
        
        for ( int icol = 0; icol != 16; ++icol ){
            TableColumn column = columnModel.getColumn( icol );
            
            column.setMinWidth( 32 );
            column.setMaxWidth( 32 );
        }       
    }
    
    public String toString() {
        
        String str = "";
        
        for ( int ibit = 0; ibit != 16; ++ibit ){
            str += bits[ibit];
        }
        
        return str;
    }
    public void update( String readback_bits ) {
       
        for ( int ibit = 0; ibit != readback_bits.length() && ibit != 16; ++ibit ){
            bits[ibit] = readback_bits.substring(ibit, ibit);
        }
    }
}

