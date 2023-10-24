; ***********************************************************************
; * 	kernel_f000.asm 						*
; *									*
; *	a.k.a. The Basic Input and Output System (BIOS)			*
; ***********************************************************************
; *									*
; *  	LINUX: 		./asm/build.sh asm/kernel_f000			*
; *		or							*	
; *	WINDOWS:	asm kernel_f000					*
; *									*
; *	build.sh:							*
; *		#!/bin/bash						*
; *		#lwasm -9 test.asm --format=ihex -otest.hex -ltest.lst	*
; *		echo "lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst"	*
; *		lwasm -9 $1.asm --format=ihex -o$1.hex -l$1.lst		*
; *									*
; *	asm.bat:							*
; *     	@set arg1=%1						*
; *     	asm6809 -H -9 %arg1%.asm -o %arg1%.hex -l %arg1%.lst	*
; ***********************************************************************
		INCLUDE "memory_map.asm"

; Software Vectors
	org	$0000	
VECT_RSRVD	fdb	RSRVD_start	; This will likely be used as the EXEC vector
VECT_SWI3 	fdb	SWI3_start	; SWI3 Software Interrupt Vector
VECT_SWI2 	fdb	SWI2_start	; SWI2 Software Interrupt Vector
VECT_FIRQ 	fdb	FIRQ_start	; FIRQ Software Interrupt Vector
VECT_IRQ  	fdb	IRQ_start	; IRQ Software Interrupt Vector
VECT_SWI  	fdb	SWI_start	; SWI / SYS Software Interrupt Vector
VECT_NMI  	fdb	NMI_start	; NMI Software Interrupt Vector	
VECT_RESET	fdb	kernel_start	; RESET Software Interrupt Vecto	r	
	
; Kernel Jump Vector Calls	
	org 	KERNEL_ROM		
KRNL_RSRVD	jmp	[VECT_RSRVD]	; This will likely be used as t	he EXEC vector
KRNL_SWI3 	jmp	[VECT_SWI3 ]	; SWI3 Software Interrupt Vector	
KRNL_SWI2 	jmp	[VECT_SWI2 ]	; SWI2 Software Interrupt Vector
KRNL_FIRQ 	jmp	[VECT_FIRQ ]	; FIRQ Software Interrupt Vector
KRNL_IRQ  	jmp	[VECT_IRQ  ]	; IRQ Software Interrupt Vector
KRNL_SWI  	jmp	[VECT_SWI  ]	; SWI / SYS Software Interrupt Vector
KRNL_NMI  	jmp	[VECT_NMI  ]	; NMI Software Interrupt Vector
KRNL_RESET	jmp	[VECT_RESET]	; RESET Software Interrupt Vector	

; a null vectors (i.e. an infinate loop traps for vector testing)
KRNL_UNDEF	bra	KRNL_UNDEF

RSRVD_start	bra	RSRVD_start	; EXEC program
SWI3_start	bra	SWI3_start	; SWI3 Implementation
SWI2_start	bra	SWI2_start	; SWI2 Implementation
FIRQ_start	bra	FIRQ_start	; FIRQ Implementation
IRQ_start	bra	IRQ_start	; IRQ Implementation
SWI_start	bra	SWI_start	; SWI / SYS Implementation
NMI_start	bra	NMI_start	; NMI Implementation
RESET_start	bra	RESET_start	; RESET Implementation

kernel_start
		; initialize both stack pointers
		lds	#SSTACK_TOP
		ldu	#USTACK_TOP
; fill the screen
		lda	#$00
		ldx	#SCREEN_BUFFER
lp1	
		inca
		sta	,x+
		cmpx	#STD_VID_MAX
		ble	lp1

; cycle the palette colors
;		lda	#0	
;lp2		sta	DSP_PAL_IDX
;		ldy	DSP_PAL_CLR
;		leay	256,y
;		sty	DSP_PAL_CLR
;		inca
;		bra	lp2

; increment the text glyph bitmaps
;		lda	#0
;lp4		sta	DSP_GLYPH_IDX
;		ldx	#DSP_GLYPH_DATA
;lp_b		rol	,x+
;		cmpx	#DSP_GLYPH_DATA+8
;		blt	lp_b
;		inca
;		bra	lp4


; increment the characters on the screen
		lda	#1
		sta	CSR_PAL_INDX

inc_screen
		ldx	#SCREEN_BUFFER
lp3
	; cycle the cursor color
		ldd	CSR_PAL_DATA
		addd	#$0001
		ora	#$f0		; full alpha
		std	CSR_PAL_DATA

	; increment character and attribute data
		inc	,x+
		cmpx	STD_VID_MAX
		ble	lp3

;		lda	#0
;		sta	CSR_PAL_INDX
;		ldd	#$f00f
;		std	CSR_PAL_DATA
inf_loop
		bra	inc_screen


; ROM Based Hardware Vectors
		org	$FFF0
		fdb	KRNL_RSRVD	; HARD_RSRVD       RESERVED Hardware Interrupt Vector
		fdb	KRNL_SWI3  	; HARD_SWI3        SWI3 Hardware Interrupt Vector
		fdb	KRNL_SWI2  	; HARD_SWI2        SWI2 Hardware Interrupt Vector
		fdb	KRNL_FIRQ  	; HARD_FIRQ        FIRQ Hardware Interrupt Vector
		fdb	KRNL_IRQ    	; HARD_IRQ         IRQ Hardware Interrupt Vector
		fdb	KRNL_SWI    	; HARD_SWI         SWI / SYS Hardware Interrupt Vector
		fdb	KRNL_NMI    	; HARD_NMI         NMI Hardware Interrupt Vector
		fdb	KRNL_RESET 	; HARD_RESET       RESET Hardware Interrupt Vector