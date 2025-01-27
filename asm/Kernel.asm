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
KRNL_START	jmp     KRNL_BEGIN

; Notes: 
;	fcc 	stores raw character string with no default termination
;	fcs	character string with its terminators high bit set
;	fcn	character string with null termination

KRNL_PROMPT0	fcn	"Retro 6809 Kernel ROM V0.4\n"
KRNL_PROMPT1	fcn	"Emulator compiled "
KRNL_PROMPT2	fcn	"GNU General Public Liscense (GPL V3)\n"
KRNL_PROMPT3	fcn	"Copyright (C) 2024-2025 By Jay Faries\n\n"  
READY_PROMPT	fcn	"Ready\n"


; *****************************************************************************
; * MAIN KERNEL COMMAND SUBROUTINES (Prototypes)                              *
; *****************************************************************************
;	do_cls		; #0		; Clear Screen (0-255) or ($00-$FF)
;	do_color	; #1		; Change Color (0-255) or ($00-$FF)
;	do_load		; #2		; Load an Intel Hex Formatted File
;	do_exec		; #3		; Execute a Loaded Program
;	do_reset	; #4		; Reset the System
;	do_dir		; #5		; Display Files and Folders in a Folder
;	do_cd		; #6		; Change the Current Directory
; 	do_pwd		; #7		; Print Working Directory
;	do_chdir	; #8		; Alias of CD
;	do_exit		; #9		; Exit the Emulator
;	do_quit		; #10		; Also Exits the Emulator
;	do_mode		; #11		; Display Mode (0-31) or ($00-$1F)
;	do_debug	; #12		; Enter or Exit the Debugger
;	do_help		; #13		; Display usage help

KRNL_CMD_TABLE	
                fcn     "cls"	        ; #0
                fcn     "color"	        ; #1
                fcn     "load"	        ; #2
                fcn     "exec"	        ; #3
                fcn     "reset"	        ; #4
                fcn     "dir"	        ; #5
                fcn     "cd"	        ; #6
                fcn     "chdir"	        ; #7
                fcn     "pwd"	        ; #8
                fcn     "exit"	        ; #9
                fcn     "quit"	        ; #10
                fcn     "mode"	        ; #11
                fcn     "debug"	        ; #12
                fcn     "help"	        ; #13
                fcb     $FF	        ; $FF = end of list
                ; ...

KRNL_CMD_VECTS  
                fdb	do_cls		; #0
                fdb	do_color	; #1
                fdb	do_load		; #2
                fdb	do_exec		; #3
                fdb	do_reset	; #4
                fdb	do_dir		; #5
                fdb	do_cd		; #6
                fdb	do_chdir	; #7
                fdb	do_pwd		; #8
                fdb	do_exit		; #9
                fdb	do_quit		; #10
                fdb	do_mode		; #11
                fdb	do_debug	; #12
                fdb	do_help		; #13
                ; ...
KRNL_ERR_NFND 	fcn	"ERROR: Command Not Found\n"
krnl_help_str	fcc	" valid commands are:\n"
                fcc	"  cls,   color, load,\n"
                fcc	"  exec,  reset, dir,\n"
                fcc	"  cd,    chdir, pwd,\n"
                fcc	"  exit,  quit,  mode\n"
                fcn	"  debug, and help\n"

* krnl_help_str	fcc     "cls:   Clear Screen (0-255) or ($00-$FF)\n"
* 		fcc     "color: Change Color (0-255) or ($00-$FF)\n"
* 		fcc     "load:  Load an Intel Hex Formatted File\n"
* 		fcc     "exec:  Execute a Loaded Program\n"
* 		fcc     "reset: Reset the System\n"
* 		fcc     "dir:   Display Files and Folders in a Folder\n"
* 		fcc     "cd:    Change the Current Directory\n"
* 		fcc     "chdir: Alias of CD\n"
* 		fcc     "pwd:   Print Working Directory\n"
* 		fcc     "exit:  Exit the Emulator\n"
* 		fcc     "quit:  Also Exits the Emulator\n"
* 		fcc     "mode:  Display Mode (0-31) or ($00-$1F)\n"
* 		fcc     "debug: Enter or Exit the Debugger\n"
* 		fcn     "help"  Display usage help\n"


; *****************************************************************************
; * KERNAL ROUTINE SOFTWARE VECTORS                                           *
; *****************************************************************************
SYSTEM_DATA_START
                fdb     STUB_CLS        ; VECT_CLS	
                fdb     STUB_CHROUT     ; VECT_CHROUT	
                fdb     STUB_NEWLINE    ; VECT_NEWLINE	
                fdb     STUB_LINEOUT    ; VECT_LINEOUT	
                fdb     STUB_CSRPOS     ; VECT_CSRPOS	
                fdb     STUB_SCROLL     ; VECT_SCROLL	
                fdb     STUB_LINEEDIT   ; VECT_LINEEDIT	
                fdb     STUB_GETKEY     ; VECT_GETKEY	
                fdb     STUB_GETHEX     ; VECT_GETHEX	
                fdb     STUB_GETNUM     ; VECT_GETNUM	
                fdb     STUB_CMPSTR     ; VECT_CMPSTR	
                fdb     STUB_CMD_PROC   ; VECT_CMD_PROC	
                fdb     STUB_TBLSEARCH  ; VECT_TBLSEARCH	
                fdb     STUB_CPY_DWORD  ; VECT_CPY_DWORD	
                fdb     STUB_D_TO_RAWA  ; VECT_D_TO_RAWA	
                fdb     STUB_D_TO_RAWB  ; VECT_D_TO_RAWB	
                fdb     STUB_D_TO_RAWR  ; VECT_D_TO_RAWR	
                fdb     STUB_D_TO_INTA  ; VECT_D_TO_INTA	
                fdb     STUB_D_TO_INTB  ; VECT_D_TO_INTB	
                fdb     STUB_D_TO_INTR  ; VECT_D_TO_INTR	
                fdb     STUB_RAWA_TO_D  ; VECT_RAWA_TO_D	
                fdb     STUB_RAWB_TO_D  ; VECT_RAWB_TO_D	
                fdb     STUB_RAWR_TO_D  ; VECT_RAWR_TO_D	
                fdb     STUB_INTA_TO_D  ; VECT_INTA_TO_D	
                fdb     STUB_INTB_TO_D  ; VECT_INTB_TO_D	
                fdb     STUB_INTR_TO_D  ; VECT_INTR_TO_D	
                fdb     STUB_8BIT_MATH  ; VECT_8BIT_MATH	
                fdb     STUB_DSP_ACA    ; VECT_DSP_ACA	
                fdb     STUB_DSP_ACB    ; VECT_DSP_ACB	
                fdb     STUB_DSP_ACR    ; VECT_DSP_ACR	
                fdb     STUB_DSP_INTA   ; VECT_DSP_INTA	
                fdb     STUB_DSP_INTB   ; VECT_DSP_INTB	
                fdb     STUB_DSP_INTR   ; VECT_DSP_INTR	
                fdb     STUB_WRITE_ACA  ; VECT_WRITE_ACA	
                fdb     STUB_WRITE_ACB  ; VECT_WRITE_ACB	
                fdb     STUB_WRITE_ACR  ; VECT_WRITE_ACR	
                fdb     STUB_ARG_TO_A   ; VECT_ARG_TO_A	
SYSTEM_DATA_END


; *****************************************************************************
; * DEFAULT VECTORS                                                           *
; *****************************************************************************
EXEC_start	    rts                     ; EXEC program
SWI3_start	    jmp     SWI3_start      ; SWI3 Implementation
SWI2_start	    jmp     SWI2_start      ; SYS (SWI2) Implementation
FIRQ_start	    jmp     FIRQ_start      ; FIRQ Implementation
IRQ_start	    jmp     IRQ_start       ; IRQ Implementation
SWI_start	    jmp     SWI_start       ; SWI / SYS Implementation
NMI_start	    jmp     NMI_start       ; NMI Implementation
RESET_start	    jmp     RESET_start     ; RESET Implementation


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
KRNL_BEGIN	; common start up code
                ldd	    SOFT_RESET      ; load the soft reset vector
                cmpd    #0	            ; has it already been initialized?
                beq	    KRNL_COLD       ; no? Well then, do a cold start
                jmp	    [SOFT_RESET]    ; yes? Follow the warm reset vector	
                ; ...		
