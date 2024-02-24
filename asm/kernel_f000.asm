; ***********************************************************************
; * 	kernel_f000.asm 						*
; *									*
; *	a.k.a. The Basic Input and Output System (BIOS)			*
; ***********************************************************************
; *									*
; *  	LINUX: 		cd asm						*
; *			sh build.sh kernel_f000				*
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
VECT_EXEC	fdb	$0000		; This will likely be used as the EXEC vector
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
var1		fcb	0;		; 
var2		fcb	0;		;




; Kernel Jump Vector Calls	
	org 	KERNEL_ROM		
KRNL_EXEC	jmp	[VECT_EXEC]	; This will likely be used as t	he EXEC vector
KRNL_SWI3 	jmp	[VECT_SWI3 ]	; SWI3 Software Interrupt Vector	
KRNL_SWI2 	jmp	[VECT_SWI2 ]	; SWI2 Software Interrupt Vector
KRNL_FIRQ 	jmp	[VECT_FIRQ ]	; FIRQ Software Interrupt Vector
KRNL_IRQ  	jmp	[VECT_IRQ  ]	; IRQ Software Interrupt Vector
KRNL_SWI  	jmp	[VECT_SWI  ]	; SWI / SYS Software Interrupt Vector
KRNL_NMI  	jmp	[VECT_NMI  ]	; NMI Software Interrupt Vector
KRNL_RESET	jmp	[VECT_RESET]	; RESET Software Interrupt Vector	

; a null vectors (i.e. an infinate loop traps for vector testing)
KRNL_UNDEF	bra	KRNL_UNDEF

; default subs
;EXEC_start	bra	EXEC_start	; EXEC program
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
		ldu	#0
		stu	VECT_EXEC
		ldu	#SSTACK_TOP

		lda	#$0A		; $0A ~= 1.0 mhz.
		sta	SYS_STATE

;		; build DSP_GRES
;		lda	#$8E		; $8E == 4-bpp, 16:9 aspect, 512x192 resolution
;		sta	DSP_GRES
;
;		; build DSP_EXT
;		lda	#$89
;		sta	DSP_EXT		

	; begin the display
		lda	#$B4		; default character color attribute
		sta	CSR_ATTR
		sta	TXT_ATTR

		jsr	cls
		ldx	#prompt0		
		jsr	line_out	

		ldx	#prompt1
		jsr	line_out	

		; fetch compilation date
		lda	#FC_COMPDATE
		sta	FIO_COMMAND
0		lda	FIO_PATH_DATA
		beq	2f
		jsr	char_out
		bra	0b
2		lda	#$0a
		jsr	char_out
		ldx	#prompt2
		jsr	line_out

main_user_input_loop
		ldx	#ready_prompt		
		jsr	line_out
1
		jsr	line_edit	
		jsr	decode_command_line	
		cmpa	#$fe	; just pressed enter, skip the ready prompt
		beq	1b
		bra	main_user_input_loop

command_table	fcn	" "		; #0
		fcn	"cls"		; #1
		fcn	"color"		; #2
		fcn	"load"		; #3
		fcn	"exec"		; #4
		fcn	"reset"		; #5
		fcn	"dir"		; #6
		fcn	"cd"		; #7
		fcn	"chdir"		; #8
		fcn	"exit"		; #9
		fcb	$FF		; $FF = end of list
command_vector_table
		fdb	do_nothing
		fdb	do_cls
		fdb	do_color
		fdb	do_load
		fdb	do_exec
		fdb	do_reset
		fdb	do_dir
		fdb	do_chdir
		fdb	do_chdir
		fdb	do_exit

str_eq		fcn	" ";
err_syntax	fcn	"ERROR: Syntax\n";
err_dir_nf	fcn	"ERROR: Directory Not Found\n";
err_file_nf	fcn	"ERROR: File Not Found\n";
err_file_no	fcn	"ERROR: File Not Open\n";
err_wrong_file_type	fcn	"ERROR: Wrong File Type\n"

* ; test strings
* str_cls_test	fcn	"CLS command issued\n"
* str_color_test	fcn	"COLOR command issued\n"
* str_load_test	fcn	"LOAD command issued\n"
* str_exec_test	fcn	"EXEC command issued\n"
* str_reset_test	fcn	"RESET command issued\n"
* str_dir_test	fcn	"DIR command issued\n"
* str_chdir_test	fcn	"CHDIR command issued\n"
* str_exit_test	fcn	"EXIT command issued\n"

