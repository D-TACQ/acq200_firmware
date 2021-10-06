/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: ChannelTreeCellRenderer.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ChannelTreeCellRenderer.java,v 1.8 2002/05/28 19:39:37 pgm Exp $
 * $Log: ChannelTreeCellRenderer.java,v $
 * Revision 1.8  2002/05/28 19:39:37  pgm
 * uses images.Images to get pix
 *
 * Revision 1.7  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import java.beans.*;
import java.awt.*;
import javax.swing.ImageIcon;

import debug.Debug;
import dt100.Board;
import dt100.Channel;

public class ChannelTreeCellRenderer extends javax.swing.tree.DefaultTreeCellRenderer
{
    static final ImageIcon MASKED_ICON = images.Images.nochannelIcon;
    static final ImageIcon ACTIVE_ICON = images.Images.channelIcon;
    
    public Component getTreeCellRendererComponent(JTree tree,
                                              Object value,
                                              boolean sel,
                                              boolean expanded,
                                              boolean leaf,
                                              int row,
                                              boolean hasFocus ) {
        super.getTreeCellRendererComponent( 
                    tree, value, sel, expanded, leaf, row, hasFocus );
                    
        try {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;
		    Object user_object = node.getUserObject();
			
			if ( value instanceof MasterMutableTreeNode ){
			    ; // bold text ??
			}else if ( user_object instanceof Channel ){
			    Channel channel = (Channel)user_object;			
                setIcon( channel.isMasked()?MASKED_ICON: ACTIVE_ICON );
			}else if ( user_object instanceof Board ){
			    Board board = (Board)user_object;
			    this.setText( board.toString() + ": " + board.getModel() );
		    }    
        } catch ( java.lang.ClassCastException e ) {
            Debug.println( 2, "node was not a leaf! " + e );
            ; // don't cast boards ... cue to add board icon here ??   
        }
        
        return this;
	}
}
