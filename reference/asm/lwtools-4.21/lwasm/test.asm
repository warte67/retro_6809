; ./lwasm --format=ihex --list=test.lst --output=test.hex test.asm

		org	$2000

var1		fcb	$00

start		
		lda	#$80
		sta	var1
		rts
