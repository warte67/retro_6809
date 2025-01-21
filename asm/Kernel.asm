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


		        INCLUDE "Kernel_Header.asm"

		        org 	KERNEL_START
KRNL_START	    bra	    KRNL_BEGIN

KRNL_VERS_FCN	fcn	    "Kernel Version: 0.0.1"


* ; *****************************************************************************
* ; * SOFTWARE JUMP VECTORS                                                     *
* ; *****************************************************************************
SVCT_START	    fdb	#0	; (VECT_EXEC) The user EXEC vector
        	    fdb	#0	; (VECT_SWI3) SWI3 Software Interrupt Vector	
        	    fdb	#0	; (VECT_SWI2) SWI2 Software Interrupt Vector
        	    fdb	#0	; (VECT_FIRQ) FIRQ Software Interrupt Vector
        	    fdb	#0	; (VECT_IRQ)  IRQ Software Interrupt Vector
        	    fdb	#0	; (VECT_SWI)  SWI / SYS Software Interrupt Vector
        	    fdb	#0	; (VECT_NMI)  NMI Software Interrupt Vector
        	    fdb	#0	; (VECT_RESET RESET Software Interrupt Vector	
SVCT_END            


; *****************************************************************************
; * DEFAULT VECTORS                                                           *
; *****************************************************************************
EXEC_start	    jmp	EXEC_start	; EXEC program
SWI3_start	    jmp	SWI3_start	; SWI3 Implementation
SWI2_start	    jmp	SWI2_start	; SYS (SWI2) Implementation
FIRQ_start	    jmp	FIRQ_start	; FIRQ Implementation
IRQ_start	    jmp	IRQ_start	; IRQ Implementation
SWI_start	    jmp	SWI_start	; SWI / SYS Implementation
NMI_start	    jmp	NMI_start	; NMI Implementation
RESET_start	    jmp	RESET_start	; RESET Implementation


; *******************************************************************************
; * KRNL_BEGIN                                                                  *
; * 	This is the primary entry point for the Kernel Rom.                     *
; *                                                                             *
; * ENTRY REQUIREMENTS: NONE                                                    *
; *                                                                             *
; * EXIT CONDITIONS:	None                                                    *
; * EXIT CONDITIONS:    None                                                    *
; *                                                                             *
; *******************************************************************************
KRNL_BEGIN	    ; common start up code
		        ldd	    SOFT_RESET          ; load the soft reset vector
		        cmpd	#0                  ; has it already been initialized?
		        beq	    KRNL_COLD           ; no? Well then, do a cold start
		        jmp	    [SOFT_RESET]        ; yes? Follow the warm reset vector	
                ; ...		
KRNL_COLD	    ; cold reset
		        ldx	    #KRNL_HARD_VECT     ; start of the hardcoded CPU vectors
		        ldu	    #SOFT_VECTORS_DEVICE; start of the software vectors
1		        ldd	    ,x++                ; copy from hardcoded CPU vectors
		        std	    ,u++                ; copy to the software vectors
		        cmpx	#KRNL_HARD_VECT_END ; at the end yet?
		        blt	    1b                  ; nope, keep going
		        ldd	    #KRNL_WARM          ; fetch the warm reboot vector
		        std	    SOFT_RESET          ; ... and store it appropriately
                ; ...		
		        ; clear out system memory
 		        ldx	    #SYSTEM_MEMORY_DEVICE   ; start of system memory
		        ldd	    #$0000              ; clear out D
1		        stx	    VIDEO_START			; cycle first character to show progress
		        std	    ,x++                ; clear out the next word of system memory
		        cmpx	#KERNEL_ROM_DEVICE  ; at the end yet?
		        bne	    1b                  ; nope, keep going
                ; ...		
KRNL_WARM	    ; warm reboot
		        lds	    #SSTACK_TOP         ; give the stack a home
		        lda	    #$4B	            ; $4B = green on dk.green
		        sta	    _ATTRIB             ; set the default text color attribute
                ; ...		
		        ; set default video
		        lda	    #%11110001          ; (bit 7)   Extended Bitmap, 
                                            ; (bit 5-6) 256-color, 
                                            ; (bit 4)   Extended Display Enabled, 
                                            ; (bit 3)   Emulation in Windowed Mode, 
                                            ; (bit 2)   Vsync Disabled, 
                                            ; (bit 1)   Presentation (Overscan/Stretch)                                            
                                            ; (bit 0)   Standard Display Enabled
		        ldb	    #%01100000          ; (bit 7)   Standard Text Display
                                            ; (bit 5-6) Color Mode (11=256-Color) 
                                            ; (bit 0-4) Video Mode 0 (40x25 / 320x200)
		        std	    GPU_OPTIONS         ; write to the GPU_OPTIONS and GPU_MODE
                                            ; ... registers to set the video mode                
                ; ...		
		        ; set up the initial display
		        sys	    #CALL_CLS           ; Clear the Text Screen
                ldx     #KRNL_VERS_FCN      ; Point X to the Version Text
                sys     #CALL_LINEOUT       ; Display the Text
                ; ...		
                ; enable the mouse cursor
                lda     CSR_FLAGS           ; load the mouse cursor flags
                ora     #%1000'0000          ; set the enable bit
                sta     CSR_FLAGS           ; update the cursor flags
            

                ; ...
                ; infinite loop (for now)
