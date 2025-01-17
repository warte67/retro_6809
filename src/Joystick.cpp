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



int  Joystick::OnAttach(int nextAddr)
{
    // std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnAttach() Entry" << clr::RETURN;    
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "Joystick/Gamepad Controller Device Hardware Registers"; 



    ////////////////////////////////////////////////
    // (Word) JOYS_1_BTN
    //       Button Bits: Room For up to 16 Buttons  (realtime)
    mapped_register.push_back( { "JOYS_1_BTN", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(Word) Button Bits: Room For up to 16 Buttons  (realtime)" }}); nextAddr+=1;   
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
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_LTX
    //       PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_LTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_LTY
    //       PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_LTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_RTX
    //       PAD 1 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_RTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_RTY
    //       PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_1_RTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_Z1
    //       PAD 1 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_1_Z1", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_1_Z2
    //       PAD 1 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_1_Z2", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 1 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (Word) JOYS_2_BTN
    //       Button Bits: Room For up to 16 Buttons  (realtime)
    mapped_register.push_back( { "JOYS_2_BTN", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(Word) Button Bits: Room For up to 16 Buttons  (realtime)" }}); nextAddr+=1;   
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
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_LTX
    //       PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_LTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_LTY
    //       PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_LTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_RTX
    //       PAD 2 LThumb-X position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_RTX", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_RTY
    //       PAD 1 RThumb-Y position (-128 _ +127)   (realtime)
    mapped_register.push_back( { "JOYS_2_RTY", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_Z1
    //       PAD 1 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_2_Z1", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


    ////////////////////////////////////////////////
    // (char) JOYS_2_Z2
    //       PAD 2 left analog trigger (0 - 127)     (realtime)
    mapped_register.push_back( { "JOYS_2_Z2", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {   "(char) PAD 2 left analog trigger (0 - 127)     (realtime)" }}); nextAddr+=1;   
    mapped_register.push_back( { "", nextAddr,
        [this](Word nextAddr) { return IDevice::memory(nextAddr); }, 
        [this](Word nextAddr, Word data) { IDevice::memory(nextAddr, data); },
        {"" }}); nextAddr+=1;   


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
    
    // std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Exit" << clr::RETURN;
    return nextAddr - old_address;
   

    // std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnAttach() Exit" << clr::RETURN;
    return nextAddr - old_address;
}



void Joystick::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnInit() Entry" << clr::RETURN;
    
    // InitButtonStates(); 

    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnInit() Exit" << clr::RETURN;
}



void Joystick::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnQuit() Exit" << clr::RETURN;
}



void Joystick::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnActivate() Entry" << clr::RETURN;
	// if (!bJoystickWasInit)
	// {
	// 	int ret = SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
	// 	if (ret < 0)
	// 	{
	// 		std::string msg = "SDL could not initialize Joysticks and/or joysticks! SDL_Error: ";
	// 		msg += SDL_GetError();
	// 		Bus::Error(msg.c_str());
	// 		return;
	// 	}
	// 	// set the default analog dead band (dead zone)
	// 	Bus::Write(JOYS_1_DBND, 5, true);
	// 	Bus::Write(JOYS_2_DBND, 5, true);
	// 	OpenControllers();
	// 	bJoystickWasInit = true;
	// }   
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnActivate() Exit" << clr::RETURN;
}



void Joystick::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnDeactivate() Entry" << clr::RETURN;

	// if (bJoystickWasInit)
	// {
	// 	CloseControllers();
	// 	SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
	// 	bJoystickWasInit = false;
	// }   

    std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnDeactivate() Exit" << clr::RETURN;
}


void Joystick::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnEvent() Entry" << clr::RETURN;

    // switch (evnt->type)
    // {
    //     case SDL_CONTROLLERDEVICEADDED:
    //     case SDL_CONTROLLERDEVICEREMOVED:
    //     case SDL_CONTROLLERDEVICEREMAPPED:
    //     case SDL_JOYDEVICEADDED:
    //     case SDL_JOYDEVICEREMOVED:
    //         CloseControllers();
    //         OpenControllers();
    //         break;
    // }    

    //std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnEvent() Exit" << clr::RETURN;
}


void Joystick::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnUpdate() Entry" << clr::RETURN;
    (void)fElapsedTime;     // stop the compiler from complaining about unused parameters

    // // update button registers
	// if (state[0].bIsActive)
	// 	Bus::Write_Word(JOYS_1_BTN, EncodeButtonRegister(0), true);
	// else
	// 	Bus::Write_Word(JOYS_1_BTN, 0xffff, true);
	// if (state[1].bIsActive)
	// 	Bus::Write_Word(JOYS_2_BTN, EncodeButtonRegister(1), true);
	// else
	// 	Bus::Write_Word(JOYS_2_BTN, 0xffff, true);
	// EncodeAxesRegister(0);
	// EncodeAxesRegister(1);

    //std::cout << clr::indent() << clr::LT_BLUE << "Joystick::OnUpdate() Exit" << clr::RETURN;
}