KRNL_COLD	; cold reset
                ldx	    #KRNL_HARD_VECT         ; start of the hardcoded CPU vectors
                ldu	    #SOFT_VECTORS_DEVICE	; start of the software vectors
1		        ldd	    ,x++                    ; copy from hardcoded CPU vectors
                std	    ,u++                    ; copy to the software vectors
                cmpx    #KRNL_HARD_VECT_END     ; at the end yet?
                blt	    1b                      ; nope, keep going
                ldd	    #KRNL_WARM              ; fetch the warm reboot vector
                std	    SOFT_RESET              ; ... and store it appropriately
                ; CPU clock speed       
                lda     #$0f	                ; set the CPU clock speed
                sta     SYS_STATE	            ;	 to max
                ; ...		
                        ; clear out system memory
                         ldx	    #SYSTEM_MEMORY_DEVICE   ; start of system memory
                        ldd	    #$0000              ; clear out D
1		        stx	    VIDEO_START			; cycle first character to show progress
                        std	    ,x++                ; clear out the next word of system memory
                        cmpx	#USER_RAM_TOP       ; at the end yet?
                        bne	    1b                  ; nope, keep going
                                ; initialize the system	
                                ldx		#SYSTEM_DATA_START
                                ldy		#$0010
1				lda		,x+
                                sta		,y+
                                cmpx	#SYSTEM_DATA_END
                                blt		1b
                                ; CPU clock speed
                                lda		#$0a				; set the default CPU clock speed
                                sta		SYS_STATE			;	to 900 Khz.

                ; ...		
KRNL_WARM	    ; warm reboot
                        lds	    #SSTACK_TOP         ; give the stack a home
                        lda	    #$4B	            ; $4B = green on dk.green
                        sta	    _ATTRIB             ; set the default text color attribute
                ; ...		
                        * ; set default video
                        * lda	    #%11110001          ; (bit 7)   Extended Bitmap, 
                *                             ; (bit 5-6) 256-color, 
                *                             ; (bit 4)   Extended Display Enabled, 
                *                             ; (bit 3)   Emulation in Windowed Mode, 
                *                             ; (bit 2)   Vsync Disabled, 
                *                             ; (bit 1)   Presentation (Letterbox)                                            
                *                             ; (bit 0)   Standard Display Enabled
                        * ldb	    #%01100000          ; (bit 7)   Standard Text Display
                *                             ; (bit 5-6) Color Mode (11=256-Color) 
                *                             ; (bit 0-4) Video Mode 0 (40x25 / 320x200)
                        * std	    GPU_OPTIONS         ; write to the GPU_OPTIONS and GPU_MODE
                *                             ; ... registers to set the video mode                
                ; ...		
                        ; set up the initial display
                        sys	    CALL_CLS           ; Clear the Text Screen
                ldx     #KRNL_PROMPT0       ; Point X to the Kernel Version Text
                sys     CALL_LINEOUT       ; Display the Text
                ldx     #KRNL_PROMPT1       ; Point X to the Compilation Text
                sys     CALL_LINEOUT       ; Display the Text
                lda	    #FC_COMPDATE	    ; command to fetch the compilation date
                sta	    FIO_COMMAND	        ; issue the command to the FileIO device
1               lda	    FIO_PATH_DATA	    ; load a character from the response data
                beq	    2f              	; if we've received a NULL, stop looping
                sys     CALL_CHROUT        ; output the retrieved character to the console
                bra     1b                  ; continue looping while there is still data
2               lda     #$0a                ; line feed character
                sys     CALL_CHROUT        ; send the line feed to the console                
                ldx	    #KRNL_PROMPT2	    ; point to the third prompt line
                sys	    CALL_LINEOUT	    ; output it to the console
                ldx	    #KRNL_PROMPT3	    ; point to the fourth prompt line
                sys	    CALL_LINEOUT	    ; output it to the console
                ; ...		
                ; enable the mouse cursor
                lda     CSR_FLAGS           ; load the mouse cursor flags
                ora     #%1000'0000         ; set the enable bit
                sta     CSR_FLAGS           ; update the cursor flags
            
; *****************************************************************************
; * THE MAIN COMMAND LOOP                                                     *
; *****************************************************************************
; *                                                                           *
; * 	1) Displays the "Ready" prompt                                        *
; *     2) Runs the Command Input Line Editor                                 *
; *     3) Dispatches the Operating System Commands                           *
; *                                                                           *
; *****************************************************************************
KRNL_MAIN_LOOP	ldb	    _ATTRIB             ; fetch the current color attribute
                        ldx	    #READY_PROMPT	    ; the ready prompt
                sys     CALL_LINEOUT        ; output to the console

                lda	    #$7F		        ; Initialize the line editor
                sta	    EDT_BFR_LEN	        ; allow for the full sized buffer
                clr	    EDT_BFR_CSR	        ; set the buffer cursor to the start
                clr	    EDT_BUFFER	        	

                ldx	    #EDT_BUFFER	        ; point to the edit buffer
k_main_clr	    clr	    ,x+		            ; clear an entry and advance to next
                        cmpx	#KEY_END	        ; are we at the end of the buffer?
                        blt	    k_main_clr	        ;   not yet, continue looping

k_main_0	    jsr	    KRNL_LINEEDIT	    ; run the command line editor
                         jsr	    KRNL_CMD_PROC	    ;    decode the command; A = Table Index
                         tst	    EDT_BUFFER	        ; test the buffer for a null
                         beq	    k_main_cont	        ; skip, nothing was entered
                         cmpa	#$FF		        ; ERROR: command not found 
                         beq	    k_main_error	    ;    display the error
                         lsla			            ; index two byte addresses
                         leax	1,x
                         ldy	    #KRNL_CMD_VECTS	    ; the start of the command vector table
                         jsr	    [a,y]		        ; call the command subroutine
k_main_cont	    tst	    EDT_BUFFER	        ; nothing entered in the command line?
                         beq	    k_main_0	        ;   nope, skip the ready prompt
                         bra	    KRNL_MAIN_LOOP      ; back to the top of the main loop
k_main_error	ldx	    #KRNL_ERR_NFND	    ; ERROR: Command Not Found
                        jsr	    KRNL_LINEOUT	    ; send it to the console
                        bra	    k_main_cont	        ; continue within the main loop


                ; ...
                ; infinite loop (for now)

                sys     CALL_GARBAGE
KRNL_INF	    jmp 	KRNL_INF			




; *****************************************************************************
; * Command: CLS "Clear Screen"			              ARG1 = Color Attribute  *
; *****************************************************************************
do_cls			tst		,x				; test for an argument
                                beq		do_cls_0		; no argument, just go clear the screen
                                lda		,x				; first character in the argument
                                cmpa	#$ff			; $FF is also a terminator
                                beq		do_cls_0		; no argument, go clear the screen
                                jsr 	KRNL_ARG_TO_A	; fetch the numeric argument into A
                                tsta					; is the numeric value 0?
                                beq		do_cls_0		; yeah, go clear the screen
                                sta		_ATTRIB			; store the argument as the default color
do_cls_0		lda		#' '			; load the SPACE character to clear with
                                ldb		_ATTRIB			; load the color attribute
                                jsr		KRNL_CLS		; clear the screen
                                rts						; return from subroutine


; *****************************************************************************
; * Command: COLOR "Change the Color Attribute"	      ARG1 = Color Attribute  *
; *****************************************************************************
do_color		tst		,x				; test for an argument
                                beq		do_color_0		; if its zero, do nothing; just return
                                jsr 	KRNL_ARG_TO_A	; fetch the numeric argument into A
                                tsta					; is it a zero?
                                beq		do_color_0		;   yeah, return
                                cmpa	#$ff			; is it the other terminator?
                                beq		do_color_0		;   yeah, return
                                sta		_ATTRIB			; save the new default color attribute
do_color_0		rts						; return from subroutine


