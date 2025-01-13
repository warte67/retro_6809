/*** C6809.hpp *****************************************
 *       _____  __   ___   ___   ___        _                 
 *      / ____|/ /  / _ \ / _ \ / _ \      | |                
 *     | |    / /_ | (_) | | | | (_) |     | |__  _ __  _ __  
 *     | |   | '_ \ > _ <| | | |\__, |     | '_ \| '_ \| '_ \ 
 *     | |___| (_) | (_) | |_| |  / /   _  | | | | |_) | |_) |
 *      \_____\___/ \___/ \___/  /_/   (_) |_| |_| .__/| .__/ 
 *                                               | |   | |    
 *                                               |_|   |_|    
 *
 * MC6809 Central Processing Unit Emulation object
 * 
 * Under GPL v3.0 License
 *
 **************************************/
#pragma once
#ifndef __C6809_H__
#define __C6809_H__

#pragma once
#include "Bus.hpp"
#include "types.hpp"
#include <string>
#include <list>
#include <unordered_map>
#include <condition_variable>
#include <cstring>

class Bus;
class Debug;

class C6809		//		: public REG
{
	friend class Bus;
	friend class Debug;

public:
	C6809(Bus* p_bus);
	~C6809();
	void Init();

	// Device type registers
	inline static int s_sys_state = 0x0A;		// system speed 0-15
											// 00:   10 kHz
											// 01:   25 kHz
											// 02:   50 kHz
											// 03:   75 kHz
											// 04:  100 kHz
											// 05:  150 kHz
											// 06:  225 kHz
											// 07:  350 kHz
											// 08:  500 kHz
											// 09:  750 kHz
											// 0A:  900 kHz
											// 0B: 1000 khz
											// 0C: 2000 khz
											// 0D: 3000 khz
											// 0E: 4000 khz
											// 0F: Unmetered  (10.0 mhz)


	private:
		inline static bool s_bCpuEnabled = false;

	
	public:
		inline static void IsCpuEnabled(bool b)	{ s_bCpuEnabled = b; }
		inline static bool IsCpuEnabled()		{ return s_bCpuEnabled; }

		

	static void ThreadProc();

    inline static auto hex(uint32_t n, Byte d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };
	
	std::string disasm(Word addr, Word& next);	// returns standard string containing instruction pointed to by addr

    Byte GetInstructionSize(Word opcode) { 
        if ((opcode & 0xFF00) == 0x1000 || (opcode & 0xFF00) == 0x1100) {
            return opMap[opcode].size; 
        }
        return opMap[((opcode>>8)&0xFF)].size; 
    }

	void clock_input(); // this one doesnt need to inherit from device

	// pin states
	void nmi(); // true to false transition triggers NMI
	void irq(); // true to false transition triggers IRQ
	void firq(); // true to false transition triggers FIRQ
	void reset();

