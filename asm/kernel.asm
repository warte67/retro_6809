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

		org	$FFFE
		fdb	start


		org 	KERNEL_START

start		bra	begin

vers_label	fcn	"Kernel Version: "
vers_number	fcn	"0.0.1"

begin		nop
		; clear out system memory
		ldx	#SOFT_VECTORS_DEVICE
		ldd	#$0000
1		std	,x++
		cmpx	#VIDEO_TOP
		bne	1b
		;




		
		ldd	#$0100
lp2		ldx	#VIDEO_START		
		addd	#1
lp1		std	,x++
		addd	#1
		cmpx	GPU_VIDEO_MAX
		blt	lp1
		bra	lp2



