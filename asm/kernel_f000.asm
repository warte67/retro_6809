; ***********************************************************************
; * 	kernel_f000.asm 						*
; *									*
; *	a.k.a. The Basic Input and Output System (BIOS)			*
; ***********************************************************************
; *									*
; *  	LINUX: 		./asm/build.sh asm/kernel_f000			*
; *		or							*	
; *	WINDOWS:	asm kernel_f000					*
; *									*
; *	build.sh:							*
; *		#!/bin/bash						*
; *		#lwasm -9 test.asm --format=ihex -otest.hex -ltest.lst	*
; *		echo "lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst"	*
; *		lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst		*
; *									*
; *	asm.bat:							*
; *     	@set arg1=%1						*
; *     	asm6809 -H -9 %arg1%.asm -o %arg1%.hex -l %arg1%.lst	*
; ***********************************************************************
		INCLUDE "memory_map.asm"

; Software Vectors
	org	$0000	
VECT_RSRVD	fdb	RSRVD_start	; This will likely be used as the EXEC vector
VECT_SWI3 	fdb	SWI3_start	; SWI3 Software Interrupt Vector
VECT_SWI2 	fdb	SWI2_start	; SWI2 Software Interrupt Vector
VECT_FIRQ 	fdb	FIRQ_start	; FIRQ Software Interrupt Vector
VECT_IRQ  	fdb	IRQ_start	; IRQ Software Interrupt Vector
VECT_SWI  	fdb	SWI_start	; SWI / SYS Software Interrupt Vector
VECT_NMI  	fdb	NMI_start	; NMI Software Interrupt Vector	
VECT_RESET	fdb	kernel_start	; RESET Software Interrupt Vecto	r	
	

; system zero-page variables
cursor_attrib	fcb	$e4;
cursor_addr	fdb	$0000;
var1		fcb	0;
var2		fcb	0;




; Kernel Jump Vector Calls	
	org 	KERNEL_ROM		
KRNL_RSRVD	jmp	[VECT_RSRVD]	; This will likely be used as t	he EXEC vector
KRNL_SWI3 	jmp	[VECT_SWI3 ]	; SWI3 Software Interrupt Vector	
KRNL_SWI2 	jmp	[VECT_SWI2 ]	; SWI2 Software Interrupt Vector
KRNL_FIRQ 	jmp	[VECT_FIRQ ]	; FIRQ Software Interrupt Vector
KRNL_IRQ  	jmp	[VECT_IRQ  ]	; IRQ Software Interrupt Vector
KRNL_SWI  	jmp	[VECT_SWI  ]	; SWI / SYS Software Interrupt Vector
KRNL_NMI  	jmp	[VECT_NMI  ]	; NMI Software Interrupt Vector
KRNL_RESET	jmp	[VECT_RESET]	; RESET Software Interrupt Vector	

; a null vectors (i.e. an infinate loop traps for vector testing)
KRNL_UNDEF	bra	KRNL_UNDEF

RSRVD_start	bra	RSRVD_start	; EXEC program
SWI3_start	bra	SWI3_start	; SWI3 Implementation
SWI2_start	bra	SWI2_start	; SWI2 Implementation
FIRQ_start	bra	FIRQ_start	; FIRQ Implementation
IRQ_start	bra	IRQ_start	; IRQ Implementation
SWI_start	bra	SWI_start	; SWI / SYS Implementation
NMI_start	bra	NMI_start	; NMI Implementation
RESET_start	bra	RESET_start	; RESET Implementation


kernel_start
	; initialize both stack pointers
		lds	#SSTACK_TOP
		ldu	#USTACK_TOP

