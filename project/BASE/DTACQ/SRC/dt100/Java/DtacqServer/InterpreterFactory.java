/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: InterpreterFactory.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: InterpreterFactory.java,v 1.2 2000/04/15 13:45:31 pgm Exp $
 * $Log: InterpreterFactory.java,v $
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



import java.io.*;



interface InterpreterFactory {
    public Interpreter create( OutputStream out );
}

