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
sys                 macro
                    swi2
                    fcb	\1
                    endm


; *****************************************************************************
; * SYSTEM SOFTWARE VECTORS                                                   *
; *****************************************************************************
VEC_EXEC	    equ     $0000	; User defined EXEC vector
VEC_SWI3 	    equ     $0002	; SWI3 Software Interrupt Vector
VEC_SWI2 	    equ     $0004	; SWI2 Software Interrupt Vector
VEC_FIRQ 	    equ     $0006	; FIRQ Software Interrupt Vector
VEC_IRQ  	    equ     $0008	; IRQ Software Interrupt Vector
VEC_SWI  	    equ     $000A	; SWI / SYS Software Interrupt Vector
VEC_NMI  	    equ     $000C	; NMI Software Interrupt Vector
VEC_RESET	    equ     $000E	; RESET Software Interrupt Vector

; *****************************************************************************
; * KERNAL ROUTINE SOFTWARE VECTORS                                           *
; *****************************************************************************
VEC_CLS		    equ	    $0010	; CLS function Software Vector
VEC_CHROUT	    equ	    $0012	; Character Out Software Vector
VEC_NEWLINE	    equ	    $0014	; Kernel Newline Software Vector
VEC_LINEOUT	    equ	    $0016	; String to Console Software Vector
VEC_CSRPOS	    equ	    $0018	; Cursor Position Software Vector
VEC_SCROLL	    equ	    $001A	; Scroll Text Screen Software Vector
VEC_LINEEDIT	    equ	    $001C	; Console Line Editor Software Vector
VEC_GETKEY	    equ	    $001E	; Wait for Key Press Software Vector
VEC_GETHEX	    equ	    $0020	; Wait for Hex Character Software Vector
VEC_GETNUM	    equ	    $0022	; Wait for Numeric Character Vector
VEC_CMPSTR	    equ	    $0024	; Compare two strings of arbitrary lengths Vector
VEC_CMD_PROC	    equ	    $0026	; Parse the command entered by the user Vector
VEC_TBLSEARCH	    equ	    $0028	; Table Search and Return index Vector
VEC_CPY_DWORD	    equ	    $002A	; Copy 32-bits from addr X addr Y Vector
VEC_D_TO_RAWA	    equ	    $002C	; Write the D register to RAWA  Vector
VEC_D_TO_RAWB	    equ	    $002E	; Write the D register to RAWB Vector
VEC_D_TO_RAWR	    equ	    $0030	; Write the D register to RAWR Vector
VEC_D_TO_INTA	    equ	    $0032	; Write the D register to INTA Vector
VEC_D_TO_INTB	    equ	    $0034	; Write the D register to INTB Vector
VEC_D_TO_INTR	    equ	    $0036	; Write the D register to INTR Vector
VEC_RAWA_TO_D	    equ	    $0038	; Read the RAWA float into D Vector
VEC_RAWB_TO_D	    equ	    $003A	; Read the RAWB float into D Vector
VEC_RAWR_TO_D	    equ	    $003C	; Read the RAWD float into D Vector
VEC_INTA_TO_D	    equ	    $003E	; Read the INTA integer into D Vector
VEC_INTB_TO_D	    equ	    $0040	; Read the INTB integer into D Vector
VEC_INTR_TO_D	    equ	    $0042	; Read the INTR integer into D Vector
VEC_8BIT_MATH	    equ	    $0044	; 8-bit integer math Vector
VEC_DSP_ACA	    equ	    $0046	; Send to console the float in ACA Vector
VEC_DSP_ACB	    equ	    $0048	; Send to console the float in ACB Vector
VEC_DSP_ACR	    equ	    $004A	; Send to console the float in ACA Vector
VEC_DSP_INTA	    equ	    $004C	; Send to console the integer in ACA Vector
VEC_DSP_INTB	    equ	    $004E	; Send to console the integer in ACB Vector
VEC_DSP_INTR	    equ	    $0050	; Send to console the integer in ACR Vector
VEC_WRITE_ACA	    equ	    $0052	; Write RAW float X points to into ACA Vector
VEC_WRITE_ACB	    equ	    $0054	; Write RAW float X points to into ACB Vector
VEC_WRITE_ACR	    equ	    $0056	; Write RAW float X points to into ACR Vector
VEC_ARG_TO_A	    equ	    $0058	; Convert numeric string to binary in A Vector


