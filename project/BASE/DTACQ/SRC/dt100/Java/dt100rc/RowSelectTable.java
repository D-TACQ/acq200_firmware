/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: RowSelectTable.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: RowSelectTable.java,v 1.2 2002/05/27 10:25:10 pgm Exp $
 * $Log: RowSelectTable.java,v $
 * Revision 1.2  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import javax.swing.JTable;
import javax.swing.event.ChangeEvent;
import javax.swing.table.TableCellEditor;

public class RowSelectTable extends JTable {
    // alows multi edit selection in row
    public void editingStopped(ChangeEvent e) {
        // Take in the new value
        TableCellEditor editor = getCellEditor();
        if (editor != null) {
            Object value = editor.getCellEditorValue();

            int selected_cols[] = getSelectedColumns();

            for ( int icol = 0; icol != selected_cols.length; ++icol ){
                setValueAt(value, editingRow, selected_cols[icol] );
            }

            removeEditor();
        }
    }
}