	SECTION code


; OS-9: Nothing generated. See os9-sbrk.c.
	IFNDEF OS9


_sbrk           EXPORT
program_break   IMPORT
_sbrkmax        IMPORT


* void *sbrk(size_t increment);
*
* Returns (byte *) -1 upon failure (not enough memory).
* Returns the old program break upon success
* (i.e., the address of the newly allocated buffer).
*
* Asking for an increment of zero always succeeds.
* This can be used to get the current program break.
*
* Uses the program_end label generated by the compiler at the very end
* of the generated program.
*
* Return value will be in D. Does not preserve X.
*
* The following system variables are initialized by INILIB.
*
_sbrk
	lbsr	_sbrkmax		D = max number of allocatable bytes
	cmpd	2,s			compare with 'increment'
	bhs	@sbrk_success		increment not too large: allocate
	ldd	#-1			fail
	rts
@sbrk_success
	ldd	2,s			get 'increment'
	addd	program_break,pcr	new, tentative program break
	ldx	program_break,pcr	preserve original program break
	std	program_break,pcr	advance program break
	tfr	x,d			return original program break in D
	rts


	ENDC


	ENDSECTION
