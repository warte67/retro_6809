/**** Keyboard.cpp ***************************************
 *
 *   Acts to emulate a Raspberry PI Pico #2
 *		Dispatches Keyboard Events to PICO #1
 *		Emulates the Keyboard Hardware Registers
 *
 *  Copyright (C) 2023 by Jay Faries
 ************************************/

#include "Bus.h"
#include "Keyboard.h"


Byte Keyboard::read(Word offset, bool debug) 
{
	Byte data = 0xCC;	// default unassigned memory

	switch (offset)
	{
		case CHAR_Q_LEN:	data = charQueueLen();	break;
		case CHAR_SCAN:		
			data = 0; 
			if (charQueueLen() > 0) 
				data = charScanQueue();
			break;
		case CHAR_POP:		
			data = 0;
			if (charQueueLen() > 0)
				data = charPopQueue();
			break;
		case EDT_BFR_CSR:	data = edt_bfr_csr; break;
		case EDT_ENABLE:	data = _line_editor_enable; break;
		case EDT_BUFFER:	break;		 
	}
	if (offset >= XKEY_BUFFER && offset < XKEY_BUFFER + 16)
		data = m_memory[offset - m_base];
	// this is just a raw read from the edit buffer
	if (offset >= EDT_BUFFER && offset < EDT_BUFFER + EDIT_BUFFER_SIZE)
	{
		Word index = offset - EDT_BUFFER;
		if (index >= EDIT_BUFFER_SIZE)	index = EDIT_BUFFER_SIZE-1;
		data = editBuffer[index];
	}

	// update and return
	Bus::Write(offset, data, true);
	return data;
}
void Keyboard::write(Word offset, Byte data, bool debug) 
{
	switch (offset)
	{
		//case CHAR_Q_LEN:	break;
		//case CHAR_SCAN:		break;
		//case CHAR_POP:		break;
		case EDT_BFR_CSR:	edt_bfr_csr = data; break;
		// case EDT_BUFFER:	break;
		case EDT_ENABLE: _line_editor_enable = data; break;
	}
	if (offset >= XKEY_BUFFER && offset < XKEY_BUFFER + 16)
	{
		// Just pass through... write the data as is
	}
	// this is just a raw write to the edit buffer
	if (offset >= EDT_BUFFER && offset < EDT_BUFFER + EDIT_BUFFER_SIZE)
	{
		Word index = offset - EDT_BUFFER;
		if (index >= EDIT_BUFFER_SIZE)	index = EDIT_BUFFER_SIZE-1;
		editBuffer[index] = data;
	}
	// a null write to EDT_BUFFER resets the buffer
	if (offset == EDT_BUFFER)
	{
		edt_bfr_csr = 0;
		_str_edt_buffer = "";
		for (auto& a : editBuffer)
			a = 0;
	}

	// write statically
	Bus::Write(offset, data, true);
}


Word Keyboard::OnAttach(Word nextAddr) 
{
	int size = 0;
	Word old_addr = nextAddr;

	DisplayEnum("", 0, "");
	DisplayEnum("KEY_BEGIN",   nextAddr, "Start of the Keyboard Register space");
	nextAddr += 0;

	DisplayEnum("CHAR_Q_LEN",  nextAddr, "  (Byte) # of characters waiting in queue        (Read Only)");
	nextAddr += 1;

	DisplayEnum("CHAR_SCAN",   nextAddr, "  (Byte) read next character in queue (not popped when read)");
	nextAddr += 1;

	DisplayEnum("CHAR_POP",    nextAddr, "  (Byte) read next character in queue (not popped when read)");
	nextAddr += 1;

	DisplayEnum("XKEY_BUFFER", nextAddr, "  (128 bits) 16 bytes for XK_KEY data buffer     (Read Only)");
	nextAddr += 16;

	DisplayEnum("EDT_BFR_CSR", nextAddr, "  (Byte) cursor position within edit buffer     (Read/Write)");
	nextAddr += 1;

	DisplayEnum("EDT_ENABLE", nextAddr, "  (Byte) line editor enable flag                 (Read/Write)");
	nextAddr += 1;

	DisplayEnum("EDT_BUFFER",  nextAddr, "  line editing character buffer                 (Read/Write)");
	nextAddr += EDIT_BUFFER_SIZE;

	DisplayEnum("KEY_END", nextAddr, "End of the Keyboard Register space");
	nextAddr += 0;

	return nextAddr - old_addr;
}

