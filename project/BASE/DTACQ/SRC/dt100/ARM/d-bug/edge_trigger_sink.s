;/*****************************************************************************
; *
; * File: edge_trigger_sink.s
; *
; * $RCSfile: edge_trigger_sink.s,v $
; * 
; * Copyright (C) 2001 D-TACQ Solutions Ltd
; * not to be used without owner's permission
; *
; * Description:
; *
; * $Id: edge_trigger_sink.s,v 1.26 2002/07/24 18:07:41 pgm Exp $
; * $Log: edge_trigger_sink.s,v $
; * Revision 1.26  2002/07/24 18:07:41  pgm
; * acq16 trigger offset comp - use 1st TW
; *
; * Revision 1.25  2002/03/30 14:38:38  pgm
; * lowest poss latency ext trig
; *
; * Revision 1.24  2002/03/21 11:54:51  pgm
; * untabify
; *
; * Revision 1.23  2002/03/13 21:43:09  pgm
; * correct formatting with emacs (Stroustrup)
; *
; * Revision 1.22  2002/03/08 19:32:15  pgm
; * B1591 - default no mask - its fast
; *
; * Revision 1.21  2002/03/07 08:29:31  pgm
; * FET runs to end of nsamples on atrig
; *
; * Revision 1.20  2002/03/06 09:48:57  pgm
; * SMALL_DIG_TRIG now default
; *
; * Revision 1.19  2002/02/16 17:53:34  pgm
; * for release, FET Multitrig nearly works
; *
; * Revision 1.18  2002/02/07 16:56:36  pgm
; * update to new lca
; *
; * Revision 1.17  2002/01/27 12:58:53  pgm
; * factor out, optimise 3 regular paths (4inst), trig path (5inst)
; *
; * Revision 1.16  2002/01/26 21:40:49  pgm
; * multi works, init i-cache fill good
; *
; * Revision 1.15  2002/01/25 21:55:09  pgm
; * edge trigger - this one worked, both edges even channels
; *
; * Revision 1.14  2002/01/20 20:54:00  pgm
; * threshtrig in debug
; *
; * Revision 1.13  2002/01/19 18:26:09  pgm
; * fast th almost
; *
; * Revision 1.12  2002/01/18 22:47:13  pgm
; * *** empty log message ***
; *
; * Revision 1.11  2002/01/18 18:31:30  pgm
; * v.stubbed, seems to run without crashing
; *
; * Revision 1.10  2002/01/18 10:03:26  pgm
; * sign compares, proper shifts
; *
; * Revision 1.9  2002/01/16 10:00:59  pgm
; * docs
; *
; * Revision 1.8  2002/01/13 22:21:58  pgm
; * acq32 fast threshtrig kludge coded
; *
; * Revision 1.7  2001/12/17 21:44:28  pgm
; * clean reg saving
; *
; * Revision 1.6  2001/12/13 17:41:46  pgm
; * make use of generics for sinks
; *
; * Revision 1.5  2001/12/11 21:58:04  pgm
; * multi fast paramterised - willit work ??
; *
; * Revision 1.4  2001/12/05 19:51:02  pgm
; * works, but not at 2.5MSPS
; *
; * Revision 1.2  2001/12/04 21:28:53  pgm
; * full rate threshtrig in switchable sink
; *
; *
;\*****************************************************************************/

; edge_trigger_sink.s

            AREA          edge_trigger_sink_text, CODE, READONLY

            IMPORT        G_last_trigger_line
            IMPORT        G_event_count
  		    IMPORT        G_trigger_cb 
            IMPORT        TWORD                    ; ch0 { state, threshold }, ch1 {s,t}
	        IMPORT        trigger_action_value

; exported symbols are instantiations of the FS123TH macro, see end of file
         


; TWORD Flags indicate CX in active state               
C0_ACTIVE   EQU      0x40000000
CO_THOLD    EQU      0x3fff0000
C1_ACTIVE   EQU      0x00004000
C1_THOLD    EQU      0x00003fff


CX_TRIGGRD  EQU      0x80000000      ; any ch has triggered
C0_TRIGGRD  EQU      0x80000000
C1_TRIGGRD  EQU      0x00008000


;TW_SET_LOCAL EQU        0x0800        ; copy from dataHandling.h
; deliberate
TW_SET_LOCAL EQU         0x1800

