/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: FrameWithModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: FrameWithModel.java,v 1.7.4.5 2004/09/03 08:42:26 pgm Exp $
 * $Log: FrameWithModel.java,v $
 * Revision 1.7.4.5  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.7.4.4  2004/06/11 16:56:37  pgm
 * MF display works, but can core dump also
 *
 * Revision 1.7.4.3  2004/06/01 14:35:04  pgm
 * abandoned dt100gui package expt
 *
 * Revision 1.7.4.2  2004/06/01 13:45:07  pgm
 * *** empty log message ***
 *
 * Revision 1.7  2002/05/27 19:43:52  pgm
 * numeric combos R us
 *
 * Revision 1.6  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.5  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import dt100.StreamingCallbackManager;
import dt100.StreamingAdapter;

import javax.swing.*;
import javax.swing.table.TableModel;
import java.beans.*;
import java.awt.*;
import java.awt.event.*;

import debug.Debug;

abstract public class FrameWithModel extends DtacqFrame implements ActionListener {
    protected Timer updateTimer = new Timer( 500, null );
    boolean ok_to_update;
    // WORKTODO - detect onWindowClosing, and remover from client listenerlists

    public FrameWithModel()
	{
        this.addWindowListener( new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                if ( parent != null ){
                    parent.removeComponentListener( cl );
                }
                if ( actor != null ){
                    actor.removeActionListener( FrameWithModel.this );
                }
            }
        } );
	}

	public FrameWithModel(String title)
	{
		this();
		setTitle(title);
	}

    ComponentListener cl;
    Component parent;
    AbstractButton actor;               // WORKTODO - what if > 1 ??

    public void setParent( Component parent ) {
        this.parent = parent;
        parent.addComponentListener( cl = new ComponentAdapter() {
            public void componentHidden( ComponentEvent e ){
                hide();
            }
            public void componentShown( ComponentEvent e ){
                Debug.print("show()");
                show();
            }
        }
       );
    }
    public void connectButton( AbstractButton button ){
        actor = button;
        button.addActionListener( this );
    }
    public void actionPerformed(ActionEvent e) {
        // WORKTODO - revert ??
        onCommit();
    }

    public void clearParent() {
        parent.removeComponentListener( cl );
    }
    public void setModel( TableModel model ) {
        ;
    }

    protected void onCommit() {}
    protected void onRevert() {}

    protected void initMonitor() {
            this.addComponentListener( new ComponentAdapter() {
            public void componentShown(ComponentEvent event)
            {
                updateTimer.start();
                StreamingCallbackManager.instance().register( getStreamingAdapter() );
            }
            public void componentHidden(ComponentEvent event)
            {
                updateTimer.stop();
                StreamingCallbackManager.instance().unRegister( getStreamingAdapter() );
            }
        } );

        updateTimer.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                ok_to_update = true;
            }
        });

    }

    protected StreamingAdapter getStreamingAdapter() { return null;}
}
