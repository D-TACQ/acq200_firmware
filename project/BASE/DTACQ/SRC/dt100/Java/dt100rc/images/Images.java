package images;

import java.awt.Image;
import java.awt.Toolkit;
import java.net.URL;
import javax.swing.*;

public class Images {

    static ImageIcon getImageIconFromJar( String name ){

	//        System.err.println( "getImageFromJar( "+name+" )" );
        URL url = Images.class.getResource(name);

        if ( url != null ) {
            try {
                Image image =  Toolkit.getDefaultToolkit().getImage(url);
                if ( image == null ) {
                    System.err.println("ERROR: failed to find resource "+name);
                }else{
                    return new ImageIcon( image );
                }
            }catch( Exception e ){
                System.err.println( "getDefaultToolkit( "+name+" ) FAILED "+e );
            }
        }
        name = "images/"+name;
        System.err.println( "hmm, let's try the file option "+ name );
        return new ImageIcon( name );       // maybe we're not in a jar after all!
    }
   static URL getUrlFromJar( String name ){

       //        System.err.println( "getImageFromJar( "+name+" )" );
        URL url = Images.class.getResource(name);

        if ( url != null ) {
            return url;
        }else{
            name = "images/"+name;
            try {
                url = symantec.itools.net.RelativeURL.getURL( name );
            } catch ( java.net.MalformedURLException e ){
                System.err.println( "we never gonna make this URL "+name+"\n"+e );
            }
            return url;
        }
    }
    public static final ImageIcon logo      = getImageIconFromJar( "logo.gif");
    public static final URL       logoURL   = getUrlFromJar( "logo.gif" );
    public static final ImageIcon frameIcon = getImageIconFromJar( "d-tacq-icon.gif" );

    public static final ImageIcon newIcon   = getImageIconFromJar( "new.gif" );
    public static final ImageIcon openIcon  = getImageIconFromJar( "open.gif" );
    public static final ImageIcon saveIcon  = getImageIconFromJar( "save.gif" );
    public static final ImageIcon cutIcon   = getImageIconFromJar( "cut.gif" );
    public static final ImageIcon copyIcon  = getImageIconFromJar("copy.gif" );
    public static final ImageIcon pasteIcon = getImageIconFromJar("paste.gif" );
    public static final ImageIcon aboutIcon = getImageIconFromJar("about.gif" );
    public static final ImageIcon channelIcon = getImageIconFromJar("sine.gif" );
    public static final ImageIcon nochannelIcon = getImageIconFromJar("nosine.gif" );

    public static final ImageIcon progress04 = getImageIconFromJar("progress04.gif");
    public static final ImageIcon progress14 = getImageIconFromJar("progress14.gif");
    public static final ImageIcon progress24 = getImageIconFromJar("progress24.gif");
    public static final ImageIcon progress34 = getImageIconFromJar("progress34.gif");
    public static final ImageIcon progress44 = getImageIconFromJar("progress44.gif");
    
    public static void main( String[] args ) {
        class TestFrame extends JFrame {
            public TestFrame() {
                JLabel label = new JLabel();
                label.setText( "Hello" );
                label.setIcon( logo );
                getContentPane().add( label );
            }
        }
        System.out.println( "let the games begin" );
        new Images();
        System.out.println( "all done now" );
        JFrame frame = new TestFrame();
        frame.setSize( 300, 300 );
        frame.show();
    }

}
