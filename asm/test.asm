		org		$f000
	
start	lda		#$00
lp		sta 	$0000
		inca
		bra		lp

		org		$fffe
		fdb		$F000		