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


/*****************************************************************
 * virtual int OnAttach(int nextAddr):
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is attached to the memory map.
 * The device is responsible for allocating its own memory space
 * and returning the size of the allocation.
 * 
 * @param nextAddr The current address in the memory map.
 * @return The size of the allocation.
 ********************/
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


/*****************************************************************
 * bool OnInit():
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is initialized.
 * The device is responsible for initializing its internal state
 * and returning true if the initialization was successful.
 * 
 * @return True if the initialization was successful.
 *********************/
bool Tests::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnInit() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnQuit(): 
 * 
 * This is a virtual method that is called by the Memory class.
 * It is called when the device is being shut down.
 * The device is responsible for releasing any allocated memory
 * and any other resources.
 *
 * @return True if the shutdown was successful.
 **********************/
bool Tests::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnQuit() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnActivate():
 * 
 * This is a virtual method that is called to activate the device.
 * It is responsible for preparing the device to be operational,
 * ensuring that all necessary conditions are met for the device to
 * function.
 * 
 * @return True if the device was successfully activated, false
 *          otherwise.
 ***********************/
bool Tests::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnActivate() Exit" << clr::RETURN;
    return true;
}


/******************************************************************
 * virtual bool OnDeactivate():
 *
 * This is a virtual method that is called to deactivate the device.
 * It is responsible for releasing any resources that were allocated
 * when the device was activated, and for ensuring that all necessary
 * conditions are met for the device to be safely deactivated.
 * 
 * @return True if the device was successfully deactivated, false 
 *          otherwise.
 ***********************/
bool Tests::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnDeactivate() Exit" << clr::RETURN;
    return true;
}


/********************************************************************
 * virtual bool OnEvent(SDL_Event* evnt):
 * 
 * This is a virtual method that handles events sent to the device.
 * It processes the given SDL_Event and performs the necessary actions
 * based on the event type. 
 *
 * @param evnt A pointer to an SDL_Event structure that contains 
 * the event data to be processed.
 *
 * @return True if the event was handled successfully, false otherwise.
 ********************************************************************/

bool Tests::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnEvent() Entry" << clr::RETURN;
    if (evnt) { ; } // stop the compiler from complaining
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnEvent() Exit" << clr::RETURN;
    return true;
}


/****************************************************************
 * virtual bool OnUpdate(float fElapsedTime):
 *
 * This is a virtual method that is called periodically to allow the
 * device to update its internal state. The frequency of the calls
 * is determined by the Memory class, and the time since the last
 * call is given in the fElapsedTime parameter.
 *
 * @param fElapsedTime The time, in seconds, that has elapsed since
 *  the last call to OnUpdate.
 *
 * @return True if the update was successful, false otherwise.
 ************************************************************************/
bool Tests::OnUpdate(float fElapsedTime)
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
    return true;
}


bool Tests::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "Tests::OnRender() Exit" << clr::RETURN;
    return true;
}



// END: Tests.cpp
