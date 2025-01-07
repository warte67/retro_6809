/*** C6809.hpp *****************************************
 *       _____  __   ___   ___   ___                         
 *      / ____|/ /  / _ \ / _ \ / _ \                        
 *     | |    / /_ | (_) | | | | (_) |       ___ _ __  _ __  
 *     | |   | '_ \ > _ <| | | |\__, |      / __| '_ \| '_ \ 
 *     | |___| (_) | (_) | |_| |  / /   _  | (__| |_) | |_) |
 *      \_____\___/ \___/ \___/  /_/   (_)  \___| .__/| .__/ 
 *                                              | |   | |    
 *                                              |_|   |_|    
 *
 * MC6809 Central Processing Unit Emulation object
 * 
 * Under GPL v3.0 License
 *
 **************************************/

#include <chrono>
#include <thread>
#include <map>

#include "types.hpp"
#include "Bus.hpp"
#include "C6809.hpp"
#include "Debug.hpp"

C6809::C6809(Bus* p_bus) : A(acc.byte.A = 0), B(acc.byte.B = 0), D(acc.D = 0)
{
	//_deviceName = "CPU";
	
	m_bus = p_bus;

	A = acc.byte.A;
	B = acc.byte.B;
	D = acc.D;

	NMI = true;
	IRQ = true;
	FIRQ = true;

	// C6809::s_bHalted = true;

	Init();
	reset();
}
C6809::~C6809()
{
	opMap.clear();
}

void C6809::ThreadProc()
{
    while (Bus::IsRunning())
    {
        // main CPU clock
        using clock = std::chrono::system_clock;
        using sec = std::chrono::duration<double, std::nano>;
        static auto before_CPU = clock::now();
        const sec duration = clock::now() - before_CPU;

        // 1000.f = 1mhz, 500.0f = 2mhz, etc...
        float cycle_time = 8000.0f;     // in nanohertz
        // int cpu_speed = Bus::Inst()._sys_state;
        int cpu_speed = s_sys_state;
        switch (cpu_speed)
        {
            case 0x00: cycle_time = 39900.0f;   break;      // 25 khz
            case 0x01: cycle_time = 19900.0f;   break;      // 50 khz
            case 0x02: cycle_time = 9900.0f;    break;      // 100 khz
            case 0x03: cycle_time = 4900.0f;    break;      // 200 khz
            case 0x04: cycle_time = 2900.0f;    break;      // 333 khz
            case 0x05: cycle_time = 2300.0f;    break;      // 416 khz
            case 0x06: cycle_time = 1900.0f;    break;      // 500 khz
            case 0x07: cycle_time = 1500.0f;    break;      // 625 khz
            case 0x08: cycle_time = 1200.0f;    break;      // 769 khz
            case 0x09: cycle_time = 1100.0f;    break;      // 833 khz
            case 0x0A: cycle_time = 900.0f;     break;      // 1.0 mhz
            case 0x0B: cycle_time = 600.0f;     break;      // 1.4 mhz
            case 0x0C: cycle_time = 400.0f;     break;      // 2.0 mhz
            case 0x0D: cycle_time = 200.0f;     break;      // 3.3 mhz
            case 0x0E: cycle_time = 100.0f;     break;      // 5 mhz
            case 0x0F: cycle_time = 0.0f;       break;      // Unmetered (10 mhz)
        }
        //if (cpu_speed)
        {
            if (duration.count() > cycle_time)
            {
                before_CPU = clock::now();
                //if (!C6809::s_bHalted)
                if (s_bCpuEnabled)
                {
                    Bus::GetC6809()->clock_input();
                    //Bus::Inst()._avg_cpu_cycle_time = duration.count();
                    Bus::SetAvgCpuCycleTime( duration.count() );
                }
            }
        }
    }
}



void C6809::clock_input()
{
	// printf("C6809::clock_input() -- PC:$%04X\n", PC);
	// Bus& bus = Bus::Inst();
	// Gfx* gfx = bus.m_gfx;
	// GfxDebug* debug = gfx->m_debug;
    Debug* debug = Bus::GetDebug();

	// if (s_bHalted)	return;
	// if (s_bHalted)	return;

	if (debug->SingleStep())
	{
		if (do_interrupts())
		{
			if (cycles == 0)
			{
				// read the opcode
				opcode = read(PC);
				PC++;
				if (opcode == 0x10 || opcode == 0x11) {
					opcode <<= 8;
					opcode |= read(PC);
					PC++;
				}
				// seed the cycles
				cycles = opMap[opcode].cycles;
				// run the instruction
				if (this->opMap[opcode].operation)
					(this->*opMap[opcode].operation)();
				else
				{
					std::string er = "Invalid Instruction at $";
					er += C6809::hex(PC, 4);
					Bus::Error(er.c_str());
				}
				 if (!waiting_cwai && !waiting_sync)
				 	debug->ContinueSingleStep();
				return;
			}
			cycles--;
		}
	}
}

void C6809::nmi() {
	NMI = false;
}
void C6809::irq() {
	IRQ = false;
}
void C6809::firq() {
	FIRQ = false;
}


void C6809::do_nmi() {
	if (!waiting_cwai) {
		CC.bit.E = 1;
		psh_post(0xff, S, U);
	}
	CC.bit.F = CC.bit.I = 1;
	PC = read_word(0xfffc);
}
void C6809::do_firq() {
	if (!waiting_cwai) {
		CC.bit.E = 0;
		psh_post(0x81, S, U);
	}
	CC.bit.F = CC.bit.I = 1;
	PC = read_word(0xfff6);
}
void C6809::do_irq() {
	if (!waiting_cwai) {
		CC.bit.E = 1;
		psh_post(0xff, S, U);
	}
	CC.bit.F = CC.bit.I = 1;
	PC = read_word(0xfff8);
}
bool C6809::do_interrupts() {
	// get interrupt pin states
	bool c_nmi = NMI;
	bool c_firq = FIRQ;
	bool c_irq = IRQ;

	// check for NMI falling edge
	bool nmi_triggered = !c_nmi && nmi_previous;
	nmi_previous = c_nmi;

	if (waiting_sync) {
		// if NMI or IRQ or FIRQ asserts (flags don't matter)
		if (nmi_triggered || !c_firq || !c_irq) {
			waiting_sync = false;
		}
		else {
			return false;
		}
	}

	// look for external interrupts
	if (nmi_triggered)
	{
		if (!nmi_disabled)
			do_nmi();
	}
	else if (!c_firq && !CC.bit.F) {
		do_firq();
	}
	else if (!c_irq && !CC.bit.I) {
		do_irq();
	}
	else if (waiting_cwai) {
		return false;
	}
	// testing: return to high cycle
	NMI = true;
	FIRQ = true;
	IRQ = true;
	// testing: end

	// if we got here, then CWAI is no longer in effect
	waiting_cwai = false;

	return true;
}

void C6809::reset() {
	PC = read_word(0xfffe);

	cycles = 0;
	DP = 0x00;
	D = 0x0000;
	X = 0x0000;
	Y = 0x0000;
	U = 0x0000;
	S = 0x0000;
	CC.all = 0x00;
	CC.bit.I = 1;	// IRQ not active
	CC.bit.F = 1;	// FIRQ not active
	waiting_sync = false;	// not not in SYNC
	waiting_cwai = false;	// not in CWAI
	nmi_previous = true;	// no NMI present
	nmi_disabled = true;	// wait to enable until after S is initialized

	NMI = true;
	IRQ = true;
	FIRQ = true;
}



////  ADDRESSING MODES ///////////////////////////////////////////

