		INCLUDE "kernel_f000.sym"

		org	$0000
		fdb	start		; SOFT_EXEC

		org	$3000
start		
		bra 	begin
last_gmode	fdb	$808C

begin
		lda	TXT_ATTR

		; crank the CPU speed some
		lda	#$0E	
		sta	SYS_STATE

		; set up the default display mode
;		lda	#$00		; 2-color mode
;		lda	#$40		; 4-color mode
;		lda	#$80		; 16-color mode
;		lda	#$C0		; 256-color mode (16)
;		sta	DSP_GRES
;		; set DSP_EXT
;		lda	DSP_EXT
;		anda	#0b00000011	; change to standard ...
;		ora	#0b10001100	; ... bitmap mode
;		sta	DSP_EXT

3
		ldd	DSP_RES
		std	last_gmode

; should now be in standard bitmap graphics mode

		lda	#$00
		ldx	#STD_VID_MIN
1		sta	,x+
		inca
		cmpx	STD_VID_MAX
		blt	1b
2
		ldx	#STD_VID_MIN
1		inc	,x+
		cmpx	STD_VID_MAX
		blt	1b

		; restart if the graphics mode has changed
		ldd	DSP_RES
		cmpd	last_gmode
		bne	3b		

	; wait for a key press
		lda	CHAR_Q_LEN
		beq	2b
	; clean up the queue
		lda	CHAR_SCAN
		bne	3f
		lda	CHAR_POP
3	; clean up and return
;		puls	A
;		sta	TXT_ATTR
		jsr	cls
		rts



test_str	fcn	"\nTest:  Hello World\npress a key to exit...\n\n";