; SYS_STATE: ABCD.SSSS
;      A:0   = Error: Standard Buffer Overflow
;      B:0   = Error: Extended Buffer Overflow
;      C:0   = Error: Reserved
;      D:0   = Error: Reserved
;      S:$0  = CPU Clock  25 khz.
;      S:$1  = CPU Clock  50 khz.
;      S:$2  = CPU Clock 100 khz.
;      S:$3  = CPU Clock 200 khz.
;      S:$4  = CPU Clock 333 khz.
;      S:$5  = CPU Clock 416 khz.
;      S:$6  = CPU Clock 500 khz.
;      S:$7  = CPU Clock 625 khz.
;      S:$8  = CPU Clock 769 khz.
;      S:$9  = CPU Clock 833 khz.
;      S:$A  = CPU Clock 1.0 mhz.
;      S:$B  = CPU Clock 1.4 mhz.
;      S:$C  = CPU Clock 2.0 mhz.
;      S:$D  = CPU Clock 3.3 mhz.
;      S:$E  = CPU Clock 5.0 mhz.
;      S:$F  = CPU Clock ~10.0 mhz. (unmetered)

		lda	#$0A
		sta	SYS_STATE

;	DSP_GRES: BBRR.HHVV
;	    BB:00 = Standard Graphics 1-bpp (2-color mode)	
;	    BB:01 = Standard Graphics 2-bpp (4-color mode)	
;	    BB:10 = Standard Graphics 4-bpp (16-color mode)	
;	    BB:11 = Standard Graphics 8-bpp (256-color mode)	
;	    RR:00 = 16 / 9  	Aspect:
;	    RR:01 = 16 / 10	Aspect:
;	    RR:10 = 16 / 11	Aspect:
;	    RR:11 = 16 / 12	Aspect:
;	    HH:00 = 4x Horizontal Overscan Multiplier
;	    HH:01 = 3x Horizontal Overscan Multiplier
;	    HH:10 = 2x Horizontal Overscan Multiplier
;	    HH:11 = 1x Horizontal Overscan Multiplier
;	    VV:00 = 4x Vertical Overscan Multiplier
;	    VV:01 = 3x Vertical Overscan Multiplier
;	    VV:10 = 2x Vertical Overscan Multiplier
;	    VV:11 = 1x Vertical Overscan Multiplier

		lda	#$CA
		sta	DSP_GRES

; DSP_EXT: ABCD.EFGG
;      AA:00 = Extended Graphics 1bpp (2-color mode)
;      AA:01 = Extended Graphics 2bpp (4-color mode)
;      AA:10 = Extended Graphics 4bpp (16-color mode)
;      AA:11 = Extended Graphics 4bpp (16-color mode)
;      B:0   = Extended Graphics: DISABLED
;      B:1   = Extended Graphics: ENABLED
;      C:0   = Extended Extended Mode: BITMAP
;      C:1   = Extended Extended Mode: TILES
;      D:0   = Standard Graphics: DISABLED
;      D:1   = Standard Graphics: ENABLED
;      E:0   = Standard Display Mode: TEXT
;      E:1   = Standard Display Mode: BITMAP
;      F:0   = VSYNC OFF
;      F:1   = VSYNC ON
;      B:0   = Fullscreen Enabled( emulator only )
;      B:1   = Windowed Enabled ( emulator only )

		lda	#$C9
		sta	DSP_EXT


	; clear out the text buffer
cls		
		ldx	#SCREEN_BUFFER
		ldd	#$20B4
1		std	,x++
		cmpx	#STD_VID_MAX
		blt	1b

2		bsr	line_edit
		
		bra	cls


; output the line edit buffer
line_edit
		clr	EDT_BUFFER

6	; check for ENTER press
		lda	CHAR_SCAN	; scan for a character waiting in the queue
		cmpa	#$0d		; is it the ENTER key?
		bne	0f		; nope, continue to render the text line editor
		lda	CHAR_POP	; pop the ENTER from the queue before returning
		ldd	#$20b4		; load a colored space
		std	[cursor_addr]	; overrite the last colored cursor
		rts			; return from the line editor subroutine
0	; begine the line edit routine
		ldu	#SCREEN_BUFFER	; reserve the start of the standard buffer
		ldy	#EDT_BUFFER	; start of the character line edit buffer
		ldx	#SCREEN_BUFFER	; point to the start of the display buffer
		lda	EDT_BFR_CSR	; fetch the cursor position
		pshs	a		; save it on the stack as a local variable	
		bne	1f		; cursor position is not zero, begin text
		lda	,y		; load the first character in the buffer
		bne	4f		; if its not zero, display the cursor