// inherent
Word C6809::inh() {
	return 0;	// inherent has no operand
}
// immediate 8-bit
Word C6809::immb() {
	Word addr = PC;
	PC++;
	return addr;
}
// immediate 16-bit
Word C6809::immw() {
	Word addr = PC;
	PC += 2;
	return addr;
}
// extended
Word C6809::ext() {
	return fetch_word();
}
// direct
Word C6809::dir() {
	Word addr = (DP << 8) | (fetch_byte());
	return addr;
}
// indexed
Word C6809::idx() {
	Byte val = fetch_byte();
	Word r, * pR = nullptr;
	pR = ptrReg[(val >> 5) & 0x03];	 // pR: pointer to the Register we're dealing with
	if (!(val & 0x80))
	{								// n4, R
		r = *pR + ext5(val);
		cycles++;
	}
	else
	{
		switch (val & 0x1f)
		{
		case 0x00:				// , R+
		{
			r = *pR;
			(*pR)++;
			cycles += 2;
			break;
		}
		case 0x01: case 0x11:	// , R++
		{
			r = *pR;
			*pR += 2;
			cycles += 3;
			break;
		}
		case 0x02:				// ,-R
		{
			(*pR)--;
			r = *pR;
			cycles += 2;
			break;
		}
		case 0x03: case 0x13:	// ,--R
		{
			*pR -= 2;
			r = *pR;
			cycles += 3;
			break;
		}
		case 0x04: case 0x14:	// ,R
		{
			r = *pR;
			break;
		}
		case 0x05: case 0x15:	// B,R
		{
			r = *pR + ext8(B);
			cycles++;
			break;
		}
		case 0x06: case 0x16:	// A,R
		{
			r = *pR + ext8(A);
			cycles++;
			break;
		}
		case 0x08: case 0x18:	// n7,R
		{
			r = *pR + ext8(fetch_byte());
			cycles++;
			break;
		}
		case 0x09: case 0x19:	// n15,R
		{
			r = *pR + (Sint16)fetch_word();
			cycles += 4;
			break;
		}
		case 0x0B: case 0x1B:	// D,R
		{
			r = *pR + (Sint16)D;
			cycles += 4;
			break;
		}
		case 0x0C: case 0x1C:	// n7,PC
		{
			r = PC + ext8(fetch_byte());
			cycles += 1;
			break;
		}
		case 0x0D: case 0x1D:	// n15,PC
		{
			r = PC + (Sint16)fetch_word();
			cycles += 5;
			break;
		}
		case 0x1f:				// [n]
		{
			r = fetch_word();
			cycles += 2;
			break;
		}
		default:
		{
			std::string er = "Invalid Postbyte at $";
			er += C6809::hex(PC, 4);
			Bus::Error(er.c_str());			
			r = 0;
		}
		}
		if (val & 0x10)				// [ INDIRECTION ]
		{
			r = read_word(r);
			cycles += 3;
		}
	}
	return r;
}
// relative 8-bit
Word C6809::relb() {
	// (this version stops the odd debug/release +1 error)
	Sint16 ofs = (Sint8)fetch_byte();
	Word addr = PC + ofs;
	return addr;
}
// relative 16-bit
Word C6809::relw() {
	// (this version stops the odd debug/release +1 error)
	Sint16 ofs = (Sint16)fetch_word();
	Word addr = PC + ofs;
	return addr;
}
// invalid addressing mode
Word C6809::nula()
{
	std::string er = "Invalid Addressing Mode at $";
	er += C6809::hex(PC, 4);
	Bus::Error(er.c_str());				
	return 0;
}


///// OPERATION CODES ///////////////////////////////////////////////////


void C6809::null() {
	std::string er = "Invalid Opcode at $";
	er += C6809::hex(PC, 4);
	Bus::Error(er.c_str());				
}

void C6809::abx() { X += B; }
void C6809::adca() { do_adc(A); }
void C6809::adcb() { do_adc(B); }
void C6809::adda() { do_add(A); }
void C6809::addb() { do_add(B); }
void C6809::addd() { do_add(D); }
//	Word m = fetch_word();	// post;
//	Word t = (D & 0x7fff) + (m & 0x7fff);
//	CC.bit.V = btst(t, 15);
//	DWord	dt = (DWord)D + m;
//	CC.bit.C = btst(dt, 16);
//	D = (Word)(dt & 0xffff);
//	CC.bit.V ^= CC.bit.C;
//	CC.bit.N = btst(D, 15);
//	CC.bit.Z = !D;	
//}
void C6809::anda() { do_and(A); }
void C6809::andb() { do_and(B); }
void C6809::andc() { CC.all &= fetch_byte(); }
void C6809::asla() { do_asl(A); }
void C6809::aslb() { do_asl(B); }
void C6809::asl() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_asl(m);
	write(addr, m);
}
void C6809::asra() { do_asr(A); }
void C6809::asrb() { do_asr(B); }
void C6809::asr() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_asr(m);
	write(addr, m);
}
void C6809::bita() { do_bit(A); }
void C6809::bitb() { do_bit(B); }
void C6809::clra() { do_clr(A); }
void C6809::clrb() { do_clr(B); }
void C6809::clr() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_clr(m);
	write(addr, m);
}
void C6809::cmpa() { do_cmp(A); }
void C6809::cmpb() { do_cmp(B); }
void C6809::cmpd() { do_cmp(D); }
void C6809::cmps() { do_cmp(S); }
void C6809::cmpu() { do_cmp(U); }
void C6809::cmpx() { do_cmp(X); }
void C6809::cmpy() { do_cmp(Y); }
void C6809::coma() { do_com(A); }
void C6809::comb() { do_com(B); }
void C6809::com() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_com(m);
	write(addr, m);
}
void C6809::cwai()
{
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte n = read(addr);
	CC.all &= n;
	CC.bit.E = 1;
	psh_post(0xff, S, U);
	waiting_cwai = true;
}
void C6809::daa()
{
	Byte c = 0;
	Byte lsn = (A & 0x0f);
	Byte msn = (A & 0xf0) >> 4;
	if (CC.bit.H || (lsn > 9)) {
		c |= 0x06;
	}
	if (CC.bit.C ||
		(msn > 9) ||
		((msn > 8) && (lsn > 9))) {
		c |= 0x60;
	}
	Word t = (Word)A + c;
	CC.bit.C |= btst(t, 8);
	A = (Byte)t;
	CC.bit.N = btst(A, 7);
	CC.bit.Z = (bool)!A;
}
void C6809::deca() { do_dec(A); }
void C6809::decb() { do_dec(B); }
void C6809::dec() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_dec(m);
	write(addr, m);
}
void C6809::eora() { do_eor(A); }
void C6809::eorb() { do_eor(B); }
void C6809::exg()
{
	Byte post = fetch_byte();
	int r1 = (post & 0xf0) >> 4;
	int r2 = (post & 0x0f);
	// 16-bit registers?
	if (r1 < 6 && r2 < 6)
	{
		Word tmp = 0;
		Word r3 = r1;
		// source (left register)
		switch (r1 & 0x1f)
		{
		case 0: r1 = D;		break;
		case 1: r1 = X;		break;
		case 2: r1 = Y;		break;
		case 3: r1 = U;		break;
		case 4: r1 = S;		break;
		case 5: r1 = PC;	break;
		}
		// destination (right register)
		switch (r2 & 0x1f)
		{
		case 0: tmp = D;	D = r1;		break;
		case 1: tmp = X;	X = r1;		break;
		case 2: tmp = Y;	Y = r1;		break;
		case 3: tmp = U;	U = r1;		break;
		case 4: tmp = S;	S = r1;		break;
		case 5: tmp = PC;	PC = r1;	break;
		}
		// swap
		switch (r3 & 0x1f)
		{
		case 0: D = tmp;	break;
		case 1: X = tmp;	break;
		case 2: Y = tmp;	break;
		case 3: U = tmp;	break;
		case 4: S = tmp;	break;
		case 5: PC = tmp;	break;
		}
	}
	// 8- bit register
	else if (r1 >= 8 && r1 < 12 && r2 >= 8 && r2 < 12)
	{
		// source (left register)
		Byte tmp = 0;
		Byte r3 = r1;
		switch (r1 & 0x0f)
		{
		case 8: r1 = A;			break;
		case 9: r1 = B;			break;
		case 10: r1 = CC.all;	break;
		case 11: r1 = DP;		break;
		default: r1 = 0;		break;
		}
		// destination (right register)
		switch (r2 & 0x0f)
		{
		case 8: tmp = A;		A = r1;			break;
		case 9: tmp = B;		B = r1;			break;
		case 10: tmp = CC.all;	CC.all = r1;	break;
		case 11: tmp = DP;		DP = r1;		break;
		}
		// swap
		switch (r3 & 0x0f)
		{
		case 8: A = tmp;		break;
		case 9: B = tmp;		break;
		case 10: CC.all = tmp;	break;
		case 11: DP = tmp;		break;
		}
	}
	else {
		// unknown register in post
		printf("EXG R:???, R???");
	}
}
void C6809::inca() { do_inc(A); }
void C6809::incb() { do_inc(B); }
void C6809::inc() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_inc(m);
	write(addr, m);
}
void C6809::jmp() { Word addr_abs = (this->*opMap[opcode].addrmode)(); PC = addr_abs; }
void C6809::jsr() { Word addr_abs = (this->*opMap[opcode].addrmode)(); do_psh(S, PC); PC = addr_abs; }
void C6809::lda() { do_ld(A); }
void C6809::ldb() { do_ld(B); }
void C6809::ldd() { do_ld(D); }
void C6809::lds() { do_ld(S);  nmi_disabled = false; }
void C6809::ldu() { do_ld(U); }
void C6809::ldx() { do_ld(X); }
void C6809::ldy() { do_ld(Y); }
void C6809::leas() {
	//S = fetch_indexed_address();
	S = (this->*opMap[opcode].addrmode)();
	CC.bit.Z = !S;
}
void C6809::leau() {
	//U = fetch_indexed_address();
	U = (this->*opMap[opcode].addrmode)();
	CC.bit.Z = !U;
}
void C6809::leax() {
	//X = fetch_indexed_address();
	X = (this->*opMap[opcode].addrmode)();
	CC.bit.Z = !X;
}
void C6809::leay() {
	//Y = fetch_indexed_address();
	Y = (this->*opMap[opcode].addrmode)();
	CC.bit.Z = !Y;
}
void C6809::lsra() { do_lsr(A); }
void C6809::lsrb() { do_lsr(B); }
void C6809::lsr()
{
	Word addr = (this->*opMap[opcode].addrmode)();	Byte m = read(addr);
	do_lsr(m);
	write(addr, m);
}
void C6809::mul() {
	D = A * B;
	CC.bit.C = btst(B, 7);
	CC.bit.Z = !D;
}
void C6809::nega() { do_neg(A); }
void C6809::negb() { do_neg(B); }
void C6809::neg() {
	Word addr = (this->*opMap[opcode].addrmode)();	//fetch_word();
	Byte m = read(addr);
	do_neg(m);
	write(addr, m);
}
void C6809::nop() { }
void C6809::ora() { do_or(A); }
void C6809::orb() { do_or(B); }
void C6809::orcc() { CC.all |= fetch_byte(); }
void C6809::pshs() {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte p = read(addr_abs);
	psh_post(p, S, U);
}
void C6809::pshu() {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte p = read(addr_abs);
	psh_post(p, U, S);
}
void C6809::puls() {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte p = read(addr_abs);
	pul_post(p, S, U);
}
void C6809::pulu() {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte p = read(addr_abs);
	pul_post(p, U, S);
}
void C6809::rola() { do_rol(A); }
void C6809::rolb() { do_rol(B); }
void C6809::rol() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_rol(m);
	write(addr, m);
}
void C6809::rora() { do_ror(A); }
void C6809::rorb() { do_ror(B); }
void C6809::ror() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_ror(m);
	write(addr, m);
}
void C6809::rti() {
	pul_post(0x01, S, U);
	if (CC.bit.E) {
		if (!CC.bit.F)
			FIRQ = true;
		if (!CC.bit.I)
			IRQ = true;
		pul_post(0xfe, S, U);
	}
	else {
		pul_post(0x80, S, U);
		NMI = true;
		FIRQ = true;
	}

	NMI = true;
}
void C6809::rts() { do_pul(S, PC); }
void C6809::sbca() { do_sbc(A); }
void C6809::sbcb() { do_sbc(B); }
void C6809::sex() {
	CC.bit.N = btst(B, 7);
	CC.bit.Z = (bool)!B;
	A = CC.bit.N ? 255 : 0;
}
void C6809::sta() { do_st(A); }
void C6809::stb() { do_st(B); }
void C6809::std() { do_st(D); }
void C6809::sts() { do_st(S); }
void C6809::stu() { do_st(U); }
void C6809::stx() { do_st(X); }
void C6809::sty() { do_st(Y); }
void C6809::suba() { do_sub(A); }
void C6809::subb() { do_sub(B); }
void C6809::subd() { do_sub(D); }
void C6809::swi() {
	CC.bit.E = 1;
	psh_post(0xff, S, U);
	CC.bit.F = CC.bit.I = 1;
	PC = read_word(0xfffa);
}
void C6809::swi2() {
	CC.bit.E = 1;
	psh_post(0xff, S, U);
	PC = read_word(0xfff4);
}
void C6809::swi3() {
	CC.bit.E = 1;
	psh_post(0xff, S, U);
	PC = read_word(0xfff2);
}
void C6809::sync() {
	waiting_sync = true;
}

