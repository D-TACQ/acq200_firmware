/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: Board.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Board.java,v 1.11.4.7 2004/09/04 23:08:12 pgm Exp $
 * $Log: Board.java,v $
 * Revision 1.11.4.7  2004/09/04 23:08:12  pgm
 * B5219
 * really really fix tree view
 *
 * Revision 1.11.4.6  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.11.4.5  2004/08/14 20:44:42  pgm
 * B5216
 * fix crazy "Board as Singleton" bug
 *
 * Revision 1.11.4.4  2004/06/10 20:32:14  pgm
 * MF is go!
 *
 * Revision 1.11.4.3  2004/04/18 21:14:03  pgm
 * works with acq196
 *
 * Revision 1.11.4.2  2003/12/20 15:12:29  pgm
 * hack correct channel mask on ACQ200
 *
 * Revision 1.11.4.1  2003/12/19 21:32:15  pgm
 * acq200, slot addressing
 *
 * Revision 1.11  2002/04/21 19:44:36  pgm
 * Board calcs volts correctly
 *
 * Revision 1.10  2002/04/18 12:53:43  pgm
 * table format is good
 *
 * Revision 1.9  2002/04/15 13:20:43  pgm
 * excellent status behaviour - ship it quick
 *
 * Revision 1.7  2002/04/14 15:26:33  pgm
 * works with mixed board sets
 *
 * Revision 1.6  2001/06/25 21:35:22  pgm
 * avoid mask overrun with acq16
 *
 * Revision 1.5  2001/06/22 15:11:25  pgm
 * works with multi boards ship it quick
 *
 * Revision 1.4  2001/06/16 18:27:38  pgm
 * hacked in acq16 (12) settings
 *
 * Revision 1.3  2000/10/28 20:02:58  pgm
 * improved reliability, l&f, messaging
 *
 * Revision 1.2  2000/09/07 00:15:23  pgm
 * supports DVM, shows correct channel mapping
 *
 * Revision 1.1.1.1  2000/06/03 05:36:25  pgm
 * fromQ
 *
 * Revision 1.1.1.1  2000/05/28 16:06:35  pgm
 * uses packages
 *
 * Revision 1.5  2000/05/28 10:32:18  pgm
 * streaming works passibly
 *
 * Revision 1.4  2000/05/21 14:21:53  pgm
 * stream panel added. also abortive experiment with packages
 *
 * Revision 1.3  2000/04/26 04:34:46  pgm
 * new style master slave config
 *
 * Revision 1.2  2000/04/13 19:50:36  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
 \*****************************************************************************/

package dt100;

import javax.swing.tree.*;
import java.io.*;
import java.util.*;

import debug.Debug;

public class Board implements Serializable {
    protected int board;
    protected int slot;
    private int channel_count;
    private Channel[] channels;
    private Board master;
    private boolean active;
    private String model;
    private String model_string;
    private String board_string;

    private AnalogPrams AI = new AnalogPrams();
    private AnalogPrams AO = new AnalogPrams();

    public AnalogPrams getAIPrams() {
        return AI;
    }

    public AnalogPrams getAOPrams() {
        return AO;
    }

    private void initChannels() {
        if (channel_count > 32) {
            model = "acq196";   // WORKTODO!!
        }
        model_string = new String(model + "-" + channel_count);
    }

    private Board(int board, int channel_count) {
        //        this.channel_count = channel_count;
        // set a max number - make it even!
        this.channel_count = channel_count;
        this.board = board;
        this.slot = board;                      /* assume resource order */
        active = false;

        /** @@todo
         *  NB: create the channels ONCE ONLY. So make the max number - we may not need all of them,
         *  but, so what, Channels are cheap
         */
        channels = new Channel[channel_count];
        for (int ii = 0; ii + 1 < channel_count; ii += 2) {
            Channel lo = new Channel(this, ii + 1);
            Channel hi = new Channel(this, ii + 2);

            lo.setPairMember(hi);
            hi.setPairMember(lo);

            channels[ii] = lo;
            channels[ii + 1] = hi;
        }

        initChannels();
        board_string = new String("Board " + board);
        model_string = new String();
    }

    public Board(int iboard, Board master, int channel_count) {
        this(iboard, channel_count);
        this.master = master;
    }

    public void setChannelCount(String model, int nchannels) {
        this.model = model;
        channel_count = nchannels;
        initChannels();
    }

    public int setActiveChannelCount(int cc) {
        channel_count = cc;
        initChannels();
        return channel_count;
    }

