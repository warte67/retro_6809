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
VECT_RESET	fdb	kernel_start	; RESET Software Interrupt Vecto	

; kernel call vectors
;CHROUT_VECT	fdb	char_out_main	; CHROUT Software Vector
	

; system zero-page variables
CSR_ATTR	fcb	$B4;		; current cursor attribute
CSR_ADDR	fdb	$0000;		; current cursor address in screen memory
CSR_COL		fcb	$00;		; current cursor column
CSR_ROW		fcb	$00;		; current cursor row
TXT_ATTR	fcb	$B4		; current text color attribute
EDLIN_ANCH	fdb	$0000;		; line edit text anchor address
CMD_LN_TOKEN	fcb	$00;		; current command line token
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

	; begin the display
		jsr	cls
		ldx	#prompt0		
		jsr	line_out	
		ldx	#prompt1
		jsr	line_out	
1
		ldx	#ready_prompt		
		jsr	line_out	

		jsr	line_edit	
		jsr	decode_command_line	
		bra	1b

command_table	fcs	"CLS"		; 1
		fcs	"LOAD"		; 2
		fcs	"EXEC"		; 3
		fcs	"RESET"		; 4
		fcb	0		; 0 = non-entry or invalid command

decode_command_line	
		pshs	A, X, Y
		jsr	crunch_command_line

		puls	A, X, Y
		rts

crunch_command_line	; crunch the command line
		; 	on return:
		; 	A = tokenized command
		;	X points to argument 1
		;	Y points to argument 2
		;jsr	command_token
		rts






; output the line edit buffer
line_edit
		lda	#$ff		; 'true'
		sta	EDT_ENABLE	; enable the hardware line editor sub-system
		clr	EDT_BUFFER	; null the start of the edit buffer
		ldd	CSR_ADDR	; fetch the current cursor address
		std	EDLIN_ANCH	; store it as the anchor
6	; check for ENTER press
		lda	CHAR_SCAN	; scan for a character waiting in the queue
		cmpa	#$0d		; is it the ENTER key?
		bne	0f		; nope, continue to render the text line editor
		lda	CHAR_POP	; pop the ENTER from the queue before returning
		ldd	#$20b4		; load a colored space
		std	[CSR_ADDR]	; overrite the last colored cursor
		; home the cursor
		clr	CSR_COL		; CSR_COL = 0
		inc	CSR_ROW		; CSR_ROW = 0
		lda	CSR_ROW		; A - CSR_ROW
		cmpa	DSP_TXT_ROWS	; last line?
		blt	7f		; nope, recalc the new cursor address
		dec	CSR_ROW		; decrement CSR_ROW
		jsr	std_text_scroll	; scroll the text buffer
7	; recalculate the cursor address based on CSR_ROW and CSR_COL
		jsr 	cursor_address	; find CSR_ADDR based on CSR_ROW and CSR_COL
		stx	CSR_ADDR	; store the new CSR_ADDR
		clr	EDT_ENABLE	; disable the hardware line editor sub-system
		rts			; return from the line editor subroutine
0	; begin the line edit routine
		; ldu	#SCREEN_BUFFER	; reserve the start of the display buffer
		; ldu	EDLIN_ANCH	; reserve the start of the display buffer
		ldy	#EDT_BUFFER	; start of the character line edit buffer
		; ldx	#SCREEN_BUFFER	; point to the start of the display buffer
		ldx	EDLIN_ANCH	; point to the start of the display buffer
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
		pshs	a,b,x,u		; save these on the stack
		ldu	EDLIN_ANCH	; point to the edline start of line anchor
		lda	SYS_TIMER+1	; load the LSB of the system timer
		anda	#$07		; only concerned with these bits
		bne	1f		; if non-zero, skip color change
		lda	CSR_ATTR	; load the next cursor color
		inca			; increment it
		anda	#$0f		; mask off the foreground color
		ora	#$f0		; foreground 15 = opaque black
		sta	CSR_ATTR	; store the new attribute
		ldb	EDT_BFR_CSR	; load the cursor position
		lslb			; skip the attribue
		incb			; increment the attribute color
		sta	b, u		; update the characters color
		decb			; now index the character
		leau	b,u		; load that characters address
		stu	CSR_ADDR	; store it on the zero-page
1	; clean up and return	
		puls	a,b,x,u		; pop these off the stack
		rts			; return when done


line_out	; Display a null terminated string pointed to by X
		; begining at CSR_ROW and CSR_COL.
		pshs	a,x		; store the working registers
1	; line out main loop
		lda	,x+		; load the next character to display
		beq	2f		; skip if at null-terminator
		bmi	2f		; skip if high bit is set
		jsr	char_out	; output this character
		bra	1b		