; *****************************************************************************
; * Command: LOAD "Load a (Intel) Hex File        ARG1 = {filepath}/filename  *
; *****************************************************************************
err_file_nf		fcn		"ERROR: File Not Found\n";
err_file_no		fcn		"ERROR: File Not Open\n";
err_wrong_file	fcn		"ERROR: Wrong File Type\n"
do_load			jsr		do_arg1_helper	; fetch path data from argument 1
                                lda		#FC_LOADHEX		; FIO Command
                                sta		FIO_COMMAND		; Send the Load Hex Command
                                lda		FIO_ERROR		; Examine the Error Code
                                cmpa	#FE_NOTFOUND	; is the File Not Found bit set?
                                beq		do_ld_notfound	; ERROR: File Not Found
                                cmpa	#FE_NOTOPEN		; is the File Not Open bit set?
                                beq		do_ld_notopen	; ERROR: File Not Open
                                cmpa	#FE_WRONGTYPE	; is the Wrong File Type bit set?
                                beq		do_ld_wrong		; ERROR: Wrong File Type
                                bra		do_ld_done		; All done, return
do_ld_wrong		ldx		#err_wrong_file	; point to the error message
                                jsr		KRNL_LINEOUT	; send the text to the console
                                bra		do_ld_done		; done, return
do_ld_notopen	ldx		#err_file_no	; point to the error message
                                jsr		KRNL_LINEOUT	; send it to the console
                                bra		do_ld_done		; done, return
do_ld_notfound	ldx		#err_file_nf	; point to the error message
                                jsr		KRNL_LINEOUT	; send it to the console
do_ld_done		rts						; done, return
do_arg1_helper	clr		FIO_PATH_POS	; reset the path cursor position
do_argh_0		lda		,x+				; load the next character
                                sta		FIO_PATH_DATA	; push it into the FIO Path Data Port
                                bne		do_argh_0		; Continue until Null-Terminator
                                rts						; return from subroutine

; *****************************************************************************
; * Command: EXEC "Execute a Program"                            ARG1 = none  *
; *****************************************************************************
do_exec			jsr		[VEC_EXEC]		; call the users program
                                rts						; return from this subroutine

; *****************************************************************************
; * Command: RESET "Perform a System Reset"                      ARG1 = none  *
; *****************************************************************************
do_reset		lda		#FC_RESET		; load the FIO Command: RESET
                                sta		FIO_COMMAND		; issue the Command
                                rts						; return from subroutine

; *****************************************************************************
; * Command: DIR "List a Directorys Files and Folders"     ARG1 = {filepath}  *
; *****************************************************************************
do_dir			bsr		do_arg1_helper	; fetch path data from argument 1
                                lda		#FC_LISTDIR		; load the FIO command: LISTDIR
                                sta		FIO_COMMAND		; issue the Command
do_dir_1		lda		FIO_DIR_DATA	; load a character from the Data Port
                                beq		do_dir_2		; quit when we find the Null-Terminator
                                jsr		KRNL_CHROUT		; output the character to the console
                                bra		do_dir_1		; continue looping until done
do_dir_2		rts						; return from subroutine

; *****************************************************************************
; * Command: CD / CHDIR "Change Current Folder"            ARG1 = {filepath}  *
; *****************************************************************************
do_cd					; CD is an alias for CHDIR
do_chdir		bsr		do_arg1_helper	; fetch path data from argument 1
                                lda		#FC_CHANGEDIR	; load the FIO command: CHANGEDIR
                                sta		FIO_COMMAND		; send it; change dir
                                jmp		do_pwd			; output the current working directory

; *****************************************************************************
; * Command: PWD "Print Working Directory"  				     ARG1 = none  *
; *****************************************************************************
do_pwd			lda		#FC_GETPATH		; load the FIO command: GETPATH	
                                sta		FIO_COMMAND		; send it; fetch the current path
                                clr		FIO_PATH_POS	; reset the path cursor position
do_pwd_0		lda		FIO_PATH_DATA	; pull a character from the path data port
                                beq		do_pwd_1		; if it's a null, we're done
                                jsr		KRNL_CHROUT		; output the character to the console
                                bra		do_pwd_0		; continue looping until done
do_pwd_1		rts						; return from subroutine

; *****************************************************************************
; * Command: EXIT / QUIT "Terminate the Emulator Program"        ARG1 = none  *
; *****************************************************************************
do_exit			nop						; EXIT is an alias for QUIT
do_quit			lda		#FC_SHUTDOWN	; load the FIO command: SHUTDOWN
                                sta		FIO_COMMAND		; issue the shutdown command
                                rts						; return from subroutine

; *****************************************************************************
; * Command: MODE "Change Display Mode" (sets GMODE)    ARG1 = Graphics Mode  *
; *****************************************************************************
do_mode			tst		,x				; test for an argument
                                beq		do_mode_0		; just return if argument == zero
                                jsr 	KRNL_ARG_TO_A	; fetch the numeric argument into A 
                                anda	#%00000111		; mask out the mode bits
                sta     _LOCAL_3        ; store the mode bits
                lda     GPU_MODE_LSB    ; fetch the current mode lsb
                anda    #%11111000      ; mask out the mode bits
                ora     _LOCAL_3        ; or them back in
                                sta		GPU_MODE_LSB	; set the GMODE 
                                lda		#' '			; load a SPACE character
                                jsr		KRNL_CLS		; clear the screen
do_mode_0		rts						; return from subroutine

; *****************************************************************************
; * Command: DEBUG "Enter / Exit Debugger"                       ARG1 = none  *
; *****************************************************************************
do_debug_str	fcn		" ";
do_debug_ena	fcn		"enabled\n";
do_debug_dis	fcn		"disabled\n";
do_debug		lda		SYS_DBG_FLAGS	; load the debug hardware flags
                                anda	#$80			; test the enable bit
                                beq		do_debug_0		; Go ENABLE the debugger
                                ; DISABLE the debugger
                                lda		SYS_DBG_FLAGS	; load the debug hardware flags
                                anda	#$7f			; mask out the debugger bit
                                sta		SYS_DBG_FLAGS	; store the updated debug flags
                                ldx		#do_debug_str	; load the debugger response string
                                jsr		KRNL_LINEOUT	; send the string to the console
                                ldx		#do_debug_dis	; load the "disabled" string address
                                jsr		KRNL_LINEOUT	; send it to the console
                                rts						; return from this subroutine
do_debug_0		; ENABLE the debugger
                                lda		SYS_DBG_FLAGS	; load the debug hardware flags
                                ora		#$80			; set the debug enable flag
                                sta		SYS_DBG_FLAGS	; store the updated debug flags
                                ldx		#do_debug_str	; load the debugger response string
                                jsr		KRNL_LINEOUT	; send it to the console
                                ldx		#do_debug_ena	; load the "enabled" string start
                                jsr		KRNL_LINEOUT	; send it to the console
                                rts						; return from this subroutine

