/*** Math.hpp *******************************************
 *      __  __       _   _           _                 
 *     |  \/  |     | | | |         | |                
 *     | \  / | __ _| |_| |__       | |__  _ __  _ __  
 *     | |\/| |/ _` | __| '_ \      | '_ \| '_ \| '_ \ 
 *     | |  | | (_| | |_| | | |  _  | | | | |_) | |_) |
 *     |_|  |_|\__,_|\__|_| |_| (_) |_| |_| .__/| .__/ 
 *                                        | |   | |    
 *                                        |_|   |_| 
 *    
 * The Math device serves as a math co-processor for the
 * Retro 6809, a 1980's era single board computer. This
 * Device is a child of the IDevice class and is part of
 * the Retro 6809 emulation system.
 *
 * Released under the GPL v3.0 License.
 * Original Author: Jay Faries (warte67)
 *
 ************************************/

#pragma once

#include "IDevice.hpp"

class Math : public IDevice {

public: // PUBLIC CONSTRUCTOR / DESTRUCTOR
    Math();
    virtual ~Math();

public: // VIRTUAL METHODS

    virtual int  OnAttach(int nextAddr) override;   // attach to the memory map
    virtual bool OnTest() override;                 // return true for successful unit tests
    virtual void OnInit() override;                 // initialize    

    // not used
    virtual void OnQuit() override {};
    virtual void OnActivate() override {};
    virtual void OnDeactivate() override {};
    virtual void OnEvent(SDL_Event* evnt) override { (void)evnt; }
    virtual void OnUpdate(float fElapsedTime) override { (void)fElapsedTime;};
    virtual void OnRender() override {};
 
public: // PUBLIC ACCESSORS
    // ... 

private: // PRIVATE MEMBERS

    // ACA:  Float Accumilator A
    float aca_float = 0.0f;		// the ACA working float
    Byte aca_pos = 0;			// character position within ACA string
    std::string aca_string = "";	// internal ACA float string
    DWord aca_raw = 0;			// internal ACA raw float data
    DWord aca_int = 0;			// internal ACA integer data

    // ACB:  Float Accumilator B
    float acb_float = 0.0f;		// the ACB working float
    Byte acb_pos = 0;			// character position within ACB string
    std::string acb_string = "";	// internal ACB float string
    DWord acb_raw = 0;			// internal ACB raw float data
    DWord acb_int = 0;			// internal ACB integer data

    // ACR:  Float Accumilator R (return)
    float acr_float = 0.0f;		// the ACR working float
    Byte acr_pos = 0;			// character position within ACR string
    std::string acr_string = "";	// internal ACR float string
    DWord acr_raw = 0;			// internal ACR raw float data
    DWord acr_int = 0;			// internal ACR integer data

    // MATH_OPERATION
    Byte math_operation = 0;	// Operation 'command' 
    DWord math_random_seed = 0;	// random seed

    // HELPERS

    Byte _process_read_data(std::string& str, Byte& pos);


    void _process_write_data(Word nextAddr, Byte data, 
                 std::string& pos_string, 
                 Byte& pos, 
                 float& float_val, 
                 DWord& raw_val, 
                 DWord& int_val);

    void _process_write_raw(DWord data, 
                         DWord& raw_val, 
                         float& float_val, 
                         DWord& int_val, 
                         std::string& string_val, 
                         DWord mask, 
                         DWord shift);

    void _process_write_int(DWord data, unsigned int& int_val, 
                    float& float_val, DWord& raw_val, 
                    std::string& string_val, 
                    unsigned int mask, unsigned int shift);

    void _process_command(Byte data);


    // Byte _read_acc(Word offset, Word reg, Byte& _pos, 
    //         float& _float, std::string& _string, DWord& _raw, DWord& _int);
    // Byte _write_acc(Word offset, Byte data, Word reg, Byte& _pos, 
    //         float& _float, std::string& _string, DWord& _raw, DWord& _int);
    void _update_regs_from_float(float f_data, float& _float,
            Byte& _pos, std::string& _string, DWord& _raw, DWord& _int);
};

// END: Math.hpp