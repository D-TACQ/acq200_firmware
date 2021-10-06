/*
 * Created by IntelliJ IDEA.
 * User: Peter Milne
 * Date: May 21, 2002
 * Time: 10:59:34 AM
 * To change template for new class use 
 * Code Style | Class Templates options (Tools | IDE Options).
 */
package fileconverter;

import javax.swing.*;
import java.io.*;

class XlsFileConverter extends DefaultFileConverter {
    private XlsFileConverter() {
        super( new DefaultFileFilter( ".xls", "MS exel (R) worksheet" ) );
    }

    private static class XlsFileConverterFactory extends FileConverterFactory {
        protected FileConverter create() {
            return new XlsFileConverter();
        }
    }
    static {
        FileConverterFactory.add( (new XlsFileConverterFactory()).create() );
    }

    protected void writeTable( JTable table, java.io.BufferedWriter w ) throws java.io.IOException {}
    
    public void saveData( JTable table, File f ) {
        System.out.println( "XlsFileConverter.saveData() "+getFullFile( f )+ "STUB" );
    }
}


