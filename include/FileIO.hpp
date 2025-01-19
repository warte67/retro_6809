/*** FileIO.hpp *******************************************
 *      ______ _ _      _____ ____        _                 
 *     |  ____(_) |    |_   _/ __ \      | |                
 *     | |__   _| | ___  | || |  | |     | |__  _ __  _ __  
 *     |  __| | | |/ _ \ | || |  | |     | '_ \| '_ \| '_ \ 
 *     | |    | | |  __/_| || |__| |  _  | | | | |_) | |_) |
 *     |_|    |_|_|\___|_____\____/  (_) |_| |_| .__/| .__/ 
 *                                             | |   | |    
 *                                             |_|   |_|   
 *  
 *   A filesystem that acts to emulate a Raspberry PI Pico
 *		Implementing File Input / Output with the onboad 
 *      flash ram, USB storage and the SDCARD.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

#include "IDevice.hpp"

class FileIO : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    FileIO();
    virtual ~FileIO();

public: // VIRTUAL METHODS

    // pure virtuals
    virtual int  OnAttach(int nextAddr);            // attach to the memory map
    virtual void OnInit();                          // initialize
    virtual void OnQuit();                          // shutdown

    // unused pure virtuals
    virtual void OnActivate() {};                   // activate
    virtual void OnDeactivate() {};                 // deactivate
    virtual void OnEvent(SDL_Event* evnt) {(void)evnt;};       // handle events
    virtual void OnUpdate(float fElapsedTime) {(void)fElapsedTime;};   // update
    virtual void OnRender() {};                     // render
 
public: 

    enum FILE_ERROR {
        FE_NOERROR=0,   //  $00: no error, condition normal
        FE_NOTFOUND,    //  $01: file or folder not found
        FE_NOTOPEN,     //  $02: file not open
        FE_EOF,         //  $03: end of file
        FE_OVERRUN,     //  $04: buffer overrun
        FE_WRONGTYPE,   //  $05: wrong file type
        FE_BAD_CMD,     //  $06: invalid command
        FE_BADSTREAM,   //  $07: invalid file stream
        FE_NOT_EMPTY,   //  $08: directory not empty
        FE_FILE_EXISTS, //  $09: file already exists
        FE_INVALID_NAME //  $0A: invalid file name
    };

private: // PRIVATE MEMBERS

    void _cmd_reset();                          // Reset
    void _cmd_system_shutdown();                // SYSTEM: Shutdown
    void _cmd_system_load_comilation_date();    // SYSTEM: Load Compilation Date   
    bool _cmd_does_file_exist();                // Does File Exist (return in FIO_IODATA)            
    void _cmd_open_read();                      // Open Binary File For Reading    
    void _cmd_open_write();                     // Open Binary File For Writing    
    void _cmd_open_append();                    // Open Binary File For Appending        
    void _cmd_close_file();                     // Close File    
    void _cmd_read_byte();                      // Read Byte (into FIO_IOBYTE)    
    void _cmd_write_byte();                     // Write Byte (from FIO_IOBYTE)    
    void _cmd_load_hex_file();                  // Load Hex Format File        
    void _cmd_get_file_length();                // Get File Length (into FIO_IOWORD)            
    void _cmd_list_directory();                 // List Directory        
    void _cmd_make_directory();                 // Make Directory        
    void _cmd_change_directory();               // Change Directory            
    void _cmd_get_current_path();               // Fetch Current Path            
    void _cmd_rename_directory();               // * Rename Directory            
    void _cmd_remove_directory();               // * Delete Directory            
    void _cmd_delete_file();                    // * Delete File        
    void _cmd_rename_file();                    // * Rename file        
    void _cmd_copy_file();                      // * Copy File    
    void _cmd_seek_start();                     // * Seek Start    
    void _cmd_seek_end();                       // * Seek End    
    void _cmd_set_seek_position();              // * Set Seek Position (from FIO_IOWORD)            
    void _cmd_get_seek_position();              // * Get Seek Position (into FIO_IOWORD)    

    Byte _fread_hex_byte(std::ifstream& ifs);                           
    Word _fread_hex_word(std::ifstream& ifs);                           

    int _find_free_file_slot();                    // return a handle to an open file stream slot
    bool _bFileExists(const char* file);        // checks to see if a file exists
    void _openFile(const char* mode);           // file open helper

    Byte _fileHandle = 0;                       // file stream index (HANDLE)
    std::vector<FILE*> _vecFileStreams;         // vector of file streams

    // Byte fio_err_flags = 0;                  // error flags (deprecated)
    FILE_ERROR fio_error_code = FILE_ERROR::FE_NOERROR;    // current error code

    Byte path_char_pos = 0;                     // active character position within the file path string
    std::string filePath = "";                  // the current file path

    Byte path_alt_char_pos = 0;                 // active character position within the alt file path string
    std::string altFilePath = "";               // the alternate (copy/rename to) file path

    int dir_data_pos = 0;                       // position within the dir_data string
    std::string dir_data;                       // directory filename list container

    Byte  _io_data = 0;                         // data to read / write
    DWord _seek_pos = 0;                        // file seek position

};

// END: FileIO.hpp