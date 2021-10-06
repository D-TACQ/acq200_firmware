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

class MatlabFileConverter extends DefaultFileConverter {
    private MatlabFileConverter() {
        super( new DefaultFileFilter( ".mat", "MATLAB (R) data file" ) );
    }

    private static class MyConverterFactory extends FileConverterFactory {
        protected FileConverter create() {
            return new MatlabFileConverter();
        }
    }
    static {
        FileConverterFactory.add( (new MyConverterFactory()).create() );
    }

    protected void writeTable( JTable table, java.io.BufferedWriter w ) throws java.io.IOException {}
    
    public void saveData( JTable table, File f ) {
        System.out.println( "MatlabFileConverter.saveData() "+getFullFile( f )+" STUB" );
    }
}