1	; start of the line
		lda	EDT_BFR_CSR	; load the cursor position
		beq	3f		; if theres no character, display the cursor
2	; display up to the cursor position
		lda	,y+		; fetch a character from the edit buffer
		beq	3f		; null character (end of line)
		ldb	#$B4		; color attribute
		std	,x++		; display the character with its color	
		dec	0,s		; decrease the local cursor position
		beq	4f		; brach if the cursor is over a character
		bra	2b		; continue displaying characters
3	; cursor at end of the line
		puls	a		; repair the stack
		lda	#$20		; load a space
		sta	,x++		; display the space
		bsr	flash_the_cursor	
		bra	6b		; loop and display changes in the edit buffer
4	; cursor over a character
		puls	a		; clean up the stack
		lda	,y+		; load the next character from the buffer
		sta	,x++		; ignore the attribute
	; flash the cursor
		bsr	flash_the_cursor
5	; finish the line
		lda	,y+		; load the next character from the buffer
		beq	6b		; loop back to the top if done
		ldb	#$b4		; load the basic screen attribute
		std	,x++		; display the next character with its color
		bra	5b		; continue until the line is finished

flash_the_cursor
		pshs	a,b,u		; save these on the stack
		lda	cursor_attrib	; load the next cursor color
		inca			; increment it
		anda	#$0f		; mask off the foreground color
		ora	#$f0		; foreground 15 = opaque black
		sta	cursor_attrib	; store the new attribute
		ldb	EDT_BFR_CSR	; load the cursor position
		lslb			; skip the attribue
		incb			; increment the attribute color
		sta	b, u		; update the characters color
		decb			; now index the character
		leau	b,u		; load that characters address
		stu	cursor_addr	; store it on the zero-page
		puls	a,b,u		; pop these off the stack
		rts			; return when done








; ROM Based Hardware Vectors
		org	$FFF0
		fdb	KRNL_RSRVD	; HARD_RSRVD       RESERVED Hardware Interrupt Vector
		fdb	KRNL_SWI3  	; HARD_SWI3        SWI3 Hardware Interrupt Vector
		fdb	KRNL_SWI2  	; HARD_SWI2        SWI2 Hardware Interrupt Vector
		fdb	KRNL_FIRQ  	; HARD_FIRQ        FIRQ Hardware Interrupt Vector
		fdb	KRNL_IRQ    	; HARD_IRQ         IRQ Hardware Interrupt Vector
		fdb	KRNL_SWI    	; HARD_SWI         SWI / SYS Hardware Interrupt Vector
		fdb	KRNL_NMI    	; HARD_NMI         NMI Hardware Interrupt Vector
		fdb	KRNL_RESET 	; HARD_RESET       RESET Hardware Interrupt Vector


; *****************************************************************
;
;   Text Mode Control Characters ($00 - $1f):
; $00  00000000  Null character
; $01  00000001  Set Default Character #		
; $02  00000010  Set Attribute #			
; $03  00000011  Set FG Color #				
; $04  00000100  Set BG Color #				
; $05  00000101  Position Cursor To Column #		
; $06  00000110  Position Cursor To Row #		
; $07  00000111  Bell, Alert
; $08  00001000  Backspace
; $09  00001001  Horizontal Tab; $0A  00001010  [No Operation]				
; $0B  00001011  					
; $0C  00001100  Clear Screen To Default Character	
; $0D  00001101  Carriage Return (ENTER); $0E  00001110  					
; $0F  00001111  					
; $10  00010000  Scroll Up				
; $11  00010001  Scroll Down				
; $12  00010010  Scroll Left				
; $13  00010011  Scroll Right				
; $14  00010100  Cursor Up				
; $15  00010101  Cursor Down				
; $16  00010110  Cursor Left				
; $17  00010111  Cursor Right				
; $18  00011000  Home Cursor				
; $19  00011001  Hide Cursor				
; $1A  00011010  Show Cursor				
; $1B  00011011  [Escape]; $1C  00011100  					
; $1D  00011101  					
; $1E  00011110  					
; $1F  00011111  Uncontrolled Character ($00-1f)	
;
; *****************************************************************
