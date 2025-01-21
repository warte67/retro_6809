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

sys			macro
			swi2
			fcb	\1
			endm




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

krnl_color_attribute	fcb	#$2a	; current color attribute


			org	ROM_VECTS_DEVICE
krnl_hard_vectors			
			fdb	krnl_soft_exec_inf
			fdb	krnl_soft_swi3_inf
			fdb	krnl_swi2		; krnl_soft_swi2_inf
			fdb	krnl_soft_firq_inf
			fdb	krnl_soft_irq_inf
			fdb	krnl_soft_swi_inf
			fdb	krnl_soft_nmi_inf
			fdb	start
krnl_hard_vector_end

; ---------------------------------------------------------------------

			org 	KERNEL_START

start			bra	begin

krnl_vers_label		fcn	"Kernel Version: "
krnl_vers_number	fcn	"0.0.1"



; default fake infinite loops... just to verify vector jumps
krnl_soft_exec_inf	jmp	krnl_soft_exec_inf
krnl_soft_swi3_inf	jmp	krnl_soft_swi3_inf
krnl_soft_swi2_inf	jmp	krnl_soft_swi2_inf
krnl_soft_firq_inf	jmp	krnl_soft_firq_inf
krnl_soft_irq_inf	jmp	krnl_soft_irq_inf
krnl_soft_swi_inf	jmp	krnl_soft_swi_inf
krnl_soft_nmi_inf	jmp	krnl_soft_nmi_inf
krnl_soft_reset_inf	jmp	krnl_soft_reset_inf


; ---------------------------------------------------------------------

begin			; warm reset?
			ldd	SOFT_RESET
			cmpd	#0
			beq	krnl_cold_reset
			jmp	[SOFT_RESET]
			
krnl_cold_reset		; cold reset
			ldx	#krnl_hard_vectors
			ldu	#SOFT_VECTORS_DEVICE
1			ldd	,x++
			std	,u++
			cmpx	#krnl_hard_vector_end
			blt	1b
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

			lda	#$4B			; $4B = green on dk.green
			sta	krnl_color_attribute


			; make a system call
			sys	#$00

krnl_warm_reset_inf	jmp 	krnl_warm_reset_inf			



; ----------------------  MAIN STUFF  --------------------------------------


krnl_swi2_clear_screen
			lda	krnl_color_attribute
			ldb	#'A'

			;ldd 	#$23fe

			ldx	#VIDEO_START
1			std	,x++
			cmpx	GPU_VIDEO_MAX
			blt	1b			
			rts


			; cycle video memory
krnl_swi2_random_garbage			
			ldd	#$0100
1			ldx	#VIDEO_START		
			addd	#1
2			std	,x++
			addd	#1
			cmpx	GPU_VIDEO_MAX
			blt	2b
			bra	1b




*** krnl_swi2 *******************************************************************
** 
** System Call Handler: 
**	References the byte immediately following the SWI2 instruction and
** 	dispatches to the appropriate system call based on that value.
**
**
**********************************************************************************
krnl_swi2_call_vector	fdb	krnl_swi2_clear_screen		; $00 clear screen
			fdb	krnl_swi2_random_garbage	; $01 random garbage
krnl_swi2_call_end				


krnl_swi2		; increment the return address on the stack past the command byte
			ldu	$000a,s
			ldb	0,u
			leau	1,u
			stu	$000a,s
			ldx	#krnl_swi2_call_vector
			lslb
			leax	b,x
			cmpx	#krnl_swi2_call_end
			bge	krnl_swi2_done
			jsr	[,x]
krnl_swi2_done		rti
