	INCLUDE "memory_map.asm"

	org	$f000	

; fill the screen
start	
	lda	#$00
	ldx	#SCREEN_BUFFER
lp1	
	inca
	sta	,x+
	cmpx	STD_VID_MAX
	ble	lp1
; increment the screen
inc_screen
	ldx	#SCREEN_BUFFER
lp2
	inc	,x+
	cmpx	STD_VID_MAX
	ble	lp2

	bra	inc_screen




	org	$fffe
	fdb	$F000	; HARD_RESET