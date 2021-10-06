package dt100;

/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: Jun 10, 2004
 * Time: 2:42:51 PM
 * To change this template use File | Settings | File Templates.
 */
public final class Dt100StreamFrame {
/*
 * Bits def for BP_FC_SET_DIO
 * Direction 1=>Output
 */


// sample tagging

    /*
     * sample tag structure (ICD rev 6 )
     *
     *       d1111110000000000
     *        5432109876543210
     *        nTssssssdddddddd
     *        XT54321076543210
     *
     *  where nX { X 0..47 } is the SAMPLE NUMBER for subframe 0
     *  T                    Trigger
     *  s{5..0}              is the subframe number
     *  d{7..0}              DIO state (D0..7) on even subframe,
     *                                 (D15..8) on odd subframe,
     *  nX { X 48..63 } are extra bits (not used)
     */

    static final int SF0 = 0xfe;
    static final int SF1 = 0xed;

/*
 * generate saample tags ... should be good for non-simul too
 */
    static final int SUBFRAME_MASK = 0x3f;
    static final int NSUBFRAMES = 0x40;
    static final int NID_BITS = 0x30;

    static final int nX_bit = 15;
    static final int T_bit = 14;
    static final int s0_bit = 8;

/*
 * MULTI-FRAME: ICD rev 24
 */
    static final int MFX_SF0 = 0;             /* Single frame marker */
    static final int MFX_SF1 = 1;
    static final int MFX_MF2 = 2;             /* Multi frame marker */
    static final int MFX_MF3 = 3;
    static final int MFX_FNa = 4;             /* Frame number in Multi Frame {15:0} */
    static final int MFX_FNb = 5;
    static final int MFX_Ja = 6;             /* Jiffies {31:0} */
    static final int MFX_Jb = 7;
    static final int MFX_Jc = 8;
    static final int MFX_Jd = 9;
    static final int MFX_ESa = 10;            /* Event Signature {31:0} */
    static final int MFX_ESb = 11;
    static final int MFX_ESc = 12;
    static final int MFX_ESd = 13;
    static final int MFX_ESOFa = 14;            /* Event 2 Sample Offset {23:0} */
    static final int MFX_ESOFb = 15;
    static final int MFX_ESOFc = 16;
    static final int MFX_DIO = 17;
    static final int MFX_EDIOa = 18;
    static final int MFX_EDIOb = 19;
    static final int MFX_EDIOc = 20;
    static final int MFX_EDIOd = 21;
    static final int MFX_BLENa = 22;            /* Burst Length (frames) {15:0} */
    static final int MFX_BLENb = 23;
    static final int MFX_BDELa = 24;            /* Burst Delay (samples) {15:0} */
    static final int MFX_BDELb = 25;

    static final int MFX_OVER = 26;

    static final int MFX_MFNa = 28;            /* MFN (pulse number) */
    static final int MFX_MFNb = 29;
    static final int MFX_MFNc = 30;
    static final int MFX_MFNd = 31;


    static final int MF_MF2 = 0xf0;
    static final int MF_MF3 = 0x01;

    static final int MF_FILLa = 0xf1;
    static final int MF_FILLb = 0x11;


    public static boolean isMF(StreamFrame aFrame) {
        return aFrame.getDIO(MFX_MF2) == MF_MF2 && aFrame.getDIO(MFX_MF3) == MF_MF3;
    }

    public static int FN(StreamFrame aFrame) {
        return aFrame.getDIO(MFX_FNa) << 8 | aFrame.getDIO(MFX_FNb);
    }

    public static int Jiffies(StreamFrame aFrame) {
        return aFrame.getDIO(MFX_Ja) << 24 | aFrame.getDIO(MFX_Jb) << 16 |
               aFrame.getDIO(MFX_Jc) <<  8 | aFrame.getDIO(MFX_Jd);
    }

    public static int PulseNum(StreamFrame aFrame) {
        return aFrame.getDIO(MFX_MFNa) << 24 | aFrame.getDIO(MFX_MFNb) << 16 |
               aFrame.getDIO(MFX_MFNc) <<  8 | aFrame.getDIO(MFX_MFNd);
    }

    public static int Over(StreamFrame aFrame){
        return  aFrame.getDIO(MFX_OVER);
    }
    public static int BLEN(StreamFrame aFrame) {
        int blen = aFrame.getDIO(MFX_BLENa) << 8 | aFrame.getDIO(MFX_BLENb);
        return blen;
    }

}
