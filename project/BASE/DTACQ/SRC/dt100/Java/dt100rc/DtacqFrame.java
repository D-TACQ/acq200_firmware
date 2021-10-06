/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DtacqFrame.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqFrame.java,v 1.3 2002/06/07 14:32:52 pgm Exp $
 * $Log: DtacqFrame.java,v $
 * Revision 1.3  2002/06/07 14:32:52  pgm
 * clean build, d-tacq logo form jar
 *
 * Revision 1.2  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import javax.swing.*;

public class DtacqFrame extends javax.swing.JFrame {
    static final ImageIcon icon = images.Images.frameIcon;
    static final String T1 = "D-Tacq dt100rc - ";
    static final String T2 = "";

    public DtacqFrame() {
            setIconImage( icon.getImage() );
    }
    public void setTitle( String title ) {
        super.setTitle( T1+title+T2 );
    }
    public void initModel() {}
}
