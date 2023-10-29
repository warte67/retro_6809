/* FileIO.cpp ***************************************
 *
 *   Acts to emulate a Raspberry PI Pico
 *		Implementing File Input / Output
 *		with the onboad flash ram and
 *		the SDCARD
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#include <filesystem>
#include <iostream>
#include <algorithm>

#include "C6809.h"
#include "Bus.h"
#include "FileIO.h"


Byte FileIO::read(Word offset, bool debug)
{
	Byte data = 0xCC;	// default unassigned memory

    // data starts as default
    if (offset - m_base < m_size)
        data = m_memory[(Word)(offset - m_base)];

    switch (offset)
    {
        case FIO_ERR_FLAGS: 
        {
            Byte d = data;
            data = fio_err_flags;  
            fio_err_flags = 0;
            break;
        }
        case FIO_PATH_LEN:  data = dir_data.size(); break;
        case FIO_PATH_POS:  
        {
            data = path_char_pos;
            if (data >= filePath.size())
                data = filePath.size() - 1;
            if (filePath.size() == 0)
                data = 0;
            break;
        }
        case FIO_PATH_DATA:
        {
            data = 0;
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
            break;
        }
        case FIO_DIR_DATA: // (Byte) a series of $0a--terminated filenames
        {
            data = 0;
            if (dir_data_pos < dir_data.size())
            {
                data = (Byte)dir_data.substr(dir_data_pos, 1).at(0);
                if (dir_data_pos <= dir_data.size())
                    dir_data_pos++;
            }
            else
            {
                dir_data = "";
                data = 0;
            }
            break;
        }
    }

	// update and return
	Bus::Write(offset, data, true);
	return data;
}

void FileIO::write(Word offset, Byte data, bool debug)
{
    // printf("FileIO::write($%04X, $%02X)\n", offset, data);

    switch (offset)
    {
        case FIO_ERR_FLAGS: fio_err_flags = data; break;
        case FIO_PATH_POS:  
            if (data > filePath.size())
                data = filePath.size() - 1;
            path_char_pos = data;
            break;
        case FIO_PATH_DATA:
        {
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
            break;
        }


        case FIO_COMMAND:
        {
            switch (data)
            {                
                case 0x00: _cmd_reset();                         break;
                case 0x01: _cmd_system_shutdown();               break;
                case 0x02: _cmd_system_load_comilation_date();   break;
                case 0x03: _cmd_new_file_stream();               break;
                case 0x04: _cmd_open_file();                     break;
                case 0x05: _cmd_is_file_open();                  break;
                case 0x06: _cmd_close_file();                    break;    
                case 0x07: _cmd_read_byte();                     break;    
                case 0x08: _cmd_write_byte();                    break;
                case 0x09: _cmd_load_hex_file();                 break;    
                case 0x0A: _cmd_get_file_length();               break;        
                case 0x0B: _cmd_list_directory();                break;    
                case 0x0C: _cmd_make_directory();                break;        
                case 0x0D: _cmd_change_directory();              break;
                case 0x0E: _cmd_get_current_path();              break;
                case 0x0F: _cmd_rename_directory();              break;
                case 0x10: _cmd_remove_directory();              break;    
                case 0x11: _cmd_delete_file();                   break;
                case 0x12: _cmd_rename_file();                   break;
                case 0x13: _cmd_copy_file();                     break;
                case 0x14: _cmd_seek_start();                    break;    
                case 0x15: _cmd_seek_end();                      break;
                case 0x16: _cmd_set_seek_position();             break;    
                case 0x17: _cmd_get_seek_position();             break;    
                default:
                    data = fio_err_flags | 0x02;    // invalid command
                    break;
            }
            break;
        }
    }
	// write statically
	Bus::Write(offset, data, true);
}

void FileIO::_cmd_reset()
{
    //printf("FileIO: RESET Command Received\n");
    if (Bus::m_cpu)
        Bus::m_cpu->reset();
}

void FileIO::_cmd_system_shutdown()
{
    Bus::s_bIsRunning = false;
    printf("FileIO: SHUTDOWN Command Received\n");
    printf("\tShutting Down...\n");
}

void FileIO::_cmd_system_load_comilation_date()
{
    //printf("FileIO: COMPILE DATE Command Received\n");
    filePath = __DATE__;
    // printf("Compilation Date: %s\n", dir_data.c_str());
}

void FileIO::_cmd_new_file_stream()
{
}

void FileIO::_cmd_open_file()
{
}

void FileIO::_cmd_is_file_open()
{
}

void FileIO::_cmd_close_file()
{
}

void FileIO::_cmd_read_byte()
{
}

void FileIO::_cmd_write_byte()
{
}

void FileIO::_cmd_load_hex_file()
{
}

void FileIO::_cmd_get_file_length()
{
}

void FileIO::_cmd_list_directory()
{
    // int FILENAME_LENGTH = 24;
    int FILENAME_LENGTH = Bus::Read(DSP_TXT_COLS)/2;
    //if (FILENAME_LENGTH < 24)
    //    FILENAME_LENGTH *= 2;

    std::string path = std::filesystem::current_path().generic_string();
    std::string arg1 = "";
    for (int i = FIO_BUFFER; i <= FIO_BFR_TOP; i++)
        if ((char)Bus::Read(i) != ' ' && (char)Bus::Read(i) != 0)
            arg1 += (char)Bus::Read(i);
    std::vector<std::string> _files;

    std::string fp = path;
    std::string file = "";
    std::string ext = "";
    int dot = arg1.find('.');
    if (dot)
    {
        file = arg1.substr(0, dot);
        ext = arg1.substr(dot + 1);
    }
    // all files
    if (arg1.size() == 0 || (file == "*" && ext == "*"))   // no arg1 or arg1 = "*.*"
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            std::stringstream dir;
            if (entry.is_directory())
            {
                dir << "[" << entry.path().filename().generic_string() << "]";
                std::string stDir = dir.str();
                while (stDir.size() < FILENAME_LENGTH)
                    stDir = stDir + " ";
                //std::cout << stDir << std::endl;
                _files.push_back(stDir);
            }
        }
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            std::stringstream dir;
            if (entry.is_regular_file())
            {
                dir << entry.path().filename().generic_string();
                std::string stDir = dir.str();
                while (stDir.size() < FILENAME_LENGTH)
                    stDir = stDir + " ";
                std::cout << stDir << std::endl;
                _files.push_back(stDir);
            }
        }
    }
    else if (ext == "*" || file == "*")
    {
        std::stringstream dir;
        if (ext == "*")
        {
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                std::string strFile = entry.path().filename().string();
                strFile = strFile.substr(0, strFile.find('.'));
                std::stringstream dir;
                if (entry.is_regular_file() && file == strFile)
                {
                    dir << entry.path().filename().generic_string();
                    std::string stDir = dir.str();
                    while (stDir.size() < FILENAME_LENGTH)
                        stDir = stDir + " ";
                    _files.push_back(stDir);
                }
            }
        }
        else if (file == "*")
        {
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                std::string strExt = entry.path().extension().filename().string();
                if (strExt.find('.') == 0)
                    strExt = strExt.substr(1);
                std::stringstream dir;
                if (entry.is_regular_file() && ext == strExt)
                {
                    dir << entry.path().filename().generic_string();
                    std::string stDir = dir.str();
                    while (stDir.size() < FILENAME_LENGTH)
                        stDir = stDir + " ";
                    _files.push_back(stDir);
                }
            }
        }
    }
    else if (arg1 == "[]") // all directories
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                std::stringstream dir;
                dir << "[" << entry.path().filename().generic_string() << "]";
                std::string stDir = dir.str();
                while (stDir.size() < FILENAME_LENGTH)
                    stDir = stDir + " ";
                _files.push_back(stDir);
            }
        }
    }
    else // directories and exact matches
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            std::string strExt = entry.path().extension().filename().string();
            if (strExt.find('.') == 0)
                strExt = strExt.substr(1);
            std::string strFile = entry.path().filename().string();
            strFile = strFile.substr(0, strFile.find('.'));
            std::stringstream dir;
            if (entry.is_directory() && file == strFile)
            {
                dir << "[" << entry.path().filename().generic_string() << "]";
                std::string stDir = dir.str();
                while (stDir.size() < FILENAME_LENGTH)
                    stDir = stDir + " ";
                _files.push_back(stDir);
            }
            if (file == strFile && ext == strExt)
            {
                dir << entry.path().filename().generic_string();
                std::string stDir = dir.str();
                while (stDir.size() < FILENAME_LENGTH)
                    stDir = stDir + " ";
                _files.push_back(stDir);
            }
        }
    }
    //for (auto& f : _files)
    //    std::cout << f << std::endl;

    // clear the FIO_BUFFER
    for (int i = FIO_BUFFER; i <= FIO_BFR_TOP; i++)
        Bus::Write(i, 0);
    // build the directory list string
    dir_data = "";
    for (auto& f : _files)
        dir_data += f;
    // add the path at the end
    dir_data += "\n";
    dir_data += path;
    dir_data_pos = 0;        
}

void FileIO::_cmd_make_directory()
{
}

void FileIO::_cmd_change_directory()
{
    //printf("Change Directory To: %s\n", filePath.c_str());
    if (filePath.size() == 0)   return;
    std::string chdir = filePath + "/";
    if (std::filesystem::exists(chdir))
    {
        //printf("Directory Found\n");
        Byte data = Bus::Read(FIO_ERR_FLAGS);
        data &= ~0x40;
        Bus::Write(FIO_ERR_FLAGS, data);
        fio_err_flags = data;
        std::filesystem::current_path(chdir);  // change dir
    }
    else
    {
        //printf("ERROR: Directory Not Found!\n");
        Byte data = Bus::Read(FIO_ERR_FLAGS);
        data |= 0x40;
        Bus::Write(FIO_ERR_FLAGS, data);
        fio_err_flags = data;
    }
}

void FileIO::_cmd_get_current_path()
{
    printf("FileIO::_cmd_get_current_path()\n");
    path_char_pos = 0;
    filePath = std::filesystem::current_path().string() + "\n";
}

void FileIO::_cmd_rename_directory()
{
}

void FileIO::_cmd_remove_directory()
{
}

void FileIO::_cmd_delete_file()
{
}

void FileIO::_cmd_rename_file()
{
}

void FileIO::_cmd_copy_file()
{
}

void FileIO::_cmd_seek_start()
{
}

void FileIO::_cmd_seek_end()
{
}

void FileIO::_cmd_set_seek_position()
{
}

void FileIO::_cmd_get_seek_position()
{
}

Word FileIO::OnAttach(Word nextAddr)
{
	int size = 0;
	Word old_addr = nextAddr;

	 DisplayEnum("", 0, "");
	DisplayEnum("FIO_BEGIN", nextAddr, "Start of the FileIO register space");
	nextAddr += 0;

    //DisplayEnum("", 0, "");
    DisplayEnum("FIO_ERR_FLAGS", nextAddr, "(Byte) File IO error flags");
    DisplayEnum("", 0, "FIO_ERR_FLAGS: ABCD.EFGH");
    DisplayEnum("", 0, "     A:  file was not found");
    DisplayEnum("", 0, "     B:  directory was not found");
    DisplayEnum("", 0, "     C:  file not open");
    DisplayEnum("", 0, "     D:  end of file");
    DisplayEnum("", 0, "     E:  buffer overrun");
    DisplayEnum("", 0, "     F:  wrong file type");
    DisplayEnum("", 0, "     G:  invalid command");
    DisplayEnum("", 0, "     H:  incorrect file stream");
    nextAddr += 1;

    DisplayEnum("", 0, "");
    DisplayEnum("FIO_COMMAND", nextAddr, "(Byte) OnWrite, execute a file command (FC_<cmd>)");
    nextAddr += 1;
    Byte enumID = 0;
    DisplayEnum("", 0, "Begin FIO_COMMANDS");
    DisplayEnum("FC_RESET",     enumID++, "       Reset");
    DisplayEnum("FC_SHUTDOWN",  enumID++, "       SYSTEM: Shutdown");
    DisplayEnum("FC_COMPDATE",  enumID++, "       SYSTEM: Load Compilation Date");
    DisplayEnum("FC_NEWFILE",   enumID++, "     * New File Stream");
    DisplayEnum("FC_OPENFILE",  enumID++, "     * Open File");
    DisplayEnum("FC_ISOPEN",    enumID++, "     *Is File Open ? (returns FIO_ERR_FLAGS bit - 5)");
    DisplayEnum("FC_CLOSEFILE", enumID++, "     * Close File");
    DisplayEnum("FC_READBYTE",  enumID++, "     * Read Byte (into FIO_IOBYTE)");
    DisplayEnum("FC_WRITEBYTE", enumID++, "     * Write Byte (from FIO_IOBYTE)");
    DisplayEnum("FC_LOADHEX",   enumID++, "     * Load Hex Format File");
    DisplayEnum("FC_GETLENGTH", enumID++, "     * Get File Length (into FIO_IOWORD)");
    DisplayEnum("FC_LISTDIR",   enumID++, "       List Directory");
    DisplayEnum("FC_MAKEDIR",   enumID++, "     * Make Directory");
    DisplayEnum("FC_CHANGEDIR", enumID++, "       Change Directory");
    DisplayEnum("FC_GETPATH",   enumID++, "       Fetch Current Path");
    DisplayEnum("FC_REN_DIR",   enumID++, "     * Rename Directory");
    DisplayEnum("FC_DEL_DIR",   enumID++, "     * Delete Directory");
    DisplayEnum("FC_DEL_FILE",  enumID++, "     * Delete File");
    DisplayEnum("FC_REN_FILE",  enumID++, "     * Rename file");
    DisplayEnum("FC_COPYFILE",  enumID++, "     * Copy File");
    DisplayEnum("FC_SEEKSTART", enumID++, "     * Seek Start");
    DisplayEnum("FC_SEEKEND",   enumID++, "     * Seek End");
    DisplayEnum("FC_SET_SEEK",  enumID++, "     * Set Seek Position (from FIO_IOWORD)");
    DisplayEnum("FC_GET_SEEK",  enumID++, "     * Get Seek Position (into FIO_IOWORD)");
    DisplayEnum("", 0, "End FIO_COMMANDS");
    DisplayEnum("", 0, "");

    DisplayEnum("FIO_STREAM", nextAddr, "(Byte) current file stream index (0-15)");
    nextAddr += 1;

    DisplayEnum("FIO_MODE", nextAddr, "(Byte) Flags describing the I/O mode for the file");
    DisplayEnum("", 0, "FIO_MODE: 00AB.CDEF  (indexed by FIO_STREAM)");
    DisplayEnum("", 0, "     A:  INPUT - File open for reading");
    DisplayEnum("", 0, "     B:  OUTPUT - File open for writing");
    DisplayEnum("", 0, "     C:  BINARY - 1: Binary Mode, 0: Text Mode");
    DisplayEnum("", 0, "     D:  AT_END - Output starts at the end of the file");
    DisplayEnum("", 0, "     E:  APPEND - All output happens at end of the file");
    DisplayEnum("", 0, "     F:  TRUNC - discard all previous file data");
    nextAddr += 1;

    DisplayEnum("FIO_SEEKPOS", nextAddr, "(DWord) file seek position");
    nextAddr += 4;

    DisplayEnum("FIO_IOBYTE", nextAddr, "(Byte) input / output character");
    nextAddr += 1;

    DisplayEnum("FIO_IOWORD", nextAddr, "(Byte) input / output character");
    nextAddr += 1;

    DisplayEnum("FIO_PATH_LEN", nextAddr, "(Byte) length of the filepath");
    nextAddr += 1;

    DisplayEnum("FIO_PATH_POS", nextAddr, "(Byte) character position within the filepath");
    nextAddr += 1;

    DisplayEnum("FIO_PATH_DATA", nextAddr, "(Byte) data at the character position of the path");
    nextAddr += 1;

    DisplayEnum("FIO_DIR_DATA", nextAddr, "(Byte) a series of null-terminated filenames");
    DisplayEnum("", 0, "    NOTES: Current read-position is reset to the beginning following a ");
    DisplayEnum("", 0, "            List Directory command. The read-position is automatically ");
    DisplayEnum("", 0, "            advanced on read from this register. Each filename is ");
    DisplayEnum("", 0, "            $0a-terminated. The list itself is null-terminated.");
    nextAddr += 1;

	DisplayEnum("FIO_END", nextAddr, "End of the FileIO register space");
	nextAddr += 0;

    return nextAddr - old_addr;
}

void FileIO::OnInit() 
{
    // create a block of null file stream devices
    for (int i = 0; i < _FSTREAM_MAX; i++)
    {
        std::fstream* ifs = nullptr;
        _vecFileStreams.push_back(ifs);
    }
}
void FileIO::OnQuit() 
{
    // close all open file streams
    for (auto* fs : _vecFileStreams)
    {
        if (fs != nullptr)
        {
            if (fs->is_open())
            {
                fs->close();
                fs = nullptr;
            }
        }
    }
}


void FileIO::OnActivate() {}
void FileIO::OnDeactivate() {}
void FileIO::OnEvent(SDL_Event* evnt) {}
void FileIO::OnUpdate(float fElapsedTime) {}
void FileIO::OnRender() {}

