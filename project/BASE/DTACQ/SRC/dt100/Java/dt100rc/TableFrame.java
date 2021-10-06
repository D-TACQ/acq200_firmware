/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: TableFrame.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: TableFrame.java,v 1.15.4.1 2004/04/18 21:15:21 pgm Exp $
 * $Log: TableFrame.java,v $
 * Revision 1.15.4.1  2004/04/18 21:15:21  pgm
 * works with acq196
 *
 * Revision 1.15  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import java.text.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Iterator;
import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.table.*;

import dt100.HexString;
import fileconverter.FileConverterFactory;

class GotoSample {
    public int sample;   
    public GotoSample( int sample ) {this.sample = sample;}
}
class GotoDialog extends JDialog implements ActionListener {
    public GotoDialog( JFrame parent ) {
        super( parent, "Goto Sample", true );
        Container contentPane = getContentPane();
        JPanel p1 = new JPanel();
        p1.setLayout( new GridLayout( 1, 1 ) );
        p1.add( new JLabel( "Sample: " ) );
        p1.add( sampleText = new JTextField( "" ) );
        contentPane.add( p1 );
        
        Panel p2 = new Panel();
        okButton = addButton( p2, "Ok" );
        cancelButton = addButton( p2, "Cancel" );
        contentPane.add( "South", p2 );
        setSize( 240, 100 );
        setLocation( 10, 10 );
    }
    JButton addButton( Container c, String name ) {
        JButton button = new JButton( name );
        button.addActionListener( this );
        c.add( button );
        return button;
    }
    public void actionPerformed( ActionEvent evt ) {
        Object source = evt.getSource();
        if ( source == okButton ){
            ok = true;
            setVisible( false );
        }else if ( source == cancelButton ){
            setVisible( false );
        }
        // else forget it
    }
    public boolean showDialog( GotoSample sample ) {
        sampleText.setText( Integer.toString( sample.sample ) );
        ok = false;
        show();
        if ( ok ){
            sample.sample = Integer.valueOf( sampleText.getText() ).intValue();
        }
        return ok;
    }
    private JTextField sampleText;
    private JButton okButton;
    private JButton cancelButton;
    private boolean ok;
}

abstract class NumericTableCellRenderer extends DefaultTableCellRenderer {
    protected JLabel component = new JLabel();
    protected NumberFormat nf = NumberFormat.getInstance();

    protected void setValue(Object value) {
        setText((value == null) ? "" : value.toString());
    }
    public NumericTableCellRenderer () {
        setForeground(java.awt.Color.black);
        setFont(new Font("MonoSpaced", Font.PLAIN, 12));
        setHorizontalAlignment(SwingConstants.RIGHT);
    }
}

class FloatTableCellRenderer extends NumericTableCellRenderer {
    protected void setValue(Object value) {
        setText( nf.format(((Float)value).floatValue() ) );
    }
    public FloatTableCellRenderer() {
        nf.setMaximumFractionDigits( 3 );
        nf.setMinimumFractionDigits( 3 );
    }
}
class HexIntegerTableCellRenderer extends NumericTableCellRenderer {
    protected void setValue(Object value) {
        int cv = ((Integer)value).intValue();
        
        cv &= 0x0ffff;   // ensure unsignedness
        
        setText( "0x"+HexString.leftFill(cv, 4) );
    }
}
class IntegerTableCellRenderer extends NumericTableCellRenderer {

}
public class TableFrame extends FrameWithModel
{
    void _TableFrame() {
        JTable1.setRowSelectionAllowed( true );    
	    JTable1.getSelectionModel().setSelectionMode( 
	            ListSelectionModel.SINGLE_INTERVAL_SELECTION
		);
    }
	public TableFrame()
	{
		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		getContentPane().setLayout(new BorderLayout(0,0));
		setSize(405,305);
		setVisible(false);
		JPanel1.setLayout(new GridBagLayout());
		getContentPane().add(BorderLayout.CENTER,JPanel1);
		JPanel1.setBounds(0,0,405,305);

		JScrollPane1.setHorizontalScrollBarPolicy(javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);

		JScrollPane1.setOpaque(true);
		JPanel1.add(JScrollPane1, new com.symantec.itools.awt.GridBagConstraintsD(0,0,1,1,1.0,1.0,java.awt.GridBagConstraints.CENTER,java.awt.GridBagConstraints.BOTH,new Insets(0,0,0,0),0,0));
		JScrollPane1.setBounds(0,0,405,305);
		JScrollPane1.getViewport().add(JTable1);
		JTable1.setFont(new Font("MonoSpaced", Font.PLAIN, 12));
		JTable1.setBounds(0,0,402,0);
		//}}

		//{{INIT_MENUS
		//}}

	
		//{{REGISTER_LISTENERS
		SymMouse aSymMouse = new SymMouse();
		JTable1.addMouseListener(aSymMouse);
		//}}
		_TableFrame();
	}

	public TableFrame(String sTitle)
	{
		this();
		setLocation(50, 50);		
		setTitle(sTitle);
	}

    private void setDefaults( javax.swing.JTable table ) {
            table.setDefaultRenderer( Object.class, new DefaultTableCellRenderer() );
            table.setDefaultRenderer( Float.class, new FloatTableCellRenderer() );
            table.setDefaultRenderer( Integer.class, new HexIntegerTableCellRenderer() );
            table.getColumnModel().getColumn(0).setCellRenderer( new IntegerTableCellRenderer() );
	    table.setAutoResizeMode( JTable.AUTO_RESIZE_OFF );
    }
	public void setVisible(boolean b)
	{
        if ( b ){
            setDefaults( JTable1 );
        }
		super.setVisible(b);
	}

	static public void main(String args[])
	{
		(new TableFrame()).setVisible(true);
	}

