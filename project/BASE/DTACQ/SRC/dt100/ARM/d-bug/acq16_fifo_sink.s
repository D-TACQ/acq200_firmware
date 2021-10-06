;/*****************************************************************************
; *
; * File: fifo_sink.s
; *
; * $RCSfile: acq16_fifo_sink.s,v $
; * 
; * Copyright (C) 2001 D-TACQ Solutions Ltd
; * not to be used without owner's permission
; *
; * Description:
; *
; * $Id: acq16_fifo_sink.s,v 1.9 2002/08/12 18:24:15 pgm Exp $
; * $Log: acq16_fifo_sink.s,v $
; * Revision 1.9  2002/08/12 18:24:15  pgm
; * fixes spurious boost value
; *
; * Revision 1.8  2002/07/24 18:07:41  pgm
; * acq16 trigger offset comp - use 1st TW
; *
; * Revision 1.7  2002/03/21 15:07:11  pgm
; * merge from 1608 - multi trig safe, edge det etc
; *
; * Revision 1.6  2002/03/09 12:20:39  pgm
; * fix FS44 sink, ensure clean hw ch change
; *
; * Revision 1.5  2002/03/09 11:04:59  pgm
; * prefer bgt to bne
; *
; * Revision 1.4  2002/03/08 19:32:14  pgm
; * B1591 - default no mask - its fast
; *
; * Revision 1.3  2002/03/06 09:48:57  pgm
; * SMALL_DIG_TRIG now default
; *
; * Revision 1.2  2002/02/10 17:56:50  pgm
; * usecs timing reliable over time
; *
; * Revision 1.1  2002/02/08 22:41:24  pgm
; * multi num channel configs for acq16
; *
; * Revision 1.25  2002/02/07 16:56:36  pgm
; * update to new lca
; *
; * Revision 1.24  2002/01/18 18:33:10  pgm
; * *** empty log message ***
; *
; * Revision 1.23  2001/12/13 17:41:46  pgm
; * make use of generics for sinks
; *
; * Revision 1.22  2001/12/08 14:37:56  pgm
; * acq16 runs at 2.52MSPS - 2*6=4*3 rools OK
; *
; * Revision 1.21  2001/12/05 20:59:32  pgm
; * sink 3 x 4 is the biz
; *
; * Revision 1.20  2001/12/05 19:51:02  pgm
; * works, but not at 2.5MSPS
; *
; * Revision 1.19  2001/12/04 21:28:53  pgm
; * full rate threshtrig in switchable sink
; *
; * Revision 1.18  2001/10/22 20:00:17  pgm
; * Build 1400 edge trig works rel to Scalar
; *
; * Revision 1.17  2001/08/31 09:10:21  pgm
; * acq16 has channel mask, works on rearm
; *
; * Revision 1.16  2001/08/27 16:40:14  pgm
; * acq16 event flush runs to 8 samples worth
; *
; * Revision 1.15  2001/08/27 15:15:40  pgm
; * acq16 - uses lut for nsamples slightly faster
; *
; * Revision 1.14  2001/08/27 12:55:18  pgm
; * acq16 runs again - needs LCA bit change, caveat embedded trigger
; *
; * Revision 1.13  2001/06/27 22:49:42  pgm
; * better status handling, acq16 deeper fifo
; *
; * Revision 1.12  2001/06/23 19:03:37  pgm
; * acq16 part 1 captures runs
; *
; * Revision 1.11  2001/06/11 19:19:52  pgm
; * better
; *
; * Revision 1.10  2001/06/09 11:56:06  pgm
; * fifo_sink_notests()
; *
; * Revision 1.9  2001/05/14 17:49:50  pgm
; * AO works well - shippit quick
; *
; * Revision 1.8  2001/05/14 16:24:25  pgm
; * fifo_source, increments, AO fifo 3/4 250ksps is go
; *
; * Revision 1.7  2001/05/05 06:33:10  pgm
; * magic number removal - this works
; *
; * Revision 1.6  2001/05/04 20:37:08  pgm
; * big upgrade to improve qwual, match spec
; *
; * Revision 1.5  2001/05/04 09:31:54  pgm
; * beautify
; *
; * Revision 1.4  2001/05/04 08:10:16  pgm
; * correct trigger word detect, neaten up
; *
; *
;\*****************************************************************************/

