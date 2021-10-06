/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: JAboutDialog.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: JAboutDialog.java,v 1.87.4.28 2004/09/21 12:30:50 pgm Exp $
 * $Log: JAboutDialog.java,v $
 * Revision 1.87.4.28  2004/09/21 12:30:50  pgm
 * B5221
 * Supports host:port notation - useful for ssh tunnels to localhost
 *
 * Revision 1.87.4.27  2004/09/13 16:46:39  pgm
 * fixes striding
 *
 * Revision 1.87.4.26  2004/09/04 23:08:12  pgm
 * B5219
 * really really fix tree view
 *
 * Revision 1.87.4.25  2004/09/03 08:56:28  pgm
 * B5218
 * Refuse to upload if only masked channels selected
 *
 * Revision 1.87.4.24  2004/09/03 08:42:51  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.87.4.23  2004/08/16 19:41:22  pgm
 * mkreltag
 *
 * Revision 1.87.4.22  2004/08/14 20:44:42  pgm
 * B5216
 * fix crazy "Board as Singleton" bug
 *
 * Revision 1.87.4.21  2004/08/09 08:30:35  pgm
 * view data set remembers state between shots
 *
 * Revision 1.87.4.20  2004/07/01 13:24:43  pgm
 * mkreltag
 *
 * Revision 1.87.4.19  2004/06/30 20:03:58  pgm
 * mkreltag
 *
 * Revision 1.87.4.18  2004/06/27 23:16:52  pgm
 * B5214 live updates work (with a very very long path name)
 *
 * Revision 1.87.4.17  2004/06/27 20:53:41  pgm
 * B5213 waitfor <jiffies> job
 *
 * Revision 1.87.4.16  2004/06/27 20:41:10  pgm
 * B5212 StreamingControlPanel complete and replaced old stuff in MainFrame. Serializable? not yet - try another day
 *
 * Revision 1.87.4.15  2004/06/26 23:23:05  pgm
 * Build 5211 - Externalizable Rocks - fixed dodgy saved state, now it is MUCH smaller,
 * [pgm@islay dt100rc]$ ls -l dt100.saved
 * -rw-rw-r--    1 pgm      pgm         11992 Jun 27 00:07 dt100.saved
 * [pgm@islay dt100rc]$ ls -l dt100.saved
 * -rw-rw-r--    1 pgm      pgm         13531 Jun 27 00:09 dt100.saved
 * all the DtacqCombos work
 * AND it works with JDK1.5 - Funky!
 *
 * Revision 1.87.4.14  2004/06/26 22:57:24  pgm
 * Build 5210 MF stats look good.
 * Teed up for previous second control output.
 * (see DtacqClient.connectRaw()).
 *
 * Revision 1.87.4.13  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.87.4.12  2004/06/22 20:46:17  pgm
 * only run StreamCallbacks on Frame Accept.
 *
 * Revision 1.87.4.11  2004/06/11 10:03:59  pgm
 * C_BUILD_NUMBER = "B5203";
 *
 * Revision 1.87.4.10  2004/06/01 13:06:51  pgm
 * view and saveDataRange start and stride work.
 *
 * Revision 1.87.4.9  2004/05/30 22:16:04  pgm
 * saves large memory lengths
 *
 * Revision 1.87.4.8  2004/05/08 20:39:03  pgm
 * *** empty log message ***
 *
 * Revision 1.87.4.7  2004/05/05 19:40:38  pgm
 * B5106 - with limited threads
 *
 * Revision 1.87.4.6  2004/05/01 13:55:51  pgm
 * streaming with acq196
 *
 * Revision 1.87.4.5  2004/04/18 21:14:28  pgm
 * works with acq196
 *
 * Revision 1.87.4.4  2003/12/28 19:52:44  pgm
 * mkreltag
 *
 * Revision 1.87.4.3  2003/12/20 17:45:41  pgm
 * mkreltag
 *
 * Revision 1.87.4.2  2003/12/19 21:51:09  pgm
 * mkreltag
 *
 * Revision 1.87.4.1  2003/12/19 21:49:29  pgm
 * mkreltag
 *
 * Revision 1.87  2003/03/05 17:09:22  pgm
 * mkreltag
 *
 * Revision 1.86  2002/11/15 16:49:48  pgm
 * mkreltag
 *
 * Revision 1.85  2002/11/15 16:34:08  pgm
 * mkreltag
 *
 * Revision 1.84  2002/11/14 12:45:11  pgm
 * mkreltag
 *
 * Revision 1.83  2002/11/09 19:47:11  pgm
 * jfreechart 0.9.4
 *
 * Revision 1.82  2002/08/29 21:30:08  pgm
 * mkreltag
 *
 * Revision 1.81  2002/06/13 20:03:32  pgm
 * mkreltag
 *
 * Revision 1.80  2002/06/13 20:03:11  pgm
 * mkreltag
 *
 * Revision 1.79  2002/06/11 20:14:34  pgm
 * sub channel streaming
 *
 * Revision 1.78  2002/06/07 19:20:15  pgm
 * mkreltag
 *
 * Revision 1.77  2002/06/07 18:56:49  pgm
 * remove recurring exception on poll
 *
 * Revision 1.76  2002/06/07 17:07:54  pgm
 * mkreltag
 *
 * Revision 1.75  2002/06/07 14:36:54  pgm
 * mkreltag
 *
 * Revision 1.74  2002/06/07 14:34:00  pgm
 * B5100
 *
 * Revision 1.73  2002/06/06 18:26:45  pgm
 * mkreltag
 *
 * Revision 1.72  2002/06/06 14:07:41  pgm
 * mkreltag
 *
 * Revision 1.71  2002/06/06 07:20:02  pgm
 * mkreltag
 *
 * Revision 1.70  2002/05/31 07:59:46  pgm
 * mkreltag
 *
 * Revision 1.69  2002/05/28 10:18:51  pgm
 * *** empty log message ***
 *
 * Revision 1.68  2002/05/27 19:43:52  pgm
 * numeric combos R us
 *
 * Revision 1.67  2002/05/26 11:52:10  pgm
 * good stream frame hiding behaviour
 *
 * Revision 1.66  2002/05/24 12:35:02  pgm
 * B5000 - new look stub
 *
 * Revision 1.65  2002/05/22 17:13:24  pgm
 * B4007 tabs for trig, wavegen
 *
 * Revision 1.64  2002/05/22 09:04:24  pgm
 * *** empty log message ***
 *
 * Revision 1.63  2002/05/20 07:31:36  pgm
 * column class care
 *
 * Revision 1.62  2002/05/19 17:25:30  pgm
 * streaming with volts
 *
 * Revision 1.61  2002/05/19 09:16:08  pgm
 * dvm rides again
 *
 * Revision 1.60  2002/05/16 08:16:58  pgm
 * frig SOFT_CONT
 *
 * Revision 1.59  2002/05/15 05:26:32  pgm
 * *** empty log message ***
 *
 * Revision 1.58  2002/04/20 13:55:25  pgm
 * mkreltag
 *
 * Revision 1.57  2002/04/20 13:54:01  pgm
 * mkreltag
 *
 * Revision 1.56  2002/04/16 14:05:46  pgm
 * resource handling rocks
 *
 * Revision 1.55  2001/09/07 12:23:22  pgm
 * mkreltag
 *
 * Revision 1.54  2001/09/07 12:22:39  pgm
 * mkreltag
 *
 * Revision 1.53  2001/08/17 07:25:43  pgm
 * mkreltag
 *
 * Revision 1.52  2001/08/17 07:25:10  pgm
 * mkreltag
 *
 * Revision 1.51  2001/07/23 20:34:53  pgm
 * mkreltag
 *
 * Revision 1.50  2001/06/29 11:23:36  pgm
 * mkreltag
 *
 * Revision 1.49  2001/06/29 11:19:43  pgm
 * mkreltag
 *
 * Revision 1.48  2001/06/26 18:47:41  pgm
 * mkreltag
 *
 * Revision 1.47  2001/06/26 18:47:15  pgm
 * mkreltag
 *
 * Revision 1.46  2001/06/26 18:46:42  pgm
 * mkreltag
 *
 * Revision 1.45  2001/06/23 11:51:26  pgm
 * mkreltag
 *
 * Revision 1.44  2001/06/23 11:14:38  pgm
 * mkreltag
 *
 * Revision 1.43  2001/06/23 11:14:23  pgm
 * *** empty log message ***
 *
 * Revision 1.42  2001/06/22 22:15:52  pgm
 * works
 *
 * Revision 1.40  2001/05/15 18:50:10  pgm
 * mkreltag
 *
 * Revision 1.39  2001/05/06 19:43:23  pgm
 * mkreltag
 *
 * Revision 1.38  2001/05/03 20:28:34  pgm
 * mkreltag
 *
 * Revision 1.37  2001/05/03 19:40:01  pgm
 * add license info
 *
 * Revision 1.36  2001/04/27 20:25:38  pgm
 * mkreltag
 *
 * Revision 1.35  2001/04/16 16:28:06  pgm
 * mkreltag
 *
 * Revision 1.34  2001/04/10 20:02:52  pgm
 * mkreltag
 *
 * Revision 1.33  2001/02/24 21:13:14  pgm
 * mkreltag
 *
 * Revision 1.32  2001/02/24 21:12:24  pgm
 * mkreltag
 *
 * Revision 1.31  2001/01/02 14:23:45  pgm
 * mkreltag
 *
 * Revision 1.30  2001/01/02 14:21:29  pgm
 * mkreltag
 *
 * Revision 1.29  2001/01/02 14:20:09  pgm
 * mkreltag
 *
 * Revision 1.28  2000/12/09 13:17:51  pgm
 * mkreltag
 *
 * Revision 1.27  2000/11/12 17:38:27  pgm
 * mkreltag
 *
 * Revision 1.26  2000/11/12 17:38:05  pgm
 * dma release, works well
 *
 * Revision 1.25  2000/10/29 12:33:05  pgm
 * mkreltag
 *
 * Revision 1.24  2000/10/29 12:32:42  pgm
 * mkreltag
 *
 * Revision 1.23  2000/10/29 12:29:27  pgm
 * mkreltag
 *
 * Revision 1.22  2000/10/29 12:16:57  pgm
 * experimental
 *
 * Revision 1.21  2000/10/01 21:41:54  pgm
 * mkreltag
 *
 * Revision 1.20  2000/09/30 13:01:12  pgm
 * pre, post samples opts available
 *
 * Revision 1.19  2000/09/23 20:46:06  pgm
 * mkreltag
 *
 * Revision 1.18  2000/09/23 20:45:44  pgm
 * dio readout works correctly
 *
 * Revision 1.17  2000/09/23 14:30:59  pgm
 * mkreltag
 *
 * Revision 1.16  2000/09/23 14:30:55  pgm
 * correct format reltag
 *
 * Revision 1.15  2000/09/23 13:40:29  pgm
 * dio readback, streaming stop
 *
 * Revision 1.14  2000/09/20 22:34:05  pgm
 * streaming save to files done
 *
 * Revision 1.13  2000/09/18 13:29:19  pgm
 * mkreltag
 *
 * Revision 1.12  2000/09/18 13:27:07  pgm
 * mkreltag
 *
 * Revision 1.11  2000/08/23 20:43:27  pgm
 * mkreltag
 *
 * Revision 1.10  2000/08/23 20:33:21  pgm
 * mkreltag
 *
 * Revision 1.9  2000/08/23 20:31:50  pgm
 * mkreltag
 *
 * Revision 1.8  2000/07/29 16:23:29  pgm
 * mkreltag
 *
 * Revision 1.7  2000/06/30 21:25:12  pgm
 * Soft Continuous added
 *
 * Revision 1.6  2000/06/04 14:00:58  pgm
 * mkreltag
 *
 * Revision 1.5  2000/06/04 11:08:09  pgm
 * mkreltag
 *
 * Revision 1.4  2000/06/04 11:06:28  pgm
 * mkreltag
 *
 * Revision 1.3  2000/06/03 19:37:27  pgm
 * persistence works in most departments
 *
 * Revision 1.2  2000/06/03 08:44:13  pgm
 * SC ops (load,store,commit,edit) work
 *
 * Revision 1.1.1.1  2000/06/03 05:36:25  pgm
 * fromQ
 *
 * Revision 1.1.1.1  2000/05/28 16:06:35  pgm
 * uses packages
 *
 * Revision 1.10  2000/05/20 21:03:05  pgm
 * GridBag is good. Status resize, Clock gui etc
 *
 * Revision 1.9  2000/05/15 19:55:42  pgm
 * updated from harris
 *
 * Revision 1.8  2000/04/26 04:34:46  pgm
 * new style master slave config
 *
 * Revision 1.7  2000/04/24 20:15:57  pgm
 * getBoards, also better resize using GridBagLayout
 *
 * Revision 1.6  2000/04/18 16:48:27  pgm
 * multithreading, first stab
 *
 * Revision 1.5  2000/04/17 17:30:41  pgm
 * table and chart plot OK
 *
 * Revision 1.4  2000/04/16 10:59:23  pgm
 * data upload works!
 *
 * Revision 1.3  2000/04/15 17:42:01  pgm
 * sliders work from form designer
 *
 * Revision 1.2  2000/04/13 19:50:37  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