void Keyboard::OnInit() 
{
	// printf("void Keyboard::OnInit()\n");

	// only init once
	if (_bWasInit)	return;

	// reset the character edit buffer
	edt_bfr_csr = 0;
	for (auto a : editBuffer)
		a = 0;


	// initialize the key map
	keyMap[XKey::NONE] = 0x00;	 // INVALID, NULL, or UNASSIGNED key types
	keyMap[XKey::NUMLOCK] = SDL_SCANCODE_NUMLOCKCLEAR;   // NUMLOCK key                 
	keyMap[XKey::SCROLL] = SDL_SCANCODE_SCROLLLOCK;     // SCROLL LOCK key
	keyMap[XKey::BREAK] = 0x03;                        // [CTRL]+[BREAK] the "BREAK KEY" (also CTRL-C)
	keyMap[XKey::L_SHIFT] = SDL_SCANCODE_LSHIFT;         // LEFT-SHIFT key 
	keyMap[XKey::R_SHIFT] = SDL_SCANCODE_RSHIFT;         // RIGHT-SHIFT key   
	keyMap[XKey::L_CONTROL] = SDL_SCANCODE_LCTRL;          // LEFT-CONTROL key
	keyMap[XKey::R_CONTROL] = SDL_SCANCODE_RCTRL;          // RIGHT-CONTROL key
	keyMap[XKey::BACKSPACE] = SDL_SCANCODE_BACKSPACE;      // BACKSPACE key
	keyMap[XKey::TAB] = SDL_SCANCODE_TAB;            // TAB key
	keyMap[XKey::L_ALT] = SDL_SCANCODE_RALT;           // LEFT-ALT key
	keyMap[XKey::R_ALT] = SDL_SCANCODE_LALT;           // RIGHT-ALT key
	keyMap[XKey::GUI] = 0x02;                        // WINDOWS key 
	keyMap[XKey::RETURN] = SDL_SCANCODE_RETURN;         // RETURN key
	keyMap[XKey::SEMICOLON] = SDL_SCANCODE_SEMICOLON;      // [;][:] key
	keyMap[XKey::EQUAL] = SDL_SCANCODE_EQUALS;         // [=][+] key
	keyMap[XKey::SHIFT] = 0x7c;                        // SHIFT key  
	keyMap[XKey::CONTROL] = 0x7d;                        // CONTROL key
	keyMap[XKey::ALT] = 0x7e;                        // ALT key
	keyMap[XKey::PAUSE] = SDL_SCANCODE_PAUSE;          // PAUSE key
	keyMap[XKey::CAPITAL] = SDL_SCANCODE_CAPSLOCK;       // CAPS LOCK key
	keyMap[XKey::BACKSLASH] = SDL_SCANCODE_BACKSLASH;      // [\][|] key   
	keyMap[XKey::QUOTE] = SDL_SCANCODE_APOSTROPHE;     // [']["] key
	keyMap[XKey::SLASH] = SDL_SCANCODE_SLASH;          // [/][?] key
	keyMap[XKey::TILDE] = SDL_SCANCODE_GRAVE;          // [`][~] key
	keyMap[XKey::L_BRACKET] = SDL_SCANCODE_LEFTBRACKET;    // [[][{] key
	keyMap[XKey::R_BRACKET] = SDL_SCANCODE_RIGHTBRACKET;   // []][}] key
	keyMap[XKey::ESCAPE] = SDL_SCANCODE_ESCAPE;         // ESCAPE key
	keyMap[XKey::SPACE] = SDL_SCANCODE_SPACE;          // SPACEBAR key
	keyMap[XKey::PAGEUP] = SDL_SCANCODE_PAGEUP;         // PAGE UP key
	keyMap[XKey::PAGEDOWN] = SDL_SCANCODE_PAGEDOWN;       // PAGE DOWN key
	keyMap[XKey::END] = SDL_SCANCODE_END;            // END key
	keyMap[XKey::HOME] = SDL_SCANCODE_HOME;           // HOME key
	keyMap[XKey::LEFT] = SDL_SCANCODE_LEFT;           // LEFT ARROW key
	keyMap[XKey::UP] = SDL_SCANCODE_UP;             // UP ARROW key
	keyMap[XKey::RIGHT] = SDL_SCANCODE_RIGHT;          // RIGHT key
	keyMap[XKey::DOWN] = SDL_SCANCODE_DOWN;           // DOWN key
	keyMap[XKey::CONTEXT] = SDL_SCANCODE_APPLICATION;    // SELECT key
	keyMap[XKey::PRINTSCRN] = SDL_SCANCODE_PRINTSCREEN;    // PRINT SCREEN Bkey
	keyMap[XKey::INSERT] = SDL_SCANCODE_INSERT;         // INSERT key  
	keyMap[XKey::DELETE] = SDL_SCANCODE_DELETE;         // DELETE key 
	keyMap[XKey::NUM_0] = SDL_SCANCODE_0;				// 0 key
	keyMap[XKey::NUM_1] = SDL_SCANCODE_1;              // 1 key
	keyMap[XKey::NUM_2] = SDL_SCANCODE_2;				// 2 key
	keyMap[XKey::NUM_3] = SDL_SCANCODE_3;				// 3 key
	keyMap[XKey::NUM_4] = SDL_SCANCODE_4;				// 4 key
	keyMap[XKey::NUM_5] = SDL_SCANCODE_5;				// 5 key
	keyMap[XKey::NUM_6] = SDL_SCANCODE_6;				// 6 key
	keyMap[XKey::NUM_7] = SDL_SCANCODE_7;				// 7 key
	keyMap[XKey::NUM_8] = SDL_SCANCODE_8;				// 8 key
	keyMap[XKey::NUM_9] = SDL_SCANCODE_9;				// 9 key
	keyMap[XKey::A] = SDL_SCANCODE_A;              // A key
	keyMap[XKey::B] = SDL_SCANCODE_B;				// B key
	keyMap[XKey::C] = SDL_SCANCODE_C;				// C key
	keyMap[XKey::D] = SDL_SCANCODE_D;				// D key
	keyMap[XKey::E] = SDL_SCANCODE_E;				// E key
	keyMap[XKey::F] = SDL_SCANCODE_F;				// F key
	keyMap[XKey::G] = SDL_SCANCODE_G;				// G key
	keyMap[XKey::H] = SDL_SCANCODE_H;				// H key
	keyMap[XKey::I] = SDL_SCANCODE_I;				// I key
	keyMap[XKey::J] = SDL_SCANCODE_J;				// J key
	keyMap[XKey::K] = SDL_SCANCODE_K;				// K key
	keyMap[XKey::L] = SDL_SCANCODE_L;				// L key
	keyMap[XKey::M] = SDL_SCANCODE_M;				// M key
	keyMap[XKey::N] = SDL_SCANCODE_N;				// N key
	keyMap[XKey::O] = SDL_SCANCODE_O;				// O key
	keyMap[XKey::P] = SDL_SCANCODE_P;				// P key
	keyMap[XKey::Q] = SDL_SCANCODE_Q;				// Q key
	keyMap[XKey::R] = SDL_SCANCODE_R;				// R key
	keyMap[XKey::S] = SDL_SCANCODE_S;				// S key
	keyMap[XKey::T] = SDL_SCANCODE_T;				// T key
	keyMap[XKey::U] = SDL_SCANCODE_U;				// U key
	keyMap[XKey::V] = SDL_SCANCODE_V;				// V key
	keyMap[XKey::W] = SDL_SCANCODE_W;				// W key
	keyMap[XKey::X] = SDL_SCANCODE_X;				// X key
	keyMap[XKey::Y] = SDL_SCANCODE_Y;				// Y key
	keyMap[XKey::Z] = SDL_SCANCODE_Z;				// Z key
	keyMap[XKey::L_WIN] = SDL_SCANCODE_LGUI;           // LEFT WINDOWS key
	keyMap[XKey::R_WIN] = SDL_SCANCODE_RGUI;			// RIGHT WINDOWS key
	keyMap[XKey::COMMA] = SDL_SCANCODE_COMMA;			// [,][<] key
	keyMap[XKey::MINUS] = SDL_SCANCODE_MINUS;			// [-][_] key
	keyMap[XKey::PERIOD] = SDL_SCANCODE_PERIOD;			// [.][>] key
	keyMap[XKey::NUMPAD_0] = SDL_SCANCODE_KP_0;			// NUM PAD 0 key
	keyMap[XKey::NUMPAD_1] = SDL_SCANCODE_KP_1;			// NUM PAD 1 key
	keyMap[XKey::NUMPAD_2] = SDL_SCANCODE_KP_2;			// NUM PAD 2 key
	keyMap[XKey::NUMPAD_3] = SDL_SCANCODE_KP_3;			// NUM PAD 3 key
	keyMap[XKey::NUMPAD_4] = SDL_SCANCODE_KP_4;			// NUM PAD 4 key
	keyMap[XKey::NUMPAD_5] = SDL_SCANCODE_KP_5;			// NUM PAD 5 key
	keyMap[XKey::NUMPAD_6] = SDL_SCANCODE_KP_6;			// NUM PAD 6 key
	keyMap[XKey::NUMPAD_7] = SDL_SCANCODE_KP_7;			// NUM PAD 7 key
	keyMap[XKey::NUMPAD_8] = SDL_SCANCODE_KP_8;			// NUM PAD 8 key
	keyMap[XKey::NUMPAD_9] = SDL_SCANCODE_KP_9;			// NUM PAD 9 key
	keyMap[XKey::NP_MULT] = SDL_SCANCODE_KP_MULTIPLY;	// MULTIPLY key
	keyMap[XKey::NP_ADD] = SDL_SCANCODE_KP_PLUS;        // ADD key
	keyMap[XKey::NP_ENTER] = SDL_SCANCODE_KP_ENTER;		// NUM PAD ENTER key
	keyMap[XKey::NP_SUB] = SDL_SCANCODE_KP_MINUS;		// SUBTRACT key
	keyMap[XKey::NP_DECIMAL] = SDL_SCANCODE_KP_PERIOD;		// DECIMAL key
	keyMap[XKey::NP_DIVIDE] = SDL_SCANCODE_KP_DIVIDE;		// DEVIDE key
	keyMap[XKey::F1] = SDL_SCANCODE_F1;				// F1  key
	keyMap[XKey::F2] = SDL_SCANCODE_F2;				// F2  key
	keyMap[XKey::F3] = SDL_SCANCODE_F3;				// F3  key
	keyMap[XKey::F4] = SDL_SCANCODE_F4;				// F4  key
	keyMap[XKey::F5] = SDL_SCANCODE_F5;				// F5  key
	keyMap[XKey::F6] = SDL_SCANCODE_F6;				// F6  key
	keyMap[XKey::F7] = SDL_SCANCODE_F7;				// F7  key
	keyMap[XKey::F8] = SDL_SCANCODE_F8;				// F8  key
	keyMap[XKey::F9] = SDL_SCANCODE_F9;				// F9  key
	keyMap[XKey::F10] = SDL_SCANCODE_F10;			// F10 key
	keyMap[XKey::F11] = SDL_SCANCODE_F11;			// F11 key
	keyMap[XKey::F12] = SDL_SCANCODE_F12;            // F12 key   

	//// ASCII MAP /////////////////////////////////////////////////////

	xkToAsciiMap = {		// VK_CODE,   unshifted,   shifted

		{ XKey::NONE, 0x00, 0x00 },							// <NONE>
		{ XKey::SCROLL, 0x01, 0x01 },						// SCROLL
		{ XKey::BREAK, 0x03, 0x03 },						// Break CTRL+[BREAK] or ctrl-C
		{ XKey::L_SHIFT, 0x02, 0x02 },						// L_SHIFT
		{ XKey::R_SHIFT, 0x04, 0x04 },						// R_SHIFT
		{ XKey::L_CONTROL, 0x05, 0x05 },					// L_CONTROL
		{ XKey::R_CONTROL, 0x06, 0x06 },					// 0x06 R_CONTROL
		{ XKey::BACKSPACE, 0x08, 0x08 },					// 0x08 BACKSPACE
		{ XKey::TAB, 0x07, 0x07 },							// TAB
		{ XKey::L_ALT, 0x09, 0x09 },						// L_ALT
		{ XKey::R_ALT, 0x0a, 0x0a },						// R_ALT
		{ XKey::GUI, 0x0b, 0x0b },							// GUI						**********
		{ XKey::RETURN, 0x0d, 0x0d },						// RETURN
		{ XKey::SEMICOLON, ';', ':' },
		{ XKey::EQUAL, '=', '+' },
		{ XKey::L_WIN, 0x0c, 0x0c },						// L_WINDOWS key
		{ XKey::R_WIN, 0x0e, 0x0e },						// R_WINDOWS key
		{ XKey::NUMLOCK, 0x0f, 0x0f },						// NUMLOCK key
		{ XKey::PAUSE, 0x10, 0x10 },						// PAUSE
		{ XKey::CAPITAL, 0x11, 0x11 },						// 0x11 CAPITAL
		{ XKey::BACKSLASH, '\\', '|' },
		{ XKey::QUOTE, 0x27, 0x22 },
		{ XKey::SLASH, '/', '?'},
		{ XKey::TILDE, '`', '~'},
		{ XKey::L_BRACKET, '[', '{' },
		{ XKey::R_BRACKET, ']', '}' },
		{ XKey::ESCAPE, 0x1b, 0x1b },						// ESCAPE
		{ XKey::SPACE, ' ', ' '},
		{ XKey::PAGEUP, 0x12, 0x12 },						// PAGEUP
		{ XKey::PAGEDOWN, 0x13, 0x13 },						// PAGEDOWN
		{ XKey::END, 0x14, 0x14 },							// END
		{ XKey::HOME, 0x15, 0x15},							// HOME
		{ XKey::LEFT, 0x16, 0x16 },							// LEFT
		{ XKey::UP, 0x17, 0x17 },							// UP
		{ XKey::RIGHT, 0x18, 0x18 },						// RIGHT
		{ XKey::DOWN, 0x19, 0x19 },							// DOWN
		{ XKey::CONTEXT, 0x1a, 0x1a },						// CONTEXT
		{ XKey::PRINTSCRN, 0x1c, 0x1c },					// PRINTSCRN
		{ XKey::INSERT, 0x1d, 0x1d },						// INSERT
		{ XKey::DELETE, 0x1e, 0x1e },						// DELETE

		{ XKey::NUM_1, '1', '!' },
		{ XKey::NUM_2, '2', '@' },
		{ XKey::NUM_3, '3', '#' },
		{ XKey::NUM_4, '4', '$' },
		{ XKey::NUM_5, '5', '%' },
		{ XKey::NUM_6, '6', '^' },
		{ XKey::NUM_7, '7', '&' },
		{ XKey::NUM_8, '8', '*' },
		{ XKey::NUM_9, '9', '(' },
		{ XKey::NUM_0, '0', ')' },

		{ XKey::A, 'a', 'A' },
		{ XKey::B, 'b', 'B' },
		{ XKey::C, 'c', 'C' },
		{ XKey::D, 'd', 'D' },
		{ XKey::E, 'e', 'E' },
		{ XKey::F, 'f', 'F' },
		{ XKey::G, 'g', 'G' },
		{ XKey::H, 'h', 'H' },
		{ XKey::I, 'i', 'I' },
		{ XKey::J, 'j', 'J' },
		{ XKey::K, 'k', 'K' },
		{ XKey::L, 'l', 'L' },
		{ XKey::M, 'm', 'M' },
		{ XKey::N, 'n', 'N' },
		{ XKey::O, 'o', 'O' },
		{ XKey::P, 'p', 'P' },
		{ XKey::Q, 'q', 'Q' },
		{ XKey::R, 'r', 'R' },
		{ XKey::S, 's', 'S' },
		{ XKey::T, 't', 'T' },
		{ XKey::U, 'u', 'U' },
		{ XKey::V, 'v', 'V' },
		{ XKey::W, 'w', 'W' },
		{ XKey::X, 'x', 'X' },
		{ XKey::Y, 'y', 'Y' },
		{ XKey::Z, 'z', 'Z' },

		{ XKey::COMMA, ',', '<'},
		{ XKey::MINUS, '-', '_'},
		{ XKey::PERIOD, '.', '>'},

		{ XKey::NUMPAD_0, '0', '0' },
		{ XKey::NUMPAD_1, '1', '1' },
		{ XKey::NUMPAD_2, '2', '2' },
		{ XKey::NUMPAD_3, '3', '3' },
		{ XKey::NUMPAD_4, '4', '4' },
		{ XKey::NUMPAD_5, '5', '5' },
		{ XKey::NUMPAD_6, '6', '6' },
		{ XKey::NUMPAD_7, '7', '7' },
		{ XKey::NUMPAD_8, '8', '8' },
		{ XKey::NUMPAD_9, '9', '9' },

		{ XKey::NP_MULT, '*', '*'},
		{ XKey::NP_ADD, '+', '+'},
		{ XKey::NP_ENTER, '\n', '\n'},
		{ XKey::NP_SUB, '-', '-'},
		{ XKey::NP_DECIMAL, '.', '.'},
		{ XKey::NP_DIVIDE, '/', '/' },

		{ XKey::F1 , 0x80, 0x90 },	// 0x80 FG f     0x90 BG 0
		{ XKey::F2 , 0x81, 0x91 },	// 0x81 FG 8     0x91 BG 1
		{ XKey::F3 , 0x82, 0x92 },	// 0x82 FG 9     0x92 BG 2
		{ XKey::F4 , 0x83, 0x93 },	// 0x83 FG a     0x93 BG 3
		{ XKey::F5 , 0x84, 0x94 },	// 0x84 FG b     0x94 BG 4
		{ XKey::F6 , 0x85, 0x95 },	// 0x85 FG c     0x95 BG 5
		{ XKey::F7 , 0x86, 0x96 },	// 0x86 FG d     0x96 BG 6
		{ XKey::F8 , 0x87, 0x97 },	// 0x87 FG e     0x97 BG 7
		{ XKey::F9 , 0x88, 0x98 },	// not yet used possibly clear screen
		{ XKey::F10, 0x89, 0x99 },	// not yet used
		{ XKey::F11, 0x8a, 0x9a },	// not yet used
		{ XKey::F12, 0x8b, 0x9b },	// not yet used
	};

	// clear the edit buffer
	for (auto& b : editBuffer)
		b = 0;

	_bWasInit = true;
}
void Keyboard::OnQuit() 
{
	// printf("void Keyboard::OnQuit()\n");

	// only quit once
	if (!_bWasInit)	return;

	// nothing to do
	// ...

	_bWasInit = false;
}
void Keyboard::OnActivate() 
{
	// printf("void Keyboard::OnActivate()\n");
}
void Keyboard::OnDeactivate() {
	// printf("void Keyboard::OnDeactivate()\n");
}
void Keyboard::OnEvent(SDL_Event* evnt) {
	// printf("void Keyboard::OnEvent(SDL_Event* evnt)\n");

	SDL_Keymod km = SDL_GetModState();
	switch (evnt->type)
	{
		case SDL_KEYDOWN:
		{
			SDL_Scancode sdl_scancode = evnt->key.keysym.scancode;
			Byte sdl_sym = evnt->key.keysym.sym;
			XKey::XK xkey = TranslateSDLtoXKey(sdl_scancode);

			_doEditBuffer(XKeyToAscii(xkey));

			// find the register that should contain the bit
			Word reg = xkey / 8;
			Byte bit = this->read(XKEY_BUFFER + reg) | (1 << (xkey % 8));
			this->write(XKEY_BUFFER + reg, bit);
			Byte b = this->read(XKEY_BUFFER + reg);
			// handle the ALT keys
			if (xkey == XKey::L_ALT || xkey == XKey::R_ALT)
			{
				xkey = XKey::ALT;
				reg = xkey / 8;
				bit = this->read(XKEY_BUFFER + reg) | (1 << (xkey % 8));
				this->write(XKEY_BUFFER + reg, bit);
			}
			// handle the SHIFT keys
			if (xkey == XKey::L_SHIFT || xkey == XKey::R_SHIFT)
			{
				xkey = XKey::SHIFT;
				reg = xkey / 8;
				bit = this->read(XKEY_BUFFER + reg) | (1 << (xkey % 8));
				this->write(XKEY_BUFFER + reg, bit);
			}
			// handle the CONTROL keys
			if (xkey == XKey::L_CONTROL || xkey == XKey::R_CONTROL)
			{
				xkey = XKey::CONTROL;
				reg = xkey / 8;
				bit = this->read(XKEY_BUFFER + reg) | (1 << (xkey % 8));
				this->write(XKEY_BUFFER + reg, bit);
			}
			// don't queue key presses while in the debugger screen
			if ((km & KMOD_ALT) || (km & KMOD_CTRL) || GfxDebug::_bIsDebugActive)
				break;
			else
			{
				// push the ascii character into the character queue
				char key = XKeyToAscii(xkey);
				// push the ascii key into its queue
				if (key != 0)
					charQueue.push(key);
				break;
			}
		}
		case SDL_KEYUP:
		{
			SDL_Scancode sdl_scancode = evnt->key.keysym.scancode;
			XKey::XK xkey = TranslateSDLtoXKey(sdl_scancode);
			char key = XKeyToAscii(xkey);
			// find the register that should contain the bit
			Word reg = xkey / 8;
			Byte bit = this->read(XKEY_BUFFER + reg) & ~(1 << (xkey % 8));
			this->write(XKEY_BUFFER + reg, bit);
			// Bus::Write(XKEY_BUFFER + reg, bit, true);
			// handle release of the ALT keys
			if (xkey == XKey::L_ALT || xkey == XKey::R_ALT)
			{
				xkey = XKey::ALT;
				reg = xkey / 8;
				bit = this->read(XKEY_BUFFER + reg) & ~(1 << (xkey % 8));
				this->write(XKEY_BUFFER + reg, bit);
			}
			// handle release of the SHIFT keys
			if (xkey == XKey::L_SHIFT || xkey == XKey::R_SHIFT)
			{
				xkey = XKey::SHIFT;
				reg = xkey / 8;
				bit = this->read(XKEY_BUFFER + reg) & ~(1 << (xkey % 8));
				this->write(XKEY_BUFFER + reg, bit);
			}
			// handle release of the CONTROL keys
			if (xkey == XKey::L_CONTROL || xkey == XKey::R_CONTROL)
			{
				xkey = XKey::CONTROL;
				reg = xkey / 8;
				bit = this->read(XKEY_BUFFER + reg) & ~(1 << (xkey % 8));
				this->write(XKEY_BUFFER + reg, bit);
			}
			break;
		}
	}
}