; *****************************************************************************
; * Command: HELP basic help text message                        ARG1 = none  *
; *****************************************************************************
do_help			ldx		#krnl_help_str	; load the help message string addresses
                                jsr		KRNL_LINEOUT	; send it to the console
                                rts			; return from subroutine




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
                fdb	    SYS_CLS         ; $01 CLS 	    	
                fdb	    SYS_CHROUT	    ; $02 CHROUT		
                fdb     SYS_NEWLINE     ; $03 NEWLINE   	
                fdb     SYS_TAB         ; $04 TAB       	
                fdb     SYS_LINEOUT     ; $05 LINEOUT   	
                fdb     SYS_CSRPOS      ; $06 CSRPOS    	
                fdb     SYS_SCROLL      ; $07 SCROLL    	
                fdb     SYS_LINEEDIT	; $08 LINEEDIT		
                fdb     SYS_GETKEY	    ; $09 GETKEY    	
                                fdb		SYS_GETHEX		; $0A GETHEX		
                                fdb		SYS_GETNUM		; $0B GETNUM		
                                fdb 	SYS_CMPSTR		; $0C CMPSTR		
                                fdb		SYS_CMD_PROC	; $0D CMD_PROC		
                                fdb		SYS_TBLSEARCH	; $0E TBLSEARCH 	
                                fdb		SYS_CPY_DWORD	; $0F CPY_DWORD 	
                                fdb		SYS_D_TO_RAWA	; $10 SYS_D_TO_RAWA
                                fdb		SYS_D_TO_RAWB	; $11 SYS_D_TO_RAWB
                                fdb		SYS_D_TO_RAWR	; $12 SYS_D_TO_RAWR
                                fdb		SYS_D_TO_INTA	; $13 SYS_D_TO_INTA
                                fdb		SYS_D_TO_INTB	; $14 SYS_D_TO_INTB
                                fdb		SYS_D_TO_INTR	; $15 SYS_D_TO_INTR
                                fdb		SYS_RAWA_TO_D	; $16 SYS_RAWA_TO_D
                                fdb		SYS_RAWB_TO_D	; $17 SYS_RAWB_TO_D
                                fdb		SYS_RAWR_TO_D	; $18 SYS_RAWR_TO_D
                                fdb		SYS_INTA_TO_D	; $19 SYS_INTA_TO_D
                                fdb		SYS_INTB_TO_D	; $1A SYS_INTB_TO_D
                                fdb		SYS_INTR_TO_D	; $1B SYS_INTR_TO_D
                                fdb		SYS_8BIT_MATH	; $1C SYS_8BIT_MATH
                                fdb		SYS_DSP_ACA		; $1D SYS_DSP_ACA
                                fdb		SYS_DSP_ACB		; $1E SYS_DSP_ACB
                                fdb		SYS_DSP_ACR		; $1F SYS_DSP_ACR
                                fdb		SYS_DSP_INTA	; $20 SYS_DSP_INTA
                                fdb		SYS_DSP_INTB	; $21 SYS_DSP_INTB
                                fdb		SYS_DSP_INTR	; $22 SYS_DSP_INTR
                                fdb		SYS_WRITE_ACA	; $23 SYS_WRITE_ACA
                                fdb		SYS_WRITE_ACB	; $24 SYS_WRITE_ACB
                                fdb		SYS_WRITE_ACR	; $25 SYS_WRITE_ACR
                                fdb		SYS_ARG_TO_A	; $26 SYS_ARG_TO_A
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
KRNL_CLS	    jmp		[VEC_CLS]		; proceed through the software vector
STUB_CLS		pshs	d,x		        ; save the used registers onto the stack
                        lda	    _ATTRIB		    ; fetch the current color attribute
                        ldb	    #' '		    ; the space character
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
KRNL_CHROUT		jmp		[VEC_CHROUT]    ; proceed through the software vector
STUB_CHROUT		pshs	d,x,cc		    ; save the used registers onto the stack
                                tfr		a,b
                        lda	    _ATTRIB		    ; load the current color attribute
K_CHROUT_1	    tstb			        ; is A a null?
                        beq	    K_CHROUT_DONE	;    A is null, just return and do nothing		
                        cmpb	#$0A		    ; is it a newline character?
                        bne	    K_CHROUT_2	    ; nope, don't do a newline
                        jsr	    KRNL_NEWLINE	; advance the cursor 
                        bra	    K_CHROUT_DONE	; clean up and return
K_CHROUT_2	    cmpb	#$09		    ; is it a tab character?
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
KRNL_NEWLINE	jmp		[VEC_NEWLINE]	; proceed through the software vector
STUB_NEWLINE	pshs	D,X		        ; save the used registers onto the stack
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
KRNL_LINEOUT	jmp		[VEC_LINEOUT]	; proceed through the software vector
STUB_LINEOUT	pshs	D,X,U 		    ; save the used registers onto the stack
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
KRNL_CSRPOS	    jmp		[VEC_CSRPOS]	; proceed through the software vector
STUB_CSRPOS		pshs	D		        ; save the used registers onto the stack
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
KRNL_SCROLL	    jmp		[VEC_SCROLL]	; proceed through the software vector
STUB_SCROLL		pshs	d,x,u		    ; save the used registers onto the stack
                        ldx		#VIDEO_START	; set X to the start of the video buffer
                        tfr		x,u		        ; copy X into U
                        ldb		GPU_TCOLS	    ; B = Screen Columns
                        lslb			        ; account for the attribute byte
                        clra			        ; MSB of D needs to not be negative
                        leau	d,u		        ; U is now one line below X
K_SCROLL_0	    ldd		,u++		    ; load a character from where U points
                        std		,x++		    ; store it to where X points
                        cmpu	GPU_VIDEO_MAX	; has U exceeded the screen buffer
                        blt		K_SCROLL_0	    ; continue looping of not
                                lda		_ATTRIB
                        ldb		#' '		    ; set SPACE as the current character
K_SCROLL_1	    std		,x++		    ; and store it to where X points
                        cmpx	GPU_VIDEO_MAX	; continue looping until the bottom ...
                        blt		K_SCROLL_1	    ; ... line has been cleared
                        tst		EDT_ENABLE	    ; are we using the line editor?
                        beq		K_SCROLL_DONE	; nope, just clean up and return
                        dec		_ANCHOR_ROW	    ; yup, decrease the anchor row by one
K_SCROLL_DONE	puls	d,x,u,pc	    ; restore the registers and return

; *****************************************************************************
; * KRNL_LINEEDIT                                                             *
; * 	Engage the text line editor,                                          *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	All registers preserved.                              *
; *****************************************************************************
SYS_LINEEDIT    jsr     KRNL_LINEEDIT   ; call the text line edit kernel handler
                rti                     ; return from the interrupt
                ; ...
KRNL_LINEEDIT	jmp		[VEC_LINEEDIT]	; proceed through the software vector
STUB_LINEEDIT	pshs	D,X,U,CC	    ; save the used registers onto the stack		
                        ldd 	_CURSOR_COL	    ; load the current cursor position
                        std	    _ANCHOR_COL	    ;   use it to update the anchor position
                        lda	    #1		        ; load the enable condition
                        sta	    EDT_ENABLE	    ; to enable the line editor
KRNL_LEDIT_0	; display the line up to the cursor		
                        ldd 	_ANCHOR_COL	    ; restore the line editor anchor
                        std	    _CURSOR_COL 	; into the console cursor position
                        ldu	    #EDT_BUFFER	    ; point to the start of the edit buffer
                        ldb	    EDT_BFR_CSR	    ; the buffer csr position
                        stb	    _LOCAL_0	    ; store the edit csr position locally
KRNL_LEDIT_1	tst	    _LOCAL_0	    ; test the edit csr position
                        beq	    KRNL_LEDIT_2	; if we're there, go display the cursor
                        dec	    _LOCAL_0	    ; decrement the edit csr position
                        lda	    ,u+		        ; load the next character from the buffer
                        beq	    KRNL_LEDIT_2	; display csr if at the null terminator
                        jsr	    KRNL_CHROUT	    ; output the character to the console
                        bra	    KRNL_LEDIT_1	; loop until we're at the cursor
KRNL_LEDIT_2	; display the cursor at the end of the line
                        lda	    #' '		    ; load a blank SPACE character
                        ldb	    SYS_CLOCK_DIV	; load clock timer data
                                lslb					; times two
                        andb	#$F0		    ; B now holds color cycled attribute
                        tst	    ,u		        ; test the next character in the buffer
                        beq	    KRNL_LEDIT_3	; use the SPACE if we're at a null
                        lda	    ,u+		        ; load the next character from buffer
KRNL_LEDIT_3	; finish the line
                        jsr	    KRNL_CSRPOS	    ; load X with the current cursor position 
                                exg		a,b
                        std	    ,x		        ; store the character where X points to
                        inc	    _CURSOR_COL	    ; ipdate the cursor column number
                        ; ldb	KRNL_ATTRIB	    ; load the default color attribute
KRNL_LEDIT_4	lda	    ,u+		        ; fetch the next character from the buffer
                        beq	    KRNL_DONE	    ; if it's null, we're done
                        jsr	    KRNL_CHROUT	    ; output it to the console
                        bra	    KRNL_LEDIT_4	; continue looping until we find the null