KRNL_INF	    jmp 	KRNL_INF			


; *******************************************************************************
; * System Call Handler:                                                        *
; *                                                                             *
; *	References the byte immediately following the SWI2 instruction and          *
; * 	dispatches to the appropriate system call based on that value.          *
; *                                                                             *
; * ENTRY REQUIREMENTS: Varies                                                  *
; *                                                                             *
; * EXIT CONDITIONS:	Varies                                                  *
; *                                                                             *
; * Notes: (See Kernel_Header.asm)                                              *
; *     Common Stack Offsets:                                                   *
; *         CC  =  0,S                                                          *
; *         D   =  1,S                                                          *
; *         A   =  1,S                                                          *
; *         B   =  2,S                                                          *
; *         DP  =  3,S                                                          *
; *         X   =  4,S                                                          *
; *         Y   =  6,S                                                          *
; *         U   =  8,S                                                          *
; *         PC  = 10,S                                                          *
; *                                                                             *
; *******************************************************************************
KRNL_SYS_CALLS	fdb	    SYS_GARBAGE	    ; $00 random garbage
                fdb	    SYS_CLS         ; $01 CLS 	    (void)
                fdb	    SYS_CHROUT	    ; $02 CHROUT	(void)
                fdb     SYS_NEWLINE     ; $03 NEWLINE   (void)
                fdb     SYS_TAB         ; $04 TAB       (void)
                fdb     SYS_LINEOUT     ; $05 LINEOUT   Entry: X=string
                fdb     SYS_CSRPOS      ; $06 CSRPOS    Exit: Character Display Address in X
                fdb     SYS_SCROLL      ; $07 SCROLL    (void)                
KRNL_SYS_CALLS_END				
		

SYS_Handler	; increment the return address on the stack past the command byte
		        ldu	    $000a,S         ; fetch the command that follows the SWI2
		        ldb	    0,U             ; load the command into B
		        leau	1,U             ; address just past the command byte
		        stu	    $000a,S         ; update the return address in the stack
		        ldu	    #KRNL_SYS_CALLS ; system call vector base address
		        lslb                    ; each address is two bytes long
		        leau	B,U             ; U = system call vector effective address
		        cmpu	#KRNL_SYS_CALLS_END ; Bounds check
		        bge	    SYS_HNDLR_DONE  ; system call out of bounds; error
		        jmp	    [,U]            ; take the appropriate system call vector
SYS_HNDLR_DONE	; Error Condition -- System Call Out of Bounds
                ; ...  (ToDo)
                jmp     KRNL_GARBAGE    ; temporary fatal error
                ; ...
                rti ; Reminder: This Sub is an Interrupt


; This is just a temporary place holder for a terminal system crash.
; cycle video memory (infinite loop)
SYS_GARBAGE     jsr     KRNL_GARBAGE    ; call the kernel error handler (temp)
                rti                     ; return from the sys interrupt
KRNL_GARBAGE			
	            ldd	    #$0100          ; initialize a starting attribute/character pair
1	            ldx	    #VIDEO_START    ; point to the start of the text display
	            addd	#1              ; increment the attribute/character to display
2	            std	    ,x++            ; store the colored character to the next cell
	            addd	#1              ; increment the attribute/character to display
	            cmpx	GPU_VIDEO_MAX   ; at the end of displayed video memory?
	            blt	    2b              ; nope, keep going with the next character
	            bra	    1b              ; yup, start over.


; *****************************************************************************
; * KRNL_CLS                                                                  *
; * 	Clears the currently displayed screen buffer                          *
; *                                                                           *
; * ENTRY REQUIREMENTS: Screen will be cleared with spaces with the color     *
; *	stored in _ATTRIB.						      *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved                               *
; *****************************************************************************
SYS_CLS         jsr     KRNL_CLS        ; call the kernel Clear Screen handler
                rti                     ; return from the sys interrupt
                ; ...
