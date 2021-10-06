/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: Acq32BusProt.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Acq32BusProt.java,v 1.4 2002/05/27 14:46:25 pgm Exp $
 * $Log: Acq32BusProt.java,v $
 * Revision 1.4  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

public final class Acq32BusProt
{
    static final String setDIO = "setDIO";
    static final String getDIO = "getDIO";
    //ACQ32:getDIO=HHHHHHHHHHHHHH
    static final String getVoltsRange = "getVoltsRange";
    //ACQ32:getVoltsRange AI=-2.5V,2.5V AO=0.0V,0.0V 
    static final String getNumChannels = "getNumChannels";
    //ACQ32:getNumChannels=16
    static final String getAvailableChannels = "getAvailableChannels";
    // ACQ32:getAvailableChannels AI=12 AO=0
    static final String getSyncRoute = "getSyncRoute";
    static final String setSyncRoute = "setSyncRoute";
    
    
    static final String clearThresholds = "clearThresholds";
    static final String selectThresholdDetector = "selectThresholdDetector";
    class TD {
        static final String FAST_MULTI = "FAST_MULTI";
    };

    public static TD thresholdDetector;
    static final String setChannelThreshold = "setChannelThreshold";
    static final String setDistributor = "setDistributor";

    static final String reserveAO = "reserveAO";
    
    static final String getState = "getState";
    static final String getNumSamples = "getNumSamples";
    static final String setInternalClock = "setInternalClock";
    // setInternalClock <Hz> [DOx]
    static final String setExternalClock = "setExternalClock";
    // setExternalClock DIx [<div> DOx]
    
    static final String setModeGatedContinuous =
        "setModeGatedContinuous";
    static final String setModeTriggeredContinuous =
        "setModeTriggeredContinuous";
    static final String setMode = "setMode";
    
    static final String setArm = "setArm";
    static final String setAbort = "setAbort";
    
    static final String setSampleTaggingOn = "setSampleTagging 1";
    static final String setSampleTaggingOff= "setSampleTagging 0";
    
    static final String setChannel = "setChannel";
    
    static final String setChannelMask = "setChannelMask";
    
    static final String getFwRev = "getFwrev";
    
    static final String format = "format";
    
}