KRNL_DONE	; space at the end	
                        lda	    #' '		    ; load the SPACE character
                        jsr	    KRNL_CSRPOS	    ; fetch the cursor position into X
                        lda	    #' '		    ; load the SPACE character
                        ldb	    _ATTRIB		    ; load the current color attribute
                                exg		a,b
                        std	    ,x		        ; update the console
                        ; test for the user pressing ENTER / RETURN
                        lda	    CHAR_POP	    ; Pop the top key from the queue
                        beq	    KRNL_LEDIT_0	; loop to the top if no keys we're pressed
                        cmpa	#$0d		    ; check for the RETURN / ENTER key press
                        bne	    KRNL_LEDIT_0	; if not pressend, loop back to the top		
                        clr	    EDT_ENABLE	    ; disable the line editor		
                        jsr	    KRNL_CSRPOS	    ; load the cursor position into X
                                lda		_ATTRIB
                        ldb	    #' '		    ; load a SPACE character
                        std	    -2,x		    ; store the character, clean up artifacts
                        ldd 	_ANCHOR_COL	    ; restore the line editor anchor
                        std	    _CURSOR_COL 	; into the console cursor position
                        ldx	    #EDT_BUFFER	    ; point to the edit buffer
                        jsr	    KRNL_LINEOUT	; send the edit buffer to the console
                        puls	D,X,U,CC,PC	    ; cleanup saved registers and return


; *****************************************************************************
; * KRNL_GETKEY                                                                *
; * 	Input a character from the console. Waits for the keypress.           *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	A = key code of the key that was pressed              *
; *                         All other registers preserved                     *
; *****************************************************************************
SYS_GETKEY      jsr     KRNL_GETKEY     ; call the kernel get key handler
                rti                     ; return from interrupt
                ; ...
KRNL_GETKEY		jmp		[VEC_GETKEY]	; proceed through the software vector    
STUB_GETKEY		pshs	b,CC		    ; save the used registers onto the stack
K_GETKEY_0	    ldb	    CHAR_POP	    ; pop the next key from the queue
                        bne	    K_GETKEY_0	    ; continue until the queue is empty		
K_GETKEY_1	    ldb	    CHAR_Q_LEN	    ; how many keys are in the queue
                        beq	    K_GETKEY_1	    ; loop until a key is queued
                        lda	    CHAR_POP	    ; pop the key into A to be returned
                        puls	b,CC,PC	        ; cleanup saved registers and return


; *****************************************************************************
; * KRNL_GETHEX                                                               *
; * 	Input a hex digit from the console. Waits for the keypress.           *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	A = key code of the key that was pressed              *
; *                         All other registers preserved                     *
; *****************************************************************************
SYS_GETHEX		jsr		KRNL_GETHEX		; call the kernel GetHex handler
                                rti
                                ; ...
KRNL_GETHEX		jmp		[VEC_GETHEX]	; proceed through the software vector
STUB_GETHEX		pshs	CC				; save the used registers onto the stack
K_GETHEX_0		bsr		KRNL_GETKEY		; wait for and fetch a key press
                                cmpa	#'0'			; compare with the '0' key
                                blt		K_GETHEX_0		; keep scanning if less
                                cmpa	#'9'			; compare with the '9' key
                                bls		K_GETHEX_DONE	; found an appropriate key, return
                                cmpa	#'A'			; compare with the 'A' key
                                blt		K_GETHEX_0		; keep scanning if less
                                cmpa	#'F'			; compare with the 'F' key
                                bls		K_GETHEX_DONE	; found an appropriate key, return
                                cmpa	#'a'			; compare with the 'a' key
                                blt		K_GETHEX_0		; keep scanning if less
                                cmpa	#'f'			; compare with the 'f' key
                                bls		K_GETHEX_DONE	; found an appropriate key, return
                                bra		K_GETHEX_0		; keep scanning
K_GETHEX_DONE	puls	CC,PC			; cleanup saved registers and return


; *****************************************************************************
; * KRNL_GETNUM                                                               *
; * 	Input a numeric digit from the console. Waits for the keypress.       *
; *                                                                           *
; * ENTRY REQUIREMENTS: NONE                                                  *
; *                                                                           *
; * EXIT CONDITIONS:	A = key code of the key that was pressed              *
; *                         All other registers preserved                     *
; *****************************************************************************
SYS_GETNUM		jsr		KRNL_GETNUM		; call the kernel GetNum handler
                                rti						; return from interrupt
                                ; ...
KRNL_GETNUM		jmp		[VEC_GETNUM]	; proceed through the software vector
STUB_GETNUM		pshs	CC				; save the used registers onto the stack
K_GETNUM_0		bsr		KRNL_GETKEY		; wait for and fetch a key press
                                cmpa	#'0'			; compare with the '0' key
                                blt		K_GETNUM_0		; keep scanning if less
                                cmpa	#'9'			; compare with the '9' key
                                bls		K_GETNUM_DONE	; found an appropriate key, return
                                bra		K_GETNUM_0		; keep scanning
K_GETNUM_DONE	puls	CC,PC			; cleanup saved registers and return


; *****************************************************************************
; * KRNL_CMPSTR                                                               *
; * 	Compare two null-terminated strings of arbitrary lengths.             *
; *                                                                           *
; * ENTRY REQUIREMENTS: X = starting address of string 1                      *
; *                     Y = starting address of string 2                      *
; *                                                                           *
; * EXIT CONDITIONS:	CC = set per the comparison (less, greater, or same)  *
; *                     X = address last checked in string 1                  *
; *                     Y = address last checked in string 2                  *
; *****************************************************************************
SYS_CMPSTR		jsr		KRNL_CMPSTR		; call the kernel CMPSTR handler
                                rti						; return from the interrupt
                                ; ...
KRNL_CMPSTR		jmp		[VEC_CMPSTR]	; proceed through the software vector
STUB_CMPSTR		pshs	D				; save the used registers onto the stack		
K_CMP_LOOP		tst		,x				; test the current character in string 1
                                bne		K_CMP_1			; if its non-null, go test in string 2
                                tst		,y				; test if character in both are null
                                beq		K_CMP_EQUAL		; if so, strings are equal
                                bra		K_CMP_LESS		; is LESS if str1 is null but str2 is not
K_CMP_1			tst		,y				; char in str1 is not null, but str2 is
                                beq		K_CMP_GREATER	; return GREATER
                                lda		,x+				; compare character from string 1
                                ;
                                ora		#$20			; convert all letters to lower case
                                ;
                                cmpa	,y+				;    with character from string 2
                                blt		K_CMP_LESS		; return LESS
                                bgt		K_CMP_GREATER	; return GREATER
                                bra		K_CMP_LOOP		; otherwise continue looping
K_CMP_LESS		lda		#1				; compare 1
                                cmpa	#2				;    with 2
                                bra		K_CMP_DONE		; return LESS
K_CMP_GREATER	lda		#2				; compare 2
                                cmpa	#1				;    with 1
                                bra		K_CMP_DONE		; return GREATER
K_CMP_EQUAL		clra					; set to zero
                                cmpa	#0				; return EQUAL
K_CMP_DONE		puls	D,PC			; cleanup saved registers and return


; *****************************************************************************
; * KRNL_CMD_PROC                                                             *
; * 	Parse the command from the line edit buffer.                          *
; *                                                                           *
; * ENTRY REQUIREMENTS: Command text within EDT_BUFFER                        *
; *                                                                           *
; * EXIT CONDITIONS:	A = search string table index (or $FF if not found)   *
' *                     X & Y Modified                                        *
; *                     FIO_BUFFER will be modified                           *
; *****************************************************************************
SYS_CMD_PROC	jsr		KRNL_CMD_PROC	; call the kernel command proc handler
                                rti						; return from the interrupt
                                ; ...
KRNL_CMD_PROC	jmp		[VEC_CMD_PROC]	; proceed through the software vector
STUB_CMD_PROC	pshs	B,CC			; save the used registers onto the stack
                        ; copy EDT_BUFFER to FIO_BUFFER
                                ldx		#EDT_BUFFER		; the start of the input buffer
                                ldy		#FIO_BUFFER		; use the I/O buffer temporarily
K_CMDP_0		lda		,x+				; load a character from the input
                                cmpa	#'A'			; make sure input is in lower case
                                blt		K_CMDP_3		;   valid character if < 'A'
                                cmpa	#'Z'			; all other characters are good to go
                                bgt		K_CMDP_3		;   valid charcters above 'Z'
                                * ora	#$20			; convert all letters to lower case (DONT DO THIS HERE!!!!)
K_CMDP_3		sta		,y+				; copy it to the output
                                bne		K_CMDP_0		; branch until done copying
                        ; replace the null-terminator with $FF
                                lda		#$ff			; the new character $FF
                                sta		,y			; replace the null-terminator
                        ; replace SPACES with NULL (unless within '' or "")
                                ldx		#FIO_BUFFER		; the start of the temp buffer
