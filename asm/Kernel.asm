;*** kernel.asm ************************
 *      _  __                    _                           
 *     | |/ /___ _ __ _ __   ___| |       __ _ ___ _ __ ___  
 *     | ' // _ \ '__| '_ \ / _ \ |      / _` / __| '_ ` _ \ 
 *     | . \  __/ |  | | | |  __/ |  _  | (_| \__ \ | | | | |
 *     |_|\_\___|_|  |_| |_|\___|_| (_)  \__,_|___/_| |_| |_|
;*                
;* Released under the GPL v3.0 License.
;* Original Author: Jay Faries (warte67)                                           
;*****************************






		; INCLUDE "Memory_Map.asm"
		INCLUDE "Kernel_Header.asm"

		org 	KERNEL_START
KRNL_START	bra	KRNL_BEGIN

krnl_vers_label		fcn	"Kernel Version: "
krnl_vers_number	fcn	"0.0.1"


* ; *****************************************************************************
* ; * SOFTWARE JUMP VECTORS                                                     *
* ; *****************************************************************************
SVCT_EXEC	fdb	#0	; (VECT_EXEC) The user EXEC vector
SVCT_SWI3 	fdb	#0	; (VECT_SWI3) SWI3 Software Interrupt Vector	
SVCT_SWI2 	fdb	#0	; (VECT_SWI2) SWI2 Software Interrupt Vector
SVCT_FIRQ 	fdb	#0	; (VECT_FIRQ) FIRQ Software Interrupt Vector
SVCT_IRQ  	fdb	#0	; (VECT_IRQ)  IRQ Software Interrupt Vector
SVCT_SWI  	fdb	#0	; (VECT_SWI)  SWI / SYS Software Interrupt Vector
SVCT_NMI  	fdb	#0	; (VECT_NMI)  NMI Software Interrupt Vector
SVCT_RESET	fdb	#0	; (VECT_RESET RESET Software Interrupt Vector	

; *****************************************************************************
; * DEFAULT VECTORS                                                           *
; *****************************************************************************
EXEC_start	jmp	EXEC_start	; EXEC program
SWI3_start	jmp	SWI3_start	; SWI3 Implementation
SWI2_start	jmp	SWI2_start	; SYS (SWI2) Implementation
FIRQ_start	jmp	FIRQ_start	; FIRQ Implementation
IRQ_start	jmp	IRQ_start	; IRQ Implementation
SWI_start	jmp	SWI_start	; SWI / SYS Implementation
NMI_start	jmp	NMI_start	; NMI Implementation
RESET_start	jmp	RESET_start	; RESET Implementation

; *****************************************************************************
; * KERNAL ROUTINE SOFTWARE VECTORS                                           *
; *****************************************************************************
SYSTEM_DATA_START
		fdb	STUB_CLS	; VECT_CLS	
		fdb	STUB_CHROUT	; VECT_CHROUT	
		fdb	STUB_NEWLINE	; VECT_NEWLINE	
		fdb	STUB_LINEOUT	; VECT_LINEOUT	
		fdb	STUB_CSRPOS	; VECT_CSRPOS	
		fdb	STUB_SCROLL	; VECT_SCROLL	
		* fdb	STUB_LINEEDIT	; VECT_LINEEDIT	
		* fdb	STUB_GETKEY	; VECT_GETKEY	
		* fdb	STUB_GETHEX	; VECT_GETHEX	
		* fdb	STUB_GETNUM	; VECT_GETNUM	
		* fdb	STUB_CMPSTR	; VECT_CMPSTR	
		* fdb	STUB_CMD_PROC	; VECT_CMD_PROC	
		* fdb	STUB_TBLSEARCH	; VECT_TBLSEARCH	
		* fdb	STUB_CPY_DWORD	; VECT_CPY_DWORD	
		* fdb	STUB_D_TO_RAWA	; VECT_D_TO_RAWA	
		* fdb	STUB_D_TO_RAWB	; VECT_D_TO_RAWB	
		* fdb	STUB_D_TO_RAWR	; VECT_D_TO_RAWR	
		* fdb	STUB_D_TO_INTA	; VECT_D_TO_INTA	
		* fdb	STUB_D_TO_INTB	; VECT_D_TO_INTB	
		* fdb	STUB_D_TO_INTR	; VECT_D_TO_INTR	
		* fdb	STUB_RAWA_TO_D	; VECT_RAWA_TO_D	
		* fdb	STUB_RAWB_TO_D	; VECT_RAWB_TO_D	
		* fdb	STUB_RAWR_TO_D	; VECT_RAWR_TO_D	
		* fdb	STUB_INTA_TO_D	; VECT_INTA_TO_D	
		* fdb	STUB_INTB_TO_D	; VECT_INTB_TO_D	
		* fdb	STUB_INTR_TO_D	; VECT_INTR_TO_D	
		* fdb	STUB_8BIT_MATH	; VECT_8BIT_MATH	
		* fdb	STUB_DSP_ACA	; VECT_DSP_ACA	
		* fdb	STUB_DSP_ACB	; VECT_DSP_ACB	
		* fdb	STUB_DSP_ACR	; VECT_DSP_ACR	
		* fdb	STUB_DSP_INTA	; VECT_DSP_INTA	
		* fdb	STUB_DSP_INTB	; VECT_DSP_INTB	
		* fdb	STUB_DSP_INTR	; VECT_DSP_INTR	
		* fdb	STUB_WRITE_ACA	; VECT_WRITE_ACA	
		* fdb	STUB_WRITE_ACB	; VECT_WRITE_ACB	
		* fdb	STUB_WRITE_ACR	; VECT_WRITE_ACR	
		* fdb	STUB_ARG_TO_A	; VECT_ARG_TO_A	
SYSTEM_DATA_END


; ---------------------------------------------------------------------

KRNL_BEGIN	; warm reset?
		ldd	SOFT_RESET
		cmpd	#0
		beq	KRNL_COLD
		jmp	[SOFT_RESET]
			
KRNL_COLD	; cold reset
		ldx	#KRNL_HARD_VECT
		ldu	#SOFT_VECTORS_DEVICE
1		ldd	,x++
		std	,u++
		cmpx	#KRNL_HARD_VECT_END
		blt	1b
		ldd	#KRNL_WARM
		std	SOFT_RESET

		; ...
		; clear out system memory
 		ldx	#SYSTEM_MEMORY_DEVICE
		ldu	#VIDEO_START
		ldd	#$0000
1		stx	,u			; cycle first character to show progress
		std	,x++
		cmpx	#KERNEL_ROM_DEVICE
		bne	1b

		; initialize the system	vectors
		ldx	#SYSTEM_DATA_START
		ldy	#$0010
2		lda	,x+
		sta	,y+
		cmpx	#SYSTEM_DATA_END
		blt	2b			


		; ...
KRNL_WARM	; common start up code
		;
		; set the SP
		lds	#SSTACK_TOP
		; ...

		lda	#$4B	; $4B = green on dk.green
		sta	_ATTRIB

		; set default video
		lda	#%11110001
		ldb	#%01100000
		std	GPU_OPTIONS

		; make a system call
		sys	#$00

		clr	_CURSOR_COL
		clr	_CURSOR_ROW
		ldx 	#krnl_vers_label
		jsr	KRNL_LINEOUT
		ldx 	#krnl_vers_number
		jsr	KRNL_LINEOUT

krnl_warm_reset_inf	jmp 	krnl_warm_reset_inf			



	; cycle video memory
KRNL_GARBAGE			
	ldd	#$0100
1	ldx	#VIDEO_START		
	addd	#1
2	std	,x++
	addd	#1
	cmpx	GPU_VIDEO_MAX
	blt	2b
	bra	1b




; ** krnl_swi2 *******************************************************************
; * 
; * System Call Handler: 
; *	References the byte immediately following the SWI2 instruction and
; * 	dispatches to the appropriate system call based on that value.
; *
; *
; *********************************************************************************
KRNL_SYS_CALLS	fdb	KRNL_CLS	; $00 CLS 	(void)
		fdb	KRNL_CHROUT	; $01 CHROUT	(void)
		fdb	KRNL_LINEOUT	; $02 LINEOUT	(void) 
		fdb	KRNL_GARBAGE	; $N random garbage
KRNL_SYS_CALLS_END				
		

SYS_Handler	; increment the return address on the stack past the command byte
		ldu	$000a,s
		ldb	0,u
		leau	1,u
		stu	$000a,s
		ldx	#KRNL_SYS_CALLS
		lslb
		leax	b,x
		cmpx	#KRNL_SYS_CALLS_END
		bge	SYS_HNDLR_DONE
		jsr	[,x]
SYS_HNDLR_DONE	rti


; * TODO:
; *  	-) Remove the SYSTEM_DATA_START/SYSTEM_DATA_END software vector block.
; *	-) Remove the KERNAL ROUTINE SOFTWARE VECTORS from Kernel_Header.asm
; *	-) Rename the KRNL_SYS_CALLS to SYS_CALL_ prefix
; *		-> Remove the stack save and restore. Convert these to SYS calls with RTI.
; *		-> Update the stack to reflect return values from each function.
; *		-> Make a stack storage chart to assist in updating stacked values.



; *****************************************************************************
; * KRNL_CLS                                                                  *
; * 	Clears the currently displayed screen buffer                          *
; *                                                                           *
; * ENTRY REQUIREMENTS: Screen will be cleared with spaces with the color     *
; *	stored in _ATTRIB.						      *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved                               *
; *****************************************************************************
KRNL_CLS	jmp	[VEC_CLS]	; proceed through the software vector
STUB_CLS	pshs	d,x		; save the used registers onto the stack
		ldb	_ATTRIB		; fetch the current color attribute
		lda	#' '		; the space character
		ldx	#VIDEO_START	; index the start of the video buffer
1		std	,x++		; store a character to the buffer
		cmpx	GPU_VIDEO_MAX	; are we at the end yet?
		blt	1b		; nope, keep storing characters
		puls	d,x,pc		; cleanup and return



; *****************************************************************************
; * KRNL_CHROUT                                                               *
; * 	Outputs a character to the console at the current cursor              *
; *     position. This routine should update the cursors postion              *
; *     and handle text scrolling as needed.                                  *
; *                                                                           *
; * ENTRY REQUIREMENTS: A = Character to be displayed                         *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved                               *
; *****************************************************************************
KRNL_CHROUT	jmp	[VEC_CHROUT]	; proceed through the software vector
STUB_CHROUT	pshs	d,x,cc		; save the used registers onto the stack
		ldb	_ATTRIB		; load the current color attribute
K_CHROUT_1	tsta			; is A a null?
		beq	K_CHROUT_DONE	;    A is null, just return and do nothing		
		cmpa	#$0A		; is it a newline character?
		bne	K_CHROUT_2	; nope, don't do a newline
		jsr	KRNL_NEWLINE	; advance the cursor 
		bra	K_CHROUT_DONE	; clean up and return
K_CHROUT_2	cmpa	#$09		; is it a tab character?
		bne	K_CHROUT_0	; nope, don't do a tab
		jsr	KRNL_TAB	; tab the character position
		bra	K_CHROUT_DONE	; clean up and return
K_CHROUT_0	jsr	KRNL_CSRPOS	; position X at the cursor position
		std	,x		; display the character/attribute combo
		inc	_CURSOR_COL	; increment current cursor column position
		lda	_CURSOR_COL	; load current cursor column position					
		cmpa	GPU_TCOLS	; compare with the current screen columns
		blt	K_CHROUT_DONE	; if the csr column is okay, we're done
		jsr	KRNL_NEWLINE	; perform a new line
K_CHROUT_DONE	puls	d,x,cc,pc	; cleanup and return

		cmpx	#KRNL_SYS_CALLS_END
		bge	SYS_DONE
		jsr	[,x]
SYS_DONE	rti


; * TODO:
; *  	-) Remove the SYSTEM_DATA_START/SYSTEM_DATA_END software vector block.
; *	-) Remove the KERNAL ROUTINE SOFTWARE VECTORS from Kernel_Header.asm
; *	-) Rename the KRNL_SYS_CALLS to SYS_CALL_ prefix
; *		-> Remove the stack save and restore. Convert these to SYS calls with RTI.
; *		-> Update the stack to reflect return values from each function.
; *		-> Make a stack storage chart to assist in updating stacked values.



; *****************************************************************************
; * KRNL_NEWLINE                                                              *
; * 	Perfoms a CR/LF ($0A) on the console. Advances the current            *
; *     cursor position and scrolls the console if needed.                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
KRNL_NEWLINE	jmp	[VEC_NEWLINE]	; proceed through the software vector
STUB_NEWLINE	pshs	D,X		; save the used registers onto the stack
		clr	_CURSOR_COL	; carrage return (move to left edge)
		inc	_CURSOR_ROW	; increment the cursors row
		lda	_CURSOR_ROW	; load the current row
		cmpa	GPU_TROWS	; compared to the current screen rows
		blt	K_NEWLINE_DONE	; clean up and return if less than
		dec	_CURSOR_ROW	; move the cursor the the bottom row
		jsr	KRNL_SCROLL	; scroll the text screen up one line
K_NEWLINE_DONE	puls	D,X,PC		; restore the saved registers and return


; *****************************************************************************
; * KRNL_TAB                                                                  *
; * 	Perfoms a tab ($0A) on the console. Advances the current              *
; *     cursor position and scrolls the console if needed.                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
KRNL_TAB  ; Tab ($09)
		pshs	b		; save B
		ldb     _CURSOR_COL	; Fetch the current cursor col
		addb    #4              ; Move cursor by 4 spaces
		andb    #%11111100      ; Align to the next tab stop
		stb     _CURSOR_COL	; update the cursor column
		cmpb    GPU_TCOLS       ; Ensure column is within bounds
		blt     K_TAB_DONE	; Within bounds, we're done
		jsr     KRNL_NEWLINE   	; Handle line wrapping
K_TAB_DONE	puls	B,PC		; cleanup and return


; *****************************************************************************
; * KRNL_LINEOUT                                                              *
; * 	Outputs a string to the console                                       *
; *                                                                           *
; * ENTRY REQUIREMENTS: X = String starting address                           *
; *                         (null or neg terminated)                          *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
KRNL_LINEOUT	jmp	[VEC_LINEOUT]	; proceed through the software vector
STUB_LINEOUT	pshs	D,U,X		; save the used registers onto the stack
		tfr	x,u		; move X to U
		jsr	KRNL_CSRPOS	; set X to the cursor position 
K_LINEOUT_0	lda	,u+		; fetch the next character
		beq	K_LINEOUT_DONE	; cleanup and return if null-terminator		
		jsr	KRNL_CHROUT	; send the character to the console
		leax	1,x		; point to the next character
		bra	K_LINEOUT_0	; continue looping until done
K_LINEOUT_DONE	puls	D,U,X,pc	; restore the saved registers and return


; *****************************************************************************
; * KRNL_CSRPOS                                                              *
; * 	Loads into X the cursor position                                      *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	X = The address within the text                       *
; *                         where the cursor is positioned.                   *   
; *                     All other registers preserved.                        *
; *****************************************************************************
KRNL_CSRPOS	jmp	[VEC_CSRPOS]	; proceed through the software vector
STUB_CSRPOS	pshs	d		; save the used registers onto the stack
		lda	_CURSOR_ROW	; current cursor row
		ldb	GPU_TCOLS	; current text columns
		lslb			; times two (account for the attribute)
		mul			; row * columns
		ldx	#VIDEO_START	; the buffer starting address
		leax	d,x		; add the video base address
		ldb	_CURSOR_COL	; load the current cursor column
		lslb			; times two (account for the attribute)
		clra			; don't let B become negative, use D
		leax	d,x		; add the column to the return address
		puls	d,pc		; restore the saved registers and return

; *****************************************************************************
; * KRNL_SCROLL                                                               *
; * 	Scroll the text screen up one line and blank the bottom line.         *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
KRNL_SCROLL	jmp	[VEC_SCROLL]	; proceed through the software vector
STUB_SCROLL	pshs	d,x,u		; save the used registers onto the stack
		ldx	#VIDEO_START	; set X to the start of the video buffer
		tfr	x,u		; copy X into U
		ldb	GPU_TCOLS	; B = Screen Columns
		lslb			; account for the attribute byte
		clra			; MSB of D needs to not be negative
		leau	d,u		; U is now one line below X
K_SCROLL_0	ldd	,u++		; load a character from where U points
		std	,x++		; store it to where X points
		cmpu	GPU_VIDEO_MAX	; has U exceeded the screen buffer
		blt	K_SCROLL_0	; continue looping of not
		lda	#' '		; set SPACE as the current character
K_SCROLL_1	sta	,x++		; and store it to where X points
		cmpx	GPU_VIDEO_MAX	; continue looping until the bottom ...
		blt	K_SCROLL_1	; ... line has been cleared
		tst	EDT_ENABLE	; are we using the line editor?
		beq	K_SCROLL_DONE	; nope, just clean up and return
		dec	_ANCHOR_ROW	; yup, decrease the anchor row by one
K_SCROLL_DONE	puls	d,x,u,pc	; restore the registers and return






; *****************************************************************************
; * SUBROUTINE_TEMPLATE                                                       *
; * 	xxxxxxxxxxxxxxxxxx                                                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: A = xxxxxxxxxxx                                       *
; *                     B = xxxxxxxxxxx                                       *
; *                                                                           *
; * EXIT CONDITIONS:	D = Result                                            *
; * EXIT CONDITIONS:    All registers preserved                               *
; *                     All other registers preserved                         *
; *****************************************************************************






* ; *****************************************************************************
* ; * ROM BASED HARDWARE VECTORS                                                *
* ; *****************************************************************************
	org	ROM_VECTS_DEVICE
KRNL_HARD_VECT			
	fdb	EXEC_start	; (HARD_RSRVD) EXEC Interrupt Vector
	fdb	SWI3_start	; (HARD_SWI3 ) SWI3 Hardware Interrupt Vector
	fdb	SYS_Handler	; (HARD_SWI2 ) SWI2 Hardware Interrupt Vector
	fdb	FIRQ_start	; (HARD_FIRQ ) FIRQ Hardware Interrupt Vector
	fdb	IRQ_start	; (HARD_IRQ  ) IRQ Hardware Interrupt Vector
	fdb	SWI_start	; (HARD_SWI  ) SWI / SYS Hardware Interrupt Vector
	fdb	NMI_start	; (HARD_NMI  ) NMI Hardware Interrupt Vector
	fdb	KRNL_START	; (HARD_RESET) RESET Hardware Interrupt Vector
KRNL_HARD_VECT_END
