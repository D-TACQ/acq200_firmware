/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DebugDialog.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DebugDialog.java,v 1.6 2002/05/27 14:46:25 pgm Exp $
 * $Log: DebugDialog.java,v $
 * Revision 1.6  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.5  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import java.awt.*;
import javax.swing.*;
import debug.*;
import resources.DtacqComboBox;
import resources.UiState;

public class DebugDialog extends javax.swing.JDialog
{
	public DebugDialog(Frame parent)
	{
		super(parent);

		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		setTitle("dt100rc Debug");
		getContentPane().setLayout(new GridBagLayout());
		setSize(405,305);
		setVisible(false);
		JPanel28.setLayout(new GridBagLayout());
		getContentPane().add(JPanel28, new com.symantec.itools.awt.GridBagConstraintsD(0,0,1,1,1.0,1.0,java.awt.GridBagConstraints.CENTER,java.awt.GridBagConstraints.BOTH,new Insets(0,0,0,0),0,0));
		JPanel28.setBounds(0,0,405,305);
		JLabel6.setText("Debug Level");
		JPanel28.add(JLabel6, new com.symantec.itools.awt.GridBagConstraintsD(0,1,1,1,0.0,0.0,java.awt.GridBagConstraints.WEST,java.awt.GridBagConstraints.NONE,new Insets(0,0,10,10),0,0));
		JLabel6.setBounds(73,144,69,15);
		commandLogChk.setText("Log Commands");
		commandLogChk.setActionCommand("Log Commands");
		JPanel28.add(commandLogChk, new com.symantec.itools.awt.GridBagConstraintsD(0,0,1,1,0.0,0.0,java.awt.GridBagConstraints.NORTHWEST,java.awt.GridBagConstraints.NONE,new Insets(10,0,10,10),0,0));
		commandLogChk.setBounds(73,106,114,23);
		thrashPlot.setText("Thrash");
		thrashPlot.setActionCommand("Thrash");
		JPanel28.add(thrashPlot, new com.symantec.itools.awt.GridBagConstraintsD(0,3,1,1,0.0,0.0,java.awt.GridBagConstraints.CENTER,java.awt.GridBagConstraints.NONE,new Insets(0,0,10,10),0,0));
		thrashPlot.setBounds(92,174,75,25);
		debugLevelCombo.setEditable(true);
		JPanel28.add(debugLevelCombo, new com.symantec.itools.awt.GridBagConstraintsD(1,1,1,1,0.0,0.0,java.awt.GridBagConstraints.CENTER,java.awt.GridBagConstraints.NONE,new Insets(0,0,10,10),0,0));
		debugLevelCombo.setBounds(197,139,124,25);
		//}}

		//{{REGISTER_LISTENERS
		SymAction lSymAction = new SymAction();
		thrashPlot.addActionListener(lSymAction);
		SymItem lSymItem = new SymItem();
		commandLogChk.addItemListener(lSymItem);
		debugLevelCombo.addItemListener(lSymItem);
		//}}
	}

	public DebugDialog( Frame parent, String title )
	{
        this( parent);
        setTitle( title );
        debugLevelCombo.setModel( UiState.instance().debuglevelModel );
	}


	public void setVisible(boolean b)
	{
		if (b)
			setLocation(50, 50);
		super.setVisible(b);
	}

	static public void main(String args[])
	{
		(new DebugDialog((Frame)null, "Test Debug Dialog")).setVisible(true);
	}

	public void addNotify()
	{
		// Record the size of the window prior to calling parents addNotify.
		Dimension size = getSize();

		super.addNotify();

		if (frameSizeAdjusted)
			return;
		frameSizeAdjusted = true;

		// Adjust size of frame according to the insets
		Insets insets = getInsets();
		setSize(insets.left + insets.right + size.width, insets.top + insets.bottom + size.height);
	}

	// Used by addNotify
	boolean frameSizeAdjusted = false;

	//{{DECLARE_CONTROLS
	javax.swing.JPanel JPanel28 = new javax.swing.JPanel();
	javax.swing.JLabel JLabel6 = new javax.swing.JLabel();
	javax.swing.JCheckBox commandLogChk = new javax.swing.JCheckBox();
	javax.swing.JButton thrashPlot = new javax.swing.JButton();
	DtacqComboBox debugLevelCombo = new DtacqComboBox();
	//}}


	class SymAction implements java.awt.event.ActionListener
	{
		public void actionPerformed(java.awt.event.ActionEvent event)
		{
			Object object = event.getSource();
			if (object == thrashPlot)
				thrashPlot_actionPerformed(event);
		}
	}

	void thrashPlot_actionPerformed(java.awt.event.ActionEvent event)
	{
	    MainFrame.thrashPlotAction();
	}

	class SymItem implements java.awt.event.ItemListener
	{
		public void itemStateChanged(java.awt.event.ItemEvent event)
		{
			Object object = event.getSource();
			if (object == commandLogChk)
				commandLogChk_itemStateChanged(event);
			else if (object == debugLevelCombo)
				debugLevelCombo_itemStateChanged(event);
		}
	}

	void commandLogChk_itemStateChanged(java.awt.event.ItemEvent event)
	{
		// to do: code goes here.
		Logger.setLevel( commandLogChk.getModel().isSelected()? 1: 0 );
	}

	void debugLevelCombo_itemStateChanged(java.awt.event.ItemEvent event)
	{
		// to do: code goes here.
		Debug.setDebugLevel( Integer.parseInt( debugLevelCombo.getSelectedItem().toString() ) );
	}
}