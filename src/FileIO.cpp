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


#include <fstream>
#include <filesystem>

#include "Memory.hpp"
#include "Bus.hpp"
#include "C6809.hpp"
#include "FileIO.hpp"



/***************************
* Constructor / Destructor *
***************************/

/**
 * Default constructor for the FileIO class. Sets the device name to "FIO_DEVICE".
 */
FileIO::FileIO() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "FileIO Device Created" << clr::RETURN;
    _device_name = "FIO_DEVICE"; 
} // END: FileIO()

/**
 * Destructor for the FileIO class. Outputs a message indicating that
 * the FileIO device has been destroyed, and performs any necessary cleanup.
 */
FileIO::~FileIO() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "FileIO Device Destroyed" << clr::RETURN; 
} // END: ~FileIO()



/******************
* Virtual Methods *
******************/



/**
 * Attaches the FileIO device to the system bus and allocates the hardware
 * registers.
 *
 * @param nextAddr The starting address of the device.
 * @return The number of bytes allocated by the device.
 */
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
        [this](Word addr) {
            (void)addr;
            Byte data = fio_error_code;
            fio_error_code = FILE_ERROR::FE_NOERROR;     
            return data;       
        }, 
        [this](Word addr, Byte data) {
            (void)addr;
            fio_error_code = (FILE_ERROR)data;
        },  
        { "(Byte) FILE_ERROR enumeration result (FE_<error>)",""} });
    nextAddr++;
    Byte enumID = 0;
    mapped_register.push_back({ "FE_BEGIN"       , enumID  , nullptr, nullptr,  { "  Begin FILE_ERROR enumeration "} });
    mapped_register.push_back({ "FE_NOERROR"     , enumID++, nullptr, nullptr,  { "     no error, condition normal"} });
    mapped_register.push_back({ "FE_NOTFOUND"    , enumID++, nullptr, nullptr,  { "     file or folder not found  "} });
    mapped_register.push_back({ "FE_NOTOPEN"     , enumID++, nullptr, nullptr,  { "     file not open             "} });
    mapped_register.push_back({ "FE_EOF"         , enumID++, nullptr, nullptr,  { "     end of file               "} });
    mapped_register.push_back({ "FE_OVERRUN"     , enumID++, nullptr, nullptr,  { "     buffer overrun            "} });
    mapped_register.push_back({ "FE_WRONGTYPE"   , enumID++, nullptr, nullptr,  { "     wrong file type           "} });
    mapped_register.push_back({ "FE_BAD_CMD"     , enumID++, nullptr, nullptr,  { "     invalid command           "} });
    mapped_register.push_back({ "FE_BADSTREAM"   , enumID  , nullptr, nullptr,  { "     invalid file stream       "} });
    mapped_register.push_back({ "FE_NOT_EMPTY"   , enumID  , nullptr, nullptr,  { "     directory not empty       "} });
    mapped_register.push_back({ "FE_FILE_EXISTS" , enumID  , nullptr, nullptr,  { "     file already exists       "} });
    mapped_register.push_back({ "FE_INVALID_NAME", enumID  , nullptr, nullptr,  { "     invalid file name         "} });
    mapped_register.push_back({ "FE_LAST"        , enumID  , nullptr, nullptr,  { "  End of FILE_ERROR enumeration",""} });


    ////////////////////////////////////////////////
    // (Byte) FIO_COMMAND
    //      End of GPU Register Space
    /////
    mapped_register.push_back({ "FIO_COMMAND", nextAddr, 
        [this](Word addr) { (void)addr; return Memory::Read(addr, true); }, 
        [this](Word addr, Byte data) {
            (void)addr;
                 if ( data == MAP(FC_RESET     ) )  { _cmd_reset();                      }
            else if ( data == MAP(FC_SHUTDOWN  ) )  { _cmd_system_shutdown();            }
            else if ( data == MAP(FC_COMPDATE  ) )  { _cmd_system_load_comilation_date();}
            else if ( data == MAP(FC_FILEEXISTS) )  { _cmd_does_file_exist();            }
            else if ( data == MAP(FC_OPENREAD  ) )  { _cmd_open_read();                  }
            else if ( data == MAP(FC_OPENWRITE ) )  { _cmd_open_write();                 }
            else if ( data == MAP(FC_OPENAPPEND) )  { _cmd_open_append();                }
            else if ( data == MAP(FC_CLOSEFILE ) )  { _cmd_close_file();                 }
            else if ( data == MAP(FC_READBYTE  ) )  { _cmd_read_byte();                  }
            else if ( data == MAP(FC_WRITEBYTE ) )  { _cmd_write_byte();                 }
            else if ( data == MAP(FC_LOADHEX   ) )  { _cmd_load_hex_file();              }
            else if ( data == MAP(FC_GETLENGTH ) )  { _cmd_get_file_length();            }
            else if ( data == MAP(FC_LISTDIR   ) )  { _cmd_list_directory();             }
            else if ( data == MAP(FC_MAKEDIR   ) )  { _cmd_make_directory();             }
            else if ( data == MAP(FC_CHANGEDIR ) )  { _cmd_change_directory();           }
            else if ( data == MAP(FC_GETPATH   ) )  { _cmd_get_current_path();           }
            else if ( data == MAP(FC_REN_DIR   ) )  { _cmd_rename_directory();           }
            else if ( data == MAP(FC_DEL_DIR   ) )  { _cmd_remove_directory();           }
            else if ( data == MAP(FC_DEL_FILE  ) )  { _cmd_delete_file();                }
            else if ( data == MAP(FC_REN_FILE  ) )  { _cmd_rename_file();                }
            else if ( data == MAP(FC_COPY_FILE ) )  { _cmd_copy_file();                  }
            else if ( data == MAP(FC_SEEK_START) )  { _cmd_seek_start();                 }
            else if ( data == MAP(FC_SEEK_END  ) )  { _cmd_seek_end();                   }
            else if ( data == MAP(FC_SET_SEEK  ) )  { _cmd_set_seek_position();          }
            else if ( data == MAP(FC_GET_SEEK  ) )  { _cmd_get_seek_position();          }
        },   
        { "(Byte) Execute a File Command (FC_<cmd>)",""} });
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
        [this](Word addr) { (void)addr; return _fileHandle; }, 
        [this](Word addr, Byte data) { (void)addr; _fileHandle = data; },  
        { "(Byte) Current File Stream HANDLE (0=NONE)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (DWord) FIO_SEEKPOS
    //      File Seek Position
    /////
    mapped_register.push_back({ "FIO_SEEKPOS", nextAddr, 
        [this](Word addr) { (void)addr; return (_seek_pos>>24) & 0xFF; }, 
        [this](Word addr, Byte data) { 
            (void)addr; _seek_pos = (_seek_pos & 0xFFFFFF00) | ((data & 0xFF) << 24); 
        },   
        { "(DWord) File Seek Position"} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word addr) { (void)addr; return (_seek_pos>>16) & 0xFF; }, 
        [this](Word addr, Byte data) { 
            (void)addr; _seek_pos = (_seek_pos & 0xFFFF00FF) | ((data & 0xFF) << 16);
        },   
        {""} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word addr) { (void)addr; return (_seek_pos>> 8) & 0xFF; }, 
        [this](Word addr, Byte data) { 
            (void)addr; _seek_pos = (_seek_pos & 0xFF00FFFF) | ((data & 0xFF) << 8);
        },   
        {""} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word addr) { (void)addr; return (_seek_pos>> 0) & 0xFF;  }, 
        [this](Word addr, Byte data) { 
            (void)addr; _seek_pos = (_seek_pos & 0x00FFFFFF) | ((data & 0xFF) << 0);
        },   
        {""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_IODATA
    //      Input / Output Data
    /////
    mapped_register.push_back({ "FIO_IODATA", nextAddr, 
        [this](Word addr) { (void)addr; return _io_data; }, 
        [this](Word addr, Byte data) { (void)addr; _io_data = data; },  
        { "(Byte) Input / Output Data",""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_LEN
    //      Length of the Filepath (Read Only)
    /////
    mapped_register.push_back({ "FIO_PATH_LEN", nextAddr, 
        [this](Word addr) { (void)addr; return filePath.size(); }, 
        nullptr, // Read Only 
        { "(Byte) Length of the Primary Filepath        (Read Only)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_POS
    //      Character Position Within the Filepath
    /////
    mapped_register.push_back({ "FIO_PATH_POS", nextAddr, 
        [this](Word addr) { 
            (void)addr; 
            Byte data = path_char_pos;
            if (data >= filePath.size())
                data = filePath.size() - 1;
            if (filePath.size() == 0)
                data = 0;
            return data; 
        }, 
        [this](Word addr, Byte data) { 
            (void)addr; 
            if (data > filePath.size())
                data = filePath.size() - 1;
            path_char_pos = data;
        },  
        { "(Byte) Character Position Within the Primary Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_DATA
    //      Data at the Character Position of the Primary Path
    /////
    mapped_register.push_back({ "FIO_PATH_DATA", nextAddr, 
        [this](Word addr) { 
            (void)addr;
            Byte data = 0;
            if (filePath.size() > 0)
            {
                if (path_char_pos < filePath.size())
                {
                    data = (Byte)filePath.substr(path_char_pos, 1).at(0);
                    if (path_char_pos <= filePath.size())
                        path_char_pos++;
                }
                else
                {
                    filePath = "";
                    data = 0;
                }
            }
            return data; 
        }, 
        [this](Word addr, Byte data) { 
            (void)addr; 
            if (path_char_pos == 0)
            {
                filePath = "";
                path_char_pos = 0;
                filePath += data;
                path_char_pos++;
            }
            else
            {
                filePath = filePath.substr(0, path_char_pos);
                filePath += data;
                path_char_pos++;
            }
        },  
        { "(Byte) Data at the Character Position of the Primary Path",""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_ALT_PATH_LEN
    //      Length of the Alternate Filepath (Read Only)
    /////
    mapped_register.push_back({ "FIO_ALT_PATH_LEN", nextAddr, 
        [this](Word addr) { (void)addr; return altFilePath.size(); }, 
        nullptr, // Read Only 
        { "(Byte) Length of the alternate Filepath        (Read Only)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_ALT_PATH_POS
    //      Character Position Within the Alternate Filepath
    /////
    mapped_register.push_back({ "FIO_ALT_PATH_POS", nextAddr, 
        [this](Word addr) { 
            (void)addr; 
            Byte data = path_alt_char_pos;
            if (data >= altFilePath.size())
                data = altFilePath.size() - 1;
            if (altFilePath.size() == 0)
                data = 0;
            return data; 
        }, 
        [this](Word addr, Byte data) { 
            (void)addr; 
            if (data > altFilePath.size())
                data = altFilePath.size() - 1;
            path_alt_char_pos = data;
        },  
        { "(Byte) Character Position Within the Alternate Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_ALT_PATH_DATA
    //      Data at the Character Position of the Alternate Path
    /////
    mapped_register.push_back({ "FIO_ALT_PATH_DATA", nextAddr, 
        [this](Word addr) { 
            (void)addr;
            Byte data = 0;
            if (altFilePath.size() > 0)
            {
                if (path_alt_char_pos < altFilePath.size())
                {
                    data = (Byte)filePath.substr(path_alt_char_pos, 1).at(0);
                    if (path_alt_char_pos <= altFilePath.size())
                        path_alt_char_pos++;
                }
                else
                {
                    altFilePath = "";
                    data = 0;
                }
            }
            return data; 
        }, 
        [this](Word addr, Byte data) { 
            (void)addr; 
            if (path_char_pos == 0)
            {
                altFilePath = "";
                path_alt_char_pos = 0;
                altFilePath += data;
                path_alt_char_pos++;
            }
            else
            {
                altFilePath = altFilePath.substr(0, path_alt_char_pos);
                altFilePath += data;
                path_alt_char_pos++;
            }
        },  
        { "(Byte) Data at the Character Position of the Alternate Path",""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_DIR_DATA
    //      A Series of Null-Terminated Filenames
    /////
    mapped_register.push_back({ "FIO_DIR_DATA", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) A Series of Null-Terminated Filenames",
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


    // std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnAttach() Exit" << clr::RETURN;
    _size = nextAddr - old_address;
    return _size;
}


/**
 * OnInit() is called when the device is initialized.
 * It creates a vector of FILE* to represent the file streams
 * that are available for use by the system. There are
 * FILEHANDLESMAX of them.
 */
void FileIO::OnInit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnInit() Entry" << clr::RETURN;

    // create a block of null file stream devices
    for (int i = 0; i < FILEHANDLESMAX; i++)
    {
        FILE* ifs = nullptr;
        _vecFileStreams.push_back(ifs);
    }

    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnInit() Exit" << clr::RETURN;
}


/**
 * OnQuit() is called when the device is being shutdown.
 * It iterates through the vector of file streams and
 * closes and sets to nullptr any non-null file streams.
 */
void FileIO::OnQuit()
{
    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnQuit() Entry" << clr::RETURN;

    // close and and all open file streams
    for (auto* fs : _vecFileStreams)
    {
        if (fs != nullptr)
        {
            fclose(fs);
            fs = nullptr;
        }
    }

    std::cout << clr::indent() << clr::LT_BLUE << "FileIO::OnQuit() Exit" << clr::RETURN;
}


/**
 * _cmd_reset() is called when the FIO_COMMAND for a reset
 * (FC_RESET) is received. It sends a reset command to the
 * C6809.
 */
void FileIO::_cmd_reset()
{
    // printf("FileIO: RESET Command Received\n");
    if (Bus::GetC6809())
        Bus::GetC6809()->reset();
}


/**
 * _cmd_system_shutdown() is called to shut down the system.
 * It sets the running state of the Bus to false, effectively
 * stopping the system operations.
 */
void FileIO::_cmd_system_shutdown()
{
    Bus::IsRunning(false);
}


/**
 * _cmd_system_load_comilation_date() is called to load the compilation date of
 * the system into the FIO_PATH. It sets the FIO_PATH to the current date string
 * and sets the FIO_PATH_CHARPOS to 0.
 */
void FileIO::_cmd_system_load_comilation_date()
{
    filePath = __DATE__;
    path_char_pos = 0;
}


/**
 * _bFileExists() returns true if the file exists, false otherwise.
 * If the file does not exist, it sets the FIO_ERROR to FE_NOTFOUND.
 */
bool FileIO::_bFileExists(const char* file)
{
    if (file)
    {
        FILE* fp = fopen(file, "rb");
        if (!fp)
        {
            Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
            return false;
        }
        fclose(fp);
        return true;            
    }    
    return false;
}


/**
 * _openFile() opens a file for reading or writing with the given mode
 * and places the file pointer in the first free slot in the vector of
 * file streams. If the file cannot be opened, it sets the FIO_ERROR to
 * FE_NOTOPEN.
 *
 * @param[in] mode Mode string for fopen() (e.g., "rb", "wb", "r+b", etc.)
 */
void FileIO::_openFile(const char* mode)
{
    // if (!_bFileExists(filePath.c_str())) { return; }
	_fileHandle = _find_free_file_slot();
    _vecFileStreams[_fileHandle] = fopen(filePath.c_str(), mode);
    if (!_vecFileStreams[_fileHandle])
    {
        Memory::Write(MAP(FIO_ERROR), FE_NOTOPEN);
        return;
    } 
}


/**
 * Opens the file for reading, effectively calling fopen() with "rb".
 * If the file cannot be opened, it sets the FIO_ERROR to FE_NOTOPEN.
 */
void FileIO::_cmd_open_read()
{
    // printf("%s::_cmd_open_read()\n", Name().c_str());
    _openFile("rb");
}


/**
 * Opens the file for writing, effectively calling fopen() with "wb".
 * If the file cannot be opened, it sets the FIO_ERROR to FE_NOTOPEN.
 */
void FileIO::_cmd_open_write()
{
    // printf("%s::_cmd_open_write()\n", Name().c_str());
    _openFile("wb");
}


/**
 * Opens the file for appending, effectively calling fopen() with "ab".
 * If the file cannot be opened, it sets the FIO_ERROR to FE_NOTOPEN.
 */
void FileIO::_cmd_open_append()
{
    // printf("%s::_cmd_open_append()\n", Name().c_str());
    _openFile("ab");
}


/**
 * _cmd_close_file() closes the file stream associated with the FIO_HANDLE
 * and sets its corresponding entry in the vector of file streams to nullptr.
 * If the file is not open, it sets the FIO_ERROR to FE_NOTOPEN.
 */
void FileIO::_cmd_close_file()
{
    // printf("%s::_cmd_close_file()\n", Name().c_str());
    Byte handle = Memory::Read(MAP(FIO_HANDLE));
    if (handle==0 || _vecFileStreams[handle] == nullptr)
    {
        Memory::Write(MAP(FIO_ERROR), FE_NOTOPEN);
        return;
    }
    fclose(_vecFileStreams[handle]);
    _vecFileStreams[handle] = nullptr;
}


/**
 * Reads a single byte from the file stream associated with the FIO_HANDLE
 * and stores the read value in FIO_IODATA.
 * If the file is not open, it sets the FIO_ERROR to FE_NOTOPEN.
 * If the end of file is reached, it sets the FIO_ERROR to FE_OVERRUN.
 */
void FileIO::_cmd_read_byte()
{
    // printf("%s::_cmd_read_byte()\n", Name().c_str());
    Byte handle = Memory::Read(MAP(FIO_HANDLE));
    if (handle == 0 || _vecFileStreams[handle] == nullptr)
    {   
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTOPEN);
        return;
    }
    Byte data = (Byte)fgetc(_vecFileStreams[handle]);
    if (feof(_vecFileStreams[handle]))
    {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_OVERRUN);
        // _cmd_close_file();
        return;
    }
    Memory::Write(MAP(FIO_IODATA), data);    
}


/**
 * Writes the byte stored in FIO_IODATA to the file stream associated with the FIO_HANDLE.
 * If the file is not open, it sets the FIO_ERROR to FE_NOTOPEN.
 */
void FileIO::_cmd_write_byte()
{
    // printf("%s::_cmd_write_byte()\n", Name().c_str());
    Byte handle = Memory::Read(MAP(FIO_HANDLE));
    if (handle == 0 || _vecFileStreams[handle] == nullptr)
    {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTOPEN);
        return;
    }
    Byte data = (Byte)fputc(_io_data, _vecFileStreams[handle]);
    Memory::Write(MAP(FIO_IODATA), data);
}


/**
 * Checks if a file exists.
 *
 * Reads the file path from FIO_PATH_DATA and checks if the file exists.
 * Writes the result to FIO_IODATA (true if the file exists, false otherwise).
 * If the file does not exist, sets the FIO_ERROR to FE_NOTFOUND.
 *
 * @return true if the file exists, false otherwise.
 */
bool FileIO::_cmd_does_file_exist()
{
    bool exists = _bFileExists(filePath.c_str());
    Memory::Write(MAP(FIO_IODATA), (Byte)exists);
    if (!exists)
    {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return false;
    }
    return true;
}


/**
 * Reads two characters from the file stream and interprets them as a hexadecimal
 * representation of a byte.
 *
 * @param ifs the file stream to read from
 * @return the byte value read from the file stream
 */
Byte FileIO::_fread_hex_byte(std::ifstream& ifs)
{
    char str[3];
    long l;
    ifs.get(str[0]);
    ifs.get(str[1]);
    str[2] = '\0';
    l = strtol(str, NULL, 16);
    return (Byte)(l & 0xff);
}


/**
 * Reads four characters from the file stream and interprets them as a hexadecimal
 * representation of a 16-bit word.
 *
 * @param ifs the file stream to read from
 * @return the word value read from the file stream
 */
Word FileIO::_fread_hex_word(std::ifstream& ifs)
{
    Word ret;
    ret = _fread_hex_byte(ifs);
    ret <<= 8;
    ret |= _fread_hex_byte(ifs);
    return ret;
}

/**
 * Loads a hexadecimal format file into memory.
 *
 * This function attempts to load a file specified by `filePath` in the Intel
 * HEX format. It performs several checks to ensure the file is valid and
 * accessible:
 * - Verifies that `filePath` is not empty.
 * - Strips leading and trailing quotes from `filePath` if present.
 * - Checks if the file exists and has a `.hex` extension.
 * - Attempts to open the file and reads each line, interpreting it as a
 *   sequence of hexadecimal bytes.
 * 
 * For each line:
 * - Skips the leading colon (`:`).
 * - Reads the byte count, starting address, and record type.
 * - If the record type is 0x00 (data), reads the specified number of bytes
 *   and writes them into memory starting at the given address.
 * - If the record type is 0x01 (end of file), terminates the reading process.
 * - Discards the checksum byte and skips any trailing line endings.
 *
 * If any file operation or validation fails, the function sets an appropriate
 * error code in `FIO_ERROR` and returns without modifying memory.
 */
void FileIO::_cmd_load_hex_file()
{
    // sanity check (insanity perhaps?)
    if (filePath.size() == 0)   return;

    if (filePath[0]=='\"')
        filePath = filePath.substr(1, filePath.size()-3);

    // printf("'%s' size: %d\n",filePath.c_str(), (int)filePath.size());   
    path_char_pos=0;

    std::ifstream ifs(filePath);
    std::filesystem::path f{ filePath.c_str()};
    if (!std::filesystem::exists(f))
    {
        // printf("File '%s' Not Found\n", f.filename().string().c_str());
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        ifs.close();
        return;
    }
    // does this have a .hex extenion?
    // (wrong file type)
    std::string strExt = f.filename().extension().string();
    if (strExt != ".hex" && strExt != ".hex ")
    {
        // printf("EXTENSION: %s\n", strExt.c_str());
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        ifs.close();
        return;
    }
 
    if (!ifs.is_open())
    {
        // printf("UNABLE TO OPEN FILE '%s'\n", f.filename().string().c_str());
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTOPEN);
        ifs.close();
        return;
    }
    bool done = false;
    char c;

    while (!done)
    {
        Byte n, t;
        Word addr;
        Byte b;
        ifs.get(c);	
        if (c != ':')
        {
            ifs.close();
            Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
            return;
        }
        n = _fread_hex_byte(ifs);		// byte count for this line
        addr = _fread_hex_word(ifs);	// fetch the begin address		
        t = _fread_hex_byte(ifs);		// record type
        if (t == 0x00)
        {
            while (n--)
            {
                b = _fread_hex_byte(ifs);
                Memory::Write(addr, b, true);
                ++addr;
            }
            // Read and discard checksum byte
            (void)_fread_hex_byte(ifs);
            // skip the junk at the end of the line	
            if (ifs.peek() == '\r')	ifs.get(c);
            if (ifs.peek() == '\n')	ifs.get(c);
        }
        else if (t == 0x01)
            done = true;
    }
    // close and return
    ifs.close();
}

/**
 * _cmd_get_file_length() retrieves the size of a file specified by `filePath`.
 * 
 * If the file exists, it writes the file size to MATH_ACR_INT. If the file
 * does not exist, it sets the FIO_ERROR to FE_NOTFOUND and writes 0 to 
 * MATH_ACR_INT.
 */
void FileIO::_cmd_get_file_length()
{
    // printf("%s::_cmd_get_file_length()\n", Name().c_str());
    std::filesystem::path arg1 = filePath;
    if (!std::filesystem::exists(arg1))
    {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);  
        Memory::Write(MAP(MATH_ACR_INT), 0);
        return;
    }
    Uint32 file_size = std::filesystem::file_size(arg1);
    Memory::Write_DWord(MAP(MATH_ACR_INT), file_size);
}


/**
 * Lists the contents of a directory or files matching a specified pattern.
 *
 * This function retrieves the directory contents or files that match the specified
 * pattern in `filePath`. If `filePath` is empty, it defaults to the current working
 * directory. The results are stored in the FIO_BUFFER, with directories enclosed
 * in square brackets and files listed with their names. Wildcards can be used to
 * match files based on their stems or extensions. If a specified directory does not
 * exist, an error message is stored in `dir_data`.
 *
 * The function performs initial error checking on `current_path` and `filePath` and
 * handles both directory and file listing scenarios. In case of any exceptions, it
 * sets `dir_data` to indicate an error.
 */

void FileIO::_cmd_list_directory()
{
    //printf("%s::_cmd_list_directory()\n", Name().c_str());

    std::string current_path = std::filesystem::current_path().generic_string();
    std::vector<std::string> _files;

    // initial error checking
    if (current_path[0] == -1)   current_path = "";
    if (filePath[0] == -1)    
    {
        filePath="";
        path_char_pos = 0;
    }
    std::string seach_folder = "";
    std::filesystem::path arg1 = filePath;    
    std::string app_str = arg1.generic_string().c_str();   
    if (arg1=="") 
    {
        arg1=std::filesystem::current_path().generic_string();
        app_str = current_path;   
    }
    else
        app_str = current_path + "/" + arg1.generic_string().c_str();   
    std::string filename = arg1.filename().generic_string().c_str();
    std::string stem = arg1.stem().generic_string().c_str();
    std::string extension = arg1.extension().generic_string().c_str();


    if (std::filesystem::is_directory(app_str))
        arg1 = app_str;

    try
    {    
        // is folder
        if (std::filesystem::is_directory(arg1))
        {
            seach_folder = arg1.generic_string().c_str();          
            for (const auto& entry : std::filesystem::directory_iterator(arg1.generic_string().c_str()))
            {
                std::stringstream ssFile;
                std::string entry_filename = entry.path().filename().generic_string().c_str();
                if (entry.is_directory())
                {
                    ssFile << "  [" << entry_filename << "]";
                    _files.push_back(ssFile.str());
                }
                else
                {
                    ssFile << "    " << entry_filename;
                    _files.push_back(ssFile.str());
                }                
            }
        }
        // is file and/or wildcards
        else
        {            
            std::string parent = std::filesystem::path(arg1).parent_path().generic_string().c_str();
            app_str = current_path + "/" + parent;
            seach_folder = app_str.c_str();      
            for (const auto& entry : std::filesystem::directory_iterator(app_str.c_str()))
            {
                if (entry.is_regular_file())
                {
                    std::stringstream ssFile;
                    std::string file_stem = entry.path().stem().generic_string().c_str();
                    std::string file_ext = entry.path().extension().generic_string().c_str();
                    std::string entry_filename = entry.path().filename().generic_string().c_str();

                    bool bAllWild = false;
                    if (stem == "*" && extension == ".*")
                        bAllWild = true;
                        
                    if (entry_filename == arg1.filename().generic_string().c_str() || bAllWild)
                    {
                        ssFile << "    " << entry_filename;
                        _files.push_back(ssFile.str());
                    }
                    else if (file_stem == stem && extension == ".*")
                    {
                        ssFile << "    " << entry_filename;
                        _files.push_back(ssFile.str());
                    }                    
                    else if (file_ext == extension && stem == "*")
                    {
                        ssFile << "    " << entry_filename;
                        _files.push_back(ssFile.str());
                    }
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        dir_data = "Error: No such folder!\n";
        dir_data_pos = 0; 
        return;
    }
    // clear the FIO_BUFFER
    for (int i = MAP(FIO_BUFFER); i <= MAP(FIO_BFR_END); i++)
        Memory::Write(i, 0);

    // build the result
    dir_data = seach_folder;
    dir_data += "\n";
    std::sort(_files.begin(), _files.end());
    for (auto& f : _files)
        dir_data += (f+"\n");
    dir_data_pos = 0;      
}


/**
 * Change the current working directory to the path given by filePath.
 * filePath must be a valid path. If the path does not exist, an error
 * code of FE_NOTFOUND is written to the FIO_ERROR memory location.
 */
void FileIO::_cmd_change_directory()
{
    // printf("Change Directory To: %s\n", filePath.c_str());
    if (filePath.size() == 0)   return;

    std::string chdir = filePath;    
    if (std::filesystem::exists(chdir))
        std::filesystem::current_path(chdir);
    else
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
}


/**
 * _cmd_get_current_path() retrieves the current working directory into the
 * filePath memory location.
 */
void FileIO::_cmd_get_current_path()
{
    // printf("FileIO::_cmd_get_current_path()\n");
    path_char_pos = 0;
    filePath = std::filesystem::current_path().string() + "\n";
    // printf("%s\n", filePath.c_str());
}


/**
 * Creates a new directory specified by `filePath`.
 *
 * This function attempts to create a directory at the location specified
 * by `filePath`. If the directory does not exist, it will be created. 
 * If the directory already exists, the function sets the error code 
 * `FIO_ERROR` to `FE_FILE_EXISTS`. If the directory cannot be created due 
 * to an invalid name or other filesystem error, it sets the error code 
 * `FIO_ERROR` to `FE_INVALID_NAME`.
 */
void FileIO::_cmd_make_directory() {
    std::string current_path = std::filesystem::current_path().generic_string();
    std::filesystem::path arg1(filePath);

    if (!std::filesystem::exists(arg1)) {
        try {
            std::filesystem::create_directory(arg1);
        } catch (const std::filesystem::filesystem_error& e) {
            fio_error_code = FILE_ERROR::FE_INVALID_NAME;
            return;
        }
    }
    // If the directory already exists, set FIO_ERROR to FE_FILE_EXISTS
    if (std::filesystem::exists(arg1) && std::filesystem::is_directory(arg1)) {
        fio_error_code = FILE_ERROR::FE_FILE_EXISTS;
    }
}


/**
 * Renames the directory specified by `filePath` to the new name specified by `altFilePath`.
 *
 * This function attempts to rename the directory at the location specified
 * by `filePath` to the new name specified by `altFilePath`. If the directory
 * does not exist, it sets the error code `FIO_ERROR` to `FE_NOTFOUND`. 
 * If the directory already exists with the new name, it sets the error code 
 * `FIO_ERROR` to `FE_FILE_EXISTS`. If the directory cannot be renamed due 
 * to an invalid name or other filesystem error, it sets the error code 
 * `FIO_ERROR` to `FE_INVALID_NAME`.
 */
void FileIO::_cmd_rename_directory()
{
    std::cout << "FileIO::_cmd_rename_directory()\n";

    std::filesystem::path oldPath = filePath;
    std::filesystem::path newName = altFilePath;

    // Check if the old path is a valid directory or a file 
    // (which should be a directory in this case)
    if (!std::filesystem::exists(oldPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Ensure that oldPath is a valid directory
    if (!std::filesystem::is_directory(oldPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        return;
    }
    // Check if the new path already exists 
    std::filesystem::path newPath = oldPath.parent_path() / newName;
    if (std::filesystem::exists(newPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        return;
    }
    // Attempt to rename the directory
    try {
        std::filesystem::rename(oldPath, newPath);
        filePath = newName.string();  // Update the filePath to reflect the new directory name
    } catch (const std::filesystem::filesystem_error& e) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
    }

    // see: std::filesystem::rename() for both rename_directory and rename_file
    // https://en.cppreference.com/w/cpp/filesystem/rename
}


/**
 * Removes the directory specified by `filePath` if it is empty.
 *
 * This function attempts to remove the directory at the location specified
 * by `filePath`. If the directory does not exist, it sets the error code
 * `FIO_ERROR` to `FE_NOTFOUND`. If the directory is not empty, it sets the
 * error code `FIO_ERROR` to `FE_NOT_EMPTY`. If the directory cannot be
 * removed due to an invalid name or other filesystem error, it sets the
 * error code `FIO_ERROR` to `FE_INVALID_NAME`.
 */
void FileIO::_cmd_remove_directory()
{
    std::cout << "FileIO::_cmd_remove_directory()\n";

    const std::filesystem::path dirPath = filePath;
    // Check if the directory exists and is a directory
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Check if the directory is empty
    if (!std::filesystem::is_empty(dirPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOT_EMPTY);
        return;
    }
    try {
        std::filesystem::remove(dirPath);
        filePath = "";  // Reset filePath since the directory has been removed
    } catch (const std::filesystem::filesystem_error& e) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
    }  

    // see: std::filesystem::remove() for both remove_directory and delete_file
    // https://en.cppreference.com/w/cpp/filesystem/remove
}


/**
 * Deletes the file specified by `filePath`.
 *
 * This function checks if the file at the given `filePath` exists and is a regular file.
 * If the file does not exist, it sets the error code `FIO_ERROR` to `FE_NOTFOUND`.
 * If the path is not a regular file, it sets the error code `FIO_ERROR` to `FE_WRONGTYPE`.
 * On successful deletion, the file is removed from the filesystem.
 * If the file cannot be deleted due to a filesystem error, it sets the error code 
 * `FIO_ERROR` to `FE_BAD_CMD`.
 */
void FileIO::_cmd_delete_file()
{
    std::cout << "FileIO::_cmd_delete_file()\n";

    std::filesystem::path path = filePath;
    if (!std::filesystem::exists(path)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    if (!std::filesystem::is_regular_file(path)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        return;
    }
    try {
        std::filesystem::remove(path);
    } catch (const std::filesystem::filesystem_error& e) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
    }

    // see: std::filesystem::remove() for both remove_directory and delete_file
    // https://en.cppreference.com/w/cpp/filesystem/remove
}


/**
 * Renames the file specified by `filePath` to the name specified by `altFilePath`.
 *
 * This function checks if the `filePath` points to an existing regular file
 * and if the `altFilePath` does not already exist. If any of these conditions
 * are not met, it sets the appropriate error code in `FIO_ERROR` and returns.
 * If the renaming operation is successful, it updates `filePath` to reflect
 * the new file name. In case of a filesystem error during the renaming process,
 * it sets the error code in `FIO_ERROR` to `FE_BAD_CMD`.
 */
void FileIO::_cmd_rename_file()
{
    std::cout << "FileIO::_cmd_rename_file()\n";

    std::filesystem::path oldPath = filePath;
    std::filesystem::path newName = altFilePath;
    // Check if the old path is a valid directory or a file 
    // (which should be a directory in this case)
    if (!std::filesystem::exists(oldPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Ensure that oldPath is a valid file
    if (!std::filesystem::is_regular_file(oldPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        return;
    }
    // Check if the new path already exists 
    std::filesystem::path newPath = oldPath.parent_path() / newName;
    if (std::filesystem::exists(newPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        return;
    }
    // Attempt to rename the directory
    try {
        std::filesystem::rename(oldPath, newPath);
        filePath = newName.string();  // Update the filePath to reflect the new directory name
    } catch (const std::filesystem::filesystem_error& e) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
    }

    // see: std::filesystem::rename() for both rename_directory and rename_file
    // https://en.cppreference.com/w/cpp/filesystem/rename
}


/**
 * Copies the file specified by `filePath` to the location specified by `altFilePath`.
 *
 * This function checks if the `filePath` points to an existing regular file
 * and if the `altFilePath` does not already exist as a file. If any of these
 * conditions are not met, it sets the appropriate error code in `FIO_ERROR`
 * and returns. If the copying operation is successful, it updates `filePath`
 * to reflect the new file name. In case of a filesystem error during the
 * copying process, it sets the error code in `FIO_ERROR` to `FE_BAD_CMD`.
 */
void FileIO::_cmd_copy_file()
{
    std::cout << "FileIO::_cmd_copy_file()\n";

    const std::filesystem::path srcPath = filePath;
    const std::filesystem::path destPath = altFilePath;
    // Check if the source file exists and is a regular file
    if (!std::filesystem::exists(srcPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Ensure that srcPath is a valid file
    if (!std::filesystem::is_regular_file(srcPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        return;
    }
    // Check if the destination path already exists (which would be a file in this case)
    if (std::filesystem::exists(destPath)) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_FILE_EXISTS);
        return;
    }
    try {
        std::filesystem::copy(srcPath, destPath);
        filePath = destPath.filename().string();  // Update the filePath to reflect the new destination filename
    } catch (const std::filesystem::filesystem_error& e) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
    }
    // see: std::filesystem::copy() to copy files, folders, and symlinks
    // https://en.cppreference.com/w/cpp/filesystem/copy
}


/**
 * Moves the file pointer to the beginning of the file and stores its current position.
 *
 * If no file is currently open for reading/writing using the provided handle index,
 * it sets the error code `FIO_ERROR` to `FE_NOTFOUND`.
 */
void FileIO::_cmd_seek_start()
{
    std::cout << "FileIO::_cmd_seek_start()\n";

    // Check if a file is currently open for reading/writing using the provided handle index
    if (_fileHandle == 0 || _vecFileStreams[_fileHandle] == nullptr) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Move the file pointer to the end of the file and get its current position using tellg()
    fseek(_vecFileStreams[_fileHandle], 0, SEEK_SET);
    // Set the seek position
    _seek_pos = ftell(_vecFileStreams[_fileHandle]);
    // https://en.cppreference.com/w/cpp/io/c/fseek
}


/**
 * Moves the file pointer to the end of the file and updates the current
 * seek position.
 *
 * If no file is currently open for reading/writing using the provided handle index,
 * it sets the error code `FIO_ERROR` to `FE_NOTFOUND`.
 * If the end of the file is reached unexpectedly, it sets the error code
 * `FIO_ERROR` to `FE_BADSTREAM`.
 */
void FileIO::_cmd_seek_end()
{
    std::cout << "FileIO::_cmd_seek_end()\n";

    // Check if a file is currently open for reading/writing using the provided handle index
    if (_fileHandle == 0 || _vecFileStreams[_fileHandle] == nullptr) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Move the file pointer to the end of the file and get its current position using tellg()
    fseek(_vecFileStreams[_fileHandle], 0, SEEK_END);
    // Check if seeking to the end was successful
    if (feof(_vecFileStreams[_fileHandle])) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
        return;
    }
    // Set the seek position
    _seek_pos = ftell(_vecFileStreams[_fileHandle]);

    // https://en.cppreference.com/w/cpp/io/c/fseek
}


/**
 * Moves the file pointer to the position stored in `_seek_pos` and
 * updates the current seek position.
 *
 * If no file is currently open for reading/writing using the provided
 * handle index, it sets the error code `FIO_ERROR` to `FE_NOTFOUND`.
 * If seeking to the end was past the end of the file, it sets the error
 * code `FIO_ERROR` to `FE_BADSTREAM`.
 */
void FileIO::_cmd_set_seek_position()
{
    std::cout << "FileIO::_cmd_set_seek_position()\n";

    // Check if a file is currently open for reading/writing using the provided handle index
    if (_fileHandle == 0 || _vecFileStreams[_fileHandle] == nullptr) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Move the file pointer to the end of the file and get its current position using tellg()
    fseek(_vecFileStreams[_fileHandle], _seek_pos, SEEK_SET);
    // Check if seeking was past the end.
    if (feof(_vecFileStreams[_fileHandle])) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
        return;
    }
    // Set the seek position
    _seek_pos = ftell(_vecFileStreams[_fileHandle]);

    // https://en.cppreference.com/w/cpp/io/c/fsetpos
}


/**
 * Retrieves the current position of the file pointer and updates the internal seek position.
 * 
 * If no file is currently open for reading/writing using the provided handle index,
 * it sets the error code `FIO_ERROR` to `FE_NOTFOUND`.
 * If retrieving the current position fails, it sets the error code `FIO_ERROR` 
 * to `FE_BADSTREAM`.
 */
void FileIO::_cmd_get_seek_position()
{
    std::cout << "FileIO::_cmd_get_seek_position()\n";

    // Check if a file is currently open for reading/writing using the provided handle index
    if (_fileHandle == 0 || _vecFileStreams[_fileHandle] == nullptr) {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        return;
    }
    // Get the current position of the file pointer using ftell()
    if (ftell(_vecFileStreams[_fileHandle]) != -1) {
        _seek_pos = ftell(_vecFileStreams[_fileHandle]);
    } else {
        Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
    }

    // https://en.cppreference.com/w/cpp/io/c/fgetpos
}


/**
 * Finds an empty slot in the file stream vector and returns its index.
 *
 * Iterates from 1 to FILEHANDLESMAX, and returns the first empty slot it finds.
 * If no empty slot is found, it sets the error code `FIO_ERROR` to `FE_BADSTREAM`
 * and returns 0.
 *
 * @return The index of the empty slot, or 0 if no empty slot is found.
 */
int FileIO::_find_free_file_slot()
{
	// find an empty slot
	int found = 0;
	for (int t = 1; t < FILEHANDLESMAX; t++)
	{
		if (_vecFileStreams[t] == nullptr)
		{
			found = t;
			break;
		}
	}
	// too many file handles?
	if (found == 0)
	{
		Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
		return 0;
	}
	Memory::Write(MAP(FIO_HANDLE), found);
	return found;
}


// END: FileIO.cpp
