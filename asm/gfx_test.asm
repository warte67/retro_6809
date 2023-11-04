		INCLUDE "kernel_f000.sym"

		org	$0000
		fdb	start		; SOFT_EXEC

		org	$3000
start		
		; crank the CPU speed some
		lda	#$0E	
		sta	SYS_STATE
		; set DSP_GRES
		lda	#$80
		sta	DSP_GRES
		; set DSP_EXT
		lda	DSP_EXT
		anda	#0b00000011
		ora	#0b10001100
		sta	DSP_EXT

		lda	#$c5
		sta	TXT_ATTR

		ldx	#test_str
		jsr	line_out

1	; wait for a key press
		lda	CHAR_Q_LEN
		beq	1b
2	; clean up the queue
		lda	CHAR_SCAN
		bne	3f
		lda	CHAR_POP
3	; clean up and return
		rts



test_str	fcn	"\nTest:  Hello World\npress a key to exit...\n\n";
