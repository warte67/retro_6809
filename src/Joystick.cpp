/*** Joystick.cpp *******************************************
 *           _                 _   _      _                           
 *          | |               | | (_)    | |                          
 *          | | ___  _   _ ___| |_ _  ___| | __       ___ _ __  _ __  
 *      _   | |/ _ \| | | / __| __| |/ __| |/ /      / __| '_ \| '_ \ 
 *     | |__| | (_) | |_| \__ \ |_| | (__|   <   _  | (__| |_) | |_) |
 *      \____/ \___/ \__, |___/\__|_|\___|_|\_\ (_)  \___| .__/| .__/ 
 *                    __/ |                              | |   | |    
 *                   |___/                               |_|   |_|  
 *     
 *      Supports Joystick Controller input as well as most Gamepad Controllers.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/


#include "Bus.hpp"
#include "Joystick.hpp"



/***************************
* Constructor / Destructor *
***************************/

Joystick::Joystick() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Joystick Device Created" << clr::RETURN;
    _device_name = "JOYSTICK_DEVICE"; 
} // END: Joystick()

Joystick::~Joystick() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Joystick Device Destroyed" << clr::RETURN; 
} // END: ~Joystick()



/******************
* Virtual Methods *
******************/



/**
 * OnAttach
 * 
 * Called by the Bus when this device is attached to the bus.
 * 
 * @param nextAddr The base address of the device in the bus.
 * 
 * @return The size of the device in address space.
 */
