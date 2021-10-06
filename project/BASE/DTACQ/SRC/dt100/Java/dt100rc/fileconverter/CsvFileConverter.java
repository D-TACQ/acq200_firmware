/*
 * Created by IntelliJ IDEA.
 * User: Peter Milne
 * Date: May 21, 2002
 * Time: 4:02:14 PM
 * To change template for new class use
 * Code Style | Class Templates options (Tools | IDE Options).
 */
package fileconverter;

import javax.swing.*;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;
import java.io.*;


class CsvFileConverter extends DefaultFileConverter {

    private static final String SEPARATOR = ", ";
    private CsvFileConverter() {
        super( new DefaultFileFilter( ".csv", "comma separated variable [ASCII]" ) );
    }
    private static class CsvFileConverterFactory extends FileConverterFactory {
        protected FileConverter create() {
            return new CsvFileConverter();
        }
    }
    static {
        FileConverterFactory.add( (new CsvFileConverterFactory()).create() );
    }

    protected void writeTable( JTable table, java.io.BufferedWriter w ) throws java.io.IOException {

        TableModel tableModel = table.getModel();
        int maxrows = tableModel.getRowCount();
        int maxcols = tableModel.getColumnCount();

        // heading
        for ( int col = 0; /* col != maxcols */; /* ++col */ ){
            w.write( table.getColumnName(col) );
            if ( ++col == maxcols ){
                w.newLine();
                break;
            }else{
                w.write( SEPARATOR );
            }
        }
        
        // data
        for ( int row = 0; row != maxrows; ++row ){
            for ( int col = 0; /* col != maxcols */; /* ++col */ ){
                TableCellRenderer renderer = table.getCellRenderer( row, col );
                Object value = tableModel.getValueAt( row, col );
                w.write(( (JLabel)renderer.getTableCellRendererComponent( 
                                table, value, false, false, row, col)).getText() );
                if ( ++col == maxcols ){
                    w.newLine();
                    break;
                }else{
                    w.write( SEPARATOR );
                }
            }
        }
    }
}