decode_command_line
		jsr	decode_command_token	; A = index of the command being issued
		cmpa	#$ff			; command not found?
		beq	dcl_command_not_found	; simply return if command wasn't found
		bra	dcl_command_found	; branch to command was found
dcl_command_not_found	; command wasnt found
		rts				; return from subroutine
dcl_command_found
		lsla				; A = A*2.. addjust index two byte address
		ldx	#command_vector_table	; load the start of the command vector table
		jsr	[a,x]			; call the command funnction
		rts				; return from subroutine

do_nothing	
		lda	#$fe	; did nothing
		rts

do_cls
		jsr	arg1_8bit_attrib
		bne	1f
		;sta	TXT_ATTR	; remove TXT_ATTR from arg1_8bit_attrib and write it here
		jsr	cls
		bra	0f
1	; syntax error
		ldx	#err_syntax		; set string to ERROR: Syntax
		jsr	line_out		; output the string
0	; clean up and return
		rts


do_color	; COLOR $##
		jsr	arg1_8bit_attrib
		bne	1f
		;sta	TXT_ATTR	; remove TXT_ATTR from arg1_8bit_attrib and write it here
		bra	0f
1	; syntax error
		ldx	#err_syntax		; set string to ERROR: Syntax
		jsr	line_out		; output the string
0	; clean up and return
		rts

	
do_load		; LOAD "file_name"

		clr	FIO_PATH_POS	; reset the file path cursor position
		jsr	decode_command_find_arg_1
		nop	; X should point to the start of the first argument			
		;ldx	#EDT_BUFFER
1
		lda	,x+
		sta	FIO_PATH_DATA
		bne	1b

		lda	#FC_LOADHEX
		sta	FIO_COMMAND	

		lda	FIO_ERR_FLAGS
		cmpa	#$80
		beq	2f		; file not found	
		cmpa	#$20
		beq	4f		; file not open
		cmpa	#$04		
		beq	5f		; wrong file type
		bra	3f
5	; wrong file type
		ldx	#err_wrong_file_type
		jsr	line_out
		bra	3f
4	; file not open
		ldx	#err_file_no
		jsr	line_out
		bra	3f
2	; file not found
		ldx	#err_file_nf
		jsr	line_out
3
		rts



		
do_exec		; EXEC $####

	; save the system settings
;		lda	DSP_GRES
;		pshs	A
;		lda	DSP_EXT
;		pshs	A
		lda	TXT_ATTR
		pshs	A
		lda	SYS_STATE
		pshs	A

	; EXEC
		ldd	#0	
		cmpd	VECT_EXEC
		beq	3f
		jsr	[VECT_EXEC]
3
	; restore the system settings
		puls	A
		sta	SYS_STATE
		puls	A
		sta	TXT_ATTR
;		puls	A
;		cmpa	DSP_EXT
;		beq	1f
;		sta	DSP_EXT
1		
;		puls	A
;		cmpa	DSP_GRES
;		beq	2f
;		sta	DSP_GRES
2
;		jsr 	cls
		rts

do_reset	; RESET
		lda	#FC_RESET
		sta	FIO_COMMAND
		rts
		;jmp	[VECT_RESET]
		; rts

do_dir		; DIR
		jsr	decode_command_find_arg_1
		nop	; X should point to the start of the first argument			

		; copy argument 1 to the FIO_BUFFER
		ldy	#FIO_BUFFER
1		lda	,x+
		sta	,y+
		bne	1b
		nop	; argument one should now be copied to the FIO_BUFFER		

		lda	#FC_LISTDIR	; COMMAND: List Directory
		sta	FIO_COMMAND		
2	; output the result
		lda	FIO_DIR_DATA
		beq	3f
		jsr	char_out
		bra	2b
3	; send a new line character
		lda	#$0a
		jsr	char_out
	; cleanup and retrun
		rts

do_chdir	; CHDIR  "directory_path"
		jsr	decode_command_find_arg_1
		nop	; X should point to the start of the first argument
		clr	FIO_PATH_POS	; reset the cursor position in the file path