	// getters
	inline Word getPC()    { std::lock_guard<std::mutex> lock(_register_mutex); return PC; }
	inline Word getU()     { std::lock_guard<std::mutex> lock(_register_mutex); return U; }
	inline Word getS()     { std::lock_guard<std::mutex> lock(_register_mutex); return S; }
	inline Word getX()     { std::lock_guard<std::mutex> lock(_register_mutex); return X; }
	inline Word getY()     { std::lock_guard<std::mutex> lock(_register_mutex); return Y; }
	inline Byte getDP()    { std::lock_guard<std::mutex> lock(_register_mutex); return DP; }
	inline Byte getA()     { std::lock_guard<std::mutex> lock(_register_mutex); return A; }
	inline Byte getB()     { std::lock_guard<std::mutex> lock(_register_mutex); return B; }
	inline Word getD()     { std::lock_guard<std::mutex> lock(_register_mutex); return D; }
	inline Byte getCC()    { std::lock_guard<std::mutex> lock(_register_mutex); return CC.all; }
	inline bool getCC_E()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.E != 0; }
	inline bool getCC_F()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.F != 0; }
	inline bool getCC_H()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.H != 0; }
	inline bool getCC_I()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.I != 0; }
	inline bool getCC_N()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.N != 0; }
	inline bool getCC_Z()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.Z != 0; }
	inline bool getCC_V()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.V != 0; }
	inline bool getCC_C()  { std::lock_guard<std::mutex> lock(_register_mutex); return CC.bit.C != 0; }
	inline Byte getCycles(){ std::lock_guard<std::mutex> lock(_register_mutex); return cycles; }
    // setters
	inline void setPC(Word pPc)    { std::lock_guard<std::mutex> lock(_register_mutex); PC = pPc; }
	inline void setU(Word pU)      { std::lock_guard<std::mutex> lock(_register_mutex); U = pU; }
	inline void setS(Word pS)      { std::lock_guard<std::mutex> lock(_register_mutex); S = pS; }
	inline void setX(Word pX)      { std::lock_guard<std::mutex> lock(_register_mutex); X = pX; }
	inline void setY(Word pY)      { std::lock_guard<std::mutex> lock(_register_mutex); Y = pY; }
	inline void setD(Word pD)      { std::lock_guard<std::mutex> lock(_register_mutex); D = pD; }
	inline void setA(Byte pA)      { std::lock_guard<std::mutex> lock(_register_mutex); A = pA; }
	inline void setB(Byte pB)      { std::lock_guard<std::mutex> lock(_register_mutex); B = pB; }
	inline void setCC(Byte pCC)    { std::lock_guard<std::mutex> lock(_register_mutex); CC.all = pCC; }
	inline void setCC_E(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.E = bSet; }
	inline void setCC_F(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.F = bSet; }
	inline void setCC_H(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.H = bSet; }
	inline void setCC_I(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.I = bSet; }
	inline void setCC_N(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.N = bSet; }
	inline void setCC_Z(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.Z = bSet; }
	inline void setCC_V(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.V = bSet; }
	inline void setCC_C(bool bSet) { std::lock_guard<std::mutex> lock(_register_mutex); CC.bit.C = bSet; }
	inline void setDP(Byte pDP)    { std::lock_guard<std::mutex> lock(_register_mutex); DP = pDP; }

	// memory access

	Byte fetch_byte() { Byte data = read(PC); PC++; return data; }
	Word fetch_word() { Word data = read_word(PC); PC += 2; return data; }

	Byte read(Word offset)						
	{ 
		Byte d = Bus::Read(offset);
		return d; 
	}
	void write(Word offset, Byte data)			
	{ 
		Bus::Write(offset, data);
	}
	Word read_word(Word offset)					{ return Memory::Read_Word(offset); }
	void write_word(Word offset, Word data)		{ Memory::Write_Word(offset, data); }

	Byte debug_read(Word offset)				{ return Memory::Read(offset, true);}
	void debug_write(Word offset, Byte data)	{ Memory::Write(offset, data, true); }
	Word debug_read_word(Word offset)			{ return Memory::Read_Word(offset, true); }
	void debug_write_word(Word offset, Word data) { Memory::Write_Word(offset, data, true); }