void C6809::tfr()
{
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte post = read(addr_abs);
	int r1 = (post & 0xf0) >> 4;
	int r2 = (post & 0x0f);
	// 16-bit registers?
	if (r1 < 6 && r2 < 6)
	{
		// source (left register)
		switch (r1 & 0x1f)
		{
		case 0: r1 = D;		break;
		case 1: r1 = X;		break;
		case 2: r1 = Y;		break;
		case 3: r1 = U;		break;
		case 4: r1 = S;		break;
		case 5: r1 = PC;	break;
		default: r1 = 0;	break;
		}
		// destination (right register)
		switch (r2 & 0x1f)
		{
		case 0: D = r1;		break;
		case 1: X = r1;		break;
		case 2: Y = r1;		break;
		case 3: U = r1;		break;
		case 4: S = r1;		break;
		case 5: PC = r1;	break;
		}
	}
	// 8- bit register
	else if (r1 >= 8 && r1 < 12 && r2 >= 8 && r2 < 12)
	{
		// source (left register)
		switch (r1 & 0x0f)
		{
		case 8:  r1 = A;		break;
		case 9:  r1 = B;		break;
		case 10: r1 = CC.all;	break;
		case 11: r1 = DP;		break;
		default: r1 = 0;		break;
		}
		// destination (right register)
		switch (r2 & 0x1f)
		{
		case 8:  A = r1;		break;
		case 9:  B = r1;		break;
		case 10: CC.all = r1;	break;
		case 11: DP = r1;		break;
		}
	}
	else {
		// unknown register in post
		printf("TFR R:???, R???");
	}
}
void C6809::tsta() { do_tst(A); }
void C6809::tstb() { do_tst(B); }
void C6809::tst() {
	Word addr = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr);
	do_tst(m);
	write(addr, m);
}

// branch instructions
void C6809::bcc() { do_br(!CC.bit.C); }		// Branch if Carry Clear:
void C6809::lbcc() { do_br(!CC.bit.C); }		// 			(C != 0)
void C6809::bcs() { do_br(CC.bit.C); }		// Branch if Carry Set:
void C6809::lbcs() { do_br(CC.bit.C); }		// 			(C == 0)
void C6809::bne() { do_br(!CC.bit.Z); }		// Branch if Not Equal:
void C6809::lbne() { do_lbr(!CC.bit.Z); }		//			(Z != 0)
void C6809::beq() { do_br(CC.bit.Z); }		// Branch if Equal:
void C6809::lbeq() { do_lbr(CC.bit.Z); }		// 			(Z == 0)
void C6809::bvc() { do_br(!CC.bit.V); }		// Branch if N0 Overflow:
void C6809::lbvc() { do_lbr(!CC.bit.V); }		// 			(V != 0)
void C6809::bvs() { do_br(CC.bit.V); }		// Branch if Overflow:
void C6809::lbvs() { do_lbr(CC.bit.V); }		// 			(V == 0)
void C6809::bmi() { do_br(CC.bit.N); }		// Branch if Minus (negative):
void C6809::lbmi() { do_lbr(CC.bit.N); }		// 			(N != 0)
void C6809::bpl() { do_br(!CC.bit.N); }		// Branch if Plus (positive):
void C6809::lbpl() { do_lbr(!CC.bit.N); }		// 			(N == 0)
// signed conditional branches
void C6809::bgt() { do_br(!(CC.bit.Z | (CC.bit.N ^ CC.bit.V))); }		// Branch if Greater Than (signed)
void C6809::lbgt() { do_lbr(!(CC.bit.Z | (CC.bit.N ^ CC.bit.V))); }	// Branch if Greater Than (signed)
void C6809::ble() { do_br(CC.bit.Z | (CC.bit.N ^ CC.bit.V)); }		// Branch if Less or Equal (signed)
void C6809::lble() { do_lbr(CC.bit.Z | (CC.bit.N ^ CC.bit.V)); }		// Branch if Less or Equal (signed)
void C6809::bge() { do_br(!CC.bit.N ^ CC.bit.V); }		// Branch if Greater or Equal (signed)
void C6809::lbge() { do_lbr(!CC.bit.N ^ CC.bit.V); }		// Branch if Greater or Equal (signed)
void C6809::blt() { do_br(CC.bit.N ^ CC.bit.V); }			// Branch if Less than (signed)
void C6809::lblt() { do_lbr(CC.bit.N ^ CC.bit.V); }		// Branch if Less than (signed)
// unsigned conditional branches
void C6809::bhi() { do_br(!(CC.bit.C | CC.bit.Z)); }		// Branch if Higher (unsigned)
void C6809::lbhi() { do_lbr(!(CC.bit.C | CC.bit.Z)); }	// Branch if Higher (unsigned)
void C6809::bls() { do_br(CC.bit.C | CC.bit.Z); }			// Branch if Lower or Same (unsigned)
void C6809::lbls() { do_lbr(CC.bit.C | CC.bit.Z); }		// Branch if lower or Same (unsigned)
//void C6809::bhs() { do_br(!(CC.bit.Z | (CC.bit.N ^ CC.bit.V))); }		// Branch if Higher or Same (unsigned)
//void C6809::lbhs() { do_lbr(!(CC.bit.Z | (CC.bit.N ^ CC.bit.V))); }	// Branch if Higher or Same (unsigned)
//void C6809::blo() { do_br(CC.bit.N ^ CC.bit.V); }			// Branch if Lower (unsigned)
//void C6809::lblo() { do_lbr(CC.bit.N ^ CC.bit.V); }		// Branch if Lower (unsigned)
// simple branches
void C6809::bsr() { Word addr_abs = (this->*opMap[opcode].addrmode)(); do_psh(S, PC); PC = addr_abs; }		// Branch to Subroutine
void C6809::lbsr() { Word addr_abs = (this->*opMap[opcode].addrmode)(); do_psh(S, PC); PC = addr_abs; }		// Branch to Subroutine
void C6809::bra() { do_br(1); }							// Branch Always
void C6809::lbra() { do_lbr(1); }							// Branch Always
void C6809::brn() { do_br(0); }							// Branch Never
void C6809::lbrn() { do_lbr(0); }							// Branch Never


void C6809::pg2() { }
void C6809::pg3() { }

///// HELPER FUNCTIONS //////////////////////////////////////////////////

