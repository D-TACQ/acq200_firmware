/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: AnalogTriggerTableModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: AnalogTriggerTableModel.java,v 1.5.4.3 2004/06/01 14:35:04 pgm Exp $
 * $Log: AnalogTriggerTableModel.java,v $
 * Revision 1.5.4.3  2004/06/01 14:35:04  pgm
 * abandoned dt100gui package expt
 *
 * Revision 1.5.4.2  2004/06/01 13:45:07  pgm
 * *** empty log message ***
 *
 * Revision 1.5  2002/05/27 19:43:52  pgm
 * numeric combos R us
 *
 * Revision 1.4  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.3  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import dt100.Channel;
import dt100.Dt100;
import dt100.Board;
import dt100.AnalogTrigger;

import java.awt.*;
import java.io.Serializable;
import java.util.Iterator;
import javax.swing.*;
import javax.swing.table.*;

import resources.DtacqComboBox;
import resources.NumericComboBoxEditor;
import resources.VoltsRange;


public class AnalogTriggerTableModel extends AbstractTableModel {

    static final int COL_CHANNEL = 0;
    static final int COL_ACTIVE = 1;
    static final int COL_EDGE = 3;
    static final int COL_LEVEL = 2;
    static final int COL_GRAPH = 4;         // ambitious - WORKTODO
    static final int NCOLS = 5;

    int rowCount;
    public int getRowCount()        { return rowCount; }
    public int getColumnCount()     { return NCOLS; }

    Channel channels[];
    int ichannel;

    private void addChannel( Channel channel ) {
        channels[ichannel++] = channel;
    }
    public AnalogTriggerTableModel() {
        Iterator it = Dt100.instance().getActiveBoards().iterator();

        while( it.hasNext() ){
            Board b = (Board)it.next();
            rowCount += b.nActiveChannels();
        }

        channels = new Channel[rowCount];
        ichannel = 0;

        it = Dt100.instance().getActiveBoards().iterator();


        while( it.hasNext() ){
            dt100.Board b = (dt100.Board)it.next();
            int maxc = b.nActiveChannels();
            for ( int ic = 0; ic != maxc; ++ic ){
                addChannel( b.getActiveChannel( ic ) );
            }
        }
    }

    Boolean bf = new Boolean( false );

    public Object getValueAt( int row, int col ) {
        switch( col ) {
            case COL_CHANNEL:
                return channels[row].makeIDString();
            case COL_ACTIVE:
                return channels[row].getAnalogTrigger().isEnabled();
            case COL_EDGE:
                return channels[row].getAnalogTrigger().getP2();
            case COL_LEVEL:
                return channels[row].getAnalogTrigger().getP1();
            case COL_GRAPH:
                return bf;
            default:
                return bf;
        }
    }
    public Class getColumnClass( int col ) {
        switch( col ) {
            case COL_ACTIVE:
                return Boolean.class;
            case COL_GRAPH:
                return String.class;
            default:
                return String.class;
        }
    }
    void setAnalogCellEditor() {


    }
    public boolean isCellEditable(int rowIndex, int columnIndex){
        switch ( columnIndex ){
            case COL_ACTIVE:
                return true;
            case COL_EDGE:
            case COL_LEVEL:
                return channels[rowIndex].getAnalogTrigger() != AnalogTrigger.getNullTrigger();
            default:
                return false;
        }
    }
    TableCellEditor getLevelEditor() {

        NumericComboBoxEditor editor = new NumericComboBoxEditor( new VoltsRange( -10f, 10f ));
        DtacqComboBox level_combo = new DtacqComboBox( editor );
        level_combo.setModel( resources.UiState.instance().analogTriggerLevelComboModel );
        level_combo.setEditable( true );

        return new DefaultCellEditor( level_combo );
    }

    static final String RISING = "Rising";
    static final String FALLING= "Falling";
    static final String DEF_LEVEL = "+3.14 V";
    static final String DEF_EDGE = RISING;

    TableCellEditor getEdgeEditor() {
        JComboBox edge_combo = new JComboBox();
        edge_combo.addItem( RISING );
        edge_combo.addItem( FALLING );
        return new DefaultCellEditor( edge_combo );
    }
    public void setValueAt(Object aValue,  int row, int col ){
        System.out.println( "WORKTODO - edit analog trigger" );
        //channels[row].set( col, aValue.toString() );

        AnalogTrigger nullt = AnalogTrigger.getNullTrigger();
        AnalogTrigger current = channels[row].getAnalogTrigger();

        String level;
        String edge;

        switch( col ){
            default:
                return;
            case COL_ACTIVE:
                AnalogTrigger at = nullt;
                if ( ((Boolean)aValue).booleanValue() ){
                    at = AnalogTrigger.getEdgeTrigger( DEF_LEVEL, DEF_EDGE );
                }
                channels[row].setAnalogTrigger( at );
                fireTableDataChanged();
                return;
             case COL_EDGE:
                edge = aValue.toString();
                level = current==nullt? DEF_LEVEL: current.getP1();
                break;
             case COL_LEVEL:
                edge = current==nullt? DEF_EDGE: current.getP2();
                level = aValue.toString();

                break;
        }

        channels[row].setAnalogTrigger( AnalogTrigger.getEdgeTrigger( level, edge ) );
        fireTableDataChanged();
    }

    public String getColumnName( int c ) {
        switch( c ) {
            case COL_CHANNEL:
                return "Channel";
            case COL_ACTIVE:
                return "Active?";
            case COL_EDGE:
                return "Rising Edge";
            case COL_LEVEL:
                return "Level";
            case COL_GRAPH:
                return "Snap";
            default:
            return "????";
        }
    }

     void setColumnModel( TableColumnModel columnModel ) {
        columnModel.getColumn( COL_LEVEL ).setCellEditor( getLevelEditor() );
        columnModel.getColumn( COL_EDGE ).setCellEditor( getEdgeEditor() );
    }
    // template type of trigger. Only one type allowed.
    // WORKTODO .. accomodate levelt rigger too
    AnalogTrigger defTrigger = AnalogTrigger.getEdgeTrigger( DEF_LEVEL, DEF_EDGE );

    public void setTable( JTable table ) {
        table.setModel ( this );
        table.setRowSelectionAllowed( false );
        table.setCellSelectionEnabled( true );
        setColumnModel( table.getColumnModel() );
    }

    public Iterator getChannels() {
        return new Iterator() {
            int cursor;

            public boolean hasNext() { return cursor < rowCount; }
            public Object next()     { return channels[cursor++]; }
            public void remove()     {}
        };
    }
}