KRNL_CLS	    pshs	d,x		        ; save the used registers onto the stack
		        ldb	    _ATTRIB		    ; fetch the current color attribute
		        lda	    #' '		    ; the space character
		        ldx	    #VIDEO_START	; index the start of the video buffer
1		        std	    ,x++		    ; store a character to the buffer
		        cmpx	GPU_VIDEO_MAX	; are we at the end yet?
		        blt	    1b		        ; nope, keep storing characters
                clr     _CURSOR_COL     ; clear the current cursor position ...
                clr	    _CURSOR_ROW     ; ... column and row the home (top/left)
		        puls	d,x,pc		    ; cleanup and return


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
SYS_CHROUT      lda     1,S             ; fetch A from the stack
                jsr     KRNL_CHROUT     ; call the character out kernel handler
                rti                     ; return from the sys interrupt
                ; ...
KRNL_CHROUT	    pshs	d,x,cc		    ; save the used registers onto the stack
		        ldb	    _ATTRIB		    ; load the current color attribute
K_CHROUT_1	    tsta			        ; is A a null?
		        beq	    K_CHROUT_DONE	;    A is null, just return and do nothing		
		        cmpa	#$0A		    ; is it a newline character?
		        bne	    K_CHROUT_2	    ; nope, don't do a newline
		        jsr	    KRNL_NEWLINE	; advance the cursor 
		        bra	    K_CHROUT_DONE	; clean up and return
K_CHROUT_2	    cmpa	#$09		    ; is it a tab character?
		        bne	    K_CHROUT_0	    ; nope, don't do a tab
		        jsr	    KRNL_TAB	    ; tab the character position
		        bra	    K_CHROUT_DONE	; clean up and return
K_CHROUT_0	    jsr	    KRNL_CSRPOS	    ; position X at the cursor position
		        std	    ,x		        ; display the character/attribute combo
		        inc	    _CURSOR_COL	    ; increment current cursor column position
		        lda	    _CURSOR_COL	    ; load current cursor column position					
		        cmpa	GPU_TCOLS	    ; compare with the current screen columns
		        blt	    K_CHROUT_DONE	; if the csr column is okay, we're done
		        jsr	    KRNL_NEWLINE	; perform a new line
K_CHROUT_DONE	puls	d,x,cc,pc	    ; cleanup and return


; *****************************************************************************
; * KRNL_NEWLINE                                                              *
; * 	Perfoms a CR/LF ($0A) on the console. Advances the current            *
; *     cursor position and scrolls the console if needed.                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
SYS_NEWLINE     jsr     KRNL_NEWLINE    ; call the newline kernel handler
                rti                     ; return from the sys interrupt
                ; ...
KRNL_NEWLINE	pshs	D,X		        ; save the used registers onto the stack
		        clr	    _CURSOR_COL	    ; carrage return (move to left edge)
		        inc	    _CURSOR_ROW	    ; increment the cursors row
		        lda	    _CURSOR_ROW	    ; load the current row
		        cmpa	GPU_TROWS	    ; compared to the current screen rows
		        blt	    K_NEWLINE_DONE	; clean up and return if less than
		        dec	    _CURSOR_ROW	    ; move the cursor the the bottom row
		        jsr	    KRNL_SCROLL	    ; scroll the text screen up one line
K_NEWLINE_DONE	puls	D,X,PC		    ; restore the saved registers and return


; *****************************************************************************
; * KRNL_TAB                                                                  *
; * 	Perfoms a tab ($0A) on the console. Advances the current              *
; *     cursor position and scrolls the console if needed.                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
SYS_TAB         jsr     KRNL_TAB        ; call the tab character kernel handler
                rti                     ; return from the sys interrupt
                ; ...
KRNL_TAB        pshs	b		        ; save B
                ldb     _CURSOR_COL	    ; Fetch the current cursor col
                addb    #4              ; Move cursor by 4 spaces
                andb    #%11111100      ; Align to the next tab stop
                stb     _CURSOR_COL	    ; update the cursor column
                cmpb    GPU_TCOLS       ; Ensure column is within bounds
                blt     K_TAB_DONE	    ; Within bounds, we're done
                jsr     KRNL_NEWLINE   	; Handle line wrapping
K_TAB_DONE	    puls	B,PC		    ; cleanup and return