1	; 
		lda	,x+
		sta	FIO_PATH_DATA
		bne	1b
		lda	#FC_CHANGEDIR
		sta	FIO_COMMAND
		bra	2f
	; check for errors		
		lda	FIO_ERR_FLAGS
		anda	#$40
		beq	2f
		ldx	#err_dir_nf
		jsr	line_out
		clr	FIO_ERR_FLAGS
2	;
		lda	#FC_GETPATH
		sta	FIO_COMMAND
		jsr	char_out	
3	;
		lda	FIO_PATH_DATA
		beq	4f
		jsr	char_out
		bra	3b		
4	; cleanup and return	
		rts

do_exit		; EXIT  "directory_path"
		* ldx	#str_exit_test
		* jsr	line_out
		lda	#FC_SHUTDOWN
		sta	FIO_COMMAND
		;jsr	[VECT_CHDIR]
		rts

decode_command_find_arg_1	; position X to the start of the first argument
		pshs	A		; store the working registers
		ldx	#EDT_BUFFER	; point to the start of the edit buffer
1		lda	,x+		; load a character from the edit buffer
		beq	2f		; cleanup and return if its a null-terminator
		cmpa	#' '		; is it a SPACE character?
		bne	1b		; no, continue searching
2		puls	A		; restore the working registers
		rts			; return



decode_16bit_hex_digit
	; decode the LSB of the A accumilator into a value from 0-15
	; return B = 0-15   or $ff for invalid
		pshs	B		; store the working registers
		tfr	A, B		; transfer input A to working value B
		subb	#'0'		; subtract the ascii value of 0
		cmpb	#$09		; compare it to 9
		bgt	1f		; if its greater than 9 it's garbage
		bra	0f		; otherwise its's valid 
1	; check for alpha character
		tfr	A, B		; transfer input A to working value B
		orb	#$20		; force alpha character to lowercase
		cmpb	#'f'		; if the character is greater than F
		bgt	2f		;	it's invalid
		subb	#'a'-10		; convert ascii to numeric value
		bra	0f		; we're done, clean and return
2	; invalid character
		puls	B		; restore the stack
		lda	#$ff		; load $ff as an error code
		rts			; return from subroutine
0	; clean up and return
		tfr	B, A		; return the result to A
		puls	B		; restore the stack		
		rts			; return from subroutine


; TODO:  This sub needs to be generalized; read arg1 as a raw hex number
;		remove the TXT_ATTRIB stuff
;
arg1_8bit_attrib
	; returns in A the 2-digit hexidecimal value
	;	Z flag set if valid
	;	Z flag clear if invalid
		; Decode Argument ONE (color attribute in hex $FB)
		pshs 	B,X			; store the working registers
		jsr	decode_command_to_first_argument	; position X at the start of ARG1
		; default CMD with no argument
		lda	,x+			; laod the first character of the arg1
		beq	1f			; if its a NULL, just do the CLS and return
		cmpa	#'$'			; if its a $
		beq	2f			; check for a valid 8-bit hex value
3	; syntax error	(This could become its own standalone subroutine)
		andcc	#$FB			; clear the Z bit
		bra	0f			; clean up and return
2	; CMD $## = CMD with 8-bit hexidecimal argument
		ldb	2,x			; load two bytes in from the start of arg1
		beq	3b			; 	single digit hex is a syntax error
		cmpb	#' '			; check fo a space
		bne	3b			;	single digit hex is a syntax error
4	; X now points to space terminated ARG 1
		clra				; clear the local variable 
		pshs	A			; local in 0,S		
		lda	,x+			; fetch the next character
		bsr	decode_16bit_hex_digit	; convert hex nibble to values 0-15
		lsla				; shift left one bit
		lsla				; shift left one bit
		lsla				; shift left one bit
		lsla				; should now be most significant nibble
		sta	0,S			; update the local
		lda	,x+			; load the next character
		bsr	decode_16bit_hex_digit	; convert the hex nibble to value 0-15
		ora	0,S			; or it with the local
		sta	0,S			; and update the lcoal
		puls	A			; A = the local variable
		sta	TXT_ATTR		; store the new value in the TXT_ATTR
1	; CMD and return	
		orcc	#$04			; set the Z bit
0	; clean up and return		
		puls	B,X			; restore the working registers
		rts				; return from subroutine

