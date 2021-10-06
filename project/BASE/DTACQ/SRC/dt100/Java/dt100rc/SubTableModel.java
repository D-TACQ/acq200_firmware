/*
 * Created by IntelliJ IDEA.
 * User: Peter Milne
 * Date: 20-May-02
 * Time: 12:25:34
 * To change template for new class use 
 * Code Style | Class Templates options (Tools | IDE Options).
 */

import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;

public class SubTableModel extends AbstractTableModel{
// adapter to existing table model.
// eliminates first (index) column
//

    protected TableModel raw_model;
    protected int start;
    protected int finish;
    protected int first_column;

    public SubTableModel( TableModel raw_model, int start, int finish, boolean zap_first_column ) {
        this.raw_model = raw_model;
        this.start = start;
        this.finish = finish;
        this.first_column = zap_first_column? 1: 0;
    }

    public int getColumnCount() {
        return raw_model.getColumnCount()-first_column;
    }
    public Class getColumnClass( int columnIndex ) {
        return raw_model.getColumnClass( columnIndex+first_column );
    }
    public int getRowCount() {
        return finish-start;
    }
    public Object getValueAt( int row, int col ) {
        return raw_model.getValueAt( row+start, col+first_column );
    }
    public String getColumnName( int col ) {
        return raw_model.getColumnName( col+first_column );
    }
}