int  Joystick::OnAttach(int nextAddr)
{
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "Joystick/Gamepad Controller Device Hardware Registers"; 


    ////////////////////////////////////////////////
    // (Byte) JOYS_1_FLAGS
    //       PAD 1 Condition Flags   (read/write)
    mapped_register.push_back( { "JOYS_1_FLAGS", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   
            "(Byte) Gamepad/Joystick #1 Condition Flags:     (Read Only)",
            "           0000'0000: Not Connected",      
            "           0000'1111: Controller Type",
            "           0001'0000: (reserved)",
            "           0010'0000: (reserved)",
            "           0100'0000: Is a Gamepad",
            "           1000'0000: Is a Joystick",
            "",
            "       Gamepad Controller Type (bits 0-3):",
            "           0:  Unknown",
            "           1:  Standard",
            "           2:  Xbox360",
            "           3:  XboxOne",
            "           4:  PS3",
            "           5:  PS4",
            "           6:  PS5",
            "           7:  Nintendo Switch Pro",
            "           8:  Nintendo Switch Joycon Left",
            "           9:  Nintendo Switch Joycon Right",
            "",
            "       Joystick Controller Type (bits 0-3):",
            "           0:  Unknown",
            "           1:  Gamepad",
            "           2:  Wheel",
            "           3:  Arcade Stick",
            "           4:  Flight Stick",
            "           5:  Dance Pad",
            "           6:  Guitar",
            "           7:  Drum Kit",
            "           8:  Arcade Pad",
            "           9:  Throttle",
            ""
        }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) JOYS_1_BTN
    //       Button Bits: Room For up to 16 Buttons  (realtime)
    mapped_register.push_back( { "JOYS_1_BTN", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(Word) Gamepad Controller Button Bits:         (Read Only)",
            "           0000'0000'0000'0000 = Nothing Pressed",
            "           0000'0000'0000'0001 = A",  
            "           0000'0000'0000'0010 = B",  
            "           0000'0000'0000'0100 = X",   
            "           0000'0000'0000'1000 = Y",   
            "           0000'0000'0001'0000 = L.Shoulder",   
            "           0000'0000'0010'0000 = R.Shoulder",
            "           0000'0000'0100'0000 = Back",   
            "           0000'0000'1000'0000 = Start",
            "           0000'0001'0000'0000 = Misc 1",   
            "           0000'0010'0000'0000 = Misc 2",  
            "           0000'0100'0000'0000 = Misc 3",   
            "           0000'1000'0000'0000 = Guide",  
            "           0001'0000'0000'0000 = DPad Up   ",   
            "           0010'0000'0000'0000 = DPad Down ",
            "           0100'0000'0000'0000 = DPad Left ",   
            "           1000'0000'0000'0000 = DPad Right",     
            "           1111'1111'1111'1111 = Not Connected",""}}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Byte) JOYS_1_DBND
    //       PAD 1 analog deadband; default is 5   (read/write)
    mapped_register.push_back( { "JOYS_1_DBND", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(Byte) PAD 1 analog deadband; default is 5   (read/write)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_LTX
    //       PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_LTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_LTY
    //       PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_LTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_RTX
    //       PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_RTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_RTY
    //       PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_RTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_Z1
    //       PAD 1 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_1_Z1", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_Z2
    //       PAD 1 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_1_Z2", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 left analog trigger (0 - 127)     (realtime)","" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Byte) JOYS_2_FLAGS
    //       PAD 2 Condition Flags   (read/write)
    mapped_register.push_back( { "JOYS_2_FLAGS", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   
            "(Byte) Gamepad/Joystick #2 Condition Flags:     (Read Only)",
            "           0000'0000: Not Connected",      
            "           0000'1111: Controller Type",
            "           0001'0000: (reserved)",
            "           0010'0000: (reserved)",
            "           0100'0000: Is a Gamepad",
            "           1000'0000: Is a Joystick",
            "",
            "       Gamepad Controller Type (bits 0-3):",
            "           0:  Unknown",
            "           1:  Standard",
            "           2:  Xbox360",
            "           3:  XboxOne",
            "           4:  PS3",
            "           5:  PS4",
            "           6:  PS5",
            "           7:  Nintendo Switch Pro",
            "           8:  Nintendo Switch Joycon Left",
            "           9:  Nintendo Switch Joycon Right",
            "",
            "       Joystick Controller Type (bits 0-3):",
            "           0:  Unknown",
            "           1:  Gamepad",
            "           2:  Wheel",
            "           3:  Arcade Stick",
            "           4:  Flight Stick",
            "           5:  Dance Pad",
            "           6:  Guitar",
            "           7:  Drum Kit",
            "           8:  Arcade Pad",
            "           9:  Throttle",
            ""
        }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) JOYS_2_BTN
    //       Button Bits: Room For up to 16 Buttons  (realtime)
    mapped_register.push_back( { "JOYS_2_BTN", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
         {   "(Word) Button Bits: Room For up to 16 Buttons  (realtime)",
            "       Joystick Button Bits:",
            "           0000'0000'0000'0000 = Nothing Pressed",
            "           0000'0000'0000'0001 = Button 1 ",  
            "           0000'0000'0000'0010 = Button 2 ",  
            "           0000'0000'0000'0100 = Button 3 ",   
            "           0000'0000'0000'1000 = Button 4 ",   
            "           0000'0000'0001'0000 = Button 5 ",   
            "           0000'0000'0010'0000 = Button 6 ",
            "           0000'0000'0100'0000 = Button 7 ",   
            "           0000'0000'1000'0000 = Button 8 ",
            "           0000'0001'0000'0000 = Button 9 ",   
            "           0000'0010'0000'0000 = Button 10",  
            "           0000'0100'0000'0000 = Button 11",   
            "           0000'1000'0000'0000 = Button 12",  
            "           0001'0000'0000'0000 = Hat Up   ",   
            "           0010'0000'0000'0000 = Hat Down ",
            "           0100'0000'0000'0000 = Hat Left ",   
            "           1000'0000'0000'0000 = Hat Right",   
            "           1111'1111'1111'1111 = Not Connected",""}}); nextAddr+=1;  
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Byte) JOYS_2_DBND
    //       PAD 2 analog deadband; default is 5   (read/write)
    mapped_register.push_back( { "JOYS_2_DBND", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(Byte) PAD 2 analog deadband; default is 5   (read/write)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_LTX
    //       PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_LTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_LTY
    //       PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_LTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_RTX
    //       PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_RTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_RTY
    //       PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_RTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_Z1
    //       PAD 1 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_2_Z1", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_Z2
    //       PAD 2 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_2_Z2", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Constant) JOY_END
    //      End of GPU Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "JOYS_END", nextAddr, 
        nullptr, nullptr,  { "End of Joystick/Gamepad Device Register Space"} });
    nextAddr++;
    
    
    ////////////////////////////////////////////////
    // (Constant) JOY_TOP
    //      Top of GPU Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "JOYS_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of Joystick/Gamepad Device Register Space", "---"}});
    
    // return with the size of the new device
    _size = nextAddr - old_address;
    return _size;
}


