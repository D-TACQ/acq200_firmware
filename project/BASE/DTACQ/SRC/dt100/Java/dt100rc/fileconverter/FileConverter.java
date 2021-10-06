/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: FileConverter.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: FileConverter.java,v 1.2 2002/05/27 10:25:11 pgm Exp $
 * $Log: FileConverter.java,v $
 * Revision 1.2  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package fileconverter;

import javax.swing.filechooser.FileFilter;
import javax.swing.JTable;
import java.io.File;

public interface FileConverter {
    public  FileFilter getFileFilter();
    public void saveData( JTable table, File f );
}
