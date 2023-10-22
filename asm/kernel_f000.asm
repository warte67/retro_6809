; kernel_f000.asm
;
;  ./asm/build.sh asm/kernel_f000
;
	INCLUDE "memory_map.asm"

	org	$f000	

start	

; no-color screen fill
	ldy	#$1f00
	ldd	#$001f		; LDD loads A and B in wrong order
	;lda	#$00
	;ldb	#$1f
	ldx	#SCREEN_BUFFER
lp0	
	sty	,x++		
	;std	,x++		; STD stores A and B in correct order
	;sta	,x+
	;stb	,x+
	cmpx	STD_VID_MAX
	ble	lp0
	bra	start

; fill the screen
;	lda	#$00
;	ldx	#SCREEN_BUFFER
;lp1	
;	inca
;	sta	,x+
;	cmpx	STD_VID_MAX
;	ble	lp1

; cycle the palette colors
;	lda	#0	
;lp2	sta	DSP_PAL_IDX
;	ldy	DSP_PAL_CLR
;	leay	256,y
;	sty	DSP_PAL_CLR
;	inca
;	bra	lp2

; increment the text glyph bitmaps
;	lda	#0
;lp4	sta	DSP_GLYPH_IDX
;	ldx	#DSP_GLYPH_DATA
;lp_b	rol	,x+
;	cmpx	#DSP_GLYPH_DATA+8
;	blt	lp_b
;	inca
;	bra	lp4


; increment the characters on the screen
	lda	#1
	sta	CSR_PAL_INDX

inc_screen
	ldx	#SCREEN_BUFFER
lp3
;	; cycle the cursor color
;	ldd	CSR_PAL_DATA
;	addd	#$0001
;	orb	#$f0		; full alpha
;	std	CSR_PAL_DATA
	ldd	#$F0F0
	std	CSR_PAL_DATA

	; increment character and attribute data
	inc	,x++
	cmpx	STD_VID_MAX
	ble	lp3
	bra	inc_screen




	org	$FFF0
	fdb	$0000	; HARD_RSRVD 	Motorola RESERVED Hardware Interrupt Vector
	fdb	$0000	; HARD_SWI3  	SWI3 Hardware Interrupt Vector
	fdb	$0000	; HARD_SWI2  	SWI2 Hardware Interrupt Vector
	fdb	$0000	; HARD_FIRQ  	FIRQ Hardware Interrupt Vector
	fdb	$0000	; HARD_IRQ   	IRQ Hardware Interrupt Vector
	fdb	$0000	; HARD_SWI   	SWI / SYS Hardware Interrupt Vector
	fdb	$0000	; HARD_NMI   	NMI Hardware Interrupt Vector
	fdb	$F000	; HARD_RESET	RESET Hardware Interrupt Vector