/// \brief OnInit - Initialize the Joystick/Gamepad Device
///
/// This function will initialize the Joystick/Gamepad Device. It will set the default
/// dead band (dead zone) for the analog sticks and initialize the SDL joystick/gamepad
/// subsystem. If the subsystem is not already initialized, it will open all gamepads
/// and joysticks it can find and set their internal state to active.
///
/// \return void
void Joystick::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnInit() Entry" << clr::RETURN;    
    InitButtonStates(); 
	if (!bJoystickWasInit)
	{
		int ret = SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD);
		if (ret < 0)
		{
			std::string msg = "SDL could not initialize Joysticks and/or joysticks! SDL_Error: ";
			msg += SDL_GetError();
			Bus::Error(msg.c_str());
			return;
		}
		// set the default analog dead band (dead zone)
		Memory::Write(MAP(JOYS_1_DBND), 5, true);
		Memory::Write(MAP(JOYS_2_DBND), 5, true);
		OpenControllers();
		bJoystickWasInit = true;
	}   
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnInit() Exit" << clr::RETURN;
}



/// \brief OnQuit - Shutdown the Joystick/Gamepad Device
///
/// This function will shutdown the Joystick/Gamepad Device. It will close all
/// open controllers and shutdown the SDL joystick/gamepad subsystem.
///
/// \return void
void Joystick::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnQuit() Entry" << clr::RETURN;
	if (bJoystickWasInit)
	{
		CloseControllers();
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD);
		bJoystickWasInit = false;
	}   
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnQuit() Exit" << clr::RETURN;
}


/// \brief Handle SDL events related to gamepad and joystick connections.
///
/// This function processes SDL events that indicate the addition, removal, or remapping
/// of gamepads and joysticks. When such events are detected, it closes all currently open
/// controllers and reopens them to update their states.
///
/// \param evnt The SDL event containing information about the gamepad or joystick event.

void Joystick::OnEvent(SDL_Event* evnt)
{
    switch (evnt->type)
    {
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
        case SDL_EVENT_GAMEPAD_REMAPPED:
        case SDL_EVENT_JOYSTICK_ADDED:
        case SDL_EVENT_JOYSTICK_REMOVED:
            CloseControllers();
            OpenControllers();
            break;
    }    
}


/// \brief Update the button and axis registers.
///
/// This function is called once per frame and updates the button and axis registers
/// based on the current state of the joysticks and gamepads.
void Joystick::OnUpdate(float fElapsedTime)
{
    (void)fElapsedTime;     // stop the compiler from complaining about unused parameters
    // update button registers
	if (state[0].bIsActive)
		Memory::Write_Word(MAP(JOYS_1_BTN), EncodeButtonRegister(0), true);
	else
		Memory::Write_Word(MAP(JOYS_1_BTN), 0xffff, true);
	if (state[1].bIsActive)
		Memory::Write_Word(MAP(JOYS_2_BTN), EncodeButtonRegister(1), true);
	else
		Memory::Write_Word(MAP(JOYS_2_BTN), 0xffff, true);
	EncodeAxesRegister(0);
	EncodeAxesRegister(1);

    EncodeConditionFlags(0);
    EncodeConditionFlags(1);
}



