/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: ChannelModel.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ChannelModel.java,v 1.5.4.2 2004/09/04 23:08:12 pgm Exp $
 * $Log: ChannelModel.java,v $
 * Revision 1.5.4.2  2004/09/04 23:08:12  pgm
 * B5219
 * really really fix tree view
 *
 * Revision 1.5.4.1  2004/09/03 08:42:25  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.5  2002/04/14 22:24:53  pgm
 * MasterMutableTreeNode rools ok, ditto ActionListeners
 *
 * Revision 1.3  2002/04/14 15:26:33  pgm
 * works with mixed board sets
 *
 * Revision 1.2  2000/09/23 20:45:44  pgm
 * dio readout works correctly
 *
 * Revision 1.1.1.1  2000/06/03 05:36:25  pgm
 * fromQ
 *
 * Revision 1.1.1.1  2000/05/28 16:06:35  pgm
 * uses packages
 *
 * Revision 1.4  2000/05/21 14:21:53  pgm
 * stream panel added. also abortive experiment with packages
 *
 * Revision 1.3  2000/04/26 04:34:46  pgm
 * new style master slave config
 *
 * Revision 1.2  2000/04/13 19:50:37  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



import javax.swing.tree.*;
import java.util.*;

import debug.*;
import dt100.*;

class MasterMutableTreeNodeImpl extends DefaultMutableTreeNode
        implements MasterMutableTreeNode {
    public MasterMutableTreeNodeImpl(Object o) {
        super(o);
    }

    public String toString() {
        return "Master: " + getUserObject().toString();
    }
}

public class ChannelModel extends javax.swing.tree.DefaultTreeModel {
    public ChannelModel() {
        super(null);
    }

    public ChannelModel(TreeNode root, boolean asksAllowsChildren) {
        super(root, asksAllowsChildren);
    }

    private  DefaultMutableTreeNode getRootNode() {
        return (DefaultMutableTreeNode) getRoot();
    }
    private void setRootString(String rootString) {
        (getRootNode()).setUserObject(rootString);
    }

    private static ChannelModel _instance;

    public void onConfigChange(Dt100 theDt100) {
        DefaultMutableTreeNode rootNode =
                    new DefaultMutableTreeNode("default");
        setRoot(rootNode);
        _instance.setRootString(new String("Host:" + theDt100.getHostname()));
        DefaultMutableTreeNode masterNode;
        Board masterBoard = theDt100.getMaster();
        if (masterBoard != null) {
            // store in tree st label is unique, but it still shows up as a board
            masterNode = new MasterMutableTreeNodeImpl(masterBoard);

            _instance.insertNodeInto(masterNode, _instance.getRootNode(), _instance.getRootNode().getChildCount());
        } else {
            masterNode = _instance.getRootNode(); // well, no master  actually
        }
        Iterator it = theDt100.getActiveBoards().iterator();

        while (it.hasNext()) {
            Board board = (Board) (it.next());
//               System.err.println("ChannelModel.instance Board " + board + " hash " + board.hashCode());
            board.insertInModel(_instance, masterNode);
        }

        Iterator it2 = theDt100.getInactiveBoards().iterator();

        while (it2.hasNext()) {
            ((Board) (it2.next())).insertInModel(_instance, _instance.getRootNode());
        }
    }
    public static ChannelModel instance() {
        if (_instance == null) {
            _instance = new ChannelModel();
        }
        return _instance;
    }
}