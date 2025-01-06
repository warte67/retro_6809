/*** Tests.cpp ******************************************
 *      _____       _                         
 *     |_   _|__ __| |_ ___      __ _ __ _ __ 
 *       | |/ -_|_-<  _(_-<  _  / _| '_ \ '_ \
 *       |_|\___/__/\__/__/ (_) \__| .__/ .__/
 *                                 |_|  |_|   
 *  
 *********************************/

// #include "Bus.hpp"
#include "GPU.hpp"
#include "Memory.hpp"
#include "Tests.hpp"


/****************
 * Read / Write *
 ***************/ 

Byte Tests::OnRead(Word offset) 
{ 
    Byte data = IDevice::OnRead(offset);

    if (offset == MAP(TESTS_ONE))        { data = _tests_one;  }
    else if (offset == MAP(TESTS_TWO))   { data = _tests_two;  }

    return data; 
} // END: Tests::OnRead()

void Tests::OnWrite(Word offset, Byte data) 
{ 
    if (offset == MAP(TESTS_ONE))       { _tests_one = data; }
    else if (offset == MAP(TESTS_TWO))  { _tests_two = data; } 

    IDevice::OnWrite( offset, data);
} // END: Tests::OnWrite()



/***************************
* Constructor / Destructor *
***************************/

Tests::Tests() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Tests Created" << clr::RETURN;
    _device_name = "TESTS_DEVICE"; 
    _size = 2;
} // END: Tests()

Tests::~Tests() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Tests Destroyed" << clr::RETURN; 
} // END: ~Tests()



/******************
* Virtual Methods *
******************/


int  Tests::OnAttach(int nextAddr)
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnAttach() Entry" << clr::RETURN;

    Word old_address=nextAddr;
    this->heading = "TESTS_DEVICE";
    register_node new_node;
    new_node = { "TESTS_START", nextAddr,  {    "Start of Test Registers Space" } };
    mapped_register.push_back(new_node);

    new_node = { "TESTS_ONE", nextAddr,  { "(Byte) First Test Register",
                                         "- bit 7    = TEST_ENABLE",
                                         "- bit 6    = (reserved)",
                                         "- bit 5    = (reserved)",
                                         "- bit 4    = (reserved)",
                                         "- bit 3    = (reserved)",
                                         "- bit 2    = (reserved)",
                                         "- bit 1    = (reserved)",
                                         "- bit 0    = TEST_INC_VID_MODES",
                                         ""} }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "TESTS_TWO", nextAddr,  { "(Byte) Second Test Register",
                                         "- bits 0-7  = (reserved)",
                                         ""} }; nextAddr+=1;
    mapped_register.push_back(new_node);

    new_node = { "TESTS_END", nextAddr,  { "End of Tests Registers Space"} }; nextAddr+=1;
    mapped_register.push_back(new_node);
    new_node = { "TESTS_TOP", nextAddr,  { "Top of Tests Registers Space","---"} };
    mapped_register.push_back(new_node);

    _size = nextAddr - old_address;    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnAttach() Exit" << clr::RETURN;

    return _size;   // return the size of the allocation
}


void Tests::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnInit() Exit" << clr::RETURN;
}

void Tests::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnQuit() Exit" << clr::RETURN;
}


void Tests::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnActivate() Exit" << clr::RETURN;
}


void Tests::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnDeactivate() Exit" << clr::RETURN;
}

void Tests::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnEvent() Exit" << clr::RETURN;
}


void Tests::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnUpdate() Entry" << clr::RETURN;

    static float delta_time = fElapsedTime;
    static float running_time = fElapsedTime;
    if (running_time > delta_time + 0.125f) {
        delta_time = fElapsedTime;
        running_time = fElapsedTime;

        // TESTS ENABLED?
        if ( (_tests_one & TestFlags::TEST_ENABLE) )
        {

            // (bit 0) video mode changes 
            if ( (_tests_one & TestFlags::TEST_INC_VID_MODES) )
            {   
                static Word video_mode = 0x0180;    
                video_mode++;
                if (video_mode & 0b0001'1110'0000'0000) video_mode += 0b0001'1110'0000'0000;
                Word mode = video_mode | 0b1001'0001'0000'0000;
                //mode |= 0b1000'1000'0000'0000;   // force full screen
                Memory::Write_Word(MAP(GPU_OPTIONS), mode);
                // std::cout << "Tests::TEST_INC_VID_MODES:: (0x" << clr::hex(video_mode,4) << ") Video Mode: " << clr::hex(mode,4) << std::endl;
            }

            // increment foreground
            if ( (_tests_one & TestFlags::TEST_ANIM_FG) )
            {   
                static Byte data = 0;
                static Byte color = 0x00;
                for (int i=MAP(VIDEO_START); i<MAP(VIDEO_TOP); i+=2) { 
                    Memory::Write(i+0, color++);  
                    Memory::Write(i+1, data++);  
                }
                color--;
                data++;
            }
        }
    }
    running_time += fElapsedTime;

    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnUpdate() Exit" << clr::RETURN;
}


void Tests::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnRender() Exit" << clr::RETURN;
}



// END: Tests.cpp