/// \brief Initialize the button mapping tables for gamepads and joysticks.
///
/// This function initializes the button mapping tables that are used to translate
/// SDL gamepad and joystick button presses into the 16-bit button register format
/// used by the emulator. The mapping tables are used by the EncodeButtonRegister
/// function to translate the button presses into the correct bits in the button
/// register.
void Joystick::InitButtonStates()
{
	gpadBtnMap.clear();
	joysBtnMap.clear();

    // 16 buttons including a single hat on a Gamepad
    Byte btn = 0;
	gpadBtnMap[SDL_GAMEPAD_BUTTON_SOUTH] = btn++;			// GPAD_BTN::BTN_A
	gpadBtnMap[SDL_GAMEPAD_BUTTON_EAST] = btn++;			// GPAD_BTN::BTN_B
	gpadBtnMap[SDL_GAMEPAD_BUTTON_WEST] = btn++;			// GPAD_BTN::BTN_X
	gpadBtnMap[SDL_GAMEPAD_BUTTON_NORTH] = btn++;			// GPAD_BTN::BTN_Y

	gpadBtnMap[SDL_GAMEPAD_BUTTON_LEFT_SHOULDER] = btn++;	// GPAD_BTN::BTN_LS
	gpadBtnMap[SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER] = btn++;	// GPAD_BTN::BTN_RS
	gpadBtnMap[SDL_GAMEPAD_BUTTON_BACK] = btn++;			// GPAD_BTN::BTN_BACK
	gpadBtnMap[SDL_GAMEPAD_BUTTON_START] = btn++;		    // GPAD_BTN::BTN_START

	gpadBtnMap[SDL_GAMEPAD_BUTTON_MISC1] = btn++;	        // GPAD_BTN::BTN_MISC1
	gpadBtnMap[SDL_GAMEPAD_BUTTON_MISC2] = btn++;	        // GPAD_BTN::BTN_MISC2
	gpadBtnMap[SDL_GAMEPAD_BUTTON_MISC3] = btn++;	        // GPAD_BTN::BTN_MISC3
	gpadBtnMap[SDL_GAMEPAD_BUTTON_GUIDE] = btn++;		    // GPAD_BTN::BTN_GUIDE

	gpadBtnMap[SDL_GAMEPAD_BUTTON_DPAD_UP] = btn++;		    // GPAD_BTN::BTN_DPAD_UP
	gpadBtnMap[SDL_GAMEPAD_BUTTON_DPAD_DOWN] = btn++;	    // GPAD_BTN::BTN_DPAD_DOWN
	gpadBtnMap[SDL_GAMEPAD_BUTTON_DPAD_LEFT] = btn++;	    // GPAD_BTN::BTN_DPAD_LEFT
	gpadBtnMap[SDL_GAMEPAD_BUTTON_DPAD_RIGHT] = btn++;	    // GPAD_BTN::BTN_DPAD_RIGHT

    // 16 buttons including a single hat on a joystick
	btn = 0;
	joysBtnMap[JOYS_BTN::BTN_1] = btn++;
	joysBtnMap[JOYS_BTN::BTN_2] = btn++;
	joysBtnMap[JOYS_BTN::BTN_3] = btn++;
	joysBtnMap[JOYS_BTN::BTN_4] = btn++;

	joysBtnMap[JOYS_BTN::BTN_5] = btn++;
	joysBtnMap[JOYS_BTN::BTN_6] = btn++;
	joysBtnMap[JOYS_BTN::BTN_7] = btn++;
	joysBtnMap[JOYS_BTN::BTN_8] = btn++;

	joysBtnMap[JOYS_BTN::BTN_9] = btn++;
	joysBtnMap[JOYS_BTN::BTN_10] = btn++;
	joysBtnMap[JOYS_BTN::BTN_11] = btn++;
	joysBtnMap[JOYS_BTN::BTN_12] = btn++;

	joysBtnMap[JOYS_BTN::BTN_HAT_UP] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_DOWN] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_LEFT] = btn++;
	joysBtnMap[JOYS_BTN::BTN_HAT_RIGHT] = btn++;

}


