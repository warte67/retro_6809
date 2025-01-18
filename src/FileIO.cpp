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
        nullptr,
        nullptr,  
        { "(Byte) Current File Stream HANDLE (0=NONE)"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (DWord) FIO_SEEKPOS
    //      File Seek Position
    /////
    mapped_register.push_back({ "FIO_SEEKPOS", nextAddr, 
        nullptr, 
        nullptr,  
        { "(DWord) File Seek Position"} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        nullptr, 
        nullptr,  
        {""} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        nullptr, 
        nullptr,  
        {""} });
    nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        nullptr, 
        nullptr,  
        {""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_IODATA
    //      Input / Output Data
    /////
    mapped_register.push_back({ "FIO_IODATA", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) Input / Output Data",""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_LEN
    //      Length of the Filepath
    /////
    mapped_register.push_back({ "FIO_PATH_LEN", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) Length of the Primary Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_POS
    //      Character Position Within the Filepath
    /////
    mapped_register.push_back({ "FIO_PATH_POS", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) Character Position Within the Primary Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_PATH_DATA
    //      Data at the Character Position of the Primary Path
    /////
    mapped_register.push_back({ "FIO_PATH_DATA", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) Data at the Character Position of the Primary Path",""} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_ALT_PATH_LEN
    //      Length of the Alternate Filepath
    /////
    mapped_register.push_back({ "FIO_ALT_PATH_LEN", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) Length of the Alternate Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_ALT_PATH_POS
    //      Character Position Within the Alternate Filepath
    /////
    mapped_register.push_back({ "FIO_ALT_PATH_POS", nextAddr, 
        nullptr, 
        nullptr,  
        { "(Byte) Character Position Within the Alternate Filepath"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) FIO_ALT_PATH_DATA
    //      Data at the Character Position of the Alternate Path
    /////
    mapped_register.push_back({ "FIO_ALT_PATH_DATA", nextAddr, 
        nullptr, 
        nullptr,  
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




void FileIO::_cmd_reset()
{
    // printf("FileIO: RESET Command Received\n");
    if (Bus::GetC6809())
        Bus::GetC6809()->reset();
}

void FileIO::_cmd_system_shutdown()
{
    Bus::IsRunning(false);
}

void FileIO::_cmd_system_load_comilation_date()
{
    filePath = __DATE__;
    path_char_pos = 0;
}

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

void FileIO::_openFile(const char* mode)
{
    // if (!_bFileExists(filePath.c_str())) { return; }
	_fileHandle = _FindOpenFileSlot();
    _vecFileStreams[_fileHandle] = fopen(filePath.c_str(), mode);
    if (!_vecFileStreams[_fileHandle])
    {
        Memory::Write(MAP(FIO_ERROR), FE_NOTOPEN);
        return;
    } 
}

void FileIO::_cmd_open_read()
{
    // printf("%s::_cmd_open_read()\n", Name().c_str());
    _openFile("rb");
}

void FileIO::_cmd_open_write()
{
    // printf("%s::_cmd_open_write()\n", Name().c_str());
    _openFile("wb");
}
void FileIO::_cmd_open_append()
{
    // printf("%s::_cmd_open_append()\n", Name().c_str());
    _openFile("ab");
}

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


// load_hex helpers
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
Word FileIO::_fread_hex_word(std::ifstream& ifs)
{
    Word ret;
    ret = _fread_hex_byte(ifs);
    ret <<= 8;
    ret |= _fread_hex_byte(ifs);
    return ret;
}

void FileIO::_cmd_load_hex_file()
{
    // // lambda to convert integer to hex string
    // auto hex = [](uint32_t n, uint8_t digits)
    // {
    // 	std::string s(digits, '0');
    // 	for (int i = digits - 1; i >= 0; i--, n >>= 4)
    // 		s[i] = "0123456789ABCDEF"[n & 0xF];
    // 	return s;
    // };

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

void FileIO::_cmd_get_current_path()
{
    // printf("FileIO::_cmd_get_current_path()\n");
    path_char_pos = 0;
    filePath = std::filesystem::current_path().string() + "\n";
    // printf("%s\n", filePath.c_str());
}

void FileIO::_cmd_make_directory()
{
    std::string current_path = std::filesystem::current_path().generic_string();
    std::filesystem::path arg1 = filePath;    
    std::filesystem::create_directory(arg1);    
}

void FileIO::_cmd_rename_directory()
{
    std::cout << "FileIO::_cmd_rename_directory()\n";

    // VERIFY AI CODE ...

        // std::filesystem::path oldPath = filePath;
        // std::filesystem::path newPath = filePath.substr(0, filePath.find_last_of('/')) + "/" + Memory::Read(MAP(FIO_IODATA));
        // if (!std::filesystem::exists(oldPath)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // if (std::filesystem::exists(newPath)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        //     return;
        // }
        // try {
        //     std::filesystem::rename(oldPath, newPath);
        // } catch (const std::filesystem::filesystem_error& e) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
        // }

    // ... VERIFY AI CODE

    // see: std::filesystem::rename() for both rename_directory and rename_file
    // https://en.cppreference.com/w/cpp/filesystem/rename
}
void FileIO::_cmd_remove_directory()
{
    std::cout << "FileIO::_cmd_remove_directory()\n";

    // VERIFY AI CODE ...
        
        // std::filesystem::path path = filePath;
        // if (!std::filesystem::exists(path)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // if (!std::filesystem::is_directory(path)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        //     return;
        // }
        // try {
        //     std::filesystem::remove_all(path);
        // } catch (const std::filesystem::filesystem_error& e) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
        // }

    // ... VERIFY AI CODE

    // see: std::filesystem::remove() for both remove_directory and delete_file
    // https://en.cppreference.com/w/cpp/filesystem/remove
}

void FileIO::_cmd_delete_file()
{
    std::cout << "FileIO::_cmd_delete_file()\n";

    // VERIFY AI CODE ...

        // std::filesystem::path path = filePath;
        // if (!std::filesystem::exists(path)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // if (!std::filesystem::is_regular_file(path)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        //     return;
        // }
        // try {
        //     std::filesystem::remove(path);
        // } catch (const std::filesystem::filesystem_error& e) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
        // }

    // ... VERIFY AI CODE

    // see: std::filesystem::remove() for both remove_directory and delete_file
    // https://en.cppreference.com/w/cpp/filesystem/remove
}


void FileIO::_cmd_rename_file()
{
    std::cout << "FileIO::_cmd_rename_file()\n";

    // VERIFY AI CODE ...

        // std::filesystem::path oldPath = filePath;
        // std::filesystem::path newPath = filePath.substr(0, filePath.find_last_of('/')) + "/" + Memory::Read(MAP(FIO_IODATA));
        // if (!std::filesystem::exists(oldPath)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // if (std::filesystem::exists(newPath)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        //     return;
        // }
        // try {
        //     std::filesystem::rename(oldPath, newPath);
        // } catch (const std::filesystem::filesystem_error& e) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
        // }

    // ... VERIFY AI CODE

    // see: std::filesystem::rename() for both rename_directory and rename_file
    // https://en.cppreference.com/w/cpp/filesystem/rename
}

void FileIO::_cmd_copy_file()
{
    std::cout << "FileIO::_cmd_copy_file()\n";

    // VERIFY AI CODE ...

        // std::filesystem::path source = filePath;
        // std::filesystem::path destination = filePath.substr(0, filePath.find_last_of('/')) + "/" + Memory::Read(MAP(FIO_IODATA));
        // if (!std::filesystem::exists(source)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // if (std::filesystem::exists(destination)) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_WRONGTYPE);
        //     return;
        // }
        // try {
        //     std::filesystem::copy(source, destination);
        // } catch (const std::filesystem::filesystem_error& e) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BAD_CMD);
        // }

    // ... VERIFY AI CODE

    // see: std::filesystem::copy() to copy files, folders, and symlinks
    // https://en.cppreference.com/w/cpp/filesystem/copy
}

void FileIO::_cmd_seek_start()
{
    std::cout << "FileIO::_cmd_seek_start()\n";

    // VERIFY AI CODE ...

        // std::ifstream file(filePath, std::ios::binary);
        // if (!file.is_open()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // file.seekg(0);
        // if (!file.good()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
        // }

    // ... VERIFY AI CODE

    // https://en.cppreference.com/w/cpp/io/c/fseek
}

void FileIO::_cmd_seek_end()
{
    std::cout << "FileIO::_cmd_seek_end()\n";

    // VERIFY AI CODE ...

        // std::ifstream file(filePath, std::ios::binary);
        // if (!file.is_open()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // file.seekg(0, std::ios::end);
        // if (!file.good()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
        // }

    // ... VERIFY AI CODE

    // https://en.cppreference.com/w/cpp/io/c/fseek
}

void FileIO::_cmd_set_seek_position()
{
    std::cout << "FileIO::_cmd_set_seek_position()\n";

    // VERIFY AI CODE ...

        // std::ifstream file(filePath, std::ios::binary);
        // if (!file.is_open()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // Word position = Memory::Read(MAP(FIO_IOWORD));
        // file.seekg(position);
        // if (!file.good()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
        // }

    // ... VERIFY AI CODE

    // https://en.cppreference.com/w/cpp/io/c/fsetpos
}

void FileIO::_cmd_get_seek_position()
{
    std::cout << "FileIO::_cmd_get_seek_position()\n";

    // VERIFY AI CODE ...

        // std::ifstream file(filePath, std::ios::binary);
        // if (!file.is_open()) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_NOTFOUND);
        //     return;
        // }
        // Word position = file.tellg();
        // if (position == -1) {
        //     Memory::Write(MAP(FIO_ERROR), FILE_ERROR::FE_BADSTREAM);
        //     return;
        // }
        // Memory::Write(MAP(FIO_IOWORD), position);    

    // ... VERIFY AI CODE

    // https://en.cppreference.com/w/cpp/io/c/fgetpos
}


// helper: return a handle to an open file stream slot
int FileIO::_FindOpenFileSlot()
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
