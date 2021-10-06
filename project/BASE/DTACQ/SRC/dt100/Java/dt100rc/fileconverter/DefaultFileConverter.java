/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DefaultFileConverter.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DefaultFileConverter.java,v 1.3 2002/05/27 10:25:11 pgm Exp $
 * $Log: DefaultFileConverter.java,v $
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package fileconverter;

import javax.swing.JTable;
import javax.swing.filechooser.FileFilter;
import java.io.*;

public abstract class DefaultFileConverter implements FileConverter {
    protected final DefaultFileFilter ff;

    public DefaultFileConverter( DefaultFileFilter ff ) { this.ff = ff; }

    public  FileFilter getFileFilter() {
        return ff;
    }
    
    public File getFullFile( File f ) {
        return ff.getFullFile( f );   
    }
    protected abstract void writeTable( JTable table, java.io.BufferedWriter w ) throws java.io.IOException;
    
    public void saveData( JTable table, File f ) {
        try {
            File ff = getFullFile( f );
            BufferedWriter w = new BufferedWriter(new FileWriter( ff ));
            writeTable( table, w );
            w.close();
        } catch ( IOException e ) {
            System.err.println( "ERROR: failed to open file " + f.getPath() );
        }
        System.out.println( "CsvFileConverter.saveData() "+getFullFile( f ) );
    }
}