decode_command_token
	; Return:
	;	A = Token ID
	;	A = $FF command not found
	;	X = start of first argument
		pshs	B,X,Y
	; initial initialization
		clra			; clear the token ID
		pshs	A		; 	store it locally @ 0,S 
		ldy	#command_table	; load the start of the command table
dct_0	; main loop
		ldx	#EDT_BUFFER	; load the start of the edit buffer
		jsr	str_cmp_nmz	; tokenize the command
		beq	dct_cmd_found	; command was found
	; not found at this entry in the table	
		ldx	#EDT_BUFFER	; reset X to the start of the edit buffer
dct_1	; progress Y to the next entry
		lda	,y+		; load character in the list entry
		cmpa	#$ff		; end of list?
		bne	dct_2		; 	no, continue
		sta	0,S		; store the end-of-list as token ID
		bra	dct_cmd_not_found	; cleanup and restore
dct_2	; loop conditions
		cmpa	#$00		; is it a null-terminator?
		bne	dct_1		; nope not yet, loop
		inc	0,S		; increment the token ID
		bra	dct_0		; loop to start checking the next entry
dct_cmd_not_found
		ldx	#err_syntax	; load address of the "ERROR: Syntax" string
		bra	dct_output	; go output the syntax error string
dct_cmd_found	; command was found in the table
		bra	dct_done	; skip to routine done
dct_output	; output the status string
		jsr	line_out	; output the syntax error
dct_done	; clean up and return
		puls	A		; clean up the stack, Token ID in A
		puls	B,X,Y		; restore the working registers
		rts			; return from subroutine

decode_command_to_first_argument
	; look for the first ' ' or '$' character to start ARG1
	; null character is end
		pshs	A
		ldx	#EDT_BUFFER	; point to the start of the edit buffer
1
		lda	,x+		; fetch next character from the buffer
		beq	dctfa_done	; branch if null-terminator?
		cmpa	#' '
		beq	dctfa_done
		cmpa	#'$'
		beq	dctfa_done
		bra	1b
dctfa_done
		puls	A
		rts



str_cmp_nmz	; compare two null/space/"-terminated strings
		; 	on return:
		; 	A = tokenized command
		;	X points to argument 1
		;	Y points to argument 2
		;	returns: compare condition in the Z and N condition flags
		pshs	B,X,Y		; store the working registers
		lda	#0		; clear the token flag
		pshs	A		; 	and store it locally @ 0,S
str_cmp_loop	; is the string 1 character a terminator?
		lda	,x		; fetch a character from the first string
		beq	str1_cmp_term	; is a null-terminator? verify other string
		cmpa	#' '		; is it a space character?
		beq	str1_cmp_term	; branch to verify other strings termination
		cmpa	#'"'		; is it a double quote?
		beq	str1_cmp_term	; branch to verify other strings termination
		cmpa	#$0d		; is it an ENTER character?
		beq	str1_cmp_term	; branch to verify other strings termination
		; is the string 2 character a terminator?
		ldb	,y		; fetch a character from the second string
		beq	str2_cmp_term	; is a null-terminator? verify other string
		cmpb	#' '		; is it a space character?
		beq	str2_cmp_term	; branch to verify other strings termination
		cmpb	#'"'		; is it a double-quote character?
		beq	str2_cmp_term	; branch to verify other strings termination
		cmpb	#$0d		; is it an ENTER character?
		beq	str2_cmp_term	; branch to verify other strings termination
str_cmp_equal	; are characters are same, loop
		ora	#$20		; A to lowercase
		leax	1,x		; increment X
		cmpa	,y+		; compare characters and increment Y
		beq	str_cmp_loop	; loop if characters are equal
		bra	str_cmp_differ	; loop finished, not equal
str1_cmp_term	; string 1 has terminated, check string 2 termination
		ldb	,y		; load a character from the second string
		beq	str_cmp_done	; is it a null-terminator? strings are equal
		cmpb	#' '		; is it a SPACE terminator?
		beq	str_cmp_done	;	strings are equal
		cmpb	#'"'		; is it a double-quote terminator?
		beq	str_cmp_done	;	strings are equal
		cmpb	#$0d		; is it an ENTER terminator?
		beq	str_cmp_done	;	strings are equal
		bra	str_cmp_differ	; otherwise, strings are different
