/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DtacqComboBox.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqComboBox.java,v 1.5 2002/05/28 10:16:01 pgm Exp $
 * $Log: DtacqComboBox.java,v $
 * Revision 1.5  2002/05/28 10:16:01  pgm
 * allows JComboBox refs to get special model effect
 *
 * Revision 1.4  2002/05/27 19:40:16  pgm
 * numeric combos R us
 *
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package resources;

import javax.swing.*;
import javax.swing.text.Keymap;
import java.awt.event.*;
import java.io.*;

public class DtacqComboBox extends JComboBox implements Serializable {



    static {
        JTextField f = new JTextField();
        KeyStroke enter = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0);
        Keymap map = f.getKeymap();
        map.removeKeyStrokeBinding(enter);
    }

    private void addEditElementToList() {
        Object newItem =  getEditor().getItem();
        ((DtacqComboBoxModel)getModel()).addElement( newItem, false );
        getModel().setSelectedItem( newItem );
    }
    private void init() {
        setEditable( true );
        this.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e ) {

            }
        } );
        getEditor().getEditorComponent().addFocusListener( new FocusAdapter() {
            public void focusLost(FocusEvent e) {
                // add the element to the list on onFocus lost.
                // doesn't matter if it's already there, underneath it's a set!
                addEditElementToList();
                debug.Debug.println( "Focus Lost : "+getEditor().getItem() );
            }
        } );
        getEditor().addActionListener( new EditComboActionListener( this ) );
    }
    public DtacqComboBox() {
        init();
    }
    public DtacqComboBox( ComboBoxEditor editor ){
        setEditor( editor );
        init();
    }
    class EditComboActionListener implements ActionListener {
        JComboBox cb;

        EditComboActionListener( JComboBox cb ) {
            this.cb = cb;
        }
        public void actionPerformed(ActionEvent e) {
            addEditElementToList();
            debug.Debug.println( "EditComboActionListener.actionPerformed "+getEditor().getItem());
            //           combo_box.setSelectedIndex( 0 );
        }
    }
    /** override JComboBoxModel.setModel() to allow special behaviour for DtacqComboBoxModel case
     *  (allows selection to be persistent)
     */
    public void setModel(ComboBoxModel aModel) {
        if ( aModel instanceof DtacqComboBoxModel ){
            setModel( (DtacqComboBoxModel)aModel );
        }else{
            super.setModel( aModel );
        }
    }
    public void setModel( DtacqComboBoxModel aModel ){
        try {
            Object selectedItem = aModel.getSelectedItem();
            super.setModel( aModel );
            setSelectedItem( selectedItem );
        } catch( NullPointerException e ){
            System.err.println( "DtacqComboBox.setModel() NulLPointerException "+e );
            e.printStackTrace();
        }
    }

    public Object getSelectedItem() {
        Object object = super.getSelectedItem();
        if ( object == null ){
            object = "";
        }
        return object;
    }

    static final String[] tests = {
        "cat", "dog", "rat", "rat", "zebra", "aardvark"
    };

    public static void main( String[] args ) {

        System.out.println( "DtacqComboBox Unit Test" );
        debug.Debug.setDebugLevel(1);

        class TestFrame extends JFrame {


            DtacqComboBox cb = new DtacqComboBox();
            DtacqComboBox ncb = new DtacqComboBox( new NumericComboBoxEditor( new VoltsRange(-10f,10f)));

            DtacqComboBoxModel model = new DtacqComboBoxModel( tests );
            DtacqComboBoxModel nmodel = new DtacqComboBoxModel();
            JTextField currentValue;

            private final String path = "dtacqcomboboxtest.state";

            class LoadAction extends AbstractAction {
                public LoadAction() {
                    putValue( Action.NAME, "Load" );
                }
                public void actionPerformed( ActionEvent e ){
                    System.out.println( "Load from "+path );
                    try {
                        ObjectInputStream in = new ObjectInputStream(
                                new FileInputStream( path )
                        );
                        model = (DtacqComboBoxModel)in.readObject();
                        nmodel = (DtacqComboBoxModel)in.readObject();
                    } catch ( Exception ee ) {
                        System.out.println( "ERROR: failed to load setup "+ee );
                        ee.printStackTrace();
                        model = new DtacqComboBoxModel( tests );
                    }

                    cb.setModel( model );
                    ncb.setModel( nmodel );
                }
            }
            class SaveAction extends AbstractAction {
                public SaveAction() {
                    putValue( Action.NAME, "Save" );
                }
                public void actionPerformed( ActionEvent e ){
                    System.out.println( "Save to "+path );
                    try {
                        ObjectOutputStream out = new ObjectOutputStream(
                                new FileOutputStream( path )
                        );
                        out.writeObject( model );
                        out.writeObject( nmodel );
                    } catch ( Exception ee ) {
                        System.out.println( "ERROR: failed to save setup "+ee );
                        ee.printStackTrace();
                    }
                }
            }
            public TestFrame( DtacqComboBox _cb ) {
                cb = _cb;
                addWindowListener( new WindowAdapter() {
                    public void windowClosing( WindowEvent e ) {
                        System.exit( 0 );
                    }
                } );
                setTitle( "DtacqComboBox test frame" );
                setSize( 200, 200 );
                JPanel p = new JPanel();
                getContentPane().add( p, "Center" );
                p.add( cb );
                p.add( new ActionButton( new LoadAction() ) );
                p.add( new ActionButton( new SaveAction() ) );
                p.add( new JTextField( "Default Input", 20 ) );
                p.add( ncb );
                ncb.setModel( nmodel );

                currentValue = new JTextField( "current value", 20 );
                currentValue.setEditable( false );
                p.add( currentValue );

                cb.addActionListener( new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        String newText;

                        if ( cb.getSelectedItem() != null ){
                            newText = cb.getSelectedItem().toString();
                        }else{
                            newText = "null";
                        }

                        currentValue.setText( newText.toString() );
                    }
                } );

                cb.setModel( model );
            }

        }

        JFrame frame = new TestFrame( new DtacqComboBox() );
        frame.show();
    }
}

class ActionButton extends JButton {
    public ActionButton( Action a ) {
        setText( (String)a.getValue(Action.NAME) );
        Icon icon = (Icon)a.getValue(Action.SMALL_ICON);
        if ( icon != null ){
            setIcon( icon );
        }
        addActionListener( a );
    }
}