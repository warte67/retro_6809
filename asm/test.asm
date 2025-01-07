; test.asm
;

		INCLUDE "Memory_Map.asm"

		org	0xFFF0
		fdb	start


		org $2400

start		ldd	#$0100
lp2		ldx	#VIDEO_START		
lp1		std	,x++
		addd	#1
		cmpx	#VIDEO_START + $0500
		blt	lp1
		bra	lp2