void Keyboard::_doEditBuffer(char xkey)
{
	// basic keyboard buffer character line editor
	if (!_line_editor_enable)
		return;

	// if (Bus::Read(CHAR_SCAN))
	{
		Byte c = xkey;
		auto itr = _str_edt_buffer.begin() + edt_bfr_csr;
		Byte Cols = Bus::Read(DSP_TXT_COLS);

		if (c == XKeyToAscii(XKey::LEFT))
		{
			// Bus::Read(CHAR_POP);
			if (edt_bfr_csr > 0)
				edt_bfr_csr--;
		}
		else if (c == XKeyToAscii(XKey::RIGHT))
		{
			// Bus::Read(CHAR_POP);
			if (edt_bfr_csr < _str_edt_buffer.size())
				edt_bfr_csr++;
		}
		else if (c == XKeyToAscii(XKey::UP))
		{
			// Bus::Read(CHAR_POP);
			if (edt_bfr_csr > Cols)
				edt_bfr_csr -= Cols;
		}
		else if (c == XKeyToAscii(XKey::DOWN))
		{
			// Bus::Read(CHAR_POP);
			if (edt_bfr_csr + Cols < _str_edt_buffer.size())
				edt_bfr_csr += Cols;
		}
		else if (c == XKeyToAscii(XKey::BACKSPACE))
		{
			// Bus::Read(CHAR_POP);
			if (edt_bfr_csr > 0)
			{
				std::string _right = _str_edt_buffer.substr(edt_bfr_csr);
				_str_edt_buffer = _str_edt_buffer.substr(0, edt_bfr_csr - 1);
				_str_edt_buffer += _right;
				edt_bfr_csr--;
			}
		}
		else if (c == XKeyToAscii(XKey::DELETE))
		{
			// Bus::Read(CHAR_POP);
			if (edt_bfr_csr < _str_edt_buffer.size())
			{
				std::string _right = _str_edt_buffer.substr(edt_bfr_csr + 1);
				_str_edt_buffer = _str_edt_buffer.substr(0, edt_bfr_csr);
				_str_edt_buffer += _right;
			}
		}
		else if (c == XKeyToAscii(XKey::END))
		{
			// Bus::Read(CHAR_POP);
			edt_bfr_csr = _str_edt_buffer.size();
		}
		else if (c == XKeyToAscii(XKey::HOME))
		{
			// Bus::Read(CHAR_POP);
			edt_bfr_csr = 0;
		}
		else if (c >= 0x20 && c < 128)
		{
			if (_str_edt_buffer.size() < editBuffer.size() - 1)
			{
				// Bus::Read(CHAR_POP);
				_str_edt_buffer.insert(itr, c);
				edt_bfr_csr++;
			}
		}
	}

	editBuffer.at(0) = ' ';
	//for (auto &a : editBuffer)
	//	a = 0;
	Word i = 0;
	for (auto& a : _str_edt_buffer)
		editBuffer.at(i++) = a;
	editBuffer.at(i) = ' ';
}



