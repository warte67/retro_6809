		INCLUDE "kernel_f000.sym"

		org	$0000
		fdb	start		; SOFT_EXEC

		org	$3000
start		
		; crank the CPU speed some
		lda	#$0E	
		sta	SYS_STATE

* 		; write float string into ACA
* 		clr	MATH_ACA_POS
* 		ldx	#test_aca_string
* st_1		lda	,x+
* 		sta	MATH_ACA_DATA
* 		beq	st_2
* 		bmi	st_2
* 		bra	st_1
* st_2
* 		; write float string into ACB
* 		clr	MATH_ACB_POS
* 		ldx	#test_acb_string
* st_3		lda	,x+
* 		sta	MATH_ACB_DATA
* 		beq	3f
* 		bmi	3f
* 		bra	st_3

		;
		ldd	#$0000
		std	MATH_ACA_INT
		ldd	#$0145
		std	MATH_ACA_INT+2	
		;
		ldd	#$0000
		std	MATH_ACB_INT
		ldd	#$0010
		std	MATH_ACB_INT+2
		;

		


		; display ACA
3		clr	MATH_ACA_POS
1		lda	MATH_ACA_DATA
		beq	2f
		jsr	char_out
		bra	1b
2
		lda	#$0a
		jsr	char_out

		; display ACB
		clr	MATH_ACB_POS
1		lda	MATH_ACB_DATA
		beq	2f
		jsr	char_out
		bra	1b
2
		lda	#$0a
		jsr	char_out

st_4
		; perform a math operation
		* clr 	MATH_ACA_INT+0
		* clr 	MATH_ACA_INT+1
		* clr 	MATH_ACA_INT+2
		* clr 	MATH_ACA_INT+3
		* inc 	MATH_ACA_INT+3
		* lda	#MOP_RND_SEED
		* sta	MATH_OPERATION

		lda	#MOP_RANDOM
		sta	MATH_OPERATION

		; display ACR (result)
		clr	MATH_ACR_POS
1		lda	MATH_ACR_DATA
		beq	2f
		jsr	char_out
		bra	1b
2
		lda	#$0a
		jsr	char_out




		; output true / false result
* 		ldx	#str_true
* 		tst	MATH_ACR_INT+3
* 		beq	st_5
* 		bra	st_6
* st_5
* 		ldx	#str_false
* st_6
* 		jsr	line_out

done	; clean up and return
		lda	#$0a
		jsr	char_out
		rts




test_aca_string	fcn	"5.3"
test_acb_string	fcn	"3.141529"
str_true	fcn	"TRUE"
str_false	fcn	"FALSE"
test_raw	fcb	$40
		fcb	$49
		fcb	$0e
		fcb	$d0
