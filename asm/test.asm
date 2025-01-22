; test.asm
;

		INCLUDE "Kernel_Header.asm"

		org	$0000		; point at the EXEC vector
		fdb	start		; set the starting addresss of the program

		org $2400		; point to the start of user memory

start			
		ldd	#$0100
lp2		ldx	#VIDEO_START		
		addd	#1
lp1		std	,x++
		addd	#1

	; test for a key press
		tst	CHAR_Q_LEN
		bne	cleanup

		cmpx	GPU_VIDEO_MAX
		blt	lp1
		bra	lp2
	
cleanup	; clean up the queue
		lda	CHAR_SCAN
		bne	done
		lda	CHAR_POP
done	; clean up and return

		sys	CALL_CLS
		rts




