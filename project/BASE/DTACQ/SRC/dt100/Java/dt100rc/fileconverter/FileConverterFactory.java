/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: FileConverterFactory.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: FileConverterFactory.java,v 1.3 2002/05/27 10:25:11 pgm Exp $
 * $Log: FileConverterFactory.java,v $
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package fileconverter;

import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.io.File;

public abstract class FileConverterFactory {
    // factory class, must subclass to add more coverters
    protected abstract FileConverter create();

    private static List converters;

    public static List getConverters() {
        return converters;
    }
    private static void loadClass( String name ) {
        try {
            Class.forName( "fileconverter."+name );
        } catch (ClassNotFoundException e){
            System.out.println( "WARNING:FileConverter class " + name + " not found" );
        }
    }
    private static void init() {
        if ( converters == null ){
            converters = new ArrayList();
            loadClass( "CsvFileConverter" );
            loadClass( "XlsFileConverter" );
            loadClass( "FamosFileConverter" );
            loadClass( "MatlabFileConverter" );
            loadClass( "RawFileConverter" );
        }
    }
    public static void addFileFilters( JFileChooser jfc ) {
        init();

        Iterator it = FileConverterFactory.getConverters().iterator();

        while( it.hasNext() ){
            jfc.addChoosableFileFilter( ((FileConverter)it.next()).getFileFilter() );
        }
    }
    static void add( FileConverter converter ){
        converters.add( converter );
    }

    public static void saveFile( JTable table, JFileChooser jfc ) {
        Iterator it = getConverters().iterator();

        while( it.hasNext() ){
            FileConverter fc = (FileConverter)it.next();
            if ( fc.getFileFilter() == jfc.getFileFilter() ||
                 fc.getFileFilter().accept( jfc.getSelectedFile()) ){
                fc.saveData( table, jfc.getSelectedFile() );
                return;
            }
        }

        System.out.println( "WARNING: file "+jfc.getSelectedFile()+" not saved" );
    }
}



class RawFileConverter extends DefaultFileConverter {
    private RawFileConverter() {
        super( new DefaultFileFilter( ".raw", "raw binary" ) );
    }

    private static class RawFileConverterFactory extends FileConverterFactory {
        protected FileConverter create() {
            return new RawFileConverter();
        }
    }
    static {
        FileConverterFactory.add( (new RawFileConverterFactory()).create() );
    }

    protected void writeTable( JTable table, java.io.BufferedWriter w ) throws java.io.IOException {}

    public void saveData( JTable table, File f ) {
        System.out.println( "RawFileConverter.saveData() "+((DefaultFileFilter)getFileFilter()).getFullFile( f )+ "STUB" );
    }
}


