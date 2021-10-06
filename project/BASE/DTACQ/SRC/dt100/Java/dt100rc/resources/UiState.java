/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: UiState.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: UiState.java,v 1.7.4.3 2004/06/26 23:23:05 pgm Exp $
 * $Log: UiState.java,v $
 * Revision 1.7.4.3  2004/06/26 23:23:05  pgm
 * Build 5211 - Externalizable Rocks - fixed dodgy saved state, now it is MUCH smaller,
 * [pgm@islay dt100rc]$ ls -l dt100.saved
 * -rw-rw-r--    1 pgm      pgm         11992 Jun 27 00:07 dt100.saved
 * [pgm@islay dt100rc]$ ls -l dt100.saved
 * -rw-rw-r--    1 pgm      pgm         13531 Jun 27 00:09 dt100.saved
 * all the DtacqCombos work
 * AND it works with JDK1.5 - Funky!
 *
 * Revision 1.7.4.2  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.7.4.1  2004/05/01 13:53:16  pgm
 * add 64, 96 ch ops for streaming
 *
 * Revision 1.7  2002/06/07 18:56:49  pgm
 * remove recurring exception on poll
 *
 * Revision 1.6  2002/05/28 10:17:20  pgm
 * new choices
 *
 * Revision 1.5  2002/05/28 07:46:33  pgm
 * numeric ranges implemented
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

import java.io.*;
import java.util.Collection;

import debug.Debug;

public class UiState implements Serializable{
    // Singleton
    public DtacqComboBoxModel clockratesModel;
    public DtacqComboBoxModel hostnamesModel;
    public DtacqComboBoxModel nsamplesModel;
    public DtacqComboBoxModel debuglevelModel;
    public DtacqComboBoxModel presamplesModel;
    public DtacqComboBoxModel analogTriggerLevelComboModel;
    public DtacqComboBoxModel dvmChannelModel;
    public DtacqComboBoxModel streamSubrateModel;
    public DtacqComboBoxModel streamMaxChannelsModel;

    public static UiState instance() {
        if ( _instance == null ){
            _instance = new UiState();
        }
        return _instance;
    }

    private static UiState _instance;

    /* first value is default */
    private static final String[] hostnames = { "dtacq-dt100", "dt001" };
    private static final String[] int_clock_rates = {
        "10MHz", "6MHz", "2MHz", "1MHz",
        "250kHz", "200kHz", "100kHz", " 50kHz",
        " 20kHz", " 10kHz", "  5kHz",
        "  2kHz", "  1kHz"
    };
    private static final String[] nsamples_choices = {
        "1", "100", "1k", "10k", "100k", "1M"
    };
    private static final String[] presamples_choices = {
        "0",  "100", "1k", "10k", "100k", "1M", "max"
    };
    private static final String[] debuglevel_choices = {
        "0" , "1", "2" , "5" , "10"
    };
    private static final String[] analogLevels_choices = {
        "+10V", "+5V", "+2V", "+1V", "0V", "-1V", "-2V", "-5V", "-10V"
    };

     static String[] makeChoices( int start, int stop, int stride ) {
         // returns a linear series start .. start+n*stride .. stop
        int count;
        int len;
        int xx;

        for ( len = 0, xx = start; xx <= stop; ++len, xx += stride ){
            ;
        }
        String choices[] = new String[len];

        for ( int ii = 0; ii != len; ++ii ){
            choices[ii] = String.valueOf( start + ii*stride );
        }
        return choices;
    }
    static String[] makeLogChoices( int start, int stop ){
        // returns a log series, start .. 1x10eN, 2x10eN, 5x10eN, .. stop
        int decades;
        int xx;

        for ( decades = 1, xx = start; xx <= stop; xx *= 10, decades++ ){
            ;
        }
        String[] choices = new String[decades*3];

        int ii;

        for ( ii = 0, xx = start; ii != choices.length; ){
            choices[ii++] = String.valueOf( xx );
            choices[ii++] = String.valueOf( 2*xx );
            choices[ii++] = String.valueOf( 5*xx );
            xx *= 10;
        }
        return choices;
    }

    private static final String[] dvmChannel_choices = makeChoices( 1, 32, 1 );
    private static final String[] streamMaxChannels_choices = {
        "64", "1", "2", "4", "8", "12", "16", "32", "64", "96"
    };
    private static final String[] streamSubrate_choices = makeLogChoices( 1, 1000 );