void C6809::psh_post(Byte post, Word& s, Word& u) {
	// as in pg 19 Motorola MC6809 Tech Sheet
	if (btst(post, 7)) do_psh(s, PC);
	if (btst(post, 6)) do_psh(s, u);
	if (btst(post, 5)) do_psh(s, Y);
	if (btst(post, 4)) do_psh(s, X);
	if (btst(post, 3)) do_psh(s, DP);
	if (btst(post, 2)) do_psh(s, B);
	if (btst(post, 1)) do_psh(s, A);
	if (btst(post, 0)) do_psh(s, CC.all);
}
void C6809::pul_post(Byte post, Word& s, Word& u) {
	// as in pg 19 Motorola MC6809 Tech Sheet
	if (btst(post, 0)) do_pul(s, CC.all);
	if (btst(post, 1)) do_pul(s, A);
	if (btst(post, 2)) do_pul(s, B);
	if (btst(post, 3)) do_pul(s, DP);
	if (btst(post, 4)) do_pul(s, X);
	if (btst(post, 5)) do_pul(s, Y);
	if (btst(post, 6)) do_pul(s, u);
	if (btst(post, 7)) do_pul(s, PC);
}
void C6809::do_psh(Word& sp, Byte val) {
	write(--sp, val);
}
void C6809::do_psh(Word& sp, Word val) {
	write(--sp, (Byte)val);
	write(--sp, (Byte)(val >> 8));
}
void C6809::do_pul(Word& sp, Byte& val) {
	val = read(sp++);
}
void C6809::do_pul(Word& sp, Word& val) {
	val = read(sp++) << 8;
	val |= read(sp++);
}