    public void setMaster(Board master) {
        this.master = master;
    }

    public boolean isMaster() {
        return this == master;
    }

    public String getModel() {
        return model_string;
    }

    public String toString() {
        return board_string;
    }

    public boolean isActive() {
        return active;
    }

    public void setActive(boolean active) {
        this.active = active;
    }

    public void insertInModel(DefaultTreeModel model,
                              MutableTreeNode after) {
        DefaultMutableTreeNode board_node = new DefaultMutableTreeNode(this);

        model.insertNodeInto(board_node, after, after.getChildCount());

        if (!isActive()) return;

        /*
* WORKTODO: use an iterator
*/
        int active_channel_count =
                setActiveChannelCount(getAIPrams().available_channels);

        for (int ii = 0; ii != active_channel_count; ++ii) {
            model.insertNodeInto(new DefaultMutableTreeNode(getChannel(ii)),
                    board_node, board_node.getChildCount());
        }
    }

    public int getBoard() {
        return board;
    }

    public void setSlot(int slot) {
        this.slot = slot;
        board_string = new String("Slot " + slot);
    }

    public int getSlot() {
        return slot;
    }

    Channel getChannel(int ichannel) {
        return channels[ichannel];
    }

    public int nActiveChannels() {
        int n_active = 0;

        for (int ichannel = 0; ichannel != channel_count; ++ichannel) {
            if (!getChannel(ichannel).isMasked()) {
                n_active++;
            }
        }

        return n_active;
    }

    public int getChannelCount() {
        return channel_count;
    }

    public Channel getActiveChannel(int iactive_channel) {
        // match the i'th active channel, starting from the zero'th
        int n_active = -1;

        for (int ichannel = 0; ichannel != channel_count; ++ichannel) {
            if (!getChannel(ichannel).isMasked()) {
                if (++n_active == iactive_channel) {
                    return getChannel(ichannel);
                }
            }
        }
        return null;
    }

    public void clearChannels() {
        for (int ichan = 0; ichan != channel_count; ++ichan) {
            getChannel(ichan).clearNumSamples();
            getChannel(ichan).clearUploading();
        }
    }

    public static ResultCallback newVoltsRangeCallback(final Board board) {
        return new ResultCallback() {
            private Board cb_board = board;

            // ACQ32:getVoltsRange AI=-2.5V,2.5V AO=0.0V,0.0V
            public void run() {
                Debug.println("newVoltsRangeCallback().run " + result);

                if (result.startsWith(Acq32BusProt.getVoltsRange)) {
                    StringTokenizer tizer = new StringTokenizer(result.substring(Acq32BusProt.getVoltsRange.length()),
                            " =V,");

                    String[] tokens = new String[6];
                    int itoken = 0;

                    while (tizer.hasMoreTokens()) {
                        tokens[itoken++] = tizer.nextToken();
                    }

                    if (itoken != 6) {
                        System.out.println("VoltsRangeCallback tokenizing failed " + itoken + " " + result);
                    }
                    cb_board.AI.vmin = Float.parseFloat(tokens[1]);
                    cb_board.AI.vmax = Float.parseFloat(tokens[2]);
                    cb_board.AO.vmin = Float.parseFloat(tokens[4]);
                    cb_board.AO.vmax = Float.parseFloat(tokens[5]);
                }
            }
        };
    }

    public static ResultCallback newAvailableChannelsCallback(final Board board) {
        return new ResultCallback() {
            private Board cb_board = board;

            // ACQ32:getAvailableChannels AI=12 AO=0
            public void run() {
                System.out.println("newAvailableChannelsCallback().run " + result);
                if (result.startsWith(Acq32BusProt.getAvailableChannels)) {
                    String ps = result.substring(Acq32BusProt.getAvailableChannels.length());
                    StringTokenizer tizer = new StringTokenizer(ps, " =");

                    String ai = "", ao = "", old = "", cur;

                    while (tizer.hasMoreTokens()) {
                        cur = tizer.nextToken();
                        if (old != null) {
                            if (old.compareTo("AI") == 0) {
                                ai = cur;
                            } else if (old.compareTo("AO") == 0) {
                                ao = cur;
                            }
                        }
                        old = cur;
                    }

                    /*
                     * hack alert WARNING - set channel count
                     */

                    cb_board.AI.available_channels = Integer.parseInt(ai);
                    cb_board.setActiveChannelCount(cb_board.AI.available_channels);
                    cb_board.AO.available_channels = Integer.parseInt(ao);
                }
            }
        };
    }
}