; Channel Masks

M01        EQU        0x0003
M23        EQU        0x000c
M45        EQU        0x0030
M67        EQU        0x00c0
M89        EQU        0x0300
MAB        EQU        0x0c00

SMALLDIGTRIG EQU    1                ; set this to one if small dig trig mod made
USE_VFUNC    EQU    0

; MACROS to Evaluate trigger
;    EDGTRIG
;        if ( GETSTATE == ACTIVE ){
;            if ( THXTRIG != TRIGGERED ){
;                continue;
;            }else{
;                goto ontrig
;            }
;        }else{
;            SETIFACTIVE
;        }
;
;            
; evaluate trigger for channel 0, 1, jump to $ontrig0, $ontrig1 respectively
; $previous is coded 0Adddddddddddddddddd0Adddddddddddddd
;                    000000000000000000001111111111111111
;                    C0_ACTIVE|CO_THRESH|C1_ACTIVE|C1_THRESH
;
; For each channel, shift the ACTIVE bit into CARRY to test if active
;
; Compare C0 first, C1 is insignificant
; Then compare C1, C0 has been shifted out
; 

            
; GETSTATE                  
                                                                    
        MACRO 
        GETSTATE $tmp_threshold, $previous, $previous_shift, $on_inactive
        ; align the state word for compare with data, branch on_inactive            
        movs     $tmp_threshold, $previous, ASL #$previous_shift
        bcc      $on_inactive
        MEND
          

        MACRO
        EXTTRIG
 IF USE_VFUNC = 1
		ldr      r0, =G_trigger_cb       ; NB ASSUMES r2, r3 preserved!!
		mov      r14,pc
		ldr      pc,[r0]                 ; G_trigger_cb()
 ELSE
        ldr      r0, =0x40011030         ; Q32_DIO_DIR
	    ldr      r1, =trigger_action_value; DO2
	    ldrh     r1,[r1]
	    strh     r1,[r0]
 ENDIF	    	
	    MEND

; THXTRIG

        MACRO
        THXTRIG  $falling, $data, $datashift, $threshold, $on_normal
                
; compare $data with $threshold and jump the right way if threshold crossed             
; do the compare backwards st data can be shifted if necessary
; and branch on_normal first - this is the most frequently used path
; fall thru if triggered

        cmp      $threshold, $data, ASL #$datashift
 IF $falling = 1
        ble      $on_normal               ; if threshold <= data goto normal
 ELSE
        bge      $on_normal               ; if threshold >= data goto normal
 ENDIF
        EXTTRIG
        MEND


; SETIFACT


        MACRO
        SETIFACT $falling, $data, $datashift, $threshold, $storereg, $flag
; set active flag in $storereg if we are on an active edge                  
        cmp      $threshold, $data, ASL #$datashift
 IF $falling = 1
        orrlt    $storereg, $storereg, #$flag    ; if data > threshold => set flag
 ELSE
        orrgt    $storereg, $storereg, #$flag    ; if data < threshold => set flag
 ENDIF
        MEND

	    	
; EDGETRIG


        MACRO       
$atrig  EDGTRIG  $falling, $data, $tmp_threshold, $previous
$atrig.0
        GETSTATE $tmp_threshold, $previous, 2, $atrig.inactive0
$atrig.active0
        THXTRIG  $falling, $data, 0, $tmp_threshold, $atrig.0_99
$atrig.triggered0
        orr      $previous, $previous, #CX_TRIGGRD
;       orr      $previous, $previous, #C0_TRIGGRD      ; same as CX
        b        $atrig.0_99
$atrig.inactive0
        SETIFACT $falling, $data, 0, $tmp_threshold, $previous, C0_ACTIVE
$atrig.0_99
                
$atrig.atrig1
        GETSTATE $tmp_threshold, $previous, 18, $atrig.inactive1
$atrig.active1
        THXTRIG  $falling, $data, 16, $tmp_threshold, $atrig.1_99
$atrig.triggered1           
        orr      $previous, $previous, #CX_TRIGGRD
        orr      $previous, $previous, #C1_TRIGGRD
        b        $atrig.1_99
$atrig.inactive1
        SETIFACT $falling, $data, 16, $tmp_threshold, $previous, C1_ACTIVE
$atrig.1_99
        MEND
            


