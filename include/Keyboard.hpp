/*** Keyboard.hpp *******************************************
 *      _  __          _                         _       _                 
 *     | |/ /         | |                       | |     | |                
 *     | ' / ___ _   _| |__   ___   __ _ _ __ __| |     | |__  _ __  _ __  
 *     |  < / _ \ | | | '_ \ / _ \ / _` | '__/ _` |     | '_ \| '_ \| '_ \ 
 *     | . \  __/ |_| | |_) | (_) | (_| | | | (_| |  _  | | | | |_) | |_) |
 *     |_|\_\___|\__, |_.__/ \___/ \__,_|_|  \__,_| (_) |_| |_| .__/| .__/ 
 *                __/ |                                       | |   | |    
 *               |___/                                        |_|   |_|     
 * 
 *   Acts to emulate a Raspberry PI PICO2 #2
 *		Dispatches Keyboard Events to PICO2 #1
 *		Emulates the Keyboard Hardware Registers
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

#include <unordered_map>
#include <queue>
#include <array>

#include "types.hpp"
#include "IDevice.hpp"
#include "XK_Keycodes.hpp"


class Keyboard : public IDevice 
{

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Keyboard();
    virtual ~Keyboard();

public: // VIRTUAL METHODS

    // virtuals used
    virtual void OnInit() override;                 // initialize
    virtual void OnEvent(SDL_Event* evnt) override; // handle events
    virtual int  OnAttach(int nextAddr) override;   // attach to the memory map

    // virtuals not used
    virtual void OnQuit() override {};
    virtual void OnActivate() override {};
    virtual void OnDeactivate() override {};
    virtual void OnUpdate(float fElapsedTime) override { (void)fElapsedTime; };
    virtual void OnRender() override {};

public: // PUBLIC MEMBER FUNCTIONS

    inline int charQueueLen() { return charQueue.size(); }
    inline Byte charScanQueue() { if (charQueue.size() == 0) { return 0; } return charQueue.front(); }
    inline Byte charPopQueue();
    

    XKey::XK TranslateSDLtoXKey(SDL_Scancode k);
    XKey::XK AscToXKey(Byte asc);
    char XKeyToAscii(XKey::XK xKey);
    bool IsKeyDown(Byte xk);     

private: // PRIVATE MEMBERS

 	std::unordered_map<XKey::XK, int> keyMap;
	std::vector<std::tuple<XKey::XK, char, char>> xkToAsciiMap;
	std::queue <Byte> charQueue;		            // ascii character queue
	std::array <Byte, EDIT_BUFFER_SIZE> editBuffer;	// internal line edit character buffer
	Byte edt_bfr_csr = 0;				            // internal edit character buffer cursor position
	std::string _str_edt_buffer = "";
	bool _line_editor_enable = false;
    Byte _line_editor_length = (Byte)EDIT_BUFFER_SIZE-1;

    // helpers
	void _doEditBuffer(char  xkey);         

};

// END: Keyboard.hpp