str2_cmp_term	; string 2 has terminated, check string 1 for termination
		lda	,x		; load a character from the first string
		beq	str_cmp_done	; is it a null-terminator? strings are equal
		cmpa	#' '		; is it SPACE terminator?
		beq	str_cmp_done	;	strings are equal
		cmpa	#'"'		; is it a double-space terminator?
		beq	str_cmp_done	;	strings are equal
		cmpa	#$0d		; is it an ENTER terminator?
		beq	str_cmp_done	; 	strings are equal
		bra	str_cmp_differ	; otherwise, strings are different
		; characters differ, drop out
str_cmp_differ	lda	#$ff		; set the token flag
		sta	0,S		; 	store it locally @ 0,S
str_cmp_done	; done comparing the two strings
		; tst	0,S		; set the CC appropriate to the local flag (0,S)
		puls	A		; correct the stack
		puls	B,X,Y		; restore the working registers
		cmpa	#$00
		rts			; return






; output the line edit buffer
line_edit
;	; clear out the input queue
;		lda	CHAR_Q_LEN
;		beq	8f
;9		lda	CHAR_POP
;		bne	9b
;8
		lda	#$ff		; 'true'
		sta	EDT_ENABLE	; enable the hardware line editor sub-system
		clr	EDT_BUFFER	; null the start of the edit buffer
		ldd	CSR_ADDR	; fetch the current cursor address
		std	EDLIN_ANCH	; store it as the anchor
6	; check for ENTER press
		lda	CHAR_POP	; scan for a character waiting in the queue
		cmpa	#$0d		; is it the ENTER key?
		bne	0f		; nope, continue to render the text line editor
		; lda	CHAR_POP	; pop the ENTER from the queue before returning
		lda	#$20		; load a space
		ldb	TXT_ATTR	; with color
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
		; ldu	#STD_VID_MIN	; reserve the start of the display buffer
		; ldu	EDLIN_ANCH	; reserve the start of the display buffer
		ldy	#EDT_BUFFER	; start of the character line edit buffer
		; ldx	#STD_VID_MIN	; point to the start of the display buffer
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
		ldb	TXT_ATTR	; color attribute
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
		ldb	TXT_ATTR	; load the basic screen attribute
		std	,x++		; display the next character with its color
		bra	5b		; continue until the line is finished

flash_the_cursor
		pshs	a,b,x,u		; save these on the stack
	; check to see if the cursor is below the visible screen
	; this can happen if the overscan registers have changed
	; when it does happen, simply clean up the stack
	; and jmp to the main input loop
		ldu	CSR_ADDR
		cmpu	STD_VID_MAX
		blt	0f
		jsr	cls
		puls	d,x,u
		puls	x,u
		jmp	main_user_input_loop
0	; continue normally
		ldu	EDLIN_ANCH	; point to the edline start of line anchor
		lda	SYS_TIMER+1	; load the LSB of the system timer
		lsra			; shift right
		lsra			; shift right
		lsra			; shift right
;		lsra			; shift right
		anda	#$07		; only concerned with these bits
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
		ldx	#STD_VID_MIN	; point to the start of the text buffer
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
		ldy	#STD_VID_MIN	; Y = start of the standard video buffer
		ldb	DSP_TXT_COLS	; B = number of displayed text columns
		;lslb			; B = B * 2 -- account for the attribute byte
		ldx	#STD_VID_MIN	; X = start of the standard video buffer
		; B can become negative in the 4x horizontal overscan modes
		; 	instead of shifting B left and using it to index once
		;       simply load effective address twice
		leax	b,x		; X = one line down
		leax	b,x		; X = one line down
1	; scroll up one line
		ldd	,x++		; load character at X 
		std	,y++		; store it at Y
		cmpx	STD_VID_MAX	; at the end of the display?
		blt	1b		; loop if not
	; fill the bottom line with spaces
		ldx	#STD_VID_MIN	; X = start of the standard video buffer
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
		ldx	#STD_VID_MIN	; start of the standard video buffer
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





		org	$fe00
prompt0		fcn	"Retro 6809 Kernel v.0.22.6 \n"
prompt1		fcn	"Emulator compiled on "
prompt2		fcn	"Copyright (C) 2024 By Jay Faries\n\n"
ready_prompt	fcn	"Ready\n"




; ROM Based Hardware Vectors
		org	$FFF0
		fdb	KRNL_EXEC	; HARD_RSRVD       EXEC Interrupt Vector
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
