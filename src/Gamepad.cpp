/*** Gamepad.cpp *******************************************
 *       _____                                      _                        
 *      / ____|                                    | |                       
 *     | |  __  __ _ _ __ ___   ___ _ __   __ _  __| |       ___ _ __  _ __  
 *     | | |_ |/ _` | '_ ` _ \ / _ \ '_ \ / _` |/ _` |      / __| '_ \| '_ \ 
 *     | |__| | (_| | | | | | |  __/ |_) | (_| | (_| |  _  | (__| |_) | |_) |
 *      \_____|\__,_|_| |_| |_|\___| .__/ \__,_|\__,_| (_)  \___| .__/| .__/ 
 *                                 | |                          | |   | |    
 *                                 |_|                          |_|   |_|   
 *     
 *      Supports Gamepad Controller input as well as most Joystick Controllers.
 * 
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "Gamepad.hpp"



/***************************
* Constructor / Destructor *
***************************/

Gamepad::Gamepad() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Gamepad Created" << clr::RETURN;
    _device_name = "Gamepad"; 
} // END: Gamepad()

Gamepad::~Gamepad() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Gamepad Destroyed" << clr::RETURN; 
} // END: ~Gamepad()



/******************
* Virtual Methods *
******************/



int  Gamepad::OnAttach(int nextAddr)
{
    SetBaseAddress(nextAddr);
    
    // std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnAttach() Entry" << clr::RETURN;    


    // *** EXAMPLE: ************************************************************************
    // 
    // // std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Entry" << clr::RETURN;
    // 
    // // SetBaseAddress(nextAddr);
    // // Word old_address=nextAddr;
    // // this->heading = "GPU Device Hardware Registers";
    // 
    // ...
    //
    // ////////////////////////////////////////////////
    // // (Word) GPU_VIDEO_MAX
    // //       Video Buffer Maximum (Read Only)
    // /////
    // mapped_register.push_back( { "GPU_VIDEO_MAX", nextAddr,
    //     [this](Word nextAddr) { (void)nextAddr; return _gpu_video_max >> 8; }, 
    //     nullptr, {   
    //         "(Word) Video Buffer Maximum (Read Only)",
    //         " Note: This will change to reflect",
    //         "       the size of the last cpu",
    //         "       accessible memory location",
    //         "       of the currently active",
    //         "       standard video mode.",""
    //     }}); nextAddr+=1;
    // mapped_register.push_back( { "", nextAddr,
    //     [this](Word nextAddr) { (void)nextAddr; return _gpu_video_max & 0xFF; }, 
    //     nullptr, {""}}); nextAddr+=1;
    //
    // ...
    //
    // ////////////////////////////////////////////////
    // // (Constant) GPU_END
    // //      End of GPU Register Space
    // /////
    // nextAddr--;
    // mapped_register.push_back({ "GPU_END", nextAddr, 
    //     nullptr, nullptr,  { "End of GPU Register Space"} });
    // nextAddr++;
    //
    //
    // ////////////////////////////////////////////////
    // // (Constant) GPU_TOP
    // //      Top of GPU Register Space
    // //      (start of the next device)
    // /////
    // mapped_register.push_back({ "GPU_TOP", nextAddr, 
    // nullptr, nullptr,  { "Top of GPU Register Space", "---"}});
    // 
    // // std::cout << clr::indent() << clr::CYAN << "GPU::OnAttach() Exit" << clr::RETURN;
    // return nextAddr - old_address;
    //
    // *** END EXAMPLE ************************************************************************
    

    // std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnAttach() Exit" << clr::RETURN;
    return 0;
}



void Gamepad::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnInit() Exit" << clr::RETURN;
}



void Gamepad::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnQuit() Exit" << clr::RETURN;
}



void Gamepad::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnActivate() Exit" << clr::RETURN;
}



void Gamepad::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnDeactivate() Exit" << clr::RETURN;
}


void Gamepad::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnEvent() Entry" << clr::RETURN;
    (void)evnt;     // stop the compiler from complaining about unused parameters
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnEvent() Exit" << clr::RETURN;
}


void Gamepad::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnUpdate() Entry" << clr::RETURN;
    (void)fElapsedTime;     // stop the compiler from complaining about unused parameters
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnUpdate() Exit" << clr::RETURN;
}


void Gamepad::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Gamepad::OnRender() Exit" << clr::RETURN;
}



// END: Gamepad.cpp