void Keyboard::OnUpdate(float fElapsedTime) 
{

}

//void Keyboard::OnRender() {}

int Keyboard::charQueueLen() { return charQueue.size(); }

Byte Keyboard::charScanQueue() {
	if (charQueue.size() == 0)
		return 0;
	return charQueue.front();
}

Byte Keyboard::charPopQueue() {
	if (charQueue.size() == 0)
		return 0;
	Byte ret = charQueue.front();
	charQueue.pop();
	write(CHAR_Q_LEN, charQueue.size());
	return ret;
}

XKey::XK Keyboard::TranslateSDLtoXKey(SDL_Scancode k)
{
	for (auto& a : keyMap)
		if (a.second == (Byte)k)
			return a.first;
	return XKey::NONE;
}


char Keyboard::XKeyToAscii(XKey::XK xKey)
{
	char ret = 0;
	// is shift key held?
	bool bIsShifted = false;
	if (IsKeyDown(XKey::SHIFT))
		bIsShifted = true;
	for (auto& a : xkToAsciiMap)
	{
		XKey::XK xk = std::get<0>(a);
		char s1 = std::get<1>(a);
		char s2 = std::get<2>(a);
		if (xk == xKey && !bIsShifted)  // not shifted
			return s1;
		if (xk == xKey && bIsShifted)  // is shifted
			return s2;
	}
	return ret;
}

XKey::XK Keyboard::AscToXKey(Byte asc)
{
	bool bIsShifted = false;
	if (IsKeyDown(XKey::SHIFT))
		bIsShifted = true;

	XKey::XK ret = XKey::NONE;
	for (auto& a : xkToAsciiMap)
	{
		if (bIsShifted)
		{
			if (std::get<2>(a) == asc)
			{
				ret = std::get<0>(a);
				break;
			}
		}
		else
		{
			if (std::get<1>(a) == asc)
			{
				ret = std::get<0>(a);
				break;
			}
		}
	}
	return ret;
}

bool Keyboard::IsKeyDown(Byte xk)
{
	// find the register that should contain the bit
	Word reg = xk / 8;
	bool b = this->read(XKEY_BUFFER + reg) & (1 << (xk % 8));
	return b;
}
