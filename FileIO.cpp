/* FileIO.cpp ***************************************
 *
 *   Acts to emulate a Raspberry PI Pico
 *		Implementing File Input / Output
 *		with the onboad flash ram and
 *		the SDCARD
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#include "Bus.h"
#include "FileIO.h"


Byte FileIO::read(Word offset, bool debug)
{
	Byte data = 0xCC;	// default unassigned memory


	// update and return
	Bus::Write(offset, data, true);
	return data;
}

void FileIO::write(Word offset, Byte data, bool debug)
{

	// write statically
	Bus::Write(offset, data, true);
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
    DisplayEnum("", 0, "     G:  too many file streams");
    DisplayEnum("", 0, "     H:  incorrect file stream");
    nextAddr += 1;

    DisplayEnum("FIO_COMMAND", nextAddr, "(Byte) OnWrite, execute a file command");
    DisplayEnum("", 0, "     $00:  Reset/Null");
    DisplayEnum("", 0, "     $00:  SYSTEM: Shutdown");
    DisplayEnum("", 0, "     $00:  SYSTEM: Load Compilation Date");
    DisplayEnum("", 0, "     $00:  New File Stream");
    DisplayEnum("", 0, "     $00:  Open File");
    DisplayEnum("", 0, "     $00:  Is File Open? (returns FIO_ERR_FLAGS bit-5)");
    DisplayEnum("", 0, "     $00:  Close File");
    DisplayEnum("", 0, "     $00:  Read Byte");
    DisplayEnum("", 0, "     $00:  Write Byte");
    DisplayEnum("", 0, "     $00:  Load Hex Format File");
    DisplayEnum("", 0, "     $00:  Get File Length");
    DisplayEnum("", 0, "     $00:  Load Binary File to address FIO_BFRADR");
    DisplayEnum("", 0, "     $00:  Save Binary File FIO_BFRLEN bytes from FIO_BFRADR");
    DisplayEnum("", 0, "     $00:  List Directory");
    DisplayEnum("", 0, "     $00:  Make Directory");
    DisplayEnum("", 0, "     $00:  Change Directory");
    DisplayEnum("", 0, "     $00:  Rename Directory");
    DisplayEnum("", 0, "     $00:  Remove Directory");
    DisplayEnum("", 0, "     $00:  Delete File");
    DisplayEnum("", 0, "     $00:  Rename file");
    DisplayEnum("", 0, "     $00:  Copy File");
    DisplayEnum("", 0, "     $00:  Seek Start");
    DisplayEnum("", 0, "     $00:  Seek End");
    DisplayEnum("", 0, "     $00:  Set Seek Position"); // std::streampos?
    DisplayEnum("", 0, "     $00:  Get Seek Position"); // std::streampos?
    nextAddr += 1;

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

    DisplayEnum("FIO_IODATA", nextAddr, "(Byte) input / output character");
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

	return Word();
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

