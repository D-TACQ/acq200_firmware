;/*****************************************************************************
; *
; * File: fifo_sink.s
; *
; * $RCSfile: fifo_sink.s,v $
; * 
; * Copyright (C) 2001 D-TACQ Solutions Ltd
; * not to be used without owner's permission
; *
; * Description:
; *
; * $Id: fifo_sink.s,v 1.30 2002/07/22 20:01:34 pgm Exp $
; * $Log: fifo_sink.s,v $
; * Revision 1.30  2002/07/22 20:01:34  pgm
; * fixes gap in pre gpem P1
; *
; * Revision 1.29  2002/07/18 18:43:05  pgm
; * 44M stub, 64b copy test
; *
; * Revision 1.28  2002/03/21 11:54:51  pgm
; * untabify
; *
; * Revision 1.27  2002/03/13 21:43:09  pgm
; * correct formatting with emacs (Stroustrup)
; *
; * Revision 1.26  2002/02/08 22:41:24  pgm
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

        AREA          fast_fifo_sink, CODE, READONLY

        EXPORT        fifo_sink
        EXPORT        fifo_source
        EXPORT        fifo_sink_notests
	    EXPORT        memcpy64
                    
        IMPORT        G_last_trigger_line
        IMPORT        G_event_count

; STASH4L - save 4 long words (4 channel pairs) according to channel mask
                      
        MACRO
$label  STASH4L    $dest, $mask, $cursor
        tst        $mask, #$cursor
        strne      r4, [$dest], #4
        tst        $mask, #$cursor:SHL:2
        strne      r5, [$dest], #4
        tst        $mask, #$cursor:SHL:4
        strne      r6, [$dest], #4
        tst        $mask, #$cursor:SHL:6
        strne      r7, [$dest], #4
               
        MEND

                                         
fifo_sink
;int fifo_sink(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2
;    unsigned mask                  ; r3
;)
;{
        stmdb    r13!,{r4-r11,r14}      ;
                                        ; r4-7 pairs[4]
        ldr      r8,=0xaa558:SHR:3      ; r8  is trigger  id - 17 bits worth
        ldr      r9,  [r0,#0]           ; r9  u32* pdata = *ppdest;
        ldr      r10, [r1,#0]           ; r10 u32* pfifo = *ppfifo; 
                                        ; r11 int isample
        mov      r11, #0                ; for ( isample = 0;
        b        loop_test

loop_end
        add      r11,r11,#1             ;       ++isample ){
                            
loop_test                   
        cmp      r11,r2                 ;       isample != nsamples;
        beq      loop_done

first8
; we decide it's a trigger: - first 4 pairs must hold trigger word

;
; it's a drop out on first fail test, for max speed on non-trigger data
;
        ldmia    r10!,{r4-r7}           ;  r4 := pair[0], ... r7 := pair[3]
                                        ; check for embedded trigger here
                        
        cmp      r8, r4, LSR #15
        bne      no_trigger

        cmp      r8, r5, LSR #15
        bne      no_trigger
                    
        cmp      r8, r6, LSR #15
        bne      no_trigger
                    
        cmp      r8, r7, LSR #15
        bne      no_trigger
                    
; OK . this has to be a trigger   
                    
        ldr      r8, =G_last_trigger_line
        stmia    r8,{r4-r7}             ; caller can see whole thing if reqd

        ldr      r8, =G_event_count
        ldr      r7,[r8]
        add      r7, r7, #1
        str      r7,[r8]
                                
        add      r11, r11, r4, LSL #16  ; encode trigger
                                    
        ldmia    r10!,{r4-r7}           ; flush the rest of the sample
        ldmia    r10!,{r4-r7}
        ldmia    r10!,{r4-r7}
        b        loop_done              ; drop out

; no trigger - this is regular data, STASH it
                                
no_trigger      
        STASH4L  r9, r3, 0x00000003

second8
        ldmia    r10!,{r4-r7}            ; r4 := pair[4], .. r7 := pair[7]
        STASH4L  r9, r3, 0x00000300

third8
        ldmia    r10!,{r4-r7}            ; r4 := pair[8] ... r7 := pair[11]
        STASH4L  r9, r3, 0x00030000      

fourth8
        ldmia    r10!,{r4-r7}            ; r4 := pair[12] ...r7 := pair[15]
        STASH4L  r9, r3, 0x03000000
fifo_done
        b        loop_end

; clean up and leave town

loop_done
        str      r10, [r1,#0]                ; *ppfifo = pfifo
        str      r9,  [r0,#0]                ; *ppdest = pdest
        mov      r0, r11                     ;  return isample;             
        ldmia    r13!,{r4-r11,pc}

fifo_source
;extern int fifo_source (
;    unsigned* pdest         // R0
;    unsigned** ppsource     // R1
;    int nsamples            // R2 multiples of 4 only please
;);
; ASSERT nsamples <= 256

        stmdb    r13!,{r4-r9,r14}               ; r4-r7 - temps
        mov      r8, r0                         ; r8  dest
        ldr      r9, [r1,#0]                    ; r9  u32* pdata = *ppdest;
                    
        mov      r0, #0                         ; r0 is word count
        movs     r2, r2, LSR # 2;               ; r2  loop counter      
                       
        b        source_loop_test
            
source_loop_top
        add      r0, r0, #4           ; word count for return
        subs     r2, r2, #1           ; quads to go count
                        
source_loop_test
        beq      source_done        
                
        ldmia    r9!, {r4-r7}        ; load next 4 words, incrementing ptr
        stmia    r8!, {r4-r7}        ; store without incrementing ptr
        b        source_loop_top
        
source_done             
        str      r9, [r1,#0]         ; *ppdest = cursor
        ldmia    r13!,{r4-r9,pc}





fifo_sink_notests
;int fifo_sink_notests(
;    unsigned** ppdest,             ; r0
;    unsigned** ppfifo,             ; r1
;    int nsamples,                  ; r2
;)
;{
        stmdb    r13!,{r4-r11,r14}      ;
                                        ; r4-7 pairs[4]
        ldr      r9,  [r0,#0]           ; r9  u32* pdata = *ppdest;
        ldr      r10, [r1,#0]           ; r10 u32* pfifo = *ppfifo; 
                                        ; r11 int isample
        mov      r11, #0                ; for ( isample = 0;
        b        fsn_loop_test

fsn_loop_end
        add      r11,r11,#1             ;       ++isample ){
                            
fsn_loop_test               
        cmp      r11,r2                 ;       isample != nsamples;
        beq      fsn_loop_done

        ldmia    r10!,{r4-r7}
        stmia    r9!, {r4-r7}

        ldmia    r10!,{r4-r7}
        stmia    r9!, {r4-r7}

        ldmia    r10!,{r4-r7}
        stmia    r9!, {r4-r7}

        ldmia    r10!,{r4-r7}
        stmia    r9!, {r4-r7}
                
        b        fsn_loop_end

; clean up and leave town

fsn_loop_done
        str      r10, [r1,#0]                ; *ppfifo = pfifo
        str      r9,  [r0,#0]                ; *ppdest = pdest
        mov      r0, r11                     ;  return isample;       
        ldmia    r13!,{r4-r11,pc}

;void *memcpy64(void *dest, const void *src );
memcpy64
        stmdb    r13!,{r4-r7,r14}
	
	    ldmia    r1!,{r4-r7}
	    stmia    r0!,{r4-r7}
	    
	    ldmia    r1!,{r4-r7}
	    stmia    r0!,{r4-r7}
	    
	    ldmia    r1!,{r4-r7}
	    stmia    r0!,{r4-r7}
	    
	    ldmia    r1!,{r4-r7}
	    stmia    r0!,{r4-r7}
	
	    ldmia    r13!,{r4-r7,pc}

            END