// void Joystick::InitButtonStates()
// {
// 	Byte btn = 0;
// 	gpadBtnMap.clear();
// 	joysBtnMap.clear();

// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_A] = btn++;			// GPAD_BTN::BTN_A
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_B] = btn++;			// GPAD_BTN::BTN_B
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_X] = btn++;			// GPAD_BTN::BTN_X
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_Y] = btn++;			// GPAD_BTN::BTN_Y
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_BACK] = btn++;			// GPAD_BTN::BTN_BACK
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_START] = btn++;		// GPAD_BTN::BTN_START
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = btn++;	// GPAD_BTN::BTN_LS
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = btn++;	// GPAD_BTN::BTN_RS
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_UP] = btn++;		// GPAD_BTN::BTN_DPAD_UP
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = btn++;	// GPAD_BTN::BTN_DPAD_DOWN
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = btn++;	// GPAD_BTN::BTN_DPAD_LEFT
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = btn++;	// GPAD_BTN::BTN_DPAD_RIGHT
// 	gpadBtnMap[SDL_CONTROLLER_BUTTON_GUIDE] = btn++;		// GPAD_BTN::BTN_GUIDE

// 	btn = 0;
// 	joysBtnMap[JOYS_BTN::BTN_1] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_2] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_3] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_4] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_5] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_6] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_7] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_8] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_HAT_UP] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_HAT_DOWN] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_HAT_LEFT] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_HAT_RIGHT] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_9] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_10] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_11] = btn++;
// 	joysBtnMap[JOYS_BTN::BTN_12] = btn++;
// }


// void Joystick::OpenControllers()
// {
// 	int num_joysticks = SDL_NumJoysticks();
// 	for (int index = 0; index < num_joysticks; index++)
// 	{
// 		if (SDL_IsGameController(index)) // is a Joystick
// 		{
// 			state[index].bIsJoystick = true;
// 			state[index].name = SDL_GameControllerNameForIndex(index);
// 			state[index].controller = SDL_GameControllerOpen(index);
// 			if (state[index].controller == nullptr)
// 			{
// 				std::string msg = "SDL could not open a Joystick! SDL_Error: ";
// 				msg += SDL_GetError();
// 				Bus::Error(msg.c_str());
// 				return;
// 			}
// 			state[index].bIsActive = true;
// 		}
// 		else // is a joystick
// 		{
// 			state[index].bIsJoystick = false;
// 			state[index].joystick = SDL_JoystickOpen(index);
// 			if (state[index].joystick == nullptr)
// 			{
// 				std::string msg = "SDL could not open a joystick! SDL_Error: ";
// 				msg += SDL_GetError();
// 				Bus::Error(msg.c_str());
// 				return;
// 			}
// 			state[index].name = SDL_JoystickName(state[index].joystick);
// 			state[index].bIsActive = true;
// 		}
// 	}
// }

// void Joystick::CloseControllers()
// {
// 	state[0].bIsActive = false;
// 	Bus::Write_Word(JOYS_1_BTN, 0xffff, true);	// bus->debug_write_word(JOYS_1_BTN, 0xffff);
// 	state[1].bIsActive = false;
// 	Bus::Write_Word(JOYS_2_BTN, 0xffff, true);	// bus->debug_write_word(JOYS_2_BTN, 0xffff);
// }


