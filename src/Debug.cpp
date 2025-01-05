/*** Device.cpp *******************************************
 *      _____       _                                   
 *     |  __ \     | |                                  
 *     | |  | | ___| |__  _   _  __ _   ___ _ __  _ __  
 *     | |  | |/ _ \ '_ \| | | |/ _` | / __| '_ \| '_ \ 
 *     | |__| |  __/ |_) | |_| | (_| || (__| |_) | |_) |
 *     |_____/ \___|_.__/ \__,_|\__, (_)___| .__/| .__/ 
 *                               __/ |     | |   | |    
 *                              |___/      |_|   |_| 
 *  
 ************************************/

#include "Debug.hpp"
#include "Memory.hpp"



/****************
 * Read / Write *
 ***************/ 

Byte Debug::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);
    // DBG_BRK_ADDR          = 0xFE05,   // (Word) Address of current breakpoint
    // DBG_FLAGS             = 0xFE07,   // (Byte) Debug Specific Hardware Flags:
    //           - bit 7: Debug Enable
    //           - bit 6: Single Step Enable
    //           - bit 5: Clear All Breakpoints
    //           - bit 4: Update Breakpoint at DEBUG_BRK_ADDR
    //           - bit 3: FIRQ  (on low (0) to high (1) edge)
    //           - bit 2: IRQ   (on low (0) to high (1) edge)
    //           - bit 1: NMI   (on low (0) to high (1) edge)
    //           - bit 0: RESET (on low (0) to high (1) edge)

    if (MAP(DBG_BRK_ADDR) == offset) { 
        data = _dbg_brk_addr; 
    } 
    else if (MAP(DBG_FLAGS) == offset) { 
        data = _dbg_flags; 
    }

    return data; 
} // END: Debug::OnRead()

void Debug::OnWrite(Word offset, Byte data) 
{ 
    IDevice::OnWrite( offset, data);
} // END: Debug::OnWrite()



/***************************
* Constructor / Destructor *
***************************/

Debug::Debug() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Debug Created" << clr::RETURN;
    _device_name = "DEBUG_DEVICE"; 
} // END: Debug()

Debug::~Debug() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Debug Destroyed" << clr::RETURN; 
} // END: ~Debug()



/******************
* Virtual Methods *
******************/



int  Debug::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnAttach() Entry" << clr::RETURN;
    if (nextAddr == 0) { ; } // stop the compiler from complaining
    
    Word old_address=nextAddr;
    this->heading = "Debug Hardware Registers:";
    register_node new_node;
    new_node = { "DBG_BRK_ADDR", nextAddr,  { "(Word) Address of current breakpoint"} }; nextAddr+=2;
    mapped_register.push_back(new_node);

    new_node = { "DBG_FLAGS", nextAddr,  {  "(Byte) Debug Specific Hardware Flags:",
                                            "- bit 7: Debug Enable",
                                            "- bit 6: Single Step Enable",
                                            "- bit 5: Clear All Breakpoints",
                                            "- bit 4: Update Breakpoint at DEBUG_BRK_ADDR",
                                            "- bit 3: FIRQ  (on low {0} to high {1} edge)",
                                            "- bit 2: IRQ   (on low {0} to high {1} edge)",
                                            "- bit 1: NMI   (on low {0} to high {1} edge)",
                                            "- bit 0: RESET (on low {0} to high {1} edge)",
                                            "" } }; // nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "DBG_END", nextAddr,  { "End of Debug Registers"} }; nextAddr++;
    mapped_register.push_back(new_node);

    new_node = { "DBG_TOP", nextAddr,  { "Top of Debug Registers", "---"} }; // nextAddr++;
    mapped_register.push_back(new_node);

    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnAttach() Exit" << clr::RETURN;    
    _size = nextAddr - old_address;
    return _size; 
}



bool Debug::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnInit() Exit" << clr::RETURN;
    return true;
}



bool Debug::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnQuit() Exit" << clr::RETURN;
    return true;
}



bool Debug::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnActivate() Exit" << clr::RETURN;
    return true;
}



bool Debug::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Debug::OnDeactivate() Exit" << clr::RETURN;
    return true;
}


bool Debug::OnEvent(SDL_Event* evnt)
{
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    return true;
}


bool Debug::OnUpdate(float fElapsedTime)
{
    if (fElapsedTime==0.0f) { ; } // stop the compiler from complaining
    // ...
     return true;
}


bool Debug::OnRender()
{
    // ...
    return true;
}



// END: Debug.cpp
