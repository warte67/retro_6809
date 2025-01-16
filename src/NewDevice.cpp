/*** NewDevice.cpp *******************************************
 *      _   _                     _____                   _                     _                     
 *     | \ | |                   |  __ \                 (_)                   | |                    
 *     |  \| |   ___  __      __ | |  | |   ___  __   __  _    ___    ___      | |__    _ __    _ __  
 *     | . ` |  / _ \ \ \ /\ / / | |  | |  / _ \ \ \ / / | |  / __|  / _ \     | '_ \  | '_ \  | '_ \ 
 *     | |\  | |  __/  \ V  V /  | |__| | |  __/  \ V /  | | | (__  |  __/  _  | | | | | |_) | | |_) |
 *     |_| \_|  \___|   \_/\_/   |_____/   \___|   \_/   |_|  \___|  \___| (_) |_| |_| | .__/  | .__/ 
 *                                                                                     | |     | |    
 *                                                                                     |_|     |_|    
 * This file serves meerly as a template as a
 * starting point for new devices.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "NewDevice.hpp"



/***************************
* Constructor / Destructor *
***************************/

NewDevice::NewDevice() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "NewDevice Created" << clr::RETURN;
    _device_name = "NewDevice"; 
} // END: NewDevice()

NewDevice::~NewDevice() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "NewDevice Destroyed" << clr::RETURN; 
} // END: ~NewDevice()



/******************
* Virtual Methods *
******************/



int  NewDevice::OnAttach(int nextAddr)
{
    SetBaseAddress(nextAddr);
    
    // std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnAttach() Entry" << clr::RETURN;    


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
    

    // std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnAttach() Exit" << clr::RETURN;
    return 0;
}



void NewDevice::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnInit() Exit" << clr::RETURN;
}



void NewDevice::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnQuit() Exit" << clr::RETURN;
}



void NewDevice::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnActivate() Exit" << clr::RETURN;
}



void NewDevice::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnDeactivate() Exit" << clr::RETURN;
}


void NewDevice::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnEvent() Entry" << clr::RETURN;
    (void)evnt;     // stop the compiler from complaining about unused parameters
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnEvent() Exit" << clr::RETURN;
}


void NewDevice::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnUpdate() Entry" << clr::RETURN;
    (void)fElapsedTime;     // stop the compiler from complaining about unused parameters
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnUpdate() Exit" << clr::RETURN;
}


void NewDevice::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "NewDevice::OnRender() Exit" << clr::RETURN;
}



// END: NewDevice.cpp
