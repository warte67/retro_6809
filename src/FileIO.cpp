/*** FileIO.cpp *******************************************
 *      ______ _ _      _____ ____         _____             
 *     |  ____(_) |    |_   _/ __ \       / ____|            
 *     | |__   _| | ___  | || |  | |     | |     _ __  _ __  
 *     |  __| | | |/ _ \ | || |  | |     | |    | '_ \| '_ \ 
 *     | |    | | |  __/_| || |__| |  _  | |____| |_) | |_) |
 *     |_|    |_|_|\___|_____\____/  (_)  \_____| .__/| .__/ 
 *                                              | |   | |    
 *                                              |_|   |_|    
 * 
 *   A filesystem that acts to emulate a Raspberry PI Pico
 *		Implementing File Input / Output with the onboad 
 *      flash ram, USB storage and the SDCARD.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#include "FileIO.hpp"



/***************************
* Constructor / Destructor *
***************************/

FileIO::FileIO() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "FileIO Device Created" << clr::RETURN;
    _device_name = "FIO_DEVICE"; 
} // END: FileIO()

FileIO::~FileIO() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "FileIO Device Destroyed" << clr::RETURN; 
} // END: ~FileIO()



/******************
* Virtual Methods *
******************/



int  FileIO::OnAttach(int nextAddr)
{
    // std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnAttach() Entry" << clr::RETURN;    

    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "File I/O Device Hardware Registers";


    ////////////////////////////////////////////////
    // (Byte) FIO_ERROR
    //      End of GPU Register Space
    /////
    mapped_register.push_back({ "FIO_ERROR", nextAddr, 
        nullptr, nullptr,  { "(Byte) FILE_ERROR enumeration result (FE_<error>)",""} });
    nextAddr++;
    Byte enumID = 0;
    mapped_register.push_back({ "FE_BEGIN"    , enumID  , nullptr, nullptr,  { "  Begin FILE_ERROR enumeration "} });
    mapped_register.push_back({ "FE_NOERROR"  , enumID++, nullptr, nullptr,  { "     no error, condition normal"} });
    mapped_register.push_back({ "FE_NOTFOUND" , enumID++, nullptr, nullptr,  { "     file or folder not found  "} });
    mapped_register.push_back({ "FE_NOTOPEN"  , enumID++, nullptr, nullptr,  { "     file not open             "} });
    mapped_register.push_back({ "FE_EOF"      , enumID++, nullptr, nullptr,  { "     end of file               "} });
    mapped_register.push_back({ "FE_OVERRUN"  , enumID++, nullptr, nullptr,  { "     buffer overrun            "} });
    mapped_register.push_back({ "FE_WRONGTYPE", enumID++, nullptr, nullptr,  { "     wrong file type           "} });
    mapped_register.push_back({ "FE_BAD_CMD"  , enumID++, nullptr, nullptr,  { "     invalid command           "} });
    mapped_register.push_back({ "FE_BADSTREAM", enumID  , nullptr, nullptr,  { "     invalid file stream       "} });
    mapped_register.push_back({ "FE_LAST"     , enumID  , nullptr, nullptr,  { "  End of FILE_ERROR enumeration",""} });

    
    ////////////////////////////////////////////////
    // (Byte) FIO_COMMAND
    //      End of GPU Register Space
    /////
    mapped_register.push_back({ "FIO_COMMAND", nextAddr, 
        nullptr, nullptr,  { "(Byte) Execute a File Command (FC_<cmd>)",""} });
    nextAddr++;
    enumID = 0;
    mapped_register.push_back({ "FC_BEGIN"     , enumID  , nullptr, nullptr,  { "  Begin FIO_COMMAND enumeration           "} });
    mapped_register.push_back({ "FC_RESET"     , enumID++, nullptr, nullptr,  { "    Reset                                 "} });
    mapped_register.push_back({ "FC_SHUTDOWN"  , enumID++, nullptr, nullptr,  { "    SYSTEM: Shutdown                      "} });
    mapped_register.push_back({ "FC_COMPDATE"  , enumID++, nullptr, nullptr,  { "    SYSTEM: Load Compilation Date         "} });
    mapped_register.push_back({ "FC_FILEEXISTS", enumID++, nullptr, nullptr,  { "    Does File Exist (return in FIO_IODATA)"} });
    mapped_register.push_back({ "FC_OPENREAD"  , enumID++, nullptr, nullptr,  { "    Open Binary File For Reading          "} });
    mapped_register.push_back({ "FC_OPENWRITE" , enumID++, nullptr, nullptr,  { "    Open Binary File For Writing          "} });
    mapped_register.push_back({ "FC_OPENAPPEND", enumID++, nullptr, nullptr,  { "    Open Binary File For Appending        "} });
    mapped_register.push_back({ "FC_CLOSEFILE" , enumID++, nullptr, nullptr,  { "    Close File                            "} });
    mapped_register.push_back({ "FC_READBYTE"  , enumID++, nullptr, nullptr,  { "    Read Byte (into FIO_IOBYTE)           "} });
    mapped_register.push_back({ "FC_WRITEBYTE" , enumID++, nullptr, nullptr,  { "    Write Byte (from FIO_IOBYTE)          "} });
    mapped_register.push_back({ "FC_LOADHEX"   , enumID++, nullptr, nullptr,  { "    Load Hex Format File                  "} });
    mapped_register.push_back({ "FC_GETLENGTH" , enumID++, nullptr, nullptr,  { "    Get File Length (into FIO_IOWORD)     "} });
    mapped_register.push_back({ "FC_LISTDIR"   , enumID++, nullptr, nullptr,  { "    List Directory                        "} });
    mapped_register.push_back({ "FC_MAKEDIR"   , enumID++, nullptr, nullptr,  { "    Make Directory                        "} });
    mapped_register.push_back({ "FC_CHANGEDIR" , enumID++, nullptr, nullptr,  { "    Change Directory                      "} });
    mapped_register.push_back({ "FC_GETPATH"   , enumID++, nullptr, nullptr,  { "    Fetch Current Path                    "} });
    mapped_register.push_back({ "FC_REN_DIR"   , enumID++, nullptr, nullptr,  { "    Rename Directory                      "} });
    mapped_register.push_back({ "FC_DEL_DIR"   , enumID++, nullptr, nullptr,  { "    Delete Directory                      "} });
    mapped_register.push_back({ "FC_DEL_FILE"  , enumID++, nullptr, nullptr,  { "    Delete File                           "} });
    mapped_register.push_back({ "FC_REN_FILE"  , enumID++, nullptr, nullptr,  { "    Rename File                           "} });
    mapped_register.push_back({ "FC_COPY_FILE" , enumID++, nullptr, nullptr,  { "    Copy File                             "} });
    mapped_register.push_back({ "FC_SEEK_START", enumID++, nullptr, nullptr,  { "    Seek Start                            "} });
    mapped_register.push_back({ "FC_SEEK_END"  , enumID++, nullptr, nullptr,  { "    Seek End                              "} });
    mapped_register.push_back({ "FC_SET_SEEK"  , enumID++, nullptr, nullptr,  { "    Set Seek Position (from FIO_IOWORD)   "} });
    mapped_register.push_back({ "FC_GET_SEEK"  , enumID  , nullptr, nullptr,  { "    Get Seek Position (into FIO_IOWORD)   "} });
    mapped_register.push_back({ "FC_LAST"      , enumID  , nullptr, nullptr,  { "  End FIO_COMMAND enumeration             ",""} });


    ////////////////////////////////////////////////
    // (Byte) FIO_HANDLE
    //      Current File Stream HANDLE (0=NONE)
    /////
    mapped_register.push_back({ "FIO_HANDLE", nextAddr, 
        nullptr, nullptr,  { "(Byte) Current File Stream HANDLE (0=NONE)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (DWord) FIO_SEEKPOS
    //      File Seek Position
    /////
    mapped_register.push_back({ "FIO_SEEKPOS", nextAddr, 
        nullptr, nullptr,  { "(DWord) File Seek Position"} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        nullptr, nullptr,  {""} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        nullptr, nullptr,  {""} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        nullptr, nullptr,  {""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_IODATA
    //      Input / Output Data
    /////
    mapped_register.push_back({ "FIO_IODATA", nextAddr, 
        nullptr, nullptr,  { "(Byte) Input / Output Data"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_LEN
    //      Length of the Filepath
    /////
    mapped_register.push_back({ "FIO_PATH_LEN", nextAddr, 
        nullptr, nullptr,  { "(Byte) Length of the Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_POS
    //      Character Position Within the Filepath
    /////
    mapped_register.push_back({ "FIO_PATH_POS", nextAddr, 
        nullptr, nullptr,  { "(Byte) Character Position Within the Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_DATA
    //      Data at the Character Position of the Path
    /////
    mapped_register.push_back({ "FIO_PATH_DATA", nextAddr, 
        nullptr, nullptr,  { "(Byte) Data at the Character Position of the Path",""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_DIR_DATA
    //      A Series of Null-Terminated Filenames
    /////
    mapped_register.push_back({ "FIO_DIR_DATA", nextAddr, 
        nullptr, nullptr,  { "(Byte) A Series of Null-Terminated Filenames",
                             "  NOTE: Current read-position is reset to the beginning",
                             "    following a List Directory command. The read-position",
                             "    is automatically advanced on read from this register.",
                             "    Each filename is $0A-terminated. The list itself is",
                             "    null-terminated.",""} }); 
    nextAddr++;


    ////////////////////////////////////////////////
    // (Constant) FIO_END
    //      End of FIO Device Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "FIO_END", nextAddr, 
        nullptr, nullptr,  { "End of FIO Device Register Space"} });
    nextAddr++;
    
    
    ////////////////////////////////////////////////
    // (Constant) FIO_TOP
    //      Top of FIO Device Register Space
    /////
    mapped_register.push_back({ "FIO_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of FIO Device Register Space", "---"}});




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
    //     [this](Word nextAddr, data) { (void)nextAddr; (void)data; /* do something or leave as nullptr for read only */}, {   
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
    

    // std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnAttach() Exit" << clr::RETURN;
    return nextAddr - old_address;
}



void FileIO::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnInit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnInit() Exit" << clr::RETURN;
}



void FileIO::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnQuit() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnQuit() Exit" << clr::RETURN;
}



void FileIO::OnActivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnActivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnActivate() Exit" << clr::RETURN;
}



void FileIO::OnDeactivate()
{
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnDeactivate() Entry" << clr::RETURN;
    // ...
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnDeactivate() Exit" << clr::RETURN;
}


void FileIO::OnEvent(SDL_Event* evnt)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnEvent() Entry" << clr::RETURN;
    (void)evnt;     // stop the compiler from complaining about unused parameters
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnEvent() Exit" << clr::RETURN;
}


void FileIO::OnUpdate(float fElapsedTime)
{
    //std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnUpdate() Entry" << clr::RETURN;
    (void)fElapsedTime;     // stop the compiler from complaining about unused parameters
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnUpdate() Exit" << clr::RETURN;
}


void FileIO::OnRender()
{
    //std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnRender() Entry" << clr::RETURN;
    // ...
    //std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnRender() Exit" << clr::RETURN;
}



// END: FileIO.cpp