/// \brief Open the first two gamepads or joysticks found.
///
/// This function is called during OnInit() and OnEvent() when the system detects
/// a change in the availability of gamepads or joysticks. It will open the first
/// two gamepads or joysticks it finds and set their internal state to active.
///
/// \return void
void Joystick::OpenControllers()
{
    int num_joysticks;
    SDL_JoystickID* jstk_id = SDL_GetJoysticks(&num_joysticks);
    if (num_joysticks > 2)  num_joysticks = 2;

	for (int num = 0; num < num_joysticks; num++)
	{
        SDL_JoystickID index = jstk_id[num];
		if (SDL_IsGamepad(index)) // It's a Gamepad?
		{
			state[num].bIsJoystick = false;
			state[num].name = SDL_GetGamepadNameForID(index);
			state[num].gamepad = SDL_OpenGamepad(index);
			if (state[num].gamepad == nullptr)
			{
				std::string msg = "SDL could not open a Gamepad! SDL_Error: ";
				msg += SDL_GetError();
				Bus::Error(msg.c_str(), __FILE__, __LINE__);
				return;
			}
			state[num].bIsActive = true;
		}
		else // It's a Joystick
		{
			state[num].bIsJoystick = true;
			state[num].joystick = SDL_OpenJoystick(index);
			if (state[num].joystick == nullptr)
			{
				std::string msg = "SDL could not open a joystick! SDL_Error: ";
				msg += SDL_GetError();
				Bus::Error(msg.c_str(), __FILE__, __LINE__);
				return;
			}
			state[num].name = SDL_GetJoystickNameForID(index);
			state[num].bIsActive = true;
		}
	}
}

/// \brief CloseControllers - Deactivate and close all gamepad or joystick controllers.
///
/// This function deactivates and closes the first two gamepad or joystick controllers
/// that are currently open. It updates the state to inactive, writes default button 
/// values to memory, and releases resources associated with each controller.
void Joystick::CloseControllers()
{
    // Close Controller 0
	state[0].bIsActive = false;
	Memory::Write_Word(MAP(JOYS_1_BTN), 0xffff, true);
	Memory::Write(MAP(JOYS_1_FLAGS), 0, true);
    if (state[0].gamepad) {
        SDL_CloseGamepad(state[0].gamepad);
        state[0].gamepad = nullptr;
    }
    if (state[0].joystick) {
        SDL_CloseJoystick(state[0].joystick);
        state[0].joystick = nullptr;
    }
    // Close Controller 1
	state[1].bIsActive = false;
	Memory::Write_Word(MAP(JOYS_2_BTN), 0xffff, true);
	Memory::Write(MAP(JOYS_2_FLAGS), 0, true);
    if (state[1].gamepad) {
        SDL_CloseGamepad(state[1].gamepad);
        state[1].gamepad = nullptr;
    }
    if (state[1].joystick) {
        SDL_CloseJoystick(state[1].joystick);
        state[1].joystick = nullptr;
    }
}


/// \brief EncodeButtonRegister - Encode gamepad or joystick buttons into a 16-bit register
///
/// This function encodes the state of the first 16 buttons of a gamepad or joystick
/// into a 16-bit register. The register is returned as a Word value.
///
/// \param id - The ID of the gamepad or joystick (0 or 1).
///
/// \return The encoded button register as a Word value.
Word Joystick::EncodeButtonRegister(int id)
{
	id %= 2;
	Word ret = 0xffff;
	if (state[id].bIsActive)
	{
		ret = 0;
		// encode only the first 16 buttons
		for (int btn = 0; btn < 16; btn++)
		{
			// Gamepad?
			if (!state[id].bIsJoystick)
			{
				if (SDL_GetGamepadButton(state[id].gamepad, (SDL_GamepadButton)btn))
					ret |= (1 << gpadBtnMap[btn]);
			}
			else // Joystick
			{
				if (SDL_GetJoystickButton(state[id].joystick, btn))
					ret |= (1 << joysBtnMap[btn]);
				// encode hat
				Byte hat = SDL_GetJoystickHat(state[id].joystick, 0);
				if (hat == SDL_HAT_UP || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_LEFTUP)
					ret |= 0x1000;
				if (hat == SDL_HAT_DOWN || hat == SDL_HAT_RIGHTDOWN || hat == SDL_HAT_LEFTDOWN)
					ret |= 0x2000;
				if (hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN)
					ret |= 0x4000;
				if (hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN)
					ret |= 0x8000;
			}
		}
	}
	return ret;
}


