/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: SC_ChannelTableModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: SC_ChannelTableModel.java,v 1.6 2002/05/27 10:25:10 pgm Exp $
 * $Log: SC_ChannelTableModel.java,v $
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


class SC_ChannelTableModel extends AbstractTableModel {
    
    static final int NROWS = 32;
    static final int NCOLS = 4;
    
    public int getRowCount()        { return NROWS; }
    public int getColumnCount()     { return NCOLS; }
    

    public SC_ChannelDef channels[] = new SC_ChannelDef[NROWS];
    
    public SC_ChannelTableModel() {
        for ( int ichannel = 0; ichannel != NROWS; ++ichannel ){
            channels[ichannel] = new SC_ChannelDef( ichannel );    
        }
    }
   
    public Object getValueAt( int row, int col ) {
        return new Integer( channels[row].get( col ) );
    }
    public boolean isCellEditable(int rowIndex, int columnIndex){
        return columnIndex != 0;
    }
    public void setValueAt(Object aValue,  int row, int col ){
        channels[row].set( col, aValue.toString() );
    }
                       
    public String getColumnName( int c ) {
        switch( c ) {
            case 0:
                return "Channel";
            case 1:
                return "Gain 1";
            case 2:
                return "Gain 2";
            case 3:
                return "Excitation [V]";
            default:
                return "???";
        }
    }   
    
    TableCellEditor getG2Editor() {

        JComboBox g2_combo = new JComboBox();
        
        

        g2_combo.addItem( new Integer( 1 ) );
        g2_combo.addItem( new Integer( 2 ) );
        g2_combo.addItem( new Integer( 4 ) );
        g2_combo.addItem( new Integer( 8 ) );
       
        return new DefaultCellEditor( g2_combo );
    }   
    
        
    TableCellEditor getG1Editor() {

        JComboBox g1_combo = new JComboBox();
        
        g1_combo.addItem( new Integer( 1 ) );
        g1_combo.addItem( new Integer( 10 ) );
        g1_combo.addItem( new Integer( 100 ) );
        g1_combo.addItem( new Integer( 1000 ) );
       
        return new DefaultCellEditor( g1_combo );
    }   
        
    TableCellEditor getV_ExEditor() {

        JComboBox v_ex_combo = new JComboBox();
        
        v_ex_combo.addItem( new Integer( 1 ) );
        v_ex_combo.addItem( new Integer( 2 ) );
        v_ex_combo.addItem( new Integer( 5 ) );
        v_ex_combo.addItem( new Integer( 10 ) );
       
        return new DefaultCellEditor( v_ex_combo );
    }   
    void setColumnModel( TableColumnModel columnModel ) {
        columnModel.getColumn( 1 ).setCellEditor( getG1Editor() );
        columnModel.getColumn( 2 ).setCellEditor( getG2Editor() );
        columnModel.getColumn( 3 ).setCellEditor( getV_ExEditor() );    
    }
        
    public void setTable( JTable table ) {    
        table.setModel ( this );
        table.setRowSelectionAllowed( false );
        table.setCellSelectionEnabled( true );                   

        setColumnModel( table.getColumnModel() );
    }
}