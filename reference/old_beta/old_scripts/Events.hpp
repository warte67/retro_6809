/***  Events.hpp  (singleton) *************************
 * 
 *  Wraps the SDL2 Event sub-system to implement
 *  a greatly reduced set of events for the simplified 
 *  8-bit target hardware.
 * 
 *  IMPORTANT NOTE:
 *  	The SDL event subsystem "should only be run in the thread 
 *  	that initialized the video subsystem, and for extra safety, 
 *  	you should consider only doing those things on the main 
 *  	thread in any case" (SDL Wiki).
 *  		- https://wiki.libsdl.org/SDL3/SDL_PumpEvents
 * 
 *************************/
#pragma once

#include "types.hpp"

/////////////////////////////////////////////////////
// General keyboard/mouse button state definitions //
/////////////////////////////////////////////////////
#ifndef EV_RELEASED
	#define EV_RELEASED		0
#endif
#ifndef EV_PRESSED
	#define EV_PRESSED		1
#endif


///////////////////////////////////////////////////////////
// The types of events that can be delivered. This is a  //
// greatly reduced subset of the SDL_EventTypes. Only    //
// the events that will be supported by the target       //
// hardware will be implemented here.                    //
//                                                       //
//     Note:                                             // 
//          The EV_EventType enums are NOT numerically   //
//          equivalent with their SDL counterparts.      //
//          These types are 8-bit values rather than the //
//          32-bit types of SDL2_EventType enumeration.  //
//                                                       //
///////////////////////////////////////////////////////////
typedef enum EV_EventType
{
    EV_FIRSTEVENT 			= 0x00,  	// unused (do not remove)
	// ...
	EV_QUIT,							// user-requested quit
	// ...
	EV_KEYDOWN,							// key down event
	EV_KEYUP,							// key up event
	// ...
    EV_LASTEVENT 			= 0xFF // max enum (for bounding)
} EV_EventType;

//////////////////////////////////
// Fields shared by every event //
//////////////////////////////////
typedef struct EV_CommonEvent
{
    EV_EventType type;
    DWord timestamp;  		// in milliseconds, time since app start
} EV_CommonEvent;

// ...

///////////////////////////////////////////////////
// Keyboard button event structure (event.key.*) //
///////////////////////////////////////////////////
typedef struct EV_KeyboardEvent
{
    Uint32 type;        	// SDL_KEYDOWN or SDL_KEYUP
    Uint32 timestamp;   	// In milliseconds, populated using SDL_GetTicks()
    Uint32 windowID;    	// The window with keyboard focus, if any
    Uint8 state;        	// SDL_PRESSED or SDL_RELEASED 
    Uint8 repeat;       	// Non-zero if this is a key repeat 
    Uint8 padding2;
    Uint8 padding3;
    SDL_Keysym keysym;  	// The key that was pressed or released
} EV_KeyboardEvent;

// ...

////////////////////////////////
// The "Quit Requested" Event //
////////////////////////////////
typedef struct EV_QuitEvent
{
    EV_EventType type;		// EV_Quit
    DWord timestamp;  		// in milliseconds, time since app start
} EV_QuitEvent;

// ...


////////////////////////////////////////////////////////////////////
// The EV_Event structure is the core of all event handling in    //
// this event handler. It is based on SDL2, but uses a greatly    //
// reduced set of possible events. The goal is to support for the //
// 8-bit target hardware device.                                  //
//                                                                //
// The EV_Event structure has two uses: Reading events from the   //
// event queue and placing new events onto that queue.            //
////////////////////////////////////////////////////////////////////
typedef union EV_Event
{
    EV_EventType type;			// event type, is shared with all events
	EV_CommonEvent comon;		// common event data
	// ...
	EV_KeyboardEvent key; 		// Keyboard event data
	// ...
	EV_QuitEvent quit;			// quit request event data
	// ...

} EV_Event;


/////////////////////////////////////////////////////////////////////
// The main event device class object that acts as the heartbeat   //
// of the entire event sub-system. It is responsible for static    //
// functions, It is tasked with queueing new events onto the event // 
// queue as well as delivering them when raised. This is a static  //
// device that acts more-or-less as a wrapper for the events       //
// provided by the SDL2 event sub-system.                          //
/////////////////////////////////////////////////////////////////////
class Events
{
	/////////////////////////////
	// PRIVATE SINGLETON STUFF //
	/////////////////////////////
private: 
    Events();
    ~Events();

	////////////////////////////
	// PUBLIC SINGLETON STUFF //
	////////////////////////////
public: 
	Events(const Events&) = delete;				// delete the copy constructor
	Events(Events&&) = delete;					// delete the move constructor
	Events& operator=(const Events&) = delete;	// delete the copy assignment operator
	Events& operator=(Events&&) = delete;		// delete the move assignment operator
	static Events& GetInstance() { static Events inst; return inst; }    	

	////////////////////////////
	// PUBLIC WRAPPER METHODS //
	////////////////////////////
public:
	static int EV_PollEvent(EV_Event *evnt);	// SDL_PollEvent()	
	static void EV_Quit()	{ SDL_Quit(); }		// SDL_Quit();	
	static std::string EV_GetError(void);		// SDL_GetError();
	static void EV_Delay(Uint32 ms);			// SDL_Delay();
	// static EV_GetModState

};








// END: Events.hpp