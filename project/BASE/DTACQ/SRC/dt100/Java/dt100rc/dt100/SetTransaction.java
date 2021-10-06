/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: SetTransaction.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: SetTransaction.java,v 1.6 2002/05/27 14:46:25 pgm Exp $
 * $Log: SetTransaction.java,v $
 * Revision 1.6  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.5  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

import java.io.*;
import debug.*;

public class SetTransaction extends Transaction implements Command {
    static final String SP = " ";
    
    private String command;

    public SetTransaction( String command ) {
        super( "ACQ32:", "acqcmd" );
        this.command = command;
    }
    public void encode( PrintWriter output ) {
        String s = COMMAND_PREFIX + SP + command;
        output.println( s  );
        Logger.println( "set:" + s  );
    }
    public static SetTransaction mode( String mode, int nsamples ) {
        return new SetTransaction( 
            new String( Acq32BusProt.setMode+SP+mode+SP+nsamples ) );   
    }
    public static SetTransaction 
        modeGatedContinuous( int presamples, int postsamples ) {
            return new SetTransaction( 
                new String( Acq32BusProt.setModeGatedContinuous+SP+
                            presamples+SP+postsamples               ) );
    }
    public static SetTransaction
        modeTriggeredContinuous( int presamples, int postsamples ) {
            return new SetTransaction( 
                new String( Acq32BusProt.setModeTriggeredContinuous+SP+
                            presamples+SP+postsamples               ) );
    }
    public static SetTransaction arm() {
        return new SetTransaction( Acq32BusProt.setArm );   
    }
    public static SetTransaction abort() {
        return new SetTransaction( Acq32BusProt.setAbort );   
    }
    public static SetTransaction internalClock( int hz ) {
            // WORKTODO DIO
        return new SetTransaction( Acq32BusProt.setInternalClock+SP+hz );
    }
    public static SetTransaction internalClock( int hz, String DIx ) {
            // WORKTODO DIO
        return new SetTransaction( Acq32BusProt.setInternalClock+SP+hz+SP+DIx );
    }
    
    public static SetTransaction sampleTagging( boolean enable ) {
        return new SetTransaction( 
            new String( enable? Acq32BusProt.setSampleTaggingOn:
                                Acq32BusProt.setSampleTaggingOff ) );
    }
    public static SetTransaction dio( String dio_mask ) {
        return new SetTransaction(
            new String( Acq32BusProt.setDIO + SP + dio_mask ) );
    }
    public static SetTransaction channel( int ichannel, int g1, int g2, int v_ex ) {
        return new SetTransaction(
            new String( Acq32BusProt.setChannel + SP +
                        ichannel + SP + g1 + SP + g2 + SP + v_ex ) );
    }
    public static SetTransaction channelMask( String mask ) {
        return new SetTransaction(
            new String( Acq32BusProt.setChannelMask + SP + mask ) );
    }
    public static SetTransaction format( String format ) {
        return new SetTransaction(
            new String( Acq32BusProt.format + SP + format ) );
    }
    public static SetTransaction clearAnalogThresholds() {
        return new SetTransaction ( Acq32BusProt.clearThresholds );
    }
    public static SetTransaction setChannelThreshold( String ch, String v1_raw, String v2_raw ) {
        return new SetTransaction(
                new String( Acq32BusProt.setChannelThreshold+ SP +
                            ch + SP +
                            v1_raw + SP +
                            v2_raw ) );
    }
    public static SetTransaction selectThresholdDetector( String det ) {
        return new SetTransaction(
                new String( Acq32BusProt.selectThresholdDetector + SP + det ) );
    }
    public static SetTransaction setDistributor( String d ) {
        return new SetTransaction(
                new String( Acq32BusProt.setDistributor + SP + d ) );
    }
}
