/*
 * Created by IntelliJ IDEA.
 * User: Peter Milne
 * Date: May 23, 2002
 * Time: 3:13:05 PM
 * To change template for new class use
 * Code Style | Class Templates options (Tools | IDE Options).
 */
package dt100;

import java.io.Serializable;

abstract public class AnalogTrigger implements Serializable {
    protected Boolean enabled = new Boolean( false );

    protected AnalogTrigger() {

    }

    abstract public String getP1();
    abstract public String getP2();

    public Boolean isEnabled() { return enabled; }

    public static AnalogTrigger getEdgeTrigger( String vth, String rising ) {
        return new EdgeTrigger( vth, rising );
    }
    public static AnalogTrigger getBoundedTrigger( String b1, String b2 ) {
        return new BoundedTrigger( b1, b2 );
    }
    public static AnalogTrigger getNullTrigger() {
        return NullTrigger.instance;
    }

    protected Transaction newTransaction( Channel c, String v1, String v2 ) {
        return SetTransaction.setChannelThreshold( String.valueOf( c.channel()), v1, v2 );
    }
    protected static String rawString( Channel c, String volts ) {
        String trimmed = volts.replace( 'V', ' ' ).trim();
        return String.valueOf(  c.raw( Float.parseFloat( trimmed ) ) );
    }
    public Transaction newTransaction( Channel c ) {
        return newTransaction( c, "0", "0" );
    }
}

class NullTrigger extends AnalogTrigger {
    static String p1 = "-";
    static String p2 = "-";
    public String getP1() { return p1; }
    public String getP2() { return p2; }

    public static AnalogTrigger instance = new NullTrigger();
}

class EdgeTrigger extends AnalogTrigger {
    String vth;
    String rising;

    public EdgeTrigger( String vth, String rising ) {
        enabled = new Boolean( true );
        this.vth = vth; this.rising = rising;
    }
    public String getP1() { return vth; }
    public String getP2() { return rising; }

    public Transaction newTransaction( Channel c ) {
        if ( rising.compareToIgnoreCase( "rising") == 0 ){
            return newTransaction( c, "0", rawString( c, vth ) );
        }else{
            return newTransaction( c, rawString( c, vth ), "0" );
        }
    }
}

class BoundedTrigger extends AnalogTrigger {
    String b1;
    String b2;

    public BoundedTrigger( String b1, String b2 ) {
        enabled = new Boolean( true );
        this.b1 = b1; this.b2 = b2;
    }
    public String getP1() { return b1; };
    public String getP2() { return b2; };

    public Transaction newTransaction( Channel c ) {
        return newTransaction( c, rawString( c, b1 ), rawString( c, b2 ) );
    }
}