// Word Joystick::EncodeButtonRegister(int id)
// {
// 	id %= 2;
// 	Word ret = 0xffff;
// 	if (state[id].bIsActive)
// 	{
// 		ret = 0;
// 		// encode only the first 16 buttons
// 		for (int btn = 0; btn < 16; btn++)
// 		{
// 			// Joystick?
// 			if (state[id].bIsJoystick)
// 			{
// 				if (SDL_GameControllerGetButton(state[id].controller, (SDL_GameControllerButton)btn))
// 					ret |= (1 << gpadBtnMap[btn]);
// 			}
// 			else // joystick
// 			{
// 				if (SDL_JoystickGetButton(state[id].joystick, btn))
// 					ret |= (1 << joysBtnMap[btn]);
// 				// encode hat
// 				Byte hat = SDL_JoystickGetHat(state[id].joystick, 0);
// 				if (hat == SDL_HAT_UP || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_LEFTUP)
// 					ret |= 0x0100;
// 				if (hat == SDL_HAT_DOWN || hat == SDL_HAT_RIGHTDOWN || hat == SDL_HAT_LEFTDOWN)
// 					ret |= 0x0200;
// 				if (hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN)
// 					ret |= 0x0400;
// 				if (hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN)
// 					ret |= 0x0800;
// 			}
// 		}
// 	}
// 	return ret;
// }


// void Joystick::EncodeAxesRegister(int id)
// {
// 	id %= 2;
// 	if (state[id].bIsActive)
// 	{
// 		Sint8 deadband = read(JOYS_1_DBND);
// 		if (id == 1)
// 			deadband = read(JOYS_2_DBND);

// 		if (state[id].bIsJoystick)	// is Joystick
// 		{
// 			Sint8 LTX = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_LEFTX) & 0xff00) >> 8;
// 			if (LTX < 0 && LTX > -deadband)	LTX = 0;
// 			if (LTX > 0 && LTX < deadband)	LTX = 0;
// 			Sint8 LTY = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_LEFTY) & 0xff00) >> 8;
// 			if (LTY < 0 && LTY > -deadband)	LTY = 0;
// 			if (LTY > 0 && LTY < deadband)	LTY = 0;
// 			Sint8 RTX = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_RIGHTX) & 0xff00) >> 8;
// 			if (RTX < 0 && RTX > -deadband)	RTX = 0;
// 			if (RTX > 0 && RTX < deadband)	RTX = 0;
// 			Sint8 RTY = (SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_RIGHTY) & 0xff00) >> 8;
// 			if (RTY < 0 && RTY > -deadband)	RTY = 0;
// 			if (RTY > 0 && RTY < deadband)	RTY = 0;
// 			Sint8 Z1 = SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) >> 8;
// 			Sint8 Z2 = SDL_GameControllerGetAxis(state[id].controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) >> 8;