/// \brief Encode the axes registers for gamepads and joysticks.
///
/// This function is called once per frame and updates the axes registers based on
/// the current state of the joysticks and gamepads.
///
/// \param id - The ID of the gamepad or joystick (0 or 1).
void Joystick::EncodeAxesRegister(int id)
{
	id %= 2;
	if (state[id].bIsActive)
	{
		Sint8 deadband = Memory::Read(MAP(JOYS_1_DBND));
		if (id == 1)
			deadband = Memory::Read(MAP(JOYS_2_DBND));

		if (!state[id].bIsJoystick)	// is Gamepad
		{
			Sint8 LTX = (SDL_GetGamepadAxis(state[id].gamepad, SDL_GAMEPAD_AXIS_LEFTX) & 0xff00) >> 8;
			if (LTX < 0 && LTX > -deadband)	LTX = 0;
			if (LTX > 0 && LTX < deadband)	LTX = 0;
			Sint8 LTY = (SDL_GetGamepadAxis(state[id].gamepad, SDL_GAMEPAD_AXIS_LEFTY) & 0xff00) >> 8;
			if (LTY < 0 && LTY > -deadband)	LTY = 0;
			if (LTY > 0 && LTY < deadband)	LTY = 0;
			Sint8 RTX = (SDL_GetGamepadAxis(state[id].gamepad, SDL_GAMEPAD_AXIS_RIGHTX) & 0xff00) >> 8;
			if (RTX < 0 && RTX > -deadband)	RTX = 0;
			if (RTX > 0 && RTX < deadband)	RTX = 0;
			Sint8 RTY = (SDL_GetGamepadAxis(state[id].gamepad, SDL_GAMEPAD_AXIS_RIGHTY) & 0xff00) >> 8;
			if (RTY < 0 && RTY > -deadband)	RTY = 0;
			if (RTY > 0 && RTY < deadband)	RTY = 0;
			Sint8 Z1 = SDL_GetGamepadAxis(state[id].gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) >> 8;
			Sint8 Z2 = SDL_GetGamepadAxis(state[id].gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) >> 8;

			if (id == 0)
			{
				Memory::Write(MAP(JOYS_1_LTX), LTX, true);	
				Memory::Write(MAP(JOYS_1_LTY), LTY, true);	
				Memory::Write(MAP(JOYS_1_RTX), RTX, true);	
				Memory::Write(MAP(JOYS_1_RTY), RTY, true);	
				Memory::Write(MAP(JOYS_1_Z1 ),  Z1, true);	
				Memory::Write(MAP(JOYS_1_Z2 ),  Z2, true);	
			}
			else
			{
				Memory::Write(MAP(JOYS_2_LTX), LTX, true);	
				Memory::Write(MAP(JOYS_2_LTY), LTY, true);	
				Memory::Write(MAP(JOYS_2_RTX), RTX, true);	
				Memory::Write(MAP(JOYS_2_RTY), RTY, true);	
				Memory::Write(MAP(JOYS_2_Z1 ),  Z1, true);	
				Memory::Write(MAP(JOYS_2_Z2 ),  Z2, true);	
			}
		}
		else  // is Joystick
		{
			Sint8 LTX = (SDL_GetJoystickAxis(state[id].joystick, 0) & 0xff00) >> 8;
			if (LTX < 0 && LTX > -deadband)	LTX = 0;
			if (LTX > 0 && LTX < deadband)	LTX = 0;
			Sint8 LTY = (SDL_GetJoystickAxis(state[id].joystick, 1) & 0xff00) >> 8;
			if (LTY < 0 && LTY > -deadband)	LTY = 0;
			if (LTY > 0 && LTY < deadband)	LTY = 0;
			// RTX maps the hat
			Byte hat = SDL_GetJoystickHat(state[id].joystick, 0);
			Sint8 RTX = 0;
			Sint8 RTY = 0;
			if (hat == SDL_HAT_UP || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_LEFTUP)
				RTY = -128;
			if (hat == SDL_HAT_DOWN || hat == SDL_HAT_RIGHTDOWN || hat == SDL_HAT_LEFTDOWN)
				RTY = 127;
			if (hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN)
				RTX = -128;
			if (hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN)
				RTX = 127;
			// throttle lever
			Uint8 Z1 = SDL_GetJoystickAxis(state[id].joystick, 3) >> 8;
			Z1 = 255 - (Z1 + 128);

			// rudder axis
			Sint8 Z2 = SDL_GetJoystickAxis(state[id].joystick, 2) >> 8;
			if (Z2 < 0 && Z2 > -deadband * 3)	Z2 = 0;
			if (Z2 > 0 && Z2 < deadband * 3)	Z2 = 0;

			if (id == 0)
			{
				Memory::Write(MAP(JOYS_1_LTX), LTX, true);	// bus->debug_write(JOYS_1_LTX, LTX);
				Memory::Write(MAP(JOYS_1_LTY), LTY, true);	// bus->debug_write(JOYS_1_LTY, LTY);
				Memory::Write(MAP(JOYS_1_RTX), RTX, true);	// bus->debug_write(JOYS_1_RTX, RTX);
				Memory::Write(MAP(JOYS_1_RTY), RTY, true);	// bus->debug_write(JOYS_1_RTY, RTY);
				Memory::Write(MAP(JOYS_1_Z1 ),  Z1, true);	// bus->debug_write(JOYS_1_Z1, Z1);
				Memory::Write(MAP(JOYS_1_Z2 ),  Z2, true);	// bus->debug_write(JOYS_1_Z2, Z2);
			}
			else
			{
				Memory::Write(MAP(JOYS_2_LTX), LTX, true);	// bus->debug_write(JOYS_2_LTX, LTX);
				Memory::Write(MAP(JOYS_2_LTY), LTY, true);	// bus->debug_write(JOYS_2_LTY, LTY);
				Memory::Write(MAP(JOYS_2_RTX), RTX, true);	// bus->debug_write(JOYS_2_RTX, RTX);
				Memory::Write(MAP(JOYS_2_RTY), RTY, true);	// bus->debug_write(JOYS_2_RTY, RTY);
				Memory::Write(MAP(JOYS_2_Z1 ),  Z1, true);	// bus->debug_write(JOYS_2_Z1, Z1);
				Memory::Write(MAP(JOYS_2_Z2 ),  Z2, true);	// bus->debug_write(JOYS_2_Z2, Z2);
			}
		}
	}
}

void Joystick::EncodeConditionFlags(int id)
{
	id %= 2;
    Word joys_flags = 0;
    if (id == 0)
        joys_flags = MAP(JOYS_1_FLAGS);
    else
        joys_flags = MAP(JOYS_2_FLAGS);

    // clear the bits (nothing connected)
    Byte data = 0; 
    SDL_GamepadType type = (SDL_GamepadType)SDL_GetGamepadType(state[id].gamepad);

    // Gamepad or Joystick
	if (state[id].gamepad != nullptr) {
        data |= 0x40;   // is gamepad
        data |= (type & 0x0f);
    } 
    if (state[id].joystick != nullptr) {
        data |= 0x80;   // is joystick
        data |= (type & 0x0f);
    }
    // update the flags
    Memory::Write(joys_flags, data, true);
}

// END: Joystick.cpp
