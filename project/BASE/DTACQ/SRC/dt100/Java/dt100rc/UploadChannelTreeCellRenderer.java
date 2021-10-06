/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: UploadChannelTreeCellRenderer.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: UploadChannelTreeCellRenderer.java,v 1.8 2002/06/07 17:44:42 pgm Exp $
 * $Log: UploadChannelTreeCellRenderer.java,v $
 * Revision 1.8  2002/06/07 17:44:42  pgm
 * get it right
 *
 * Revision 1.7  2002/06/07 16:17:35  pgm
 * use the gifs in the jar
 *
 * Revision 1.6  2002/05/27 10:25:10  pgm
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
import dt100.Channel;

import images.Images;

public class UploadChannelTreeCellRenderer extends javax.swing.tree.DefaultTreeCellRenderer
{
    static ImageIcon MASKED_ICON = Images.nochannelIcon;
    static ImageIcon ACTIVE_ICON = Images.channelIcon;
    static ImageIcon ACTIVE_04_ICON = Images.progress04;
    static ImageIcon ACTIVE_14_ICON = Images.progress14;
    static ImageIcon ACTIVE_24_ICON = Images.progress24;
    static ImageIcon ACTIVE_34_ICON = Images.progress34;
    static ImageIcon ACTIVE_44_ICON = Images.progress44;
    
    static int request_samples;
    
    public static void setRequestSamples( int request_samples ) {
        UploadChannelTreeCellRenderer.request_samples = request_samples;
    }
    public Component getTreeCellRendererComponent(JTree tree,
                                              Object value,
                                              boolean sel,
                                              boolean expanded,
                                              boolean leaf,
                                              int row,
                                              boolean hasFocus ) {
        super.getTreeCellRendererComponent( 
                    tree, value, sel, expanded, leaf, row, hasFocus );
                    
        if ( leaf ){
            DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;
            
            if ( node.getUserObject() instanceof Channel ){
                
                Channel channel = (Channel)node.getUserObject();
     
                if ( channel.isMasked() ){
                    setIcon( MASKED_ICON );
                }else if ( channel.isUploading() ){
                    int current = channel.getNumSamples();
                    int bar = request_samples/4;
                    
                    setIcon( current >= bar*4 ? ACTIVE_44_ICON:
                            current <= 0     ? ACTIVE_04_ICON:
                            current <  bar   ? ACTIVE_14_ICON:
                            current <  bar*2 ? ACTIVE_24_ICON:
                                                ACTIVE_34_ICON  );
                }else{
                    if ( request_samples > 0 ){
                        setIcon( channel.getNumSamples()>0? ACTIVE_44_ICON: ACTIVE_ICON );
                    }else{
                        setIcon( ACTIVE_ICON );
                    }
                }
            }
        }
        
        return this;
	}
}