; *******************************************************************************
; * KRNL_LINEOUT                                                                *
; * 	Outputs a string to the console                                         *
; *                                                                             *
; * ENTRY REQUIREMENTS: X = String starting address                             *
; *                         (null terminated)                                   *
; *                                                                             *
; * EXIT CONDITIONS:	All registers preserved.                                *
; *******************************************************************************
SYS_LINEOUT     ldx     4,S             ; fetch X from the stack
                jsr     KRNL_LINEOUT    ; call the line out kernel handler
                rti                     ; return from the sys interrupt
                ; ...
KRNL_LINEOUT	pshs	D,X,U 		    ; save the used registers onto the stack
		        tfr	    X,U		        ; move X to U
		        jsr	    KRNL_CSRPOS	    ; set X to the cursor position 
K_LINEOUT_0	    lda	    ,U+		        ; fetch the next character
		        beq	    K_LINEOUT_DONE	; cleanup and return if null-terminator		
		        jsr	    KRNL_CHROUT	    ; send the character to the console
		        leax	1,X		        ; point to the next character
		        bra	    K_LINEOUT_0	    ; continue looping until done
K_LINEOUT_DONE	puls	D,U,X,PC 	    ; restore the saved registers and return


; *******************************************************************************
; * KRNL_CSRPOS                                                                 *
; * 	Loads into X the cursor position                                        *
; *                                                                             *
; * ENTRY REQUIREMENTS:                                                         *
; *     _CURSOR_COL     =   (Byte) Current Text Cursor Column Position          *
; *     _CURSOR_ROW     =   (Byte) Current Text Cursor Row Position             *
; *                                                                             *
; * EXIT CONDITIONS:	X = The address within the text                         *
; *                         where the cursor is positioned.                     *   
; *                     All other registers preserved.                          *
; *******************************************************************************
SYS_CSRPOS      jsr     KRNL_CSRPOS     ; call the CSRPOS kernel subroutine
                stx     4,S             ; replace X in the stack so it has ...
                rti                     ; ... valid info on return
                ; ...
KRNL_CSRPOS	    pshs	D		        ; save the used registers onto the stack
		        lda	    _CURSOR_ROW	    ; current cursor row
		        ldb	    GPU_TCOLS	    ; current text columns
		        lslb			        ; times two (account for the attribute)
		        mul			            ; row * columns
		        ldx	    #VIDEO_START	; the buffer starting address
		        leax	D,X		        ; add the video base address
		        ldb	    _CURSOR_COL	    ; load the current cursor column
		        lslb			        ; times two (account for the attribute)
		        clra			        ; don't let B become negative, use D
		        leax	D,X		        ; add the column to the return address
		        puls	D,PC		    ; restore the saved registers and return

; *****************************************************************************
; * KRNL_SCROLL                                                               *
; * 	Scroll the text screen up one line and blank the bottom line.         *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
SYS_SCROLL      jsr     KRNL_SCROLL     ; call the text screen scroll handler
                rti                     ; return from the sys interrupt
                ; ...
KRNL_SCROLL	    pshs	d,x,u		    ; save the used registers onto the stack
		        ldx	#VIDEO_START	    ; set X to the start of the video buffer
		        tfr	x,u		            ; copy X into U
		        ldb	GPU_TCOLS	        ; B = Screen Columns
		        lslb			        ; account for the attribute byte
		        clra			        ; MSB of D needs to not be negative
		        leau	d,u		        ; U is now one line below X
K_SCROLL_0	    ldd	,u++		        ; load a character from where U points
		        std	,x++		        ; store it to where X points
		        cmpu	GPU_VIDEO_MAX	; has U exceeded the screen buffer
		        blt	K_SCROLL_0	        ; continue looping of not
		        lda	#' '		        ; set SPACE as the current character
K_SCROLL_1	    sta	,x++		        ; and store it to where X points
		        cmpx	GPU_VIDEO_MAX	; continue looping until the bottom ...
		        blt	K_SCROLL_1	        ; ... line has been cleared
		        tst	EDT_ENABLE	        ; are we using the line editor?
		        beq	K_SCROLL_DONE	    ; nope, just clean up and return
		        dec	_ANCHOR_ROW	        ; yup, decrease the anchor row by one
K_SCROLL_DONE	puls	d,x,u,pc	    ; restore the registers and return



; *******************************************************************************
; * SUBROUTINE_TEMPLATE                                                         *
; * 	xxxxxxxxxxxxxxxxxx                                                      *
; *                                                                             *
; * ENTRY REQUIREMENTS: A = xxxxxxxxxxx                                         *
; *                     B = xxxxxxxxxxx                                         *
; *                                                                             *
; * EXIT CONDITIONS:	D = Result                                              *
; * EXIT CONDITIONS:    All registers preserved                                 *
; *                     All other registers preserved                           *
; *******************************************************************************






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