2	; clean up and return
		puls	a,x		; restore the working registers
		rts			; return from subroutine



char_out	; output a formatted text character based on A
		; jsr	[CHROUT_VECT]
char_out_main	; the main CHROUT routine
		pshs	d,x

		cmpa	#$20		; is a printable character $20 - $ff
		bge	1f		; yup, go print it
		cmpa	#$0a		; line feed ?
		bne	2f		; no, skip to next
		jsr 	csr_new_line	; perform a new line
2	; skip to end
		bra	0f		; just skip everything
1	; output a printable character $20 - $ff
		jsr	cursor_address	; X = CSR_ADDR		
		ldb	TXT_ATTR	; fetch the default color attribute
		std	,x		; store it on screen
		inc	CSR_COL		; increase the cursors column
		; handle line wrap and scrolling
		lda	CSR_COL		; load cursor column
		cmpa	DSP_TXT_COLS	; compare with max displayed columns
		bge	1f		; if greater or equal
		bra	0f		; skip and return
1	; if (CSR_COL >= DSP_TXT_COLS)
		clr	CSR_COL		; clear CSR_COL
		inc	CSR_ROW		; increment CSR_ROW
		lda	CSR_ROW		; load CSR_ROWS
		cmpa	DSP_TXT_ROWS	; if (CSR_ROW < DSP_TXT_ROWS)
		blt	0f		; 	clean up and return
		jsr 	std_text_scroll	; scroll the text screen
		dec	CSR_ROW		; CSR_ROW--
0	; clean up and return
		puls	d,x		; restore the working registers
		rts			; return

	
cls	; clear out the text buffer	
		pshs	d, x		; store the working registers
		clr	CSR_COL		; cursor column = 0
		clr	CSR_ROW		; cursor row = 0
		jsr 	cursor_address	; X = cursor address based on CSR_COL and CSR_ROW
		ldx	#SCREEN_BUFFER	; point to the start of the text buffer
		lda	#$20		; set next character to space
		ldb	TXT_ATTR	; load the characters color
1	; cls main loop	
		std	,x++		; store the space with its color attribute
		cmpx	#STD_VID_MAX	; are we at the end of the text buffer?
		blt	1b		; 	loop if not
		puls	d, x		; restore the working registers
		rts			; return


csr_new_line	; perform a line-feed and a carriage return with scrolling
		pshs	a, x		; store the working registers
		clr	CSR_COL		; set cursor column to zero
		inc	CSR_ROW		; increment the cursor row
		lda	CSR_ROW		; A = cursor row
		cmpa	DSP_TXT_ROWS	; skip if the current cursor row
		blt	1f		;   is less than the max rows
		dec	CSR_ROW		; decrease the cursors row
		jsr	std_text_scroll	; scroll the text up one line
1	; recalculate the cursor address based on CSR_ROW and CSR_COL
		jsr 	cursor_address	; X = CSR_ADDR based on CSR_ROW and CSR_COL
		stx	CSR_ADDR	; update CSR_ADDR 
		puls	a,x		; restore the working registers
		rts			; return from the line editor subroutine



std_text_scroll	; scroll the standard text screen up one line
		pshs	d, x, y		; store the working registers
		ldy	#SCREEN_BUFFER	; Y = start of the standard video buffer
		ldb	DSP_TXT_COLS	; B = number of displayed text columns
		lslb			; B = B * 2 -- account for the attribute byte
		ldx	#SCREEN_BUFFER	; X = start of the standard video buffer
		leax	b,x		; X = one line down
1	; scroll up one line
		ldd	,x++		; load character at X 
		std	,y++		; store it at Y
		cmpx	STD_VID_MAX	; at the end of the display?
		blt	1b		; loop if not
	; fill the bottom line with spaces
		ldx	#SCREEN_BUFFER	; X = start of the standard video buffer
		ldb	DSP_TXT_ROWS	; B = number of displayed rows
		decb			; B = B - 1  last line
		lslb			; B = B * 1 -- account for the attribute byte
		lda	DSP_TXT_COLS	; A = number of displayed text columns
		mul			; D = A * B
		leax	d,x		; X = the start of the bottom row
		lda	#$20		; load up default space character
		ldb	TXT_ATTR	; and the default color
2	; store the character with its color attribute
		std	,x++		; store the space at X
		cmpx	STD_VID_MAX	; at the end of the text buffer
		blt	2b		; loop if not
	; return
		puls	d,x,y		; restore the working registers
		rts