import java.awt.*;
import javax.swing.*;

/**
 * A basic implementation of the JDialog class.
 */
public class JAboutDialog extends javax.swing.JDialog
{
    public static final String
	DTACQ_RELEASE_STRING = "dt100rc_acq32_10408162041_01_75";
	public static final String
	RC_BUILD_NUMBER = "B5221";
	
	public JAboutDialog(Frame parentFrame)
	{
		super(parentFrame);
		setModal(true);
		setTitle("dt100rc - dt100 remote control");
		getContentPane().setLayout(new GridBagLayout());
		setSize(600,400);
		setVisible(false);
		okButton.setText("OK");
		okButton.setOpaque(false);
		okButton.setActionCommand("OK");
		okButton.setMnemonic((int)'O');
		getContentPane().add(okButton, new com.symantec.itools.awt.GridBagConstraintsD(2,1,1,1,0.0,0.0,java.awt.GridBagConstraints.CENTER,java.awt.GridBagConstraints.NONE,new Insets(0,0,10,0),0,0));
		okButton.setBounds(98,59,51,25);
		//		aboutLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
		aboutLabel.setText( "dt100rc - the dt100 remote control\n" );
		aboutLabel.append(  "version:" + DTACQ_RELEASE_STRING + "\n\n" ); 
		aboutLabel.append(  "build #:" + RC_BUILD_NUMBER + "\n\n" );
		aboutLabel.append(
"    Copyright (C) 2004 D-TACQ Solutions Ltd www.d-tacq.co.uk\n\n"+
"\n"+
"    This program is free software; you can redistribute it and/or modify\n"+
"    it under the terms of the GNU General Public License as published by\n"+
"    the Free Software Foundation; either version 2 of the License, or\n"+
"    (at your option) any later version.\n"+
"\n"+
"    This program is distributed in the hope that it will be useful,\n"+
"    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"+
"    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"+
"    GNU General Public License for more details.\n"+
"\n"+
"    You should have received a copy of the GNU General Public License\n"+
"    along with this program; if not, write to the Free Software\n"+
"    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA " );
		aboutLabel.setAutoscrolls(true);
		getContentPane().add(aboutLabel, new com.symantec.itools.awt.GridBagConstraintsD(0,0,3,1,1.0,1.0,java.awt.GridBagConstraints.CENTER,java.awt.GridBagConstraints.BOTH,new Insets(0,0,0,0),0,0));
		aboutLabel.setBounds(0,0,248,59);
		aboutLabel.setEditable( false );
		//}}

		//{{REGISTER_LISTENERS
		SymWindow aSymWindow = new SymWindow();
		this.addWindowListener(aSymWindow);
		SymAction lSymAction = new SymAction();
		okButton.addActionListener(lSymAction);
		//}}
	}