K_CMDP_1		lda		,x+				; load the next character from buffer
                                beq		K_CMDP_2
                                cmpa	#$FF			; are we at the end of the buffer?
                                beq		K_CMDP_2		;   yes, go parse the buffer
                                cmpa	#"'"			; are we at a single-quote character?
                                beq		K_CPROC_SKIP	;   skip through until we find another
                                cmpa	#'"'			; are we at a double-quote character?
                                beq		K_CPROC_SKIP	;   skip through until we find another
                                cmpa	#' '			; are we at a SPACE character?
                                bne		K_CMDP_1		; nope, continue scanning	
                                clr		-1,x			; convert the SPACE to a NULL
                                bra		K_CMDP_1		; continue scanning through the buffer
K_CPROC_SKIP	cmpa	,x+				; is character a quote character?
                                beq		K_CMDP_1		;    yes, go back to scanning the buffer
                                tst		,x				; are we at a NULL?
                                bne		K_CPROC_SKIP	;    nope, keep scanning for a quote		
                                jsr		KRNL_NEWLINE	; on error: send a linefeed cleanup
                                lda		#$FF			; error: end of line found but no quote
                                bra		K_CPROC_DONE	; continue looking for a quote character
                        ; FIO_BUFFER should now be prepared for parsing
K_CMDP_2		lda		#$0a			; line feed character
                                jsr		KRNL_CHROUT		; send the line feed
                                ldy		#KRNL_CMD_TABLE	; point to the command table to search
                                ldx		#FIO_BUFFER		; point to the command to search for
                        ; X now points to the command to search for in the table
                                bsr		KRNL_TBLSEARCH	; seach the table for the command
                        ; A = index of the found search string table index
K_CPROC_DONE	puls	B,CC,PC			; cleanup saved registers and return


; *****************************************************************************
; * KRNL_TBLSEARCH                                                            *
; * 	Table Search (find the string and return its index)                   *
; *                                                                           *
; * ENTRY REQUIREMENTS: X points to a string to be searched for               *
; *                     Y points to the start of a string table               *
; *                                                                           *
; * EXIT CONDITIONS:	A = string index if found, -1 ($FF) if not found      *
; *                     X = the end of the search string(next argument)       *
; *                         All other registers preserved                     *
; *****************************************************************************
SYS_TBLSEARCH	jsr		KRNL_TBLSEARCH	; call the kernel table search handler
KRNL_TBLSEARCH	jmp		[VEC_TBLSEARCH]	; proceed through the software vector
STUB_TBLSEARCH	pshs	B,Y,U,CC		; save the used registers onto the stack
                                tfr		X,U				; save X in U
                                clra					; set the return index to 0
K_TBLS_0		tfr		U,X				; restore X
                                jsr 	KRNL_CMPSTR		; compare strings at X and at Y
                                beq		K_TBLS_DONE		; found the string in the table		
                                inca					; increment the index return value
K_TBLS_1		ldb		,y+				; look at the next character in table
                                cmpb	#$ff			; is it the $ff terminator?
                                beq		K_TBLS_NOTFOUND	; yes, the entry is not in the table
                                tstb					; are we looking at a null character?
                                bne		K_TBLS_1		; loop until the end of this entry
                                bra		K_TBLS_0		; look at the next entry
K_TBLS_NOTFOUND	lda		#$ff			; not found error code
K_TBLS_DONE		puls	B,Y,U,CC,PC		; cleanup saved registers and return


; *****************************************************************************
; * KRNL_CPY_DWORD                                                            *
; * 	Copy 32-bits from where X points to where Y points                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: X points to a DWORD to be copied from                 *
; *                     Y points to a DWORD to be copied to                   *
; *                                                                           *
; * EXIT CONDITIONS:	    All registers preserved                           *
; *****************************************************************************
SYS_CPY_DWORD	jsr		KRNL_CPY_DWORD	; call the kernel copy dword handler
                                rti						; return from the interrupt
                                ; ...
KRNL_CPY_DWORD	jmp		[VEC_CPY_DWORD]	; proceed through the software vector
STUB_CPY_DWORD	pshs 	D,CC			; save the used registers onto the stack
                                ldd		,x				; load the most-significant 16-bit word
                                std		,y				; save the most-significant 16-bit word
                                ldd		2,x				; load the least-significant 16-bit word
                                std		2,y				; save the least-significant 16-bit word
                                puls	D,CC,PC			; cleanup saved registers and return

; *******************************************************************************
; * KRNL_D_TO_RAW(A, B, or R)                                              		*
; * 	Write the D register to one of the raw float registers                	*
; *                                                                           	*
; * ENTRY REQUIREMENTS: D = 16-bit value to be written                        	*
; *                                                                           	*
; * EXIT CONDITIONS:	    All registers preserved                           	*
; *******************************************************************************
SYS_D_TO_RAWA	jsr		KRNL_D_TO_RAWA	; call the kernel D_TO_RAWA handler
                                rti						; return from the interrupt
                                ; ...
KRNL_D_TO_RAWA	jmp		[VEC_D_TO_RAWA]	; proceed through the software vector
STUB_D_TO_RAWA	pshs	CC				; save the used registers onto the stack
                                clr		MATH_ACA_RAW+0	; clear unneeded byte
                                clr		MATH_ACA_RAW+1	; clear unneeded byte
                                std		MATH_ACA_RAW+2	; store D in the ACA raw float register
                                puls	CC,PC			; cleanup saved registers and return
                
SYS_D_TO_RAWB	jsr		KRNL_D_TO_RAWB	; call the kernel D_TO_RAWB handler
                                rti						; return from the interrupt
                                ; ...
KRNL_D_TO_RAWB	jmp		[VEC_D_TO_RAWB]	; proceed through the software vector
STUB_D_TO_RAWB	pshs	CC				; save the used registers onto the stack
                                clr		MATH_ACB_RAW+0	; clear unneeded byte
                                clr		MATH_ACB_RAW+1	; clear unneeded byte
                                std		MATH_ACB_RAW+2	; store D in the ACB raw float register
                                puls	CC,PC			; cleanup saved registers and return

SYS_D_TO_RAWR	jsr		KRNL_D_TO_RAWR	; call the kernel D_TO_RAWR handler
                                rti						; return from the interrupt
                                ; ...
KRNL_D_TO_RAWR	jmp		[VEC_D_TO_RAWR]	; proceed through the software vector
STUB_D_TO_RAWR	pshs	CC				; save the used registers onto the stack
                                clr		MATH_ACR_RAW+0	; clear unneeded byte
                                clr		MATH_ACR_RAW+1	; clear unneeded byte
                                std		MATH_ACR_RAW+2	; store D in the ACR raw float register
                                puls	CC,PC			; cleanup saved registers and return

; *****************************************************************************
; * KRNL_D_TO_INT(A, B, or R)                                                 *
; * 	Write the D register to one of the FP integer registers               *
; *                                                                           *
; * ENTRY REQUIREMENTS: D = 16-bit value to be written                        *
; *                                                                           *
; * EXIT CONDITIONS:	    All registers preserved                           *
; *****************************************************************************
SYS_D_TO_INTA	jsr		KRNL_D_TO_INTA	; call the kernel D_TO_INTA handler
                                rti						; return from the interrupt
                                ; ...
KRNL_D_TO_INTA	jmp		[VEC_D_TO_INTA]	; proceed through the software vector
STUB_D_TO_INTA	pshs	CC				; save the used registers onto the stack
                                clr		MATH_ACA_INT+0	; clear unneeded byte
                                clr		MATH_ACA_INT+1	; clear unneeded byte
                                std		MATH_ACA_INT+2	; store D in the ACA integer register
                                puls	CC,PC			; cleanup saved registers and return

SYS_D_TO_INTB	jsr		KRNL_D_TO_INTB	; call the kernel D_TO_INTB handler
                                rti						; return from the interrupt
                                ; ...		
KRNL_D_TO_INTB	jmp		[VEC_D_TO_INTB]	; proceed through the software vector
STUB_D_TO_INTB	pshs	CC				; save the used registers onto the stack
                                clr		MATH_ACB_INT+0	; clear unneeded byte
                                clr		MATH_ACB_INT+1	; clear unneeded byte
                                std		MATH_ACB_INT+2	; store D in the ACB integer register
                                puls	CC,PC			; cleanup saved registers and return