    private void initDefaultState() {
        hostnamesModel               = new DtacqComboBoxModel( hostnames );
        clockratesModel              = new DtacqComboBoxModel( SampleCount.makeArray(int_clock_rates) );
        nsamplesModel                = new DtacqComboBoxModel( SampleCount.makeArray(nsamples_choices) );
        presamplesModel              = new DtacqComboBoxModel( SampleCount.makeArray(presamples_choices) );
        debuglevelModel              = new DtacqComboBoxModel( debuglevel_choices );
        analogTriggerLevelComboModel = new DtacqComboBoxModel( Voltage.makeArray(analogLevels_choices) );
        dvmChannelModel              = new DtacqComboBoxModel( SampleCount.makeArray(dvmChannel_choices) );
        streamMaxChannelsModel       = new DtacqComboBoxModel( SampleCount.makeArray(streamMaxChannels_choices) );
        streamSubrateModel           = new DtacqComboBoxModel( SampleCount.makeArray(streamSubrate_choices) );
    }
    private UiState() {
        initDefaultState();
    }

    // @@todo - this is TOO UGLY - but the default tried to save all of MainFrame ??? if we have to do this, at least use a collection ...
    private void writeObject(java.io.ObjectOutputStream out)
            throws IOException {
        out.writeObject(hostnamesModel);
        out.writeObject(clockratesModel);

        out.writeObject(nsamplesModel);
        out.writeObject(presamplesModel);
        out.writeObject(debuglevelModel);

        out.writeObject(analogTriggerLevelComboModel);
        out.writeObject(dvmChannelModel);

        try {
            out.writeObject(streamMaxChannelsModel);
        } catch(Exception e){
            System.err.println("ERROR failed to write streamMaxChannelsModel " + e);
        }

        try {
            out.writeObject(streamSubrateModel);
        }catch(Exception e){
            System.err.println("ERROR failed to write streamSubrateModel " + e);
        }

    }

    private void readObject(java.io.ObjectInputStream in)
            throws IOException, ClassNotFoundException {
        initDefaultState();
        hostnamesModel = (DtacqComboBoxModel)in.readObject();
        clockratesModel = (DtacqComboBoxModel)in.readObject();
        nsamplesModel= (DtacqComboBoxModel)in.readObject();
        presamplesModel = (DtacqComboBoxModel)in.readObject();
        debuglevelModel = (DtacqComboBoxModel)in.readObject();
        analogTriggerLevelComboModel =(DtacqComboBoxModel)in.readObject();
        dvmChannelModel = (DtacqComboBoxModel)in.readObject();

        try {
            streamMaxChannelsModel = (DtacqComboBoxModel)in.readObject();
        } catch(Exception e){
            System.err.println("ERROR failed to restore streamMaxChannelsModel, using default " + e);
            streamMaxChannelsModel = instance().streamMaxChannelsModel;
        }


        try {
            streamSubrateModel = (DtacqComboBoxModel)in.readObject();
        }catch(Exception e){
            System.err.println("ERROR failed to restore streamSubrateModel, using default " + e);
            streamSubrateModel = instance().streamSubrateModel;
        }
    }
    static public void main(String args[]) {
        System.out.println( "UiState Unit Test" );
        debug.Debug.setDebugLevel( 10 );

        UnitTest ut = new UnitTest();

        try {
            ut.loadState( "uistate.dat" );
            System.out.println( "load state done" );
        } catch ( Exception e ){
            System.err.println( "ERROR on load " + e );
            e.printStackTrace();
        }
        try {
            ut.saveState( "uistate.dat" );
            System.out.println( "save state done" );
        } catch ( Exception e ) {
            System.err.println( "ERROR on save " + e );
            e.printStackTrace();
        }
    }
}

    class UnitTest {
        UiState uistate = UiState.instance();

        void saveState( String path ) throws Exception {
            int isave = 0;
            try {
                ObjectOutputStream out = new ObjectOutputStream(
                        new FileOutputStream( path )
                );
                isave++;out.writeObject( uistate );
                out.close();

                Debug.println( "saveState OK, file:" + path );
            }
            catch ( Exception e ){
                System.err.println( "ERROR: failed to save setup at step "+isave+"\n" + e );
            }
        }
        void loadState( String path ) throws Exception
        {	                        // try to restore from store
            ObjectInputStream in = new ObjectInputStream(
                    new FileInputStream( path )
            );

            uistate = (UiState)in.readObject();
        }
    }