private:
	// Registers
    std::mutex _register_mutex;

	Word U, S;
	Word X, Y;
	Byte DP;
	Word PC;
	union {
		Word D;
		struct {
			Byte B;
			Byte A;
		} byte;
	} acc;
	Byte& A;
	Byte& B;
	Word& D;
	union {
		Byte all;
		struct {
			Byte C : 1; // Carry
			Byte V : 1; // Overflow
			Byte Z : 1; // Zero
			Byte N : 1; // Negative
			Byte I : 1; // IRQ disable
			Byte H : 1; // Half carry
			Byte F : 1; // FIRQ disable
			Byte E : 1;	// Entire
		} bit;
	} CC;

	struct INSTRUCTION {
		std::string mnem = "";							// text version of the mnemonic
		void (C6809::* operation)(void) = nullptr;	// the operation member function
		Word(C6809::* addrmode)(void) = nullptr;	// the address_mode function
		Byte cycles = 0;			// base cycles (not including internal increases)
		Byte size = 0;				// how many bytes long is this instruction?
	};

	Word* ptrReg[4] = { &X, &Y, &U, &S };


	// addressing modes:
	Word inh();		// INHERENT
	Word immb();	// IMMEDIATE 8-BIT
	Word immw();	// IMMEDIATE 16-BIT
	Word ext();		// EXTENDED
	Word dir();		// DIRECT
	Word idx();		// INDEXED
	Word relb();	// RELATIVE 8-BIT
	Word relw();	// RELATIVE 16-BIT
	Word nula();	// invalid addressing mode

	// opcodes:
	void abx(void);		void adca(void);	void adcb(void);	void adda(void);
	void addb(void);	void addd(void);	void anda(void);	void andb(void);
	void andc(void);	void asla(void);	void aslb(void);	void asl(void);
	void asra(void);	void asrb(void);	void asr(void);		void bita(void);
	void bitb(void);	void clra(void);	void clrb(void);	void clr(void);
	void cmpa(void);	void cmpb(void);	void cmpd(void);	void cmps(void);
	void cmpu(void);	void cmpx(void);	void cmpy(void);	void coma(void);
	void comb(void);	void com(void);		void cwai(void);	void daa(void);
	void deca(void);	void decb(void);	void dec(void);		void eora(void);
	void eorb(void);	void exg(void);		void inca(void);	void incb(void);
	void inc(void);		void jmp(void);		void jsr(void);		void lda(void);
	void ldb(void);		void ldd(void);		void lds(void);		void ldu(void);
	void ldx(void);		void ldy(void);		void leas(void);	void leau(void);
	void leax(void);	void leay(void);	void lsra(void);	void lsrb(void);
	void lsr(void);		void mul(void);		void nega(void);	void negb(void);
	void neg(void);		void nop(void);		void ora(void);		void orb(void);
	void orcc(void);	void pshs(void);	void pshu(void);	void puls(void);
	void pulu(void);	void rola(void);	void rolb(void);	void rol(void);
	void rora(void);	void rorb(void);	void ror(void);		void rti(void);
	void rts(void);		void sbca(void);	void sbcb(void);	void sex(void);
	void sta(void);		void stb(void);		void std(void);		void sts(void);
	void stu(void);		void stx(void);		void sty(void);		void suba(void);
	void subb(void);	void subd(void);	void swi(void);		void swi2(void);
	void swi3(void);	void sync(void);	void tfr(void);		void tsta(void);
	void tstb(void);	void tst(void);		void bcc(void);		void lbcc(void);
	void bcs(void);		void lbcs(void);	void beq(void);		void lbeq(void);
	void bge(void);		void lbge(void);	void bgt(void);		void lbgt(void);
	void bhi(void);		void lbhi(void);	void ble(void);		void lble(void);
	void bls(void);		void lbls(void);	void blt(void);		void lblt(void);
	void bmi(void);		void lbmi(void);	void bne(void);		void lbne(void);
	void bpl(void);		void lbpl(void);	void bra(void);		void lbra(void);
	void brn(void);		void lbrn(void);	void bsr(void);		void lbsr(void);
	void bvc(void);		void lbvc(void);	void bvs(void);		void lbvs(void);
	void pg2(void);		void pg3(void);		void null();


	// HELPER FUNCTIONS

	// 8 bit operations
	bool btst(Byte x, int n) { bool ret = (x & (1 << n)); return ret; }
	void bset(Byte& x, int n) { x |= (1 << n); }
	void bclr(Byte& x, int n) { x &= ~(1 << n); }

	// 16 bit operations
	bool btst(Word x, int n) { bool ret = (x & (1 << n)); return ret; }
	void bset(Word& x, int n) { x |= (1 << n); }
	void bclr(Word& x, int n) { x &= ~(1 << n); }

	// 32 bit operations
	bool btst(DWord x, int n) { bool ret = (x & (1L << n)); return ret; }
	void bset(DWord& x, int n) { x |= (1L << n); }
	void bclr(DWord& x, int n) { x &= ~(1L << n); }

	// Bit extend operations
	Word ext5(Byte x) {
		if (x & 0x10)
			return (Word)x | 0xffe0;
		else
			return (Word)x & 0x1f;
	}
	Word ext8(Byte x) {
		if (x & 0x80)
			return (Word)x | 0xff00;
		else
			return (Word)x;
	}

	void psh_post(Byte post, Word& s, Word& u);
	void pul_post(Byte post, Word& s, Word& u);
	void do_psh(Word& sp, Byte val);	void do_psh(Word& sp, Word val);
	void do_pul(Word& sp, Byte& val);	void do_pul(Word& sp, Word& val);

	void do_adc(Byte& x);	void do_add(Byte& x);	void do_add(Word& x);
	void do_and(Byte& x);
	void do_asl(Byte& x);	void do_asr(Byte& x);	void do_bit(Byte& x);
	void do_clr(Byte& x);	void do_cmp(Byte x);	void do_cmp(Word x);
	void do_com(Byte& x);	void do_dec(Byte& x);	void do_eor(Byte& x);
	void do_inc(Byte& x);	void do_ld(Byte& x);	void do_ld(Word& x);
	void do_lsr(Byte& x);	void do_neg(Byte& x);	void do_or(Byte& x);
	void do_rol(Byte& x);	void do_ror(Byte& x);	void do_sbc(Byte& x);
	void do_st(Byte x);		void do_st(Word x);		void do_sub(Byte& x);	void do_sub(Word& x);
	void do_tst(Byte& x);	void do_br(bool test);	void do_lbr(bool test);

    bool isCpuThreadReady = false;
    std::mutex mtx;
    std::condition_variable cv;

