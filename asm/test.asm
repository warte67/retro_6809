; test.asm
;

		INCLUDE "Memory_Map.asm"

		org	$FFFE
		fdb	start


		org $2400

start		ldd	#$0100
lp2		ldx	#VIDEO_START		
		addd	#1
lp1		std	,x++
		addd	#1
		cmpx	GPU_VIDEO_MAX
		blt	lp1
		bra	lp2




