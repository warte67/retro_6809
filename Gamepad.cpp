/**** Gamepad.cpp ***************************************
 *
 * The Gamepad / Joystick device interface.
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#include "Bus.h"
#include "Gamepad.h"

Byte Gamepad::read(Word offset, bool debug)
{
    Byte data = 0xCC;

    switch (offset)
    {
    default:    break;
    }

    // write statically and return
    Bus::Write(offset, data, true);
    return data;
}

void Gamepad::write(Word offset, Byte data, bool debug)
{
    switch (offset)
    {
    default:  break;
    }

    // write statically
    Bus::Write(offset, data, true);
}

Word Gamepad::OnAttach(Word nextAddr)
{
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("JOYS_BEGIN",   nextAddr, "Start of the Game Controller Register space");
    nextAddr += 0;

    DisplayEnum("JOYS_1_BTN",   nextAddr, "  (Word) button bits: room for up to 16 buttons  (realtime)");
    nextAddr += 2;
    DisplayEnum("JOYS_1_DBND",  nextAddr, "  (Byte) PAD 1 analog deadband; default is 5   (read/write)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_LTX",   nextAddr, "  (char) PAD 1 LThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_LTY",   nextAddr, "  (char) PAD 1 LThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_RTX",   nextAddr, "  (char) PAD 1 RThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_RTY",   nextAddr, "  (char) PAD 1 RThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_Z1",    nextAddr, "  (char) PAD 1 left analog trigger (0 - 127)     (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_1_Z2",    nextAddr, "  (char) PAD 1 right analog trigger (0 - 127)    (realtime)");
    nextAddr += 1;

    DisplayEnum("", 0, "");

    DisplayEnum("JOYS_2_BTN",   nextAddr, "  (Word) button bits: room for up to 16 buttons  (realtime)");
    nextAddr += 2;                           
    DisplayEnum("JOYS_2_DBND",  nextAddr, "  (Byte) PAD 2 analog deadband; default is 5   (read/write)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_LTX",   nextAddr, "  (char) PAD 2 LThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_LTY",   nextAddr, "  (char) PAD 2 LThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_RTX",   nextAddr, "  (char) PAD 2 RThumb-X position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_RTY",   nextAddr, "  (char) PAD 2 RThumb-Y position (-128 _ +127)   (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_Z1",    nextAddr, "  (char) PAD 2 left analog trigger (0 - 127)     (realtime)");
    nextAddr += 1;                           
    DisplayEnum("JOYS_2_Z2",    nextAddr, "  (char) PAD 2 right analog trigger (0 - 127)    (realtime)");
    nextAddr += 1;

    DisplayEnum("JOYS_END", nextAddr, "End of the Game Controller Register space");
    nextAddr += 0;

    return nextAddr - old_addr;
}

void Gamepad::OnInit()
{
    // printf("Gamepad::OnInit();\n");
}

void Gamepad::OnQuit()
{
    // printf("Gamepad::OnQuit();\n");
}

void Gamepad::OnActivate()
{
    // printf("Gamepad::OnActivate();\n");
}

void Gamepad::OnDeactivate()
{
    // printf("Gamepad::OnDeactivate();\n");
}

void Gamepad::OnEvent(SDL_Event* evnt)
{
}

void Gamepad::OnUpdate(float fElapsedTime)
{
}