;
; STASH - macros for selectively storing data


        MACRO
$label  STASHL     $dest, $src, $mask, $cursor
        tst        $mask, #$cursor                ; if (mask&cusrsor) str pair
        strne      $src, [$dest], #4           
        MEND

        MACRO
$stashl STASHLINE $uses_mask, $m1, $m2, $m3, $m4 
$stashl
 IF $uses_mask = 1
        STASHL   r9, r5, r3, $m1       ; store masked channels to memory
        STASHL   r9, r6, r3, $m2
        STASHL   r9, r7, r3, $m3
        STASHL   r9, r8, r3, $m4
 ELSE
        stmia    r9!,{r5-r8}          
 ENDIF
        MEND

        MACRO
$stashx4 STASHX4  $uses_mask, $wx, $wy, $mx, $my
$stashx4
 IF $uses_mask = 1
        STASHL   r9, $wx, r3, $mx       ; store masked channels to memory
        STASHL   r9, $wy, r3, $my
 ELSE
        stmia    r9!,{$wx,$wy}          
 ENDIF
        MEND
                    
            
        MACRO
$stashl4 STASHL4    $uses_mask, $m1, $m2
        STASHX4    $uses_mask, r5, r6, $m1, $m2
        MEND

        MACRO
$stasht4 STASHT4    $uses_mask, $m3, $m4
        STASHX4    $uses_mask, r7, r8, $m3, $m4
        MEND

        MACRO
        DEBUG_MARKER $w1, $marker, $next_job
        mov        $w1, #$marker
        b          $next_job
        MEND




;******************************************************************************

; fs123 - the main macro that instantiates the whole function
            
        MACRO