; fifo_sink.s

            AREA          acq16_fast_fifo_sink, CODE, READONLY

SMALLDIGTRIG EQU    1                ; set this to one if small dig trig mod made

; consume the acq16 fifo in blocks of 3 lines, 192b, 48 longs

FIFOBLKLW EQU    48                ; long words in block

; define sizeof of digital trigger

 IF SMALLDIGTRIG = 1
DIGTRIGLW EQU    16
 ELSE
DIGTRIGLW EQU    FIFOBLKLW
 ENDIF
  
; EXPORT the following - macro does it better!		    
;		    EXPORT        fifo_sink_12ch
;		    EXPORT        fifo_sink_12ch_nomask
;		    EXPORT        fifo_sink_12ch_nomask4
;		    EXPORT        fifo_sink_12ch_nomask_notrigger
		    
	        IMPORT        G_last_trigger_line
		    IMPORT        G_event_count

; Channel masks

M01        EQU        0x0003
M23        EQU        0x000c
M45        EQU        0x0030
M67        EQU        0x00c0
M89        EQU        0x0300
MAB        EQU        0x0c00
MCD        EQU        0x3000
MEF        EQU        0xc000


            MACRO
$label      STASHL     $dest, $src, $mask, $cursor
            tst        $mask, #$cursor                ; if (mask&cursor) store pair
	        strne      $src, [$dest], #4	       
	        MEND

	    
            MACRO
$stashline  STASHLINE $uses_mask, $m1, $m2, $m3, $m4 
$stashline
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
            STASH6L    $uses_mask, $mask
 IF $uses_mask = 1
            STASHL   r9, r3, $mask, 0x0003    ; store masked channels to memory
	        STASHL   r9, r4, $mask, 0x000c
		    STASHL   r9, r5, $mask, 0x0030
		    STASHL   r9, r6, $mask, 0x00c0
		    STASHL   r9, r7, $mask, 0x0300
		    STASHL   r9, r8, $mask, 0x0c00
 ELSE
            stmia    r9!,{r3-r8}          ; store entire sample to memory          
 ENDIF   		    
            MEND
			 	       	         

            MACRO
	        SINK6    $uses_mask, $mask
		    ldmia    r10!,{r3-r8}
     	    STASH6L  $uses_mask, $mask
		    MEND
		    
            MACRO