	public void addNotify()
	{
		// Record the size of the window prior to calling parents addNotify.
		Dimension size = getSize();

		super.addNotify();

		if (frameSizeAdjusted)
			return;
		frameSizeAdjusted = true;

		// Adjust size of frame according to the insets and menu bar
		Insets insets = getInsets();
		javax.swing.JMenuBar menuBar = getRootPane().getJMenuBar();
		int menuBarHeight = 0;
		if (menuBar != null)
			menuBarHeight = menuBar.getPreferredSize().height;
		setSize(insets.left + insets.right + size.width, insets.top + insets.bottom + size.height + menuBarHeight);
	}

	// Used by addNotify
	boolean frameSizeAdjusted = false;

	//{{DECLARE_CONTROLS
	javax.swing.JPanel JPanel1 = new javax.swing.JPanel();
	javax.swing.JScrollPane JScrollPane1 = new javax.swing.JScrollPane();
	javax.swing.JTable JTable1 = new javax.swing.JTable();
	//}}

	//{{DECLARE_MENUS
	//}}

    boolean set_renderers = false;


    public void setModel( TableModel model ) {
        JTable1.setModel( model );
    }
    public void setIntegerFormatHex() {
        JTable1.setDefaultRenderer( Integer.class, new HexIntegerTableCellRenderer() );
    }
    public void setIntegerFormatDecimal() {
        JTable1.setDefaultRenderer( Integer.class, new IntegerTableCellRenderer() );
    }



	class SymMouse extends java.awt.event.MouseAdapter
	{
		public void mouseClicked(java.awt.event.MouseEvent event)
		{
			Object object = event.getSource();
			if (object == JTable1)
				JTable1_mouseClicked(event);
		}
	}

    TableModel makeSubModel( boolean zap_index_col ) {
        return new SubTableModel(
                JTable1.getModel(),
                JTable1.getSelectionModel().getMinSelectionIndex(),
                JTable1.getSelectionModel().getMaxSelectionIndex(),
                zap_index_col );
    }
    JPopupMenu initTableMenu() {
        JPopupMenu menu = new JPopupMenu( "Table Menu" );

        menu.add( new AbstractAction( "Goto Top" ) {

                public void actionPerformed( ActionEvent evt ) {
                    JScrollBar vsb = JScrollPane1.getVerticalScrollBar();
                    vsb.setValue( vsb.getMinimum() );
                    System.out.println( "Table.Top "+ vsb.getMinimum());
                }
            }
        );


        menu.add( new AbstractAction( "Goto ..." ) {
                int maxsamples() {
                    return JTable1.getModel().getRowCount();
                }
                void cursorTo( int sample ) {
                    JScrollBar vsb = JScrollPane1.getVerticalScrollBar();
                    long tval = vsb.getMaximum();
                    tval *= sample;
                    tval /= maxsamples();
                    vsb.setValue( (int)tval );
                }
	            public void actionPerformed( ActionEvent evt ) {
	                GotoDialog gd = new GotoDialog( TableFrame.this );
	                GotoSample sample = new GotoSample( maxsamples()/2 );

	                if ( gd.showDialog( sample ) ){
	                    cursorTo( sample.sample );
	                }

		            System.out.println( "Table.Goto ..." );
	            }
            }
        );
        menu.add( new AbstractAction( "Goto End" ) {
                public void actionPerformed( ActionEvent evt ) {
                    JScrollBar vsb = JScrollPane1.getVerticalScrollBar();
                    vsb.setValue( vsb.getMaximum() );
                    System.out.println( "Table.End "+ vsb.getMaximum() );
                }
            }
        );
        menu.add( new AbstractAction( "Select All" ) {
	            public void actionPerformed( ActionEvent evt ) {
	                JTable1.selectAll();
                    System.out.println( "Table.Select All" );
	            }
            }
        );
        menu.addSeparator();
        menu.add( new AbstractAction( "Save As .." ) {
	            public void actionPerformed( ActionEvent evt ) {
                    saveFileAction( JTable1 );
                }
            }
        );
        menu.add( new AbstractAction( "Save Selection As .." ) {
	            public void actionPerformed( ActionEvent evt ) {
                    javax.swing.JTable saveTable = new javax.swing.JTable();
                    saveTable.setModel( makeSubModel( true ) );
                    setDefaults( saveTable );
                    saveFileAction( saveTable );
	            }
            }
        );
        menu.addSeparator();
        menu.add( new AbstractAction( "Plot Selection ..." ) {
	            public void actionPerformed( ActionEvent evt ) {
                    PlotFrame pf = new PlotFrame();
		            pf.setTitle( "dt100 Data Sub Plot " );
		            pf.setPlotLabels(
		                "", "time",
		                true?"Volts": "count" );
                    pf.setModel( makeSubModel( true ) );
                    pf.setVisible( true );
	            }
            }
        );
        return menu;
    }

    private void saveFileAction( javax.swing.JTable table ) {
        JFileChooser saveFileDialog =  new JFileChooser();

        saveFileDialog.setCurrentDirectory( MainFrame.saveDataFile.getParentFile() );
        FileConverterFactory.addFileFilters( saveFileDialog );
        int result = saveFileDialog.showSaveDialog( this );

        if ( result == JFileChooser.APPROVE_OPTION ){
            FileConverterFactory.saveFile( table, saveFileDialog );
            MainFrame.saveDataFile = saveFileDialog.getSelectedFile();
        }
    }


    void JTable1_mouseClicked(java.awt.event.MouseEvent event)
	{
		// to do: code goes here.
	    if ( (event.getModifiers()&InputEvent.BUTTON3_MASK) != 0 ){
	           JPopupMenu menu = initTableMenu();
               menu.show( event.getComponent(), event.getX(), event.getY() );
	    }
	}
}