// 			if (id == 0)
// 			{
// 				Bus::Write(JOYS_1_LTX, LTX, true);	// bus->debug_write(JOYS_1_LTX, LTX);
// 				Bus::Write(JOYS_1_LTY, LTY, true);	// bus->debug_write(JOYS_1_LTY, LTY);
// 				Bus::Write(JOYS_1_RTX, RTX, true);	// bus->debug_write(JOYS_1_RTX, RTX);
// 				Bus::Write(JOYS_1_RTY, RTY, true);	// bus->debug_write(JOYS_1_RTY, RTY);
// 				Bus::Write(JOYS_1_Z1,   Z1, true);	// bus->debug_write(JOYS_1_Z1, Z1);
// 				Bus::Write(JOYS_1_Z2,   Z2, true);	// bus->debug_write(JOYS_1_Z2, Z2);
// 			}
// 			else
// 			{
// 				Bus::Write(JOYS_2_LTX, LTX, true);	// bus->debug_write(JOYS_2_LTX, LTX);
// 				Bus::Write(JOYS_2_LTY, LTY, true);	// bus->debug_write(JOYS_2_LTY, LTY);
// 				Bus::Write(JOYS_2_RTX, RTX, true);	// bus->debug_write(JOYS_2_RTX, RTX);
// 				Bus::Write(JOYS_2_RTY, RTY, true);	// bus->debug_write(JOYS_2_RTY, RTY);
// 				Bus::Write(JOYS_2_Z1,   Z1, true);	// bus->debug_write(JOYS_2_Z1, Z1);
// 				Bus::Write(JOYS_2_Z2,   Z2, true);	// bus->debug_write(JOYS_2_Z2, Z2);
// 			}
// 		}
// 		else  // is joystick
// 		{
// 			Sint8 LTX = (SDL_JoystickGetAxis(state[id].joystick, 0) & 0xff00) >> 8;
// 			if (LTX < 0 && LTX > -deadband)	LTX = 0;
// 			if (LTX > 0 && LTX < deadband)	LTX = 0;
// 			Sint8 LTY = (SDL_JoystickGetAxis(state[id].joystick, 1) & 0xff00) >> 8;
// 			if (LTY < 0 && LTY > -deadband)	LTY = 0;
// 			if (LTY > 0 && LTY < deadband)	LTY = 0;
// 			// RTX maps the hat
// 			Byte hat = SDL_JoystickGetHat(state[id].joystick, 0);
// 			Sint8 RTX = 0;
// 			Sint8 RTY = 0;
// 			if (hat == SDL_HAT_UP || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_LEFTUP)
// 				RTY = -128;
// 			if (hat == SDL_HAT_DOWN || hat == SDL_HAT_RIGHTDOWN || hat == SDL_HAT_LEFTDOWN)
// 				RTY = 127;
// 			if (hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN)
// 				RTX = -128;
// 			if (hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN)
// 				RTX = 127;
// 			// throttle lever
// 			Uint8 Z1 = SDL_JoystickGetAxis(state[id].joystick, 3) >> 8;
// 			Z1 = 255 - (Z1 + 128);

// 			// rudder axis
// 			Sint8 Z2 = SDL_JoystickGetAxis(state[id].joystick, 2) >> 8;
// 			if (Z2 < 0 && Z2 > -deadband * 3)	Z2 = 0;
// 			if (Z2 > 0 && Z2 < deadband * 3)	Z2 = 0;

// 			if (id == 0)
// 			{
// 				Bus::Write(JOYS_1_LTX, LTX, true);	// bus->debug_write(JOYS_1_LTX, LTX);
// 				Bus::Write(JOYS_1_LTY, LTY, true);	// bus->debug_write(JOYS_1_LTY, LTY);
// 				Bus::Write(JOYS_1_RTX, RTX, true);	// bus->debug_write(JOYS_1_RTX, RTX);
// 				Bus::Write(JOYS_1_RTY, RTY, true);	// bus->debug_write(JOYS_1_RTY, RTY);
// 				Bus::Write(JOYS_1_Z1,   Z1, true);	// bus->debug_write(JOYS_1_Z1, Z1);
// 				Bus::Write(JOYS_1_Z2,   Z2, true);	// bus->debug_write(JOYS_1_Z2, Z2);
// 			}
// 			else
// 			{
// 				Bus::Write(JOYS_2_LTX, LTX, true);	// bus->debug_write(JOYS_2_LTX, LTX);
// 				Bus::Write(JOYS_2_LTY, LTY, true);	// bus->debug_write(JOYS_2_LTY, LTY);
// 				Bus::Write(JOYS_2_RTX, RTX, true);	// bus->debug_write(JOYS_2_RTX, RTX);
// 				Bus::Write(JOYS_2_RTY, RTY, true);	// bus->debug_write(JOYS_2_RTY, RTY);
// 				Bus::Write(JOYS_2_Z1, Z1, true);	// bus->debug_write(JOYS_2_Z1, Z1);
// 				Bus::Write(JOYS_2_Z2, Z2, true);	// bus->debug_write(JOYS_2_Z2, Z2);
// 			}
// 		}
// 	}
// }

// END: Joystick.cpp