protected:

	// interrupts
	bool waiting_sync = false;	// not within SYNC
	bool waiting_cwai = false;	// not within CWAI
	bool nmi_previous = true;	// no NMI present
	bool nmi_disabled = true;	// wait to enable NMI until after S has been initialized
	bool NMI = true;
	bool IRQ = true;
	bool FIRQ = true;

	void do_nmi();
	void do_firq();
	void do_irq();
	bool do_interrupts();

protected:

	std::unordered_map<Word, INSTRUCTION> opMap;

	Word opcode = 0x0000;
	Byte post = 0x00;
	Byte cycles = 0;

	Bus* m_bus = nullptr;
	// Debug* m_debug = nullptr;


    // disassembly stuff (REFACTORING in progress)
// private:
//     std::mutex _bitfield_mutex;       // Mutex to protect the bitfield
//     uint8_t _bitfield_visited[8192] = {0};    // 8 KB bitfield for tracking visited addresses (64k bits)
// public:
//     inline void SetVisited_Memory(uint16_t address) {
//         _bitfield_mutex.lock();  // Lock the mutex manually
//         _bitfield_visited[address / 8] |= (1 << (address % 8));
//         _bitfield_mutex.unlock();   // Unlock the mutex manually
//     }

//     inline bool WasVisited_Memory(uint16_t address) {
//         _bitfield_mutex.lock();  // Lock the mutex manually
//         bool result = (_bitfield_visited[address / 8] & (1 << (address % 8))) != 0;  // Check if the bit is set
//         _bitfield_mutex.unlock();  // Unlock the mutex manually
//         return result;
//     }

//     inline void ClearVisited_Memory() {
//         _bitfield_mutex.lock();  // Lock the mutex manually
//         std::memset(_bitfield_visited, 0, sizeof(_bitfield_visited));
//         _bitfield_mutex.unlock();   // Unlock the mutex manually
//     }   

private:
    std::atomic<uint8_t> _bitfield_visited[8192];  // Atomic bitfield for visited memory

public:
    inline void SetVisited_Memory(uint16_t address) {
        uint16_t index = address / 8;
        uint8_t bit_pos = address % 8;
        // Atomically set the bit at the corresponding position
        _bitfield_visited[index].fetch_or(1 << bit_pos, std::memory_order_relaxed);
    }

    inline bool WasVisited_Memory(uint16_t address) {
        uint16_t index = address / 8;
        uint8_t bit_pos = address % 8;
        // Atomically check if the bit is set
        return (_bitfield_visited[index].load(std::memory_order_relaxed) & (1 << bit_pos)) != 0;
    }

    // Be sure to call this function to clear the visited memory
    // or when loading a new program or in the case of self-modifying code.
    // Another good place to call this is when the debugger is enabled or re-enabled.
    inline void ClearVisited_Memory() {
        // Clear the whole bitfield atomically
        for (size_t i = 0; i < 8192; ++i) {
            _bitfield_visited[i].store(0, std::memory_order_relaxed);
        }
    }

	// CPU speed as measured over the last second
	inline static Word _cpu_speed;
};




/****** NOTES: ****************
 
#define USE_CPU_MUTEX true
#define USE_MEMORY_MUTEX false
#define USE_IO_MUTEX false

#if USE_CPU_MUTEX
    #define CPU_LOCK(mutex) std::lock_guard<std::mutex> lock(mutex)
#else
    #define CPU_LOCK(mutex) // No-op
#endif

#if USE_MEMORY_MUTEX
    #define MEMORY_LOCK(mutex) std::lock_guard<std::mutex> lock(mutex)
#else
    #define MEMORY_LOCK(mutex) // No-op
#endif


Word getPC() {
    CPU_LOCK(_register_mutex);
    return PC;
}

Byte readMemory(Word addr) {
    MEMORY_LOCK(memory_mutex);
    return memory[addr];
}

// etc...


*****************************/

#endif		// __C6809_H__