	public void setVisible(boolean b)
	{
	    if (b)
	    {
    		Rectangle bounds = (getParent()).getBounds();
    		Dimension size = getSize();
    		setLocation(bounds.x + (bounds.width - size.width)/2,
    			        bounds.y + (bounds.height - size.height)/2);
	    }

		super.setVisible(b);
	}

	public void addNotify()
	{
		// Record the size of the window prior to calling parents addNotify.
		Dimension d = getSize();

		super.addNotify();

		if (fComponentsAdjusted)
			return;
		// Adjust components according to the insets
		Insets insets = getInsets();
		setSize(insets.left + insets.right + d.width, insets.top + insets.bottom + d.height);
		Component components[] = getContentPane().getComponents();
		for (int i = 0; i < components.length; i++)
		{
			Point p = components[i].getLocation();
			p.translate(insets.left, insets.top);
			components[i].setLocation(p);
		}
		fComponentsAdjusted = true;
	}

	// Used for addNotify check.
	boolean fComponentsAdjusted = false;

	//{{DECLARE_CONTROLS
	javax.swing.JButton okButton = new javax.swing.JButton();
	javax.swing.JTextArea aboutLabel = new javax.swing.JTextArea();
	//}}

	class SymWindow extends java.awt.event.WindowAdapter
	{
		public void windowClosing(java.awt.event.WindowEvent event)
		{
			Object object = event.getSource();
			if (object == JAboutDialog.this)
				jAboutDialog_windowClosing(event);
		}
	}

	void jAboutDialog_windowClosing(java.awt.event.WindowEvent event)
	{
		// to do: code goes here.
			 
		jAboutDialog_windowClosing_Interaction1(event);
	}

	void jAboutDialog_windowClosing_Interaction1(java.awt.event.WindowEvent event) {
		try {
			// JAboutDialog Hide the JAboutDialog
			this.setVisible(false);
		} catch (Exception e) {
		}
	}
	
	class SymAction implements java.awt.event.ActionListener
	{
		public void actionPerformed(java.awt.event.ActionEvent event)
		{
			Object object = event.getSource();
			if (object == okButton)
				okButton_actionPerformed(event);
		}
	}

	void okButton_actionPerformed(java.awt.event.ActionEvent event)
	{
		// to do: code goes here.
			 
		okButton_actionPerformed_Interaction1(event);
	}

	void okButton_actionPerformed_Interaction1(java.awt.event.ActionEvent event) {
		try {
			// JAboutDialog Hide the JAboutDialog
			this.setVisible(false);
		} catch (Exception e) {
		}
	}
}