SYS_D_TO_INTR	jsr		KRNL_D_TO_INTR	; call the kernel D_TO_INTR handler
                                rti						; return from the interrupt
                                ; ...
KRNL_D_TO_INTR	jmp		[VEC_D_TO_INTR]	; proceed through the software vector
STUB_D_TO_INTR	pshs	CC				; save the used registers onto the stack
                                clr		MATH_ACR_INT+0	; clear unneeded byte
                                clr		MATH_ACR_INT+1	; clear unneeded byte
                                std		MATH_ACR_INT+2	; store D in the ACR integer register
                                puls	CC,PC			; cleanup saved registers and return

; *******************************************************************************
; * KRNL_RAW(A, B, or R)_TO_D                                                  	*
; * 	Read one of the raw float registers into the D register          		*
; *                                                                           	*
; * ENTRY REQUIREMENTS: none                                                  	*
; *                                                                           	*
; * EXIT CONDITIONS: D = the integer value of the chosen FP register          	*
; *                         All other registers preserved                     	*
; *                                                                           	*
; *******************************************************************************
SYS_RAWA_TO_D	jsr		KRNL_RAWA_TO_D	; call the kernel RAWA_TO_D handler
                                rti						; return from the interrupt
                                ; ...
KRNL_RAWA_TO_D	jmp		[VEC_RAWA_TO_D]	; proceed through the software vector
STUB_RAWA_TO_D	pshs	CC				; save the used registers onto the stack
                                ldd		MATH_ACA_RAW+2	; load the ACA raw float value
                                puls	CC,PC			; cleanup saved registers and return

SYS_RAWB_TO_D	jsr		KRNL_RAWB_TO_D	; call the kernel RAWB_TO_D handler
                                rti						; return from the interrupt
                                ; ...
KRNL_RAWB_TO_D	jmp		[VEC_RAWB_TO_D]	; proceed through the software vector
STUB_RAWB_TO_D	pshs	CC				; save the used registers onto the stack
                                ldd		MATH_ACB_RAW+2	; load the ACB raw float value
                                puls	CC,PC			; cleanup saved registers and return

SYS_RAWR_TO_D	jsr		KRNL_RAWR_TO_D	; call the kernel RAWR_TO_D handler
                                rti						; return from the interrupt
                                ; ...
KRNL_RAWR_TO_D	jmp		[VEC_RAWR_TO_D]	; proceed through the software vector
STUB_RAWR_TO_D	pshs	CC				; save the used registers onto the stack
                                ldd		MATH_ACR_RAW+2	; load the ACR raw float value
                                puls	CC,PC			; cleanup saved registers and return

; *******************************************************************************
; * KRNL_INT(A, B, or R)_TO_D                                                 	*
; * 	Read one of the integer registers into the D register          			*
; *                                                                           	*
; * ENTRY REQUIREMENTS: none                                                  	*
; *                                                                           	*
; * EXIT CONDITIONS: D = the integer value of the chosen FP register          	*
; *                         All other registers preserved                     	*
; *                                                                           	*
; *******************************************************************************
SYS_INTA_TO_D	jsr		KRNL_INTA_TO_D	; call the kernel INTA_TO_D handler
                                rti						; return from the interrupt
                                ; ...
KRNL_INTA_TO_D	jmp		[VEC_INTA_TO_D]	; proceed through the software vector
STUB_INTA_TO_D	pshs	CC				; save the used registers onto the stack
                                ldd		MATH_ACA_INT+2	; load the ACA integer value
                                puls	CC,PC			; cleanup saved registers and return

SYS_INTB_TO_D	jsr		KRNL_INTB_TO_D	; call the kernel INTB_TO_D handler
                                rti						; return from the interrupt
                                ; ...
KRNL_INTB_TO_D	jmp		[VEC_INTB_TO_D]	; proceed through the software vector
STUB_INTB_TO_D	pshs	CC				; save the used registers onto the stack
                                ldd		MATH_ACB_INT+2	; load the ACB integer value
                                puls	CC,PC			; cleanup saved registers and return

SYS_INTR_TO_D	jsr		KRNL_INTR_TO_D	; call the kernel INTR_TO_D handler
                                rti						; return from the interrupt
                                ; ...
KRNL_INTR_TO_D	jmp		[VEC_INTR_TO_D]	; proceed through the software vector
STUB_INTR_TO_D	pshs	CC				; save the used registers onto the stack
                                ldd		MATH_ACR_INT+2	; load the ACR integer value
                                puls	CC,PC			; cleanup saved registers and return

; *****************************************************************************
; * KRNL_8BIT_MATH                                                            *
; * 	8-bit integer math                                                    *
; *                                                                           *
; * ENTRY REQUIREMENTS: A = ACA Integer                                       *
; *                     B = ACB Integer                                       *
; *                     U = Math Operation (MOP)                              *
; *                         (only least significant byte is relevant)         *
; *                                                                           *
; * EXIT CONDITIONS:	D = Result                                            *
; *                     All other registers preserved                         *
; *****************************************************************************
SYS_8BIT_MATH	jsr		KRNL_8BIT_MATH	; call the kernel 8BIT_MATH handler
                                rti						; return from the interrupt
                                ; ...
KRNL_8BIT_MATH	jmp		[VEC_8BIT_MATH]	; proceed through the software vector
STUB_8BIT_MATH	pshs	U,CC			; save the used registers onto the stack
                                ; A to ACA
                                clr		MATH_ACA_INT+0	; clear unneeded byte
                                clr		MATH_ACA_INT+1	; clear unneeded byte
                                clr		MATH_ACA_INT+2	; clear unneeded byte
                                sta		MATH_ACA_INT+3	; store A in the ACA integer register
                                ; B to ACB
                                clr		MATH_ACB_INT+0	; clear unneeded byte
                                clr		MATH_ACB_INT+1	; clear unneeded byte
                                clr		MATH_ACB_INT+2	; clear unneeded byte
                                stb		MATH_ACB_INT+3	; store B in the ACB integer register
                                ; U to MATH_OPERATION
                                tfr		U,D				; transfer the MOP instruction to D
                                stb		MATH_OPERATION	; send the MOP command (in B)
                                ; ACR to D
                                ldd		MATH_ACR_INT+2	; load the result into the D register
                                puls	U,CC,PC			; cleanup saved registers and return

; *****************************************************************************
; * KRNL_DSP_AC(A, B, or R)                                                   *
; * 	Displays the floating point number in one of the FP registers.        *
; *                                                                           *
; * ENTRY REQUIREMENTS: none                                                  *
; *                                                                           *
; * EXIT CONDITIONS:    All registers preserved                               *
; *****************************************************************************
SYS_DSP_ACA		jsr		KRNL_DSP_ACA	; call the kernel DSP_ACA handler
                                rti						; return from the interrupt
                                ; ...
KRNL_DSP_ACA	jmp		[VEC_DSP_ACA]	; proceed through the software vector
STUB_DSP_ACA	pshs	X,CC			; save the used registers onto the stack
                                ldx		#MATH_ACA_POS	; index the ACA data
                                bsr		KRNL_DSP_HELPER	; display the floating point of ACA
                                puls	X,CC,PC			; cleanup saved registers and return

SYS_DSP_ACB		jsr		KRNL_DSP_ACB	; call the kernel DSP_ACB handler
                                rti						; return from the interrupt
                                ; ...
KRNL_DSP_ACB	jmp		[VEC_DSP_ACB]	; proceed through the software vector
STUB_DSP_ACB	pshs	X,CC			; save the used registers onto the stack
                                ldx		#MATH_ACB_POS	; index the ACB data
                                bsr		KRNL_DSP_HELPER	; display the floating point of ACB
                                puls	X,CC,PC			; cleanup saved registers and return

SYS_DSP_ACR		jsr		KRNL_DSP_ACR	; call the kernel DSP_ACR handler
                                rti						; return from the interrupt
                                ; ...
KRNL_DSP_ACR	jmp		[VEC_DSP_ACR]	; proceed through the software vector
STUB_DSP_ACR	pshs	X,CC			; save the used registers onto the stack
                                ldx		#MATH_ACR_POS	; index the ACR data
                                bsr		KRNL_DSP_HELPER	; display the floating point of ACR
                                puls	X,CC,PC			; cleanup saved registers and return