void C6809::do_adc(Byte& x) {
	Word data = (this->*opMap[opcode].addrmode)();
	Byte m = read(data);	// post;
	//Byte m = fetch_byte();	// post;
	Byte t = (x & 0x0f) + (m & 0x0f) + CC.bit.C;
	CC.bit.H = btst(t, 4);		// Half carry
	t = (x & 0x7f) + (m & 0x7f) + CC.bit.C;
	CC.bit.V = btst(t, 7);		// Bit 7 carry in
	Word wt = x + m + CC.bit.C;
	CC.bit.C = btst(wt, 8);		// Bit 7 carry out
	x = wt & 0xff;
	CC.bit.V ^= CC.bit.C;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_add(Byte& x) {
	Word data = (this->*opMap[opcode].addrmode)();
	Byte m = read(data);	// post;
	//Byte m = fetch_byte();	// post;
	Byte t = (x & 0x0f) + (m & 0x0f);
	CC.bit.H = btst(t, 4);		// Half carry
	t = (x & 0x7f) + (m & 0x7f);
	CC.bit.V = btst(t, 7);		// Bit 7 carry in
	Word wt = x + m;
	CC.bit.C = btst(wt, 8);		// Bit 7 carry out
	x = wt & 0xff;
	CC.bit.V ^= CC.bit.C;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_add(Word& x) {
	Word data = (this->*opMap[opcode].addrmode)();
	Word m = read_word(data);	// post;
	Word t = (x & 0x0f) + (m & 0x0f);
	CC.bit.H = btst(t, 4);		// Half carry
	t = (x & 0x7f) + (m & 0x7f);
	CC.bit.V = btst(t, 7);		// Bit 7 carry in
	Word wt = x + m;
	CC.bit.C = btst(wt, 8);		// Bit 7 carry out
	x = wt & 0xffff;
	CC.bit.V ^= CC.bit.C;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = !x;
}

void C6809::do_and(Byte& x) {
	Word data = (this->*opMap[opcode].addrmode)();
	x = x & read(data);
	//	x = x & fetch_byte();	// post;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
	CC.bit.V = 0;
}
void C6809::do_asl(Byte& x)
{
	CC.bit.C = btst(x, 7);
	CC.bit.V = btst(x, 7) ^ btst(x, 6);
	x <<= 1;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_asr(Byte& x) {
	CC.bit.C = btst(x, 0);
	x >>= 1;	/* Shift word right */
	if ((CC.bit.N = btst(x, 6)) != 0) {
		bset(x, 7);
	}
	CC.bit.Z = (bool)!x;
}
void C6809::do_bit(Byte& x)
{
	Word data = (this->*opMap[opcode].addrmode)();
	Byte t = x & read(data);
	//Byte t = x & fetch_byte();	// post;
	CC.bit.N = btst(t, 7);
	CC.bit.V = 0;
	CC.bit.Z = (bool)!t;
}
void C6809::do_clr(Byte& x)
{
	CC.all &= 0xf0;
	CC.all |= 0x04;
	x = 0;
}
void C6809::do_cmp(Byte x) {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr_abs);
	int	t = x - m;
	CC.bit.V = btst((Byte)(x ^ m ^ t ^ (t >> 1)), 7);
	CC.bit.C = btst((Word)t, 8);
	CC.bit.N = btst((Byte)t, 7);
	CC.bit.Z = !(t & 0xff);
}
void C6809::do_cmp(Word x) {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Word m = read_word(addr_abs);
	long t = x - m;
	CC.bit.V = btst((DWord)(x ^ m ^ t ^ (t >> 1)), 15);
	CC.bit.C = btst((DWord)t, 16);
	CC.bit.N = btst((DWord)t, 15);
	CC.bit.Z = !(t & 0xffff);
}
void C6809::do_com(Byte& x) {
	x = ~x;
	CC.bit.C = 1;
	CC.bit.V = 0;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_dec(Byte& x) {
	CC.bit.V = (x == 0x80);
	x = x - 1;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_eor(Byte& x) {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	x = x ^ read(addr_abs);
	CC.bit.V = 0;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_inc(Byte& x)
{
	CC.bit.V = (x == 0x7f);
	x = x + 1;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_ld(Byte& x)
{
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	x = read(addr_abs);
	CC.bit.N = btst(x, 7);
	CC.bit.V = 0;
	CC.bit.Z = (bool)!x;
}
void C6809::do_ld(Word& x)
{
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	x = read_word(addr_abs);
	CC.bit.N = btst(x, 15);
	CC.bit.V = 0;
	CC.bit.Z = !x;
}
void C6809::do_lsr(Byte& x)
{
	CC.bit.C = btst(x, 0);
	x >>= 1;
	CC.bit.N = 0;
	CC.bit.Z = (bool)!x;
}
void C6809::do_neg(Byte& x)
{
	int	t = 0 - x;
	CC.bit.V = btst((Byte)(x ^ t ^ (t >> 1)), 7);
	CC.bit.C = btst((Word)t, 8);
	CC.bit.N = btst((Byte)t, 7);
	x = t & 0xff;
	CC.bit.Z = (bool)!x;
}
void C6809::do_or(Byte& x)
{
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	x = x | read(addr_abs);
	CC.bit.V = 0;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_rol(Byte& x)
{
	bool oc = CC.bit.C;
	CC.bit.V = btst(x, 7) ^ btst(x, 6);
	CC.bit.C = btst(x, 7);
	x = x << 1;
	if (oc) bset(x, 0);
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_ror(Byte& x)
{
	bool oc = CC.bit.C;
	CC.bit.C = btst(x, 0);
	x = x >> 1;
	if (oc) bset(x, 7);
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
	++cycles;
}
void C6809::do_sbc(Byte& x) {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr_abs);
	int t = x - m - CC.bit.C;
	CC.bit.V = btst((Byte)(x ^ m ^ t ^ (t >> 1)), 7);
	CC.bit.C = btst((Word)t, 8);
	CC.bit.N = btst((Byte)t, 7);
	x = t & 0xff;
	CC.bit.Z = (bool)!x;
}
void C6809::do_st(Byte x)
{
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Word addr = addr_abs;
	write(addr, x);
	CC.bit.V = 0;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_st(Word x)
{
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Word addr = addr_abs;
	write_word(addr, x);
	CC.bit.V = 0;
	CC.bit.N = btst(x, 15);
	CC.bit.Z = !x;
}
void C6809::do_sub(Byte& x) {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	Byte m = read(addr_abs);
	int t = x - m;
	CC.bit.V = btst((Byte)(x ^ m ^ t ^ (t >> 1)), 7);
	CC.bit.C = btst((Word)t, 8);
	CC.bit.N = btst((Byte)t, 7);
	x = t & 0xff;
	CC.bit.Z = (bool)!x;
}
void C6809::do_sub(Word& x) {
	Word addr_abs = (this->*opMap[opcode].addrmode)();
	//Byte m = read_word(addr_abs);
	Word m = read_word(addr_abs);
	int t = x - m;
	CC.bit.V = btst((Byte)(x ^ m ^ t ^ (t >> 1)), 7);
	CC.bit.C = btst((Word)t, 8);
	CC.bit.N = btst((Byte)t, 7);
	x = t & 0xffff;
	CC.bit.Z = !x;
}
void C6809::do_tst(Byte& x) {
	CC.bit.V = 0;
	CC.bit.N = btst(x, 7);
	CC.bit.Z = (bool)!x;
}
void C6809::do_br(bool test) {
	if (test)
		PC = (this->*opMap[opcode].addrmode)();	// +1;
	else
		PC++;
}
void C6809::do_lbr(bool test) {
	if (test)
	{
		Sint16 ofs = fetch_word();
		PC += ofs;
	}
	else
	{
		PC += 2;
		cycles++;
	}
}

//Word addr_abs = (this->*opMap[opcode].addrmode)(); do_psh(S, PC); PC = addr_abs;

///// INITIALIZATION ////////////////////////////////////////////////////

void C6809::Init()
{
	using C = C6809;
	// pre-initialize 3 banks of 256 instructions
	//INSTRUCTION nulInst = { "?? ", &C::null, &C::nula, 0, 0 };
	//for (int adr = 0; adr < 0x300; adr++)
	//	opMap.push_back(nulInst);

	opMap[0x0000] = { "NEG",	&C::neg,	&C::dir	,	6, 2 };
	opMap[0x0001] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x0002] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x0003] = { "COM",	&C::com,	&C::dir	,	6, 2 };
	opMap[0x0004] = { "LSR",	&C::lsr,	&C::dir	,	6, 2 };
	opMap[0x0005] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x0006] = { "ROR",	&C::ror,	&C::dir	,	6, 2 };
	opMap[0x0007] = { "ASR",	&C::asr,	&C::dir	,	6, 2 };
	opMap[0x0008] = { "ASL",	&C::asl,	&C::dir	,	6, 2 };
	opMap[0x0009] = { "ROL",	&C::rol,	&C::dir	,	6, 2 };
	opMap[0x000a] = { "DEC",	&C::dec,	&C::dir	,	6, 2 };
	opMap[0x000b] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x000c] = { "INC",	&C::inc,	&C::dir	,	6, 2 };
	opMap[0x000d] = { "TST",	&C::tst,	&C::dir	,	6, 2 };
	opMap[0x000e] = { "JMP",	&C::jmp,	&C::dir	,	3, 2 };
	opMap[0x000f] = { "CLR",	&C::clr,	&C::dir	,	6, 2 };

	opMap[0x0010] = { "PG2",	&C::pg2,	&C::nula,	0, 0 };	// page 2
	opMap[0x0011] = { "PG3",	&C::pg3,	&C::nula,	0, 0 };	// page 3
	opMap[0x0012] = { "NOP",	&C::nop,	&C::inh	,	2, 1 };
	opMap[0x0013] = { "SYNC",	&C::sync,	&C::inh	,	4, 1 };
	opMap[0x0014] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0015] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0016] = { "LBRA",	&C::lbra,	&C::relw,	5, 3 };
	opMap[0x0017] = { "LBSR",	&C::lbsr,	&C::relw,	9, 3 };
	opMap[0x0018] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0019] = { "DAA",	&C::daa,	&C::inh	,	2, 1 };
	opMap[0x001a] = { "ORCC",	&C::orcc,	&C::immb,	2, 1 };
	opMap[0x001b] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x001c] = { "ANDCC",	&C::andc,	&C::immb,	3, 2 };
	opMap[0x001d] = { "SEX",	&C::sex,	&C::inh,	2, 1 };
	opMap[0x001e] = { "EXG",	&C::exg,	&C::immb,	8, 2 };
	opMap[0x001f] = { "TFR",	&C::tfr,	&C::immb,	6, 2 };

	opMap[0x0020] = { "BRA",	&C::bra,	&C::relb,	3, 2 };
	opMap[0x0021] = { "BRN",	&C::brn,	&C::relb,	3, 2 };
	opMap[0x0022] = { "BHI",	&C::bhi,	&C::relb,	3, 2 };
	opMap[0x0023] = { "BLS",	&C::bls,	&C::relb,	3, 2 };
	opMap[0x0024] = { "BCC",	&C::bcc,	&C::relb,	3, 2 };
	opMap[0x0025] = { "BCS",	&C::bcs,	&C::relb,	3, 2 };
	opMap[0x0026] = { "BNE",	&C::bne,	&C::relb,	3, 2 };
	opMap[0x0027] = { "BEQ",	&C::beq,	&C::relb,	3, 2 };
	opMap[0x0028] = { "BVC",	&C::bvc,	&C::relb,	3, 2 };
	opMap[0x0029] = { "BVS",	&C::bvs,	&C::relb,	3, 2 };
	opMap[0x002a] = { "BPL",	&C::bpl,	&C::relb,	3, 2 };
	opMap[0x002b] = { "BMI",	&C::bmi,	&C::relb,	3, 2 };
	opMap[0x002c] = { "BGE",	&C::bge,	&C::relb,	3, 2 };
	opMap[0x002d] = { "BLT",	&C::blt,	&C::relb,	3, 2 };
	opMap[0x002e] = { "BGT",	&C::bgt,	&C::relb,	3, 2 };
	opMap[0x002f] = { "BLE",	&C::ble,	&C::relb,	3, 2 };

	opMap[0x0030] = { "LEAX",	&C::leax,	&C::idx,	4, 2 };
	opMap[0x0031] = { "LEAY",	&C::leay,	&C::idx,	4, 2 };
	opMap[0x0032] = { "LEAS",	&C::leas,	&C::idx,	4, 2 };
	opMap[0x0033] = { "LEAU",	&C::leau,	&C::idx,	4, 2 };
	opMap[0x0034] = { "PSHS",	&C::pshs,	&C::immb,	5, 2 };
	opMap[0x0035] = { "PULS",	&C::puls,	&C::immb,	5, 2 };
	opMap[0x0036] = { "PSHU",	&C::pshu,	&C::immb,	5, 2 };
	opMap[0x0037] = { "PULU",	&C::pulu,	&C::immb,	5, 2 };
	opMap[0x0038] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0039] = { "RTS",	&C::rts,	&C::inh,	5, 1 };
	opMap[0x003a] = { "ABX",	&C::abx,	&C::inh,	3, 1 };
	opMap[0x003b] = { "RTI",	&C::rti,	&C::inh,	6, 1 };
	opMap[0x003c] = { "CWAI",	&C::cwai,	&C::immb,	20, 2 };
	opMap[0x003d] = { "MUL",	&C::mul,	&C::inh,	11, 2 };
	opMap[0x003e] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x003f] = { "SWI",	&C::swi,	&C::inh,	19, 1 };

	opMap[0x0040] = { "NEGA",	&C::nega,	&C::inh,	2, 1 };
	opMap[0x0041] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0042] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0043] = { "COMA",	&C::coma,	&C::inh,	2, 1 };
	opMap[0x0044] = { "LSRA",	&C::lsra,	&C::inh,	2, 1 };
	opMap[0x0045] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0046] = { "RORA",	&C::rora,	&C::inh,	2, 1 };
	opMap[0x0047] = { "ASRA",	&C::asra,	&C::inh,	2, 1 };
	opMap[0x0048] = { "ASLA",	&C::asla,	&C::inh,	2, 1 };
	opMap[0x0049] = { "ROLA",	&C::rola,	&C::inh,	2, 1 };
	opMap[0x004a] = { "DECA",	&C::deca,	&C::inh,	2, 1 };
	opMap[0x004b] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x004c] = { "INCA",	&C::inca,	&C::inh,	2, 1 };
	opMap[0x004d] = { "TSTA",	&C::tsta,	&C::inh,	2, 1 };
	opMap[0x004e] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x004f] = { "CLRA",	&C::clra,	&C::inh,	2, 1 };

	opMap[0x0050] = { "NEGB",	&C::negb,	&C::inh,	2, 1 };
	opMap[0x0051] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0052] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0053] = { "COMB",	&C::comb,	&C::inh,	2, 1 };
	opMap[0x0054] = { "LSRB",	&C::lsrb,	&C::inh,	2, 1 };
	opMap[0x0055] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0056] = { "RORB",	&C::rorb,	&C::inh,	2, 1 };
	opMap[0x0057] = { "ASRB",	&C::asrb,	&C::inh,	2, 1 };
	opMap[0x0058] = { "ASLB",	&C::aslb,	&C::inh,	2, 1 };
	opMap[0x0059] = { "ROLB",	&C::rolb,	&C::inh,	2, 1 };
	opMap[0x005a] = { "DECB",	&C::decb,	&C::inh,	2, 1 };
	opMap[0x005b] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x005c] = { "INCB",	&C::incb,	&C::inh,	2, 1 };
	opMap[0x005d] = { "TSTB",	&C::tstb,	&C::inh,	2, 1 };
	opMap[0x005e] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x005f] = { "CLRB",	&C::clrb,	&C::inh,	2, 1 };

	opMap[0x0060] = { "NEG",	&C::neg,	&C::idx	,	6, 2 };
	opMap[0x0061] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x0062] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x0063] = { "COM",	&C::com,	&C::idx	,	6, 2 };
	opMap[0x0064] = { "LSR",	&C::lsr,	&C::idx	,	6, 2 };
	opMap[0x0065] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x0066] = { "ROR",	&C::ror,	&C::idx	,	6, 2 };
	opMap[0x0067] = { "ASR",	&C::asr,	&C::idx	,	6, 2 };
	opMap[0x0068] = { "ASL",	&C::asl,	&C::idx	,	6, 2 };
	opMap[0x0069] = { "ROL",	&C::rol,	&C::idx	,	6, 2 };
	opMap[0x006a] = { "DEC",	&C::dec,	&C::idx	,	6, 2 };
	opMap[0x006b] = { "?? ",	&C::null,	&C::nula,	6, 2 };
	opMap[0x006c] = { "INC",	&C::inc,	&C::idx	,	6, 2 };
	opMap[0x006d] = { "TST",	&C::tst,	&C::idx	,	6, 2 };
	opMap[0x006e] = { "JMP",	&C::jmp,	&C::idx	,	3, 2 };
	opMap[0x006f] = { "CLR",	&C::clr,	&C::idx	,	6, 2 };

	opMap[0x0070] = { "NEG",	&C::neg,	&C::ext	,	7, 3 };
	opMap[0x0071] = { "?? ",	&C::null,	&C::nula,	7, 3 };
	opMap[0x0072] = { "?? ",	&C::null,	&C::nula,	7, 3 };
	opMap[0x0073] = { "COM",	&C::com,	&C::ext	,	7, 3 };
	opMap[0x0074] = { "LSR",	&C::lsr,	&C::ext	,	7, 3 };
	opMap[0x0075] = { "?? ",	&C::null,	&C::nula,	7, 3 };
	opMap[0x0076] = { "ROR",	&C::ror,	&C::ext	,	7, 3 };
	opMap[0x0077] = { "ASR",	&C::asr,	&C::ext	,	7, 3 };
	opMap[0x0078] = { "ASL",	&C::asl,	&C::ext	,	7, 3 };
	opMap[0x0079] = { "ROL",	&C::rol,	&C::ext	,	7, 3 };
	opMap[0x007a] = { "DEC",	&C::dec,	&C::ext	,	7, 3 };
	opMap[0x007b] = { "?? ",	&C::null,	&C::nula,	7, 3 };
	opMap[0x007c] = { "INC",	&C::inc,	&C::ext	,	7, 3 };
	opMap[0x007d] = { "TST",	&C::tst,	&C::ext	,	7, 3 };
	opMap[0x007e] = { "JMP",	&C::jmp,	&C::ext	,	4, 3 };
	opMap[0x007f] = { "CLR",	&C::clr,	&C::ext	,	7, 3 };

	opMap[0x0080] = { "SUBA",	&C::suba,	&C::immb,	2, 2 };
	opMap[0x0081] = { "CMPA",	&C::cmpa,	&C::immb,	2, 2 };
	opMap[0x0082] = { "SBCA",	&C::sbca,	&C::immb,	2, 2 };
	opMap[0x0083] = { "SUBD",	&C::subd,	&C::immw,	4, 3 };
	opMap[0x0084] = { "ANDA",	&C::anda,	&C::immb,	2, 2 };
	opMap[0x0085] = { "BITA",	&C::bita,	&C::immb,	2, 2 };
	opMap[0x0086] = { "LDA",	&C::lda,	&C::immb,	2, 2 };
	opMap[0x0087] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x0088] = { "EORA",	&C::eora,	&C::immb,	2, 2 };
	opMap[0x0089] = { "ADCA",	&C::adca,	&C::immb,	2, 2 };
	opMap[0x008a] = { "ORA",	&C::ora,	&C::immb,	2, 2 };
	opMap[0x008b] = { "ADDA",	&C::adda,	&C::immb,	2, 2 };
	opMap[0x008c] = { "CMPX",	&C::cmpx,	&C::immw,	4, 3 };
	opMap[0x008d] = { "BSR",	&C::bsr,	&C::relb,	7, 2 };
	opMap[0x008e] = { "LDX",	&C::ldx,	&C::immw,	3, 3 };
	opMap[0x008f] = { "?? ",	&C::null,	&C::nula,	2, 1 };

	opMap[0x0090] = { "SUBA",	&C::suba,	&C::dir ,	4, 2 };
	opMap[0x0091] = { "CMPA",	&C::cmpa,	&C::dir ,	4, 2 };
	opMap[0x0092] = { "SBCA",	&C::sbca,	&C::dir ,	4, 2 };
	opMap[0x0093] = { "SUBD",	&C::subd,	&C::dir ,	6, 2 };
	opMap[0x0094] = { "ANDA",	&C::anda,	&C::dir ,	4, 2 };
	opMap[0x0095] = { "BITA",	&C::bita,	&C::dir ,	4, 2 };
	opMap[0x0096] = { "LDA",	&C::lda,	&C::dir ,	4, 2 };
	opMap[0x0097] = { "STA",	&C::sta,	&C::dir ,	4, 2 };
	opMap[0x0098] = { "EORA",	&C::eora,	&C::dir ,	4, 2 };
	opMap[0x0099] = { "ADCA",	&C::adca,	&C::dir ,	4, 2 };
	opMap[0x009a] = { "ORA",	&C::ora,	&C::dir ,	4, 2 };
	opMap[0x009b] = { "ADDA",	&C::adda,	&C::dir ,	4, 2 };
	opMap[0x009c] = { "CMPX",	&C::cmpx,	&C::dir ,	6, 2 };
	opMap[0x009d] = { "JSR",	&C::jsr,	&C::dir ,	7, 2 };
	opMap[0x009e] = { "LDX",	&C::ldx,	&C::dir ,	5, 2 };
	opMap[0x009f] = { "STX",	&C::stx,	&C::dir ,	5, 2 };

	opMap[0x00a0] = { "SUBA",	&C::suba,	&C::idx ,	4, 2 };
	opMap[0x00a1] = { "CMPA",	&C::cmpa,	&C::idx ,	4, 2 };
	opMap[0x00a2] = { "SBCA",	&C::sbca,	&C::idx ,	4, 2 };
	opMap[0x00a3] = { "SUBD",	&C::subd,	&C::idx ,	6, 2 };
	opMap[0x00a4] = { "ANDA",	&C::anda,	&C::idx ,	4, 2 };
	opMap[0x00a5] = { "BITA",	&C::bita,	&C::idx ,	4, 2 };
	opMap[0x00a6] = { "LDA",	&C::lda,	&C::idx ,	4, 2 };
	opMap[0x00a7] = { "STA",	&C::sta,	&C::idx ,	4, 2 };
	opMap[0x00a8] = { "EORA",	&C::eora,	&C::idx ,	4, 2 };
	opMap[0x00a9] = { "ADCA",	&C::adca,	&C::idx ,	4, 2 };
	opMap[0x00aa] = { "ORA",	&C::ora,	&C::idx ,	4, 2 };
	opMap[0x00ab] = { "ADDA",	&C::adda,	&C::idx ,	4, 2 };
	opMap[0x00ac] = { "CMPX",	&C::cmpx,	&C::idx ,	6, 2 };
	opMap[0x00ad] = { "JSR",	&C::jsr,	&C::idx ,	7, 2 };
	opMap[0x00ae] = { "LDX",	&C::ldx,	&C::idx ,	5, 2 };
	opMap[0x00af] = { "STX",	&C::stx,	&C::idx ,	5, 2 };

	opMap[0x00b0] = { "SUBA",	&C::suba,	&C::ext ,	5, 3 };
	opMap[0x00b1] = { "CMPA",	&C::cmpa,	&C::ext ,	5, 3 };
	opMap[0x00b2] = { "SBCA",	&C::sbca,	&C::ext ,	5, 3 };
	opMap[0x00b3] = { "SUBD",	&C::subd,	&C::ext ,	7, 3 };
	opMap[0x00b4] = { "ANDA",	&C::anda,	&C::ext ,	5, 3 };
	opMap[0x00b5] = { "BITA",	&C::bita,	&C::ext ,	5, 3 };
	opMap[0x00b6] = { "LDA",	&C::lda,	&C::ext ,	5, 3 };
	opMap[0x00b7] = { "STA",	&C::sta,	&C::ext ,	5, 3 };
	opMap[0x00b8] = { "EORA",	&C::eora,	&C::ext ,	5, 3 };
	opMap[0x00b9] = { "ADCA",	&C::adca,	&C::ext ,	5, 3 };
	opMap[0x00ba] = { "ORA",	&C::ora,	&C::ext ,	5, 3 };
	opMap[0x00bb] = { "ADDA",	&C::adda,	&C::ext ,	5, 3 };
	opMap[0x00bc] = { "CMPX",	&C::cmpx,	&C::ext ,	7, 3 };
	opMap[0x00bd] = { "JSR",	&C::jsr,	&C::ext ,	8, 3 };
	opMap[0x00be] = { "LDX",	&C::ldx,	&C::ext ,	6, 3 };
	opMap[0x00bf] = { "STX",	&C::stx,	&C::ext ,	6, 3 };

	opMap[0x00c0] = { "SUBB",	&C::subb,	&C::immb,	2, 2 };
	opMap[0x00c1] = { "CMPB",	&C::cmpb,	&C::immb,	2, 2 };
	opMap[0x00c2] = { "SBCB",	&C::sbcb,	&C::immb,	2, 2 };
	opMap[0x00c3] = { "ADDD",	&C::addd,	&C::immw,	4, 3 };
	opMap[0x00c4] = { "ANDB",	&C::andb,	&C::immb,	2, 2 };
	opMap[0x00c5] = { "BITB",	&C::bitb,	&C::immb,	2, 2 };
	opMap[0x00c6] = { "LDB",	&C::ldb,	&C::immb,	2, 2 };
	opMap[0x00c7] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	opMap[0x00c8] = { "EORB",	&C::eorb,	&C::immb,	2, 2 };
	opMap[0x00c9] = { "ADCB",	&C::adcb,	&C::immb,	2, 2 };
	opMap[0x00ca] = { "ORB",	&C::orb,	&C::immb,	2, 2 };
	opMap[0x00cb] = { "ADDB",	&C::addb,	&C::immb,	2, 2 };
	opMap[0x00cc] = { "LDD",	&C::ldd,	&C::immw,	3, 3 };
	opMap[0x00cd] = { "?? ",	&C::null,	&C::relb,	2, 1 };
	opMap[0x00ce] = { "LDU",	&C::ldu,	&C::immw,	3, 3 };
	opMap[0x00cf] = { "?? ",	&C::null,	&C::nula,	2, 1 };

	opMap[0x00d0] = { "SUBB",	&C::subb,	&C::dir ,	4, 2 };
	opMap[0x00d1] = { "CMPB",	&C::cmpb,	&C::dir ,	4, 2 };
	opMap[0x00d2] = { "SBCB",	&C::sbcb,	&C::dir ,	4, 2 };
	opMap[0x00d3] = { "ADDD",	&C::addd,	&C::dir ,	6, 2 };
	opMap[0x00d4] = { "ANDB",	&C::andb,	&C::dir ,	4, 2 };
	opMap[0x00d5] = { "BITB",	&C::bitb,	&C::dir ,	4, 2 };
	opMap[0x00d6] = { "LDB",	&C::ldb,	&C::dir ,	4, 2 };
	opMap[0x00d7] = { "STB",	&C::stb,	&C::dir ,	4, 2 };
	opMap[0x00d8] = { "EORB",	&C::eorb,	&C::dir ,	4, 2 };
	opMap[0x00d9] = { "ADCB",	&C::adcb,	&C::dir ,	4, 2 };
	opMap[0x00da] = { "ORB",	&C::orb,	&C::dir ,	4, 2 };
	opMap[0x00db] = { "ADDB",	&C::addb,	&C::dir ,	4, 2 };
	opMap[0x00dc] = { "LDD",	&C::ldd,	&C::dir ,	5, 2 };
	opMap[0x00dd] = { "STD",	&C::std,	&C::dir ,	5, 2 };
	opMap[0x00de] = { "LDU",	&C::ldu,	&C::dir ,	5, 2 };
	opMap[0x00df] = { "STU",	&C::stu,	&C::dir ,	5, 2 };

	opMap[0x00e0] = { "SUBB",	&C::subb,	&C::idx ,	4, 2 };
	opMap[0x00e1] = { "CMPB",	&C::cmpb,	&C::idx ,	4, 2 };
	opMap[0x00e2] = { "SBCB",	&C::sbcb,	&C::idx ,	4, 2 };
	opMap[0x00e3] = { "ADDD",	&C::addd,	&C::idx ,	6, 2 };
	opMap[0x00e4] = { "ANDB",	&C::andb,	&C::idx ,	4, 2 };
	opMap[0x00e5] = { "BITB",	&C::bitb,	&C::idx ,	4, 2 };
	opMap[0x00e6] = { "LDB",	&C::ldb,	&C::idx ,	4, 2 };
	opMap[0x00e7] = { "STB",	&C::stb,	&C::idx ,	4, 2 };
	opMap[0x00e8] = { "EORB",	&C::eorb,	&C::idx ,	4, 2 };
	opMap[0x00e9] = { "ADCB",	&C::adcb,	&C::idx ,	4, 2 };
	opMap[0x00ea] = { "ORB",	&C::orb,	&C::idx ,	4, 2 };
	opMap[0x00eb] = { "ADDB",	&C::addb,	&C::idx ,	4, 2 };
	opMap[0x00ec] = { "LDD",	&C::ldd,	&C::idx ,	5, 2 };
	opMap[0x00ed] = { "STD",	&C::std,	&C::idx ,	5, 2 };
	opMap[0x00ee] = { "LDU",	&C::ldu,	&C::idx ,	5, 2 };
	opMap[0x00ef] = { "STU",	&C::stu,	&C::idx ,	5, 2 };

	opMap[0x00f0] = { "SUBB",	&C::subb,	&C::ext ,	5, 3 };
	opMap[0x00f1] = { "CMPB",	&C::cmpb,	&C::ext ,	5, 3 };
	opMap[0x00f2] = { "SBCB",	&C::sbcb,	&C::ext ,	5, 3 };
	opMap[0x00f3] = { "ADDD",	&C::addd,	&C::ext ,	7, 3 };
	opMap[0x00f4] = { "ANDB",	&C::andb,	&C::ext ,	5, 3 };
	opMap[0x00f5] = { "BITB",	&C::bitb,	&C::ext ,	5, 3 };
	opMap[0x00f6] = { "LDB",	&C::ldb,	&C::ext ,	5, 3 };
	opMap[0x00f7] = { "STB",	&C::stb,	&C::ext ,	5, 3 };
	opMap[0x00f8] = { "EORB",	&C::eorb,	&C::ext ,	5, 3 };
	opMap[0x00f9] = { "ADCB",	&C::adcb,	&C::ext ,	5, 3 };
	opMap[0x00fa] = { "ORB",	&C::orb,	&C::ext ,	5, 3 };
	opMap[0x00fb] = { "ADDB",	&C::addb,	&C::ext ,	5, 3 };
	opMap[0x00fc] = { "LDD",	&C::ldd,	&C::ext ,	6, 3 };
	opMap[0x00fd] = { "STD",	&C::std,	&C::ext ,	6, 3 };
	opMap[0x00fe] = { "LDU",	&C::ldu,	&C::ext ,	6, 3 };
	opMap[0x00ff] = { "STU",	&C::stu,	&C::ext ,	6, 3 };

	//// page 2
	//// fill in invalid instructions (there's a lot of them)
	//// NOTE: if/when I rewrite for the Hitachi 6309 extended instruction set,
	////		these will be important.
	//for (int a = 0x0100; a < 0x1200; a++) {
	//	opMap[a] = { "?? ",	&C::null,	&C::nula,	2, 1 };
	//}

	opMap[0x1021] = { "LBRN",	&C::lbrn,	&C::relw ,	5, 4 };
	opMap[0x1022] = { "LBHI",	&C::lbhi,	&C::relw ,	5, 4 };
	opMap[0x1023] = { "LBLS",	&C::lbls,	&C::relw ,	5, 4 };
	opMap[0x1024] = { "LBCC",	&C::lbcc,	&C::relw ,	5, 4 };
	opMap[0x1025] = { "LBCS",	&C::lbcs,	&C::relw ,	5, 4 };
	opMap[0x1026] = { "LBNE",	&C::lbne,	&C::relw ,	5, 4 };
	opMap[0x1027] = { "LBEQ",	&C::lbeq,	&C::relw ,	5, 4 };
	opMap[0x1028] = { "LBVC",	&C::lbvc,	&C::relw ,	5, 4 };
	opMap[0x1029] = { "LBVS",	&C::lbvs,	&C::relw ,	5, 4 };
	opMap[0x102a] = { "LBPL",	&C::lbpl,	&C::relw ,	5, 4 };
	opMap[0x102b] = { "LBMI",	&C::lbmi,	&C::relw ,	5, 4 };
	opMap[0x102c] = { "LBGE",	&C::lbge,	&C::relw ,	5, 4 };
	opMap[0x102d] = { "LBLT",	&C::lblt,	&C::relw ,	5, 4 };
	opMap[0x102e] = { "LBGT",	&C::lbgt,	&C::relw ,	5, 4 };
	opMap[0x102f] = { "LBLE",	&C::lble,	&C::relw ,	5, 4 };

	opMap[0x103f] = { "SWI2",	&C::swi2,	&C::inh ,	20, 2 };
	opMap[0x1083] = { "CMPD",	&C::cmpd,	&C::immw ,	5, 4 };
	opMap[0x108c] = { "CMPY",	&C::cmpy,	&C::immw ,	5, 4 };
	opMap[0x108e] = { "LDY",	&C::ldy,	&C::immw ,	5, 4 };
	opMap[0x1093] = { "CMPD",	&C::cmpd,	&C::dir ,	7, 3 };
	opMap[0x109c] = { "CMPY",	&C::cmpy,	&C::dir ,	7, 3 };
	opMap[0x109e] = { "LDY",	&C::ldy,	&C::dir ,	6, 3 };
	opMap[0x109f] = { "STY",	&C::sty,	&C::dir ,	6, 3 };
	opMap[0x10a3] = { "CMPD",	&C::cmpd,	&C::idx ,	7, 3 };
	opMap[0x10ac] = { "CMPY",	&C::cmpy,	&C::idx ,	7, 3 };
	opMap[0x10ae] = { "LDY",	&C::ldy,	&C::idx ,	6, 3 };
	opMap[0x10af] = { "STY",	&C::sty,	&C::idx ,	6, 3 };
	opMap[0x10b3] = { "CMPD",	&C::cmpd,	&C::ext ,	8, 4 };
	opMap[0x10bc] = { "CMPY",	&C::cmpy,	&C::ext ,	8, 4 };
	opMap[0x10be] = { "LDY",	&C::ldy,	&C::ext ,	7, 4 };
	opMap[0x10bf] = { "STY",	&C::sty,	&C::ext ,	7, 4 };
	opMap[0x10ce] = { "LDS",	&C::lds,	&C::immw ,	4, 4 };
	opMap[0x10de] = { "LDS",	&C::lds,	&C::dir ,	6, 4 };
	opMap[0x10df] = { "STS",	&C::sts,	&C::dir ,	6, 3 };
	opMap[0x10ee] = { "LDS",	&C::lds,	&C::idx ,	6, 3 };
	opMap[0x10ef] = { "STS",	&C::sts,	&C::idx ,	6, 3 };
	opMap[0x10fe] = { "LDS",	&C::lds,	&C::ext ,	7, 4 };
	opMap[0x10ff] = { "STS",	&C::sts,	&C::ext ,	7, 4 };

	// page 3

	opMap[0x113f] = { "SWI3",	&C::swi3,	&C::inh ,	20, 2 };
	opMap[0x1183] = { "CMPU",	&C::cmpu,	&C::immw ,	5, 4 };
	opMap[0x118c] = { "CMPS",	&C::cmps,	&C::immw ,	5, 4 };
	opMap[0x1193] = { "CMPU",	&C::cmpu,	&C::dir ,	7, 3 };
	opMap[0x119c] = { "CMPS",	&C::cmps,	&C::dir ,	7, 3 };
	opMap[0x11a3] = { "CMPU",	&C::cmpu,	&C::idx ,	7, 3 };
	opMap[0x11ac] = { "CMPS",	&C::cmps,	&C::idx ,	7, 3 };
	opMap[0x11b3] = { "CMPU",	&C::cmpu,	&C::ext ,	8, 4 };
	opMap[0x11bc] = { "CMPS",	&C::cmps,	&C::ext ,	8, 4 };

	//for (int a = 0x0000; a < 0x1200; a++) {
	//	printf("$%04x %s\n", a, opMap[a].mnem.c_str());
	//}
}



