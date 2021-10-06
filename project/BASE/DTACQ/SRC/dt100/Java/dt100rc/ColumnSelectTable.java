/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: ColumnSelectTable.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ColumnSelectTable.java,v 1.2 2002/05/27 10:25:10 pgm Exp $
 * $Log: ColumnSelectTable.java,v $
 * Revision 1.2  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/


import javax.swing.JTable;
import javax.swing.event.ChangeEvent;
import javax.swing.table.TableCellEditor;

public class ColumnSelectTable extends JTable {
    // allows multi edit selection in column

    public void editingStopped(ChangeEvent e) {
        // Take in the new value
        TableCellEditor editor = getCellEditor();
        if (editor != null) {
            Object value = editor.getCellEditorValue();

            int selected_rows[] = getSelectedRows();

            for ( int irow = 0; irow != selected_rows.length; ++irow ){
                setValueAt(value, selected_rows[irow], editingColumn);
            }
            removeEditor();
        }
    }
}
