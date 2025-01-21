; ***** kernel_header.asm ************************************************************************
; *      _  __                    _     _   _                _                                 
; *     | |/ /___ _ __ _ __   ___| |   | | | | ___  __ _  __| | ___ _ __    __ _ ___ _ __ ___  
; *     | ' // _ \ '__| '_ \ / _ \ |   | |_| |/ _ \/ _` |/ _` |/ _ \ '__|  / _` / __| '_ ` _ \ 
; *     | . \  __/ |  | | | |  __/ |   |  _  |  __/ (_| | (_| |  __/ |_   | (_| \__ \ | | | | |
; *     |_|\_\___|_|  |_| |_|\___|_|___|_| |_|\___|\__,_|\__,_|\___|_(_)   \__,_|___/_| |_| |_|
; *                               |_____|                          
; *												
; ************************************************************************************************           

		INCLUDE "Memory_Map.asm"

; sys macro expands to SWI2 with a following command byte
sys		macro
		swi2
		fcb	\1
		endm


; *****************************************************************************
; * SYSTEM SOFTWARE VECTORS                                                   *
; *****************************************************************************
VEC_EXEC	equ	$0000	; User defined EXEC vector
VEC_SWI3 	equ	$0002	; SWI3 Software Interrupt Vector
VEC_SWI2 	equ	$0004	; SWI2 Software Interrupt Vector
VEC_FIRQ 	equ	$0006	; FIRQ Software Interrupt Vector
VEC_IRQ  	equ	$0008	; IRQ Software Interrupt Vector
VEC_SWI  	equ	$000A	; SWI / SYS Software Interrupt Vector
VEC_NMI  	equ	$000C	; NMI Software Interrupt Vector	
VEC_RESET	equ	$000E	; RESET Software Interrupt Vector	


; *******************************************************************************
; * SYS CALL CONSTANTS                                                          *
; *	usage:									*
; *		sys	CALL_CLS	; clear the screen			*	
; *		...								*
; *										*
; *		lda	#'Z'		; Load the text glyph 'Z' into A	*
; *		sys	CALL_CHROUT	; Display the 'Z' on the text screen	*	
; *		...								*
; *										*
; *										*
; *		sys	CALL_LINEOUT	; Display Null-terminated string	*
; *					; pointed to by X at the current	*
; *					; cursor location: 			*
; *						_CURSOR_COL x _CURSOR_ROW	*
; *		...								*
; *******************************************************************************
CALL_GARBAGE	equ	$00	; $00 random garbage (errror)
CALL_CLS        equ	$01 	; $01 CLS 	(void)
CALL_CHROUT	equ	$02	; $02 CHROUT	Display the Text Glyph in A
CALL_NEWLINE    equ	$03     ; $03 NEWLINE   (void)
CALL_TAB        equ	$04     ; $04 TAB       (void)
CALL_LINEOUT    equ	$05  	; $05 LINEOUT   Entry: X=string
CALL_CSRPOS     equ	$06     ; $06 CSRPOS    Exit: Character Display Address in X
CALL_SCROLL     equ	$07     ; $07 SCROLL    (void)      
CALL_LINEEDIT	equ	$08	; $08 LINEEDIT	(void)
CALL_GETKEY	equ	$09	; $09 GETKEY    Returns Key Press in A


; *****************************************************************************
; * RESERVED ZERO PAGE KERNAL VARIABLES                                       *
; *****************************************************************************
_CURSOR_COL	equ	$0010	; (Byte) current cursor horizontal position
_CURSOR_ROW	equ	$0011	; (Byte) current cursor vertical position
_ATTRIB		equ	$0012	; (Byte) current character display attribute
_ANCHOR_COL	equ	$0013	; (Byte) line edit anchor column
_ANCHOR_ROW	equ	$0014	; (Byte) line edit anchor row
_LOCAL_0	equ	$0015	; (Byte) used locally for some kernel calls
_LOCAL_1	equ	$0016	; (Byte) used locally for some kernel calls
_LOCAL_2	equ	$0017	; (Byte) used locally for some kernel calls
_LOCAL_3	equ	$0018	; (Byte) used locally for some kernel calls

ZERO_PAGE_USER	equ	_LOCAL_3+1	; start of user appropriate zero-page memory