;*** kernel.asm ************************
;*      _                        _                           
;*     | | _____ _ __ _ __   ___| |       __ _ ___ _ __ ___  
;*     | |/ / _ \ '__| '_ \ / _ \ |      / _` / __| '_ ` _ \ 
;*     |   <  __/ |  | | | |  __/ |  _  | (_| \__ \ | | | | |
;*     |_|\_\___|_|  |_| |_|\___|_| (_)  \__,_|___/_| |_| |_|
;*                
;* Released under the GPL v3.0 License.
;* Original Author: Jay Faries (warte67)                                           
;*****************************

			INCLUDE "Memory_Map.asm"

			org	SOFT_VECTORS_DEVICE

			fdb	#0	; SOFT_EXEC            				
			fdb	#0	; SOFT_SWI3            	
			fdb	#0	; SOFT_SWI2            	
			fdb	#0	; SOFT_FIRQ            	
			fdb	#0	; SOFT_IRQ             	
			fdb	#0	; SOFT_SWI             	
			fdb	#0	; SOFT_NMI             	
			fdb	#0	; SOFT_RESET           	

			org	HARD_RESET
			fdb	start

; ---------------------------------------------------------------------


			org 	KERNEL_START

start			bra	begin

vers_label		fcn	"Kernel Version: "
vers_number		fcn	"0.0.1"

begin			; warm reset?
			ldd	SOFT_RESET
			; cmpd	#0
			beq	krnl_cold_reset
			jmp	[SOFT_RESET]
			
krnl_cold_reset		; cold reset
			ldd	#krnl_warm_reset
			std	SOFT_RESET
			; ...
			; clear out system memory
krnl_clear_mem 		ldx	#SYSTEM_MEMORY_DEVICE
			ldu	#VIDEO_START
			ldd	#$0000
1			stx	,u			; cycle first character to show progress
			std	,x++
			cmpx	#KERNEL_ROM_DEVICE
			bne	1b
			; ...
krnl_warm_reset		; common start up code
			;
			; for now simply set the SP
			lds	#SSTACK_TOP
			; ...



; ----------------------  MAIN LOOP  --------------------------------------
			; cycle video memory
			ldd	#$0100
1			ldx	#VIDEO_START		
			addd	#1
2			std	,x++
			addd	#1
			cmpx	GPU_VIDEO_MAX
			blt	2b
			bra	1b



