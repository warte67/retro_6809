/* FileIO.h ***************************************
 *
 *   Acts to emulate a Raspberry PI Pico
 *		Implementing File Input / Output
 *		with the onboad flash ram and
 *		the SDCARD
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#pragma once
#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <fstream>
#include "Device.h"

class FileIO : public Device
{
public:
    FileIO() { _deviceName = "FileIO"; }
    virtual ~FileIO() {}

    // virtual overrides
    Word OnAttach(Word nextAddr) override;
    void OnInit() override;
    void OnQuit() override;
    void OnActivate() override;
    void OnDeactivate() override;
    void OnEvent(SDL_Event* evnt) override;
    void OnUpdate(float fElapsedTime) override;
    void OnRender() override;
    // register access
    Byte read(Word offset, bool debug = false) override;
    void write(Word offset, Byte data, bool debug = false) override;

protected:

    //enum FMODE
    //{
    //    NONE,
    //    INPUT,
    //    OUTPUT,
    //    BINARY,
    //    AT_END,
    //    APPEND,
    //    TRUNC,
    //};
    //struct FILE_NODE
    //{
    //    std::fstream* ptrStream = nullptr;
    //    FMODE fmode = NONE;
    //    std::string path = "";
    //};


private:
    const int _FSTREAM_MAX = 16;    // max number of file streams
    Byte _fileStreamIndex = 0;      // file stream index
    std::vector<std::fstream*> _vecFileStreams; // vector of file streams

    Byte path_char_index = 0;   // active character position within the file path string
    std::string filePath = "";  // the current file path

    int dir_data_index = 0;     // index to within the dir_data string
    std::string dir_data;       // directory filename list container
};




#endif // __FILEIO_H__