cursor_address	; Update CSR_ADDR and return the cursor address in X 
		; based on CSR_COL and CSR_ROW.
		pshs	d		; store used registers
		ldx	#SCREEN_BUFFER	; start of the standard video buffer
		lda	CSR_ROW		; A = current cursor row
		ldb	DSP_TXT_COLS	; B = number of columns displayed
		lslb			; B = B * 2 -- account for the attribute byte
		mul			; D = A * B
		leax	d,x		; X = D + X		
		ldb	CSR_COL		; B = current cursor column
		lslb			; B = B * 2 -- account for the attribute byte
		leax	b,x		; X = B + X
		stx	CSR_ADDR	; store the cursor address
		puls	d		; restore used registers
		rts			; return





;	; output the line edit buffer
;	line_edit
;			clr	EDT_BUFFER
;			ldd	CSR_ADDR
;			std	EDLIN_ANCH
;	
;	6	; check for ENTER press
;			lda	CHAR_SCAN	; scan for a character waiting in the queue
;			cmpa	#$0d		; is it the ENTER key?
;			bne	0f		; nope, continue to render the text line editor
;			lda	CHAR_POP	; pop the ENTER from the queue before returning
;			ldd	#$20b4		; load a colored space
;			std	[CSR_ADDR]	; overrite the last colored cursor
;			; home the cursor
;			clr	CSR_COL
;			inc	CSR_ROW
;			lda	CSR_ROW
;			cmpa	DSP_TXT_ROWS
;			blt	7f
;			dec	CSR_ROW
;			jsr	std_text_scroll
;	7	; recalculate the cursor address based on CSR_ROW and CSR_COL
;			jsr 	cursor_address
;			stx	CSR_ADDR		
;			rts			; return from the line editor subroutine
;	0	; begine the line edit routine
;			; ldu	#SCREEN_BUFFER	; reserve the start of the display buffer
;			; ldu	EDLIN_ANCH	; reserve the start of the display buffer
;			ldy	#EDT_BUFFER	; start of the character line edit buffer
;			; ldx	#SCREEN_BUFFER	; point to the start of the display buffer
;			ldx	EDLIN_ANCH	; point to the start of the display buffer
;			lda	EDT_BFR_CSR	; fetch the cursor position
;			pshs	a		; save it on the stack as a local variable	
;			bne	1f		; cursor position is not zero, begin text
;			lda	,y		; load the first character in the buffer
;			bne	4f		; if its not zero, display the cursor
;	1	; start of the line
;			lda	EDT_BFR_CSR	; load the cursor position
;			beq	3f		; if theres no character, display the cursor
;	2	; display up to the cursor position
;			lda	,y+		; fetch a character from the edit buffer
;			beq	3f		; null character (end of line)
;			ldb	#$B4		; color attribute
;			std	,x++		; display the character with its color	
;			dec	0,s		; decrease the local cursor position
;			beq	4f		; brach if the cursor is over a character
;			bra	2b		; continue displaying characters
;	3	; cursor at end of the line
;			puls	a		; repair the stack
;			lda	#$20		; load a space
;			sta	,x++		; display the space
;			bsr	flash_the_cursor	
;			bra	6b		; loop and display changes in the edit buffer
;	4	; cursor over a character
;			puls	a		; clean up the stack
;			lda	,y+		; load the next character from the buffer
;			sta	,x++		; ignore the attribute
;		; flash the cursor
;			bsr	flash_the_cursor
;	5	; finish the line
;			lda	,y+		; load the next character from the buffer
;			beq	6b		; loop back to the top if done
;			ldb	#$b4		; load the basic screen attribute
;			std	,x++		; display the next character with its color
;			bra	5b		; continue until the line is finished
;	
;	flash_the_cursor
;			pshs	a,b,x,u		; save these on the stack
;			ldu	EDLIN_ANCH	; point to the edline start of line anchor
;			lda	SYS_TIMER+1	; load the LSB of the system timer
;			anda	#$07		; only concerned with these bits
;			bne	1f		; if non-zero, skip color change
;			lda	CSR_ATTR	; load the next cursor color
;			inca			; increment it
;			anda	#$0f		; mask off the foreground color
;			ora	#$f0		; foreground 15 = opaque black
;			sta	CSR_ATTR	; store the new attribute
;			ldb	EDT_BFR_CSR	; load the cursor position
;			lslb			; skip the attribue
;			incb			; increment the attribute color
;			sta	b, u		; update the characters color
;			decb			; now index the character
;			leau	b,u		; load that characters address
;			stu	CSR_ADDR	; store it on the zero-page
;	1	; clean up and return	
;			puls	a,b,x,u		; pop these off the stack
;			rts			; return when done


			org	$fe00
prompt0			fcn	"Retro 6809 v0.1\n"
prompt1			fcn	"Copyright (C) 2023 By Jay Faries\n\n"
ready_prompt		fcn	"Ready\n"




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