$fs12      FS12      $uses_mask, $notrigger
; optimised for acq16, 12 channels
;int fifo_sink_12ch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
;{
$fs12
            EXPORT   $fs12
            stmdb    r13!,{r4-r12,r14} ;
	                                       ; r3-8 pairs[6] 		        
            ldr      r12,=0xaa55           ; r12 is trigger id - 16 bits worth
		    ldr      r9,  [r0,#0]          ;  r9 u32* pdata = *ppdest;
	        ldr      r10, [r1,#0]          ; r10 u32* pfifo = *ppfifo; 
                                           ; r11 int isample

            stmdb    r13!,{r0-r1}
	        mov	     r0, r3                ; r0 is channel mask (temp)
		
		    mov      r11, r2               ; isample = 0;
 	    		    
$fs12.zero_test		    
            cmp      r11,#0                ;       isample != nsamples;
            beq      $fs12.loop_done

$fs12.fifo_fetch_loop                            ; do {
            ldmia    r10!,{r3-r8}            ; s0[0-11]

 IF $notrigger = 0                    		; check for embedded trigger here, drop out asap
		    cmp      r12, r3, LSR #16
		    beq      $fs12.might_be_trigger
 ENDIF
 
$fs12.not_trigger
            STASH6L  $uses_mask, r0

	        mov      r1, #7	    
$fs12.remsink
            SINK6    $uses_mask, r0
            subs     r1,r1,#1
	        bgt      $fs12.remsink


$fs12.loop_end
            subs     r11,r11,#8            ;   } while ( --nsamples );
            bgt      $fs12.fifo_fetch_loop;

; clean up and leave town
            mov      r3, #0                     ; null trigger word
	    
$fs12.loop_done

	        ldmia    r13!,{r0-r1}	                
            str      r10, [r1,#0]                ; *ppfifo = pfifo
	        str      r9,  [r0,#0]                ; *ppdest = pdest

            sub      r0,r2,r11                   ; return isample = nsamples-nproc
	        add      r0,r0,r3,LSL #16            ;  encode trigger
                                                 ;  return TW   
; RETURN *************************************************************************	            
	        ldmia    r13!,{r4-r12,pc}
; RETURN *************************************************************************		


$fs12.might_be_trigger
		    cmp      r12, r4, LSR #16
		    bne      $fs12.not_trigger
		    
		    cmp      r12, r5, LSR #16
		    bne      $fs12.not_trigger
		    
		    cmp      r12, r6, LSR #16
		    bne      $fs12.not_trigger

; EXCEPTION PROCESSING ***********************************************************
; OK . this has to be a trigger   

$fs12.trigger_detected
            ldmia    r10!,{r4-r8}            ; draw off another 10 u32s to make 16 total
	        ldmia    r10!,{r4-r8}
    
            ldr      r12, =G_last_trigger_line
	        stmia    r12,{r3-r8}               ; caller can see the whole thing

            ldr      r12, =G_event_count       ; G_event_count += 1
	        ldr      r0,[r12]
		    add      r0, r0, #1
		    str      r0,[r12]
            b        $fs12.loop_done
	    
; EXCEPTION PROCESSING ***********************************************************
            LTORG
$fs12.99
            EXPORT   $fs12.99

            MEND

		    	    
            MACRO
$fs123      FS123      $uses_mask	    
; optimised for acq16, 12 channels, split into 3 * 4 word reads
;int fifo_sink_12ch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
;{
$fs123
            EXPORT    $fs123
	    
            stmdb    r13!,{r4-r12,r14}     ;
	                                       ; r5678 pairs[4] 		        
            ldr      r12,=0xaa55           ; r12 is trigger id - 16 bits worth
		    ldr      r9,  [r0,#0]          ;  r9 u32* pdata = *ppdest;
	        ldr      r10, [r1,#0]          ; r10 u32* pfifo = *ppfifo; 
                                           ; r11 int isample	   			             		   
		    movs     r11, r2 		    		 	    		    
$fs123.zero_test		    
            beq      $fs123.loop_done

		    
$fs123.fifo_fetch_loop                     ; do {
$fs123.load0
            ldmia    r10!,{r5-r8}          ; s0[01234567] <= fifo
		    cmp      r12, r5, LSR #16      ; check for embedded trigger
		    beq      $fs123.might_be_trigger0

$fs123.store0                              ;  s0[01234567] => mem
$fs123.st0  STASHLINE $uses_mask, M01, M23, M45, M67
	    		    
            ldmia    r10!,{r5-r8}          ; s0[89ab], s1[0123] <= fifo
		    cmp      r12, r7, LSR #16      ; check s1[01]  for embedded trigger
		    beq      $fs123.might_be_trigger1
		    
$fs123.store1                              ;  s0[89ab], s1[0123] => mem
$fs123.st1  STASHLINE $uses_mask, M89, MAB, M01, M23
				    		    	    	    
            ldmia    r10!,{r5-r8}         ; s1[456789ab] <= fifo	    	    
$fs123.store2                             ; s1[456789ab] => mem
$fs123.st2  STASHLINE $uses_mask, M45, M67, M89, MAB
	    								    
$fs123.loop_end
            subs     r11,r11,#2            ;   } while ( --nsamples );
            bne      $fs123.fifo_fetch_loop;

; clean up and leave town
            mov      r7, #0                     ; null trigger word
	    
$fs123.loop_done
	                
            str      r10, [r1,#0]                ; *ppfifo = pfifo
	        str      r9,  [r0,#0]                ; *ppdest = pdest

            sub      r0,r2,r11                   ; return isample = nsamples-nproc
	        add      r0,r0,r7,LSL #16            ;  encode trigger
                                                 ;  return TW   
; RETURN *************************************************************************	            
	        ldmia    r13!,{r4-r12,pc}
; RETURN *************************************************************************		


$fs123.might_be_trigger0

		    cmp      r12, r6, LSR #16        ; s0[23] is TW?
		    bne      $fs123.store0		    
		    cmp      r12, r7, LSR #16        ; s0[45] is TW?
		    bne      $fs123.store0		    
		    cmp      r12, r8, LSR #16        ; s0[67] is TW?
		    bne      $fs123.store0

$fs123.is_trigger0			                        
            ldmia    r10!,{r5-r6}            ; s0[89ab] <= fifo => dump
            b        $fs123.trigger_detected
	    
$fs123.might_be_trigger1

		    cmp      r12, r8, LSR #16
		    bne      $fs123.store1		    
		    stmia    r9!,{r5-r6}            ; s0[89ab] => mem

		    ldmia    r10!,{r5-r8}           ; s1[456789ab] <= fifo
		    cmp      r12, r5, LSR #16
		    bne      $fs123.store2		    
		    cmp      r12, r6, LSR #16
		    bne      $fs123.store2
		    
$fs123.is_trigger1		    
	        sub      r11,r11,#1              ; adjust count for trigger1
; EXCEPTION PROCESSING ***********************************************************
; OK . this has to be a trigger   

$fs123.trigger_detected
                                             ; clear the rest of the trigger word
            ldmia    r10!,{r3-r8}            ; 2
	        ldmia    r10!,{r3-r8}            ; 3
		    ldmia    r10!,{r3-r8}            ; 4
		    ldmia    r10!,{r3-r8}            ; 5
		    ldmia    r10!,{r3-r8}            ; 6
		    ldmia    r10!,{r3-r8}            ; 7
		    ldmia    r10!,{r3-r8}            ; 8
		    
            ldr      r12, =G_last_trigger_line
	        stmia    r12,{r3-r8}               ; caller can see the whole thing

            ldr      r12, =G_event_count       ; G_event_count += 1
	        ldr      r0,[r12]
		    add      r0, r0, #1
		    str      r0,[r12]
            b        $fs123.loop_done
; EXCEPTION PROCESSING ***********************************************************

            LTORG
$fs123.99
            EXPORT    $fs123.99	    
            MEND
	    


            MACRO
$stashm     STASHM     $dest, $src, $mask, $cursor, $temp, $uses_mask
 IF $uses_mask = 1
            tst        $mask, #$cursor
	        beq        $stashm.99
 ENDIF		
		    ldr        $temp, [$dest]
		    add        $temp, $temp, $src, LSR #2
	        str        $temp, [$dest], #4	       
		
$stashm.99		
	        MEND
		    
            MACRO
$fsm      FS123M      $uses_mask	    
; optimised for acq16, 12 channels, split into 3 * 4 word reads, takes mean of 4
;int fifo_sink_12ch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
;{
$fsm
            EXPORT    $fsm
	    
            stmdb    r13!,{r4-r12,r14}     ;
	                                       ; r5678 pairs[4] 		        
            ldr      r12,=0xaa55           ; r12 is trigger id - 16 bits worth
		    ldr      r9,  [r0,#0]          ;  r9 u32* pdata = *ppdest;
	        ldr      r10, [r1,#0]          ; r10 u32* pfifo = *ppfifo; 
                                           ; r11 int isample				   			             		   
		    movs     r11, r2
$fsm.zero_test		    
            beq      $fsm.loop_done

            mov      r14,r9                ; stash pstore
		    
$fsm.fifo_fetch_loop                     ; do {

$fsm.load0
            ldmia    r10!,{r5-r8}          ; s0[01234567] <= fifo
		    cmp      r12, r5, LSR #16      ; check for embedded trigger
		    beq      $fsm.might_be_trigger0

$fsm.store0                              ;  s0[01234567] => mem
$fsm.st00   STASHM   r9, r5, r3, M01, r4, $uses_mask  
$fsm.st01   STASHM   r9, r6, r3, M23, r4, $uses_mask
$fsm.st02   STASHM   r9, r7, r3, M45, r4, $uses_mask
$fsm.st03   STASHM   r9, r8, r3, M67, r4, $uses_mask
	    		    
            ldmia    r10!,{r5-r8}          ; s0[89ab], s1[0123] <= fifo
		    cmp      r12, r7, LSR #16      ; check s1[01]  for embedded trigger
		    beq      $fsm.might_be_trigger1
		    
$fsm.store1                              ;  s0[89ab], s1[0123] => mem
$fsm.st10   STASHM   r9, r5, r3, M89, r4, $uses_mask 
$fsm.st11   STASHM   r9, r6, r3, MAB, r4, $uses_mask
		    
		    mov      r9, r14                   ; reset pstore
		    
$fsm.st12   STASHM   r9, r7, r3, M01, r4, $uses_mask
$fsm.st13   STASHM   r9, r8, r3, M23, r4, $uses_mask
				    		    	    	    
            ldmia    r10!,{r5-r8}         ; s1[456789ab] <= fifo
	    	    
$fsm.store2                             ; s1[456789ab] => mem
$fsm.st20   STASHM   r9, r5, r3, M45, r4, $uses_mask
$fsm.st21   STASHM   r9, r6, r3, M67, r4, $uses_mask
$fsm.st22   STASHM   r9, r7, r3, M89, r4, $uses_mask
$fsm.st23   STASHM   r9, r8, r3, MAB, r4, $uses_mask
	    								    
$fsm.loop_end
            subs     r11,r11,#2                ;   } while ( --nsamples );
            beq      $fsm.fifo_fetch_done      ;
	        ands     r11,r11,#2
		    movne    r9,r14                    ; still busy, reuse pstore
		    moveq    r14,r9                    ; advance pstore
            b        $fsm.fifo_fetch_loop
	    
$fsm.fifo_fetch_done	    
; clean up and leave town
            mov      r7, #0                     ; null trigger word
	    
$fsm.loop_done
	                
            str      r10, [r1,#0]                ; *ppfifo = pfifo
	        str      r9,  [r0,#0]                ; *ppdest = pdest

            sub      r0,r2,r11                   ; return isample = nsamples-nproc
	        add      r0,r0,r7,LSL #16            ;  encode trigger
                                                 ;  return TW   
; RETURN *************************************************************************	            
	        ldmia    r13!,{r4-r12,pc}
; RETURN *************************************************************************		


$fsm.might_be_trigger0

		    cmp      r12, r6, LSR #16        ; s0[23] is TW?
		    bne      $fsm.store0		    
		    cmp      r12, r7, LSR #16        ; s0[45] is TW?
		    bne      $fsm.store0		    
		    cmp      r12, r8, LSR #16        ; s0[67] is TW?
		    bne      $fsm.store0

$fsm.is_trigger0			                        
            ldmia    r10!,{r5-r6}            ; s0[89ab] <= fifo => dump
            b        $fsm.trigger_detected
	    
$fsm.might_be_trigger1

		    cmp      r12, r8, LSR #16
		    bne      $fsm.store1		    
		    stmia    r9!,{r5-r6}            ; s0[89ab] => mem

		    ldmia    r10!,{r5-r8}           ; s1[456789ab] <= fifo
		    cmp      r12, r5, LSR #16
		    bne      $fsm.store2		    
		    cmp      r12, r6, LSR #16
		    bne      $fsm.store2
		    
$fsm.is_trigger1		    
	        sub      r11,r11,#1              ; adjust count for trigger1
; EXCEPTION PROCESSING ***********************************************************
; OK . this has to be a trigger   

$fsm.trigger_detected
                                             ; clear the rest of the trigger word
            ldmia    r10!,{r3-r8}            ; 2
	        ldmia    r10!,{r3-r8}            ; 3
		    ldmia    r10!,{r3-r8}            ; 4
		    ldmia    r10!,{r3-r8}            ; 5
		    ldmia    r10!,{r3-r8}            ; 6
		    ldmia    r10!,{r3-r8}            ; 7
		    ldmia    r10!,{r3-r8}            ; 8
		    
            ldr      r12, =G_last_trigger_line
	        stmia    r12,{r3-r8}               ; caller can see the whole thing

            ldr      r12, =G_event_count       ; G_event_count += 1
	        ldr      r0,[r12]
		    add      r0, r0, #1
		    str      r0,[r12]
            b        $fsm.loop_done
; EXCEPTION PROCESSING ***********************************************************

            LTORG
$fsm.99
            EXPORT    $fsm.99	    
            MEND
	    
; optimised for acq16, 12 channels
;int fifo_sink_12ch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
fifo_sink_12ch_mean4            FS123M 1
fifo_sink_12ch_mean4_nomask     FS123M 0

        MACRO
$lbl    STASH4 $phase, $uses_mask, $nchannels
    IF $nchannels=16
        IF $phase=0
$lbl    STASHLINE $uses_mask, M01, M23, M45, M67
        ELSE
$lbl    STASHLINE $uses_mask, M89, MAB, MCD, MEF
        ENDIF
    ENDIF
    IF $nchannels=8
$lbl    STASHLINE $uses_mask, M01, M23, M45, M67
    ENDIF
    IF $nchannels=4
$lbl    STASHLINE $uses_mask, M01, M23, M01, M23
    ENDIF
    IF $nchannels=2
$lbl    STASHLINE $uses_mask, M01, M01, M01, M01
    ENDIF
        MEND
								    
												    
; GENERIC ACQ16 fifo sink - even channel counts please!
; 
; Call this routine with a multiple # blocks of data
; A fifo block is three lines of the fifo
; TW occurs on block boundary only
; Samples per block


SPB_16    EQU    6
SPB_08    EQU    12
SPB_04    EQU    24
SPB_02    EQU    48

            MACRO
$fs44      FS44      $uses_mask, $nchannels, $spb
; optimised for acq16, 16, 8, 4, 2 channels - loop fetches 4+4 pairs per cycle
;int fifo_sink_XXch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
;{
$fs44
            EXPORT    $fs44
	                                       ; r0 is ppdest - immutable
					                       ; r1 is ppfifo - immutable
							               ; r2 is nsamples - immutable
								           ; r3 is channel mask immutabl
									       ; r4 is TEMP
            stmdb    r13!,{r4-r12,r14}     ;
	                                       ; r5678 pairs[4] 		        
            ldr      r12,=0xaa55           ; r12 is trigger id - 16 bits worth
		    ldr      r9,  [r0,#0]          ;  r9 u32* pdata = *ppdest;
	        ldr      r10, [r1,#0]          ; r10 u32* pfifo = *ppfifo; 
		                                   ; r11 int isample	   			             		   
						   
		    movs     r11, r2
		   
$fs44.zero_test		    
            ble      $fs44.loop_done

$fs44.block_fetch
            ldmia    r10!,{r5-r8}                   ; lw[0-3]
	        cmp      r12, r5, LSR #16
		    beq      $fs44.might_be_trigger
$fs44.not_trigger		    
		    STASH4   0, $uses_mask, $nchannels       
		    ldmia    r10!,{r5-r8}                    ; lw[4-7]
		    STASH4   1, $uses_mask, $nchannels

		    mov      r4, #FIFOBLKLW-8
$fs44.remblock		    
		    ldmia    r10!,{r5-r8}                   ; lw[ 8,16,24,32,40]
		    STASH4   0, $uses_mask, $nchannels
		    ldmia    r10!,{r5-r8}                   ; lw[12,20,28,36,44]
		    STASH4   1, $uses_mask, $nchannels
		    
            subs     r4,r4,#8                       ; removed 8 longwords
	        bgt      $fs44.remblock
	    								    
$fs44.loop_end
            subs     r11,r11,#$spb            ;   } while ( --nsamples );
            bgt      $fs44.block_fetch;

; clean up and leave town
            mov      r5, #0                     ; null trigger word
	    
$fs44.loop_done
	                
            str      r10, [r1,#0]                ; *ppfifo = pfifo
	        str      r9,  [r0,#0]                ; *ppdest = pdest

            sub      r0,r2,r11                   ; return isample = nsamples-nproc
	        add      r0,r0,r5,LSL #16            ;  encode trigger
                                                 ;  return TW   
; RETURN *************************************************************************	            
	        ldmia    r13!,{r4-r12,pc}
; RETURN *************************************************************************		


$fs44.might_be_trigger

		    cmp      r12, r6, LSR #16        ; s0[23] is TW?
		    bne      $fs44.not_trigger		    
		    cmp      r12, r7, LSR #16        ; s0[45] is TW?
		    bne      $fs44.not_trigger		    
		    cmp      r12, r8, LSR #16        ; s0[67] is TW?
		    bne      $fs44.not_trigger

; EXCEPTION PROCESSING ***********************************************************
; OK . this has to be a trigger   
; We want to save the FIRST TW (r5) for return, flush the rest

$fs44.trigger_detected
            ldr      r12, =G_last_trigger_line
	        stmia    r12,{r5-r8}               ; caller can see the whole thing

  		    mov      r4, #DIGTRIGLW-4
$fs44.tdflush
		    ldmia    r10!,{r6-r8,r12}                  ; lw[4,8,12...]
		    subs     r4,r4,#4
		    bgt      $fs44.tdflush
		    

            ldr      r12, =G_event_count       ; G_event_count += 1
	        ldr      r4,[r12]
		    add      r4, r4, #1
		    str      r4,[r12]
            b        $fs44.loop_done
; EXCEPTION PROCESSING ***********************************************************

            LTORG
$fs44.99
            EXPORT    $fs44.99	    
            MEND
	    



; ********************************************************************************
;
; ENTRY POINTS
;
; ********************************************************************************

; optimised for acq16, 12 channels
;int fifo_sink_12ch(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
fifo_sink_12ch            FS12 1, 0

;int fifo_sink_12ch_nomask(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
fifo_sink_12ch_nomask     FS12 0, 0

;int fifo_sink_12ch_nomask_notrigger(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
fifo_sink_12ch_nomask_notrigger	FS12 0, 1


		    ;int fifo_sink_12ch_nomask(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2 - min 8    
;    int channel_mask               ; r3
;)
fifo_sink_12ch_nomask4	FS123 0

; acq16_fifo_sink_xx - default config is NO MASK 
		    	    	    
acq16_fifo_sink_16    FS44    0, 16, SPB_16
acq16_fifo_sink_12    FS12    0,  0
acq16_fifo_sink_08    FS44    0,  8, SPB_08
acq16_fifo_sink_04    FS44    0,  4, SPB_04
acq16_fifo_sink_02    FS44    0,  2, SPB_02

acq16_fifo_sink_16m   FS44    1, 16, SPB_16
acq16_fifo_sink_12m   FS12    1,  0
acq16_fifo_sink_08m   FS44    1,  8, SPB_08
acq16_fifo_sink_04m   FS44    1,  4, SPB_04
acq16_fifo_sink_02m   FS44    1,  2, SPB_02
																    			    	    
            END