$fs123  FS123TH      $uses_mask, $falling, $data0, $data1, $data2
; optimised for acq16, 12 channels, split into 3 * 4 word reads
;int fifo_sink_12ch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
;{
$fs123                                     ; this is the EXPORTED symbol!
        EXPORT   $fs123
        stmdb    r13!,{r4-r12,r14}     ;
                                           ; r5678 pairs[4]                     
        ldr      r12,=0xaa55           ; r12 is trigger id - 17 bits worth
        ldr      r9,  [r0,#0]          ;  r9 u32* pdata = *ppdest;
        ldr      r10, [r1,#0]          ; r10 u32* pfifo = *ppfifo; 
                                           ; r11 int isample
        stmdb    r13!,{r0-r1}                                      
        ldr      r0,=TWORD
        ldr      r4,[r0]               ; r4 is state/threshold word
                                                                   
        movs     r11, r2                                                            
                    
$fs123.zero_test                    
        beq      $fs123.loop_done
                    
$fs123.fifo_fetch_loop                     ; do {
$fs123.load0
        ldmia    r10!,{r5-r8}          ; s0[01234567] <= fifo
        cmp      r12, r5, LSR #16      ; check for embedded trigger
        beq      $fs123.might_be_event0

 IF "$data0" /= "notrig"
;atrig     EDGTRIG  $falling, $data, $tmp_threshold, $previous
$fs123.et0 EDGTRIG  $falling, $data0, r0, r4
 ENDIF
$fs123.st0 STASHLINE $uses_mask, M01, M23, M45, M67 ; s0[01234567] => mem

$fs123.load1                                                
        ldmia    r10!,{r5-r8}          ; s0[89ab], s1[0123] <= fifo
        cmp      r12, r7, LSR #16      ; check s1[01]  for embedded trigger
        beq      $fs123.might_be_event1
                    
 IF "$data1" /= "notrig"
;atrig     EDGTRIG  $falling, $data, $tmp_threshold, $previous
$fs123.et1 EDGTRIG  $falling, $data1, r0, r4
 ENDIF
                    
$fs123.st1 STASHLINE $uses_mask, M89, MAB, M01, M23 ; s0[89ab], s1[0123] => mem

$fs123.load2                                                               
        ldmia    r10!,{r5-r8}         ; s1[456789ab] <= fifo

 IF "$data2" /= "notrig"
;atrig     EDGTRIG  $falling, $data, $tmp_threshold, $previous
$fs123.et2 EDGTRIG  $falling, $data2, r0, r4
 ENDIF
                                    
$fs123.st2 STASHLINE $uses_mask, M45, M67, M89, MAB ; s1[456789ab] => mem
                                                                            
$fs123.loop_end
        subs     r11,r11,#2            ;   } while ( --nsamples );
        bgt      $fs123.fifo_fetch_loop;

; clean up and leave town
        mov      r7,#0                     ; null trigger word

; report if we detected an ATRIG
        tst      r4,#CX_TRIGGRD
        movne    r7,#TW_SET_LOCAL          ; distributor will handle the rest
$fs123.loop_done
        ldr      r0,=TWORD
        str      r4,[r0]                   ; store current threshold setting
        ldmia    r13!,{r0-r1}       
        str      r10, [r1,#0]              ; *ppfifo = pfifo
        str      r9,  [r0,#0]              ; *ppdest = pdest

        sub      r0,r2,r11                 ; return isample = nsamples-nproc
        add      r0,r0,r7                  ;  encode trigger
                                           ;  return TW   
; RETURN *********************************************************************
        ldmia    r13!,{r4-r12,pc}
; RETURN *********************************************************************

	    LTORG

$fs123.might_be_event0                       ; we have s0[01234567]

        cmp      r12, r6, LSR #16        ; s0[23] is TW?
        bne      $fs123.st0                 
        cmp      r12, r7, LSR #16        ; s0[45] is TW?
        bne      $fs123.st0                 
        cmp      r12, r8, LSR #16        ; s0[67] is TW?
        bne      $fs123.st0

$fs123.is_event0                                                
        ldmia    r10!,{r5-r6}            ; s0[89ab] <= fifo => dump
        mov      r0,#0                   ; diagnostic for later
        b        $fs123.event_detected
            
$fs123.might_be_event1                       ; we have s0[89ab], s1[0123]

        cmp      r12, r8, LSR #16        ; examine s1[23]
        bne      $fs123.st1                 
        STASHL4   $uses_mask, M89, MAB   ; s0[89ab] => mem
        mov      r0, r7                  ; s1[01] in r0 in case false alarm
        mov      r1, r7                  ; s1[23] in r1
                    
        ldmia    r10!,{r5-r8}            ; s1[456789ab] <= fifo
        cmp      r12, r5, LSR #16
        bne      $fs123.no_event1        ; what happens to s1[0123] ???
            
        cmp      r12, r6, LSR #16
        beq      $fs123.is_event1
                    
$fs123.no_event1
        STASHX4  $uses_mask, r0, r1, M01, M23               
        b        $fs123.st2
                    
$fs123.is_event1                    
;       mov      r0,#1                   ; diagnostic for later
; this seems to cause a misalignment ... try remove it
;       sub      r11,r11,#1              ; adjust count for trigger1
; EXCEPTION PROCESSING ***********************************************************
; OK . this has to be an event

$fs123.event_detected

; we want to return the FIRST occurence of TW, from R5

        ldr      r12, =G_last_trigger_line
;       stmia    r12,{r0-r15}                  ; caller can see the whole thing
        stmia    r12,{r5-r8}                   ; try save a little time ...
                                               ; clear the rest of the T Aword
 IF     SMALLDIGTRIG = 1
        ldmia    r10!,{r1,r6-r8,r12}               ; pull 10 longs to make 1 line
        ldmia    r10!,{r1,r6-r8,r12} 
 ELSE            
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; pull full 8 samples worth
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; 3
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; 4
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; 5
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; 6
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; 7
        ldmia    r10!,{r0,r1,r6-r8,r12}            ; 8
 ENDIF              
		         
        ldr      r12, =G_event_count     ; G_event_count += 1
        ldr      r0,[r12]
        add      r0, r0, #1
        str      r0,[r12]
                    
        mov      r7, r5, LSL #16         ; encode trigger word
        b        $fs123.loop_done
            
        LTORG
$fs123.99                                ; this is the EXPORTED end symbol
        EXPORT   $fs123.99
                
        MEND
            

; FS123TH channel assignments - C<sample>_<cc>    r<n>

           MACRO
$label     REGSEL  $reg
           GBLS    $label
$label     SETS    "$reg"
           MEND
           
C0_01      REGSEL    r5
C0_23      REGSEL    r6
C0_45      REGSEL    r7
C0_67      REGSEL    r8

C1_89      REGSEL    r5
C1_AB      REGSEL    r6
C1_01      REGSEL    r7
C1_23      REGSEL    r8

C2_45      REGSEL    r5
C2_67      REGSEL    r6
C2_89      REGSEL    r7
C2_AB      REGSEL    r8

; no data
CZ_ZZ      REGSEL    notrig

FALLING    EQU    1
RISING     EQU    0

MASK       EQU    1
NMASK      EQU    0

; ********************************************************************************
;
; EDGE TRIGGER ENTRY POINTS
;
; DEFAULT NOW NO MASK
;
; ********************************************************************************

;$fs123                         FS123TH $mask, $falling,$data0, $data1, $data2
fifo_sink_12ch_falling_edge_01  FS123TH NMASK, FALLING, $C0_01, $C1_01, $CZ_ZZ
fifo_sink_12ch_falling_edge_23  FS123TH NMASK, FALLING, $C0_23, $C1_23, $CZ_ZZ
fifo_sink_12ch_falling_edge_45  FS123TH NMASK, FALLING, $C0_45, $CZ_ZZ, $C2_45
fifo_sink_12ch_falling_edge_67  FS123TH NMASK, FALLING, $C0_67, $CZ_ZZ, $C2_67
fifo_sink_12ch_falling_edge_89  FS123TH NMASK, FALLING, $CZ_ZZ, $C1_89, $C2_89
fifo_sink_12ch_falling_edge_AB  FS123TH NMASK, FALLING, $CZ_ZZ, $C1_AB, $C2_AB

fifo_sink_12ch_rising_edge_01   FS123TH NMASK, RISING,  $C0_01, $C1_01, $CZ_ZZ
fifo_sink_12ch_rising_edge_23   FS123TH NMASK, RISING,  $C0_23, $C1_23, $CZ_ZZ
fifo_sink_12ch_rising_edge_45   FS123TH NMASK, RISING,  $C0_45, $CZ_ZZ, $C2_45
fifo_sink_12ch_rising_edge_67   FS123TH NMASK, RISING,  $C0_67, $CZ_ZZ, $C2_67
fifo_sink_12ch_rising_edge_89   FS123TH NMASK, RISING,  $CZ_ZZ, $C1_89, $C2_89
fifo_sink_12ch_rising_edge_AB   FS123TH NMASK, RISING,  $CZ_ZZ, $C1_AB, $C2_AB

fifo_sink_12ch_rising_edge_ZZ   FS123TH NMASK, RISING,  $CZ_ZZ, $CZ_ZZ, $CZ_ZZ

fifo_sink_12ch_falling_edge_01m FS123TH MASK, FALLING,  $C0_01, $C1_01, $CZ_ZZ
fifo_sink_12ch_falling_edge_23m FS123TH MASK, FALLING,  $C0_23, $C1_23, $CZ_ZZ
fifo_sink_12ch_falling_edge_45m FS123TH MASK, FALLING,  $C0_45, $CZ_ZZ, $C2_45
fifo_sink_12ch_falling_edge_67m FS123TH MASK, FALLING,  $C0_67, $CZ_ZZ, $C2_67
fifo_sink_12ch_falling_edge_89m FS123TH MASK, FALLING,  $CZ_ZZ, $C1_89, $C2_89
fifo_sink_12ch_falling_edge_ABm FS123TH MASK, FALLING,  $CZ_ZZ, $C1_AB, $C2_AB

fifo_sink_12ch_rising_edge_01m  FS123TH MASK, RISING,   $C0_01, $C1_01, $CZ_ZZ
fifo_sink_12ch_rising_edge_23m  FS123TH MASK, RISING,   $C0_23, $C1_23, $CZ_ZZ
fifo_sink_12ch_rising_edge_45m  FS123TH MASK, RISING,   $C0_45, $CZ_ZZ, $C2_45
fifo_sink_12ch_rising_edge_67m  FS123TH MASK, RISING,   $C0_67, $CZ_ZZ, $C2_67
fifo_sink_12ch_rising_edge_89m  FS123TH MASK, RISING,   $CZ_ZZ, $C1_89, $C2_89
fifo_sink_12ch_rising_edge_ABm  FS123TH MASK, RISING,   $CZ_ZZ, $C1_AB, $C2_AB
fifo_sink_12ch_rising_edge_ZZm  FS123TH MASK, RISING,   $CZ_ZZ, $CZ_ZZ, $CZ_ZZ

            END