; *****************************************************************************
; * RESERVED ZERO PAGE KERNAL VARIABLES                                       *
; *****************************************************************************
_CURSOR_COL	    equ	    $005A	; (Byte) current cursor horizontal position
_CURSOR_ROW	    equ	    $005B	; (Byte) current cursor vertical position
_ATTRIB		    equ	    $005C	; (Byte) current character display attribute
_ANCHOR_COL	    equ	    $005D	; (Byte) line edit anchor column
_ANCHOR_ROW	    equ	    $005E	; (Byte) line edit anchor row
_LOCAL_0	    equ	    $005F	; (Byte) used locally for some kernel calls
_LOCAL_1	    equ	    $0060	; (Byte) used locally for some kernel calls
_LOCAL_2	    equ	    $0061	; (Byte) used locally for some kernel calls
_LOCAL_3	    equ	    $0062	; (Byte) used locally for some kernel calls

ZERO_PAGE_USER	    equ	    _LOCAL_3+1	; start of user appropriate zero-page memory


; *******************************************************************************
; * SYS CALL CONSTANTS                                                          *
; *	usage:		                                                        *
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
CALL_GARBAGE	    equ	    $00	    ; $00 random garbage
CALL_CLS            equ	    $01     	; $01 CLS
CALL_CHROUT	    equ	    $02	    ; $02 CHROUT
CALL_NEWLINE        equ	    $03         ; $03 NEWLINE
CALL_TAB            equ	    $04         ; $04 TAB
CALL_LINEOUT        equ	    $05      	; $05 LINEOUT
CALL_CSRPOS         equ	    $06         ; $06 CSRPOS
CALL_SCROLL         equ	    $07         ; $07 SCROLL
CALL_LINEEDIT	    equ	    $08	    ; $08 LINEEDIT
CALL_GETKEY         equ	    $09	    ; $09 GETKEY
CALL_GETHEX         equ	    $0A	    ; $0A GETHEX
CALL_GETNUM         equ	    $0B	    ; $0B GETNUM
CALL_CMPSTR         equ	    $0C	    ; $0C CMPSTR
CALL_CMD_PROC	    equ	    $0D	    ; $0D CMD_PROC
CALL_TBLSEARCH	    equ	    $0E	    ; $0E TBLSEARCH
CALL_CPY_DWORD	    equ	    $0F	    ; $0F CPY_DWORD
CALL_D_TO_RAWA	    equ	    $10	    ; $10 D_TO_RAWA
CALL_D_TO_RAWB	    equ	    $11	    ; $11 D_TO_RAWB
CALL_D_TO_RAWR	    equ	    $12	    ; $12 D_TO_RAWR
CALL_D_TO_INTA	    equ	    $13	    ; $13 D_TO_INTA
CALL_D_TO_INTB	    equ	    $14	    ; $14 D_TO_INTB
CALL_D_TO_INTR	    equ	    $15	    ; $15 D_TO_INTR
CALL_RAWA_TO_D	    equ	    $16	    ; $16 RAWA_TO_D
CALL_RAWB_TO_D	    equ	    $17	    ; $17 RAWB_TO_D
CALL_RAWR_TO_D	    equ	    $18	    ; $18 RAWR_TO_D
CALL_INTA_TO_D	    equ	    $19	    ; $19 INTA_TO_D
CALL_INTB_TO_D	    equ	    $1A	    ; $1A INTB_TO_D
CALL_INTR_TO_D	    equ	    $1B	    ; $1B INTR_TO_D
CALL_8BIT_MATH	    equ	    $1C	    ; $1C 8BIT_MATH
CALL_DSP_ACA	    equ	    $1D	    ; $1D DSP_ACA
CALL_DSP_ACB	    equ	    $1E	    ; $1E DSP_ACB
CALL_DSP_ACR	    equ	    $1F	    ; $1F DSP_ACR
CALL_DSP_INTA	    equ	    $20	    ; $20 DSP_INTA
CALL_DSP_INTB	    equ	    $21	    ; $21 DSP_INTB
CALL_DSP_INTR	    equ	    $22	    ; $22 DSP_INTR
CALL_WRITE_ACA	    equ	    $23	    ; $23 WRITE_ACA
CALL_WRITE_ACB	    equ	    $24	    ; $24 WRITE_ACB
CALL_WRITE_ACR	    equ	    $25	    ; $25 WRITE_ACR
CALL_ARG_TO_A	    equ	    $26	    ; $26 ARG_TO_A