// ***************************************************
// * disasm (NEW VERSION)
// *	Returns standard string of single instruction pointed to by addr.
// *	format:
// * ${ADDR}:{OPCODES} {INST} {OPERAND}
// ***************************************************
std::string C6809::disasm(Word addr, Word& next)
{
	std::string ret = "";
	Byte InstTab = 14;
	Byte PostTab = 19;

	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	std::string sAddress = hex(addr, 4) + " ";
	std::string sOperation = "";
	std::string sOperand = "";

	// fetch the opcode (one or two bytes)
	Word opcode = read(addr);
	Word ofs = 1;
	if (opcode == 0x10 || opcode == 0x11) {
		opcode <<= 8;
		opcode |= read(addr + 1);
		ofs++;
	}
	// post the operation bytes
	Byte length = opMap[opcode].size;
	for (int t = 0; t < length; t++)
	{
		Byte data = read(addr + t);
		sOperation += hex(data, 2);
	}
	addr += ofs;
	// disasemble the operand

	// inherent addressing
	if (opMap[opcode].addrmode == &C6809::inh) {
	}
	// 8-bit immediate
	else if (opMap[opcode].addrmode == &C6809::immb) {
		//sOperand += hex(read(addr), 2);

		// handle special case opcodes: EXG, TFR, PSH, and PUL
		if (opcode == 0x001e || opcode == 0x001f)
		{	// EXG or TFR
			std::map<Byte, std::string> R;
			R[0x00] = "D";  R[0x01] = "X"; R[0x02] = "Y"; R[0x03] = "U";  R[0x04] = "S";
			R[0x05] = "PC"; R[0x08] = "A"; R[0x09] = "B"; R[0x0a] = "CC"; R[0x0b] = "DP";
			Byte data = read(addr++);
			std::string src = R[data >> 4];
			std::string dst = R[data & 0x0f];
			sOperand += src + "," + dst;
		}
		else if (opcode >= 0x0034 && opcode <= 0x0037)
		{	// PSHS, PULS, PSHS, or PSHU
			std::map<Byte, std::string> R;
			R[0x01] = "CC"; R[0x02] = "A"; R[0x04] = "B";  R[0x08] = "DP";
			R[0x10] = "X"; 	R[0x20] = "Y"; R[0x40] = "S"; R[0x80] = "PC";
			Byte data = read(addr++);
			for (int bit = 0; bit < 8; bit++)
			{
				if (data & (1 << bit))
				{
					sOperand += R[(1 << bit)];
					sOperand += ",";
				}
			}
			sOperand.pop_back();
		}
		else
		{	// Otherwise, Immediate (8-bit) has a single post byte #$
			sOperand += "#$" + hex(read(addr), 2); addr++;
		}
	}
	// 16-bit immediate
	else if (opMap[opcode].addrmode == &C6809::immw) {
		sOperand += "#$" + hex(read(addr), 2); addr++;
		sOperand += hex(read(addr), 2); addr++;
	}
	// extended
	else if (opMap[opcode].addrmode == &C6809::ext) {
		// Extended has two post bytes $
		sOperand += "$" + hex(read(addr), 2); addr++;
		sOperand += hex(read(addr), 2); addr++;
	}
	// direct
	else if (opMap[opcode].addrmode == &C6809::dir) {
		// Direct has an 8-bit post byte (is added with the DP register)
		sOperand += "$" + hex(read(addr), 2); addr++;
	}
	// indexed
	else if (opMap[opcode].addrmode == &C6809::idx) {
		Byte post = read(addr);
		sOperation += hex(read(addr + 1), 2);
		sOperation += hex(read(addr + 2), 2);
		addr++;

		std::string regs[] = { "X", "Y", "U", "S" };
		Byte rInd = (post >> 5) & 0x03;
		// EA = 5-bit offset, R
		if ((post & 0x80) == 0) {
			char ofs = (char)(post & 0x1f);
			sOperand += "$" + hex(ofs, 4) + "," + regs[rInd];
		}
		else {
			switch (post & 0x1f) {
			case 0x00:					// R+
				sOperand += "," + regs[rInd] + "+";
				break;
			case 0x01:					// R++
				sOperand += "," + regs[rInd] + "++";
				break;
			case 0x11:					// [,R++]
				sOperand += "[," + regs[rInd] + "++]";
				break;
			case 0x02:					// -R
				sOperand += ",-" + regs[rInd];
				break;
			case 0x03:					// --R
				sOperand += ",--" + regs[rInd];
				break;
			case 0x13:					// [--R]
				sOperand += "[,--" + regs[rInd] + "]";
				break;
			case 0x04:					// 0,R
				sOperand += "," + regs[rInd];
				break;
			case 0x14:					// [0,R]
				sOperand += "[," + regs[rInd] + "]";
				break;
			case 0x05:					// B,R
				sOperand += "B," + regs[rInd];
				break;
			case 0x15:					// [B,R]
				sOperand += "[B," + regs[rInd] + "]";
				break;
			case 0x06:					// A,R
				sOperand += "A," + regs[rInd];
				break;
			case 0x16:					// [A,R]
				sOperand += "[A," + regs[rInd] + "]";
				break;
			case 0x08:					// <8-bit>,R
				sOperand += "$" + hex(read(addr), 2) + "," + regs[rInd]; addr++;
				break;
			case 0x18:					// [<8-bit>,R]
				sOperand += "[$" + hex(read(addr), 2) + "," + regs[rInd] + "]"; addr++;
				break;
			case 0x09:					// <16-bit>,R
				sOperand += "$" + hex(read_word(addr), 4) + "," + regs[rInd]; addr += 2;
				break;
			case 0x19:					// [<16-bit>,R]
				sOperand += "[$" + hex(read_word(addr), 4) + "," + regs[rInd] + "]"; addr += 2;
				break;
			case 0x0b:					// D,R
				sOperand += "D," + regs[rInd];
				break;
			case 0x1b:					// [D,R]
				sOperand += "[D," + regs[rInd] + "]";
				break;
			case 0x0c:					// <8-bit>,PC
				sOperand += "$" + hex(read(addr), 2) + ",PC"; addr++;
				break;
			case 0x1c:					// [<8-bit>,PC]
				sOperand += "[$" + hex(read(addr), 2) + ",PC]"; addr++;
				break;
			case 0x0d:					// <16-bit>,PC
				sOperand += "$" + hex(read_word(addr), 4) + ",PC"; addr += 2;
				break;
			case 0x1d:					// [<16-bit>,PC]
				sOperand += "[$" + hex(read_word(addr), 4) + ",PC]"; addr += 2;
				break;
			case 0x1f:					// [address]
				sOperand += "[$" + hex(read_word(addr), 4) + "]"; addr += 2;
				break;
			default:
				sOperand += "<ERROR>";
			}
		}
	}
	// 8-bit relative
	else if (opMap[opcode].addrmode == &C6809::relb) {
		Word ofs = addr + (char)ext8(read(addr)); addr++;
		sOperand += "$" + hex(ofs + 1, 4);
	}
	// 16-bit relative
	else if (opMap[opcode].addrmode == &C6809::relw) {
		Word ofs = addr + (int)read_word(addr) + 1; addr += 2;
		sOperand += "$" + hex(ofs + 1, 4);
	}


	// build the return string
	ret = sAddress + sOperation;
	while (ret.length() < InstTab) ret += " ";
	ret += opMap[opcode].mnem;
	while (ret.length() < PostTab) ret += " ";

	ret += sOperand;

	next = addr;
	return ret;
}