;HELPER:  X=address of a FP_POS register pointed to by X
KRNL_DSP_HELPER	pshs  	A,CC			; save the used registers onto the stack
                                clr		,x				; reset this math data port
K_DSP_FP_0		lda		1,x				; pop a character from the port
                                jsr		KRNL_CHROUT		; send it to the console
                                bne		K_DSP_FP_0		; continue if not at the null-terminator
                                puls	A,CC,PC			; cleanup saved registers and return

; *****************************************************************************
; * KRNL_DSP_INT(A, B, or R)                                                  *
; * 	Displays the integer number in one of the FP registers.               *
; *                                                                           *
; * ENTRY REQUIREMENTS: none                                                  *
; *                                                                           *
; * EXIT CONDITIONS:    All registers preserved                               *
; *****************************************************************************	
SYS_DSP_INTA	jsr		KRNL_DSP_INTA	; call the kernel DSP_INTA handler
                                rti						; return from the interrupt
                                ; ...	
KRNL_DSP_INTA	jmp		[VEC_DSP_INTA]	; proceed through the software vector
STUB_DSP_INTA	pshs	X,CC			; save the used register onto the stack
                                ldx		#MATH_ACA_POS	; index the ACA data
                                bsr		KRNL_DSP_IHELP	; display the integer portion of ACA
                                puls	X,CC,PC			; cleanup and return

SYS_DSP_INTB	jsr		KRNL_DSP_INTB	; call the kernel DSP_INTB handler
                                rti						; return from the interrupt
                                ; ...	
KRNL_DSP_INTB	jmp		[VEC_DSP_INTB]	; proceed through the software vector
STUB_DSP_INTB	pshs	X,CC			; save the used register onto the stack
                                ldx		#MATH_ACA_POS	; index the ACB data
                                bsr		KRNL_DSP_IHELP	; display the integer portion of ACB
                                puls	X,CC,PC			; cleanup and return

SYS_DSP_INTR	jsr		KRNL_DSP_INTR	; call the kernel DSP_INTR handler
                                rti						; return from the interrupt
                                ; ...	
KRNL_DSP_INTR	jmp		[VEC_DSP_INTR]	; proceed through the software vector
STUB_DSP_INTR	pshs	X,CC			; save the used register onto the stack
                                ldx		#MATH_ACR_POS	; index the ACR data
                                bsr		KRNL_DSP_IHELP	; display the integer portion of ACR
                                puls	X,CC,PC			; cleanup and return

;HELPER:  X=address of a FP_POS register pointed to by X. Display Integer
KRNL_DSP_IHELP	pshs  	A,CC			; save the used registers onto the stack
                                clr		,x				; reset this math data port
K_DSP_INT_0		lda		1,x				; pop a character from the port
                                cmpa	#'.'			; is it the decimal point?
                                beq		K_DSP_INT_RET	;   yeah, we're done
                                jsr		KRNL_CHROUT		; no, output to the console
                                tsta					; are we at the null-terminator?
                                bne		K_DSP_INT_0		;   no, continue looping
K_DSP_INT_RET	puls	A,CC,PC			; cleanup saved registers and return


; *****************************************************************************
; * KRNL_WRITE_AC(A, B, or R)                                                 *
; * 	Sets one of the floating point registers to a FP value contained      *
; *     within a null-terminated string pointed to by X.                      *
; *                                                                           *
; * ENTRY REQUIREMENTS: X = points to a null-terminated string of numbers     *
; *                                                                           *
; * EXIT CONDITIONS:    All registers preserved                               *
; *****************************************************************************
SYS_WRITE_ACA	jsr		KRNL_WRITE_ACA	; call the kernel WRITE_ACA handler
                                rti						; return from the interrupt
                                ; ...
KRNL_WRITE_ACA	jmp		[VEC_WRITE_ACA]	; proceed through the software vector
STUB_WRITE_ACA	pshs	X,Y,CC			; save the used registers onto the stack
                                ldy		#MATH_ACA_POS	; point to the ACA chr pos register
                                bsr		KRNL_WRITE_HLP	; display the number to the console
                                puls	X,Y,CC,PC		; cleanup saved registers and return

SYS_WRITE_ACB	jsr		KRNL_WRITE_ACB	; call the kernel WRITE_ACB handler
                                rti						; return from the interrupt
                                ; ...
KRNL_WRITE_ACB	jmp		[VEC_WRITE_ACB]	; proceed through the software vector
STUB_WRITE_ACB	pshs	X,Y,CC			; save the used registers onto the stack
                                ldy		#MATH_ACB_POS	; point to the ACB chr pos register
                                bsr		KRNL_WRITE_HLP	; display the number to the console
                                puls	X,Y,CC,PC		; cleanup saved registers and return

SYS_WRITE_ACR	jsr		KRNL_WRITE_ACR	; call the kernel WRITE_ACR handler
                                rti						; return from the interrupt
                                ; ...
KRNL_WRITE_ACR	jmp		[VEC_WRITE_ACR]	; proceed through the software vector
STUB_WRITE_ACR	pshs	X,Y,CC			; save the used registers onto the stack
                                ldy		#MATH_ACR_POS	; point to the ACR chr pos register
                                bsr		KRNL_WRITE_HLP	; display the number to the console
                                puls	X,Y,CC,PC		; cleanup saved registers and return	

; X string to write, Y = ACn_POS
KRNL_WRITE_HLP	pshs	X,Y,CC			; save the used registers onto the stack
                                clr		,y+				; set the chr pos to the start
KRNL_WRITE_0	lda		,x+				; load the next char from the string
                                beq		KRNL_WRITE_DONE	; were done if it's a null-terminator
                                sta		,y				; store the char into the FP port
                                bra		KRNL_WRITE_0	; continue looping
KRNL_WRITE_DONE	puls	X,Y,CC,PC		; cleanup saved registers and return

; *****************************************************************************
; * KRNL_ARG_TO_A                                                             *
; * 	convert a numeric string (pointed to by X) to 0-25 and return it in A *
; *                                                                           *
; * ENTRY REQUIREMENTS: X = points to the string to be converted              *
; *                         Note: hex values must be preceeded                *
; *                               with a '$' character                        *
; *                                                                           *
; * EXIT CONDITIONS:	A = binary value represented by the input string      *
; *                     All other registers preserved                         *
; *****************************************************************************
SYS_ARG_TO_A	jsr		KRNL_ARG_TO_A	; call the kernel ARG_TO_A handler
                                rti						; return from the interrupt

KRNL_ARG_TO_A	jmp		[VEC_ARG_TO_A]	; proceed through the software vector
STUB_ARG_TO_A	pshs	B,X,CC			; save the used registers onto the stack
                                ldb		,x				; load character to be converted
                                cmpb	#'$'			; is it the leading '$'?
                                beq		KARG_0			;   yeah, go convert from hexidecimal
                                jsr		KRNL_WRITE_ACA	; use the FP to convert from decimal
                                lda		MATH_ACA_INT+3	; load the converted binary into A
                                bra		KARG_DONE		;   A now holds the binary, return
KARG_0			leax	1,x				; skip passed the initial '$' character
                                ldb		,x+				; load character to convert into B
                                bsr		KARG_HEX		; convert hex character to 0-15 binary
                                lslb					; shift the 4-bit data ... 
                                lslb					; ... into the most significant ...
                                lslb					; ... four-bits
                                lslb					; $n0 n = useful value
                                pshs	b				; save our work so far
                                ldb		,x+				; load the next hex character
                                bsr		KARG_HEX		; decode it to 0-15
                                ora		,s+				; merge the two and fix the stack
KARG_DONE		puls	B,X,CC,PC		; clean up and return
                        ; helper sub
KARG_HEX		pshs	b				; save it 
                                subb	#'0'			; convert to binary
                                bmi		2f				; go if not numeric
                                cmpb	#$09			; is greater than 9?
                                bls		1f				; branch if not
                                orb		#$20			; to lower case
                                subb	#$27			; reduce from 'a'
1				cmpb	#$0f			; greater than 15?
                                bls		3f				; go if not
2				ldb		#$ff			; load an error state $FF = BAD
3				cmpb	,s+				; fix the stack
                                tfr		b,a				; restore into A
                                rts						; return






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
