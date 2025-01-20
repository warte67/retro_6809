/*** Math.cpp *******************************************
 *      __  __       _   _                            
 *     |  \/  |     | | | |                           
 *     | \  / | __ _| |_| |__         ___ _ __  _ __  
 *     | |\/| |/ _` | __| '_ \       / __| '_ \| '_ \ 
 *     | |  | | (_| | |_| | | |  _  | (__| |_) | |_) |
 *     |_|  |_|\__,_|\__|_| |_| (_)  \___| .__/| .__/ 
 *                                       | |   | |    
 *                                       |_|   |_|
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

#include <cfloat>
#include <cmath>
#include <time.h>		// for srand()
#include <cstring>
#include <sstream>

#include "Bus.hpp"
#include "Math.hpp"



/***************************
* Constructor / Destructor *
***************************/

Math::Math() 
{ 
    std::cout << clr::indent_push() << clr::LT_BLUE << "Math Device Created" << clr::RETURN;
    _device_name = "MATH_DEVICE"; 
} // END: Math()

Math::~Math() 
{ 
    std::cout << clr::indent_pop() << clr::LT_BLUE << "Math Device Destroyed" << clr::RETURN; 
} // END: ~Math()



/******************
* Virtual Methods *
******************/



int  Math::OnAttach(int nextAddr)
{
    SetBaseAddress(nextAddr);
    Word old_address=nextAddr;
    this->heading = "Math Co-Processor Device Hardware Registers"; 


    ////////////////////////////////////////////////
    // (Byte) MATH_ACA_POS
    //      Character Position Within the ACA Float String
    /////
    mapped_register.push_back({ "MATH_ACA_POS", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return aca_pos; }, 
        [this](Word nextAddr, Byte data) { (void)nextAddr; aca_pos = data; },
        { "(Byte) Character Position Within the ACA Float String"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MATH_ACA_DATA
    //      ACA Float String Character Port
    /////
    mapped_register.push_back({ "MATH_ACA_DATA", nextAddr, 
        [this](Word nextAddr) { 
            (void)nextAddr; 
            return _process_read_data(aca_string, aca_pos);
        }, 
        [this](Word nextAddr, Byte data) { 
            _process_write_data(nextAddr, data, aca_string, aca_pos, aca_float, aca_raw, aca_int); 
        },
        { "(Byte) ACA Float String Character Port"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (4-Bytes) MATH_ACA_RAW
    //      ACA Raw Float Data
    /////
    mapped_register.push_back({ "MATH_ACA_RAW", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_raw >> 24) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
        (void)nextAddr; 
            _process_write_raw(data, aca_raw, aca_float, aca_int, acr_string, 0xFF000000, 24); 
        },
        { "(4-Bytes) ACA Raw Float Data"} }); nextAddr++;   
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_raw >> 16) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, aca_raw, aca_float, aca_int, aca_string, 0x00FF0000, 16);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_raw >> 8) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, aca_raw, aca_float, aca_int, aca_string, 0x0000FF00, 8);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_raw >> 0) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, aca_raw, aca_float, aca_int, aca_string, 0x000000FF, 0);
        },{ "" }}); nextAddr++;


    ////////////////////////////////////////////////
    // (4-Bytes) MATH_ACA_INT
    //      ACA Integer Data
    /////
    mapped_register.push_back({ "MATH_ACA_INT", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_int >> 24) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_int(data, aca_int, aca_float, aca_raw, aca_string, 0xFF000000, 24);
        },{ "(4-Bytes) ACA Integer Data",""} }); nextAddr++;   
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_int >> 16) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_int(data, aca_int, aca_float, aca_raw, aca_string, 0x00FF0000, 16);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_int >> 8) & 0xff; }, 
        [this](Word nextAddr, DWord data) {
            (void)nextAddr; 
            _process_write_int(data, aca_int, aca_float, aca_raw, aca_string, 0x0000FF00, 8);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_int >> 0) & 0xff; }, 
        [this](Word nextAddr, DWord data) {
            (void)nextAddr; 
            _process_write_int(data, aca_int, aca_float, aca_raw, aca_string, 0x000000FF, 0);
        },{ "" }}); nextAddr++;




    ////////////////////////////////////////////////
    // (Byte) MATH_ACB_POS
    //      Character Position Within the ACB Float String
    /////
    mapped_register.push_back({ "MATH_ACB_POS", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_pos >> 0) & 0xff; }, 
        [this](Word nextAddr, DWord data) { (void)nextAddr; acb_pos = data & 0xff; }, 
        { "(Byte) Character Position Within the ACB Float String",""} }); 
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MATH_ACB_DATA
    //      ACB Float String Character Port
    /////
    mapped_register.push_back({ "MATH_ACB_DATA", nextAddr, 
        [this](Word nextAddr) { 
            (void)nextAddr; 
            return _process_read_data(acb_string, acb_pos);
        }, 
        [this](Word nextAddr, Byte data) { 
            _process_write_data(nextAddr, data, acb_string, acb_pos, acb_float, acb_raw, acb_int);         
        },
        { "(Byte) ACB Float String Character Port"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (4-Bytes) MATH_ACB_RAW
    //      ACB Raw Float Data
    /////
    mapped_register.push_back({ "MATH_ACB_RAW", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_raw >> 24) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acb_raw, acb_float, acb_int, acb_string, 0xFF000000, 24);
        },
        { "(4-Bytes) ACB Raw Float Data"} }); nextAddr++;   
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_raw >> 16) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acb_raw, acb_float, acb_int, acb_string, 0x00FF0000, 16);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_raw >> 8) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acb_raw, acb_float, acb_int, acb_string, 0x0000FF00, 8);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_raw >> 0) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acb_raw, acb_float, acb_int, acb_string, 0x000000FF, 0);
        },{ "" }}); nextAddr++;


    ////////////////////////////////////////////////
    // (4-Bytes) MATH_ACB_INT
    //      ACB Integer Data
    /////
    mapped_register.push_back({ "MATH_ACB_INT", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_int >> 24) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_int(data, acb_int, acb_float, acb_raw, acb_string, 0xFF000000, 24);
        },{ "(4-Bytes) ACB Integer Data",""} }); nextAddr++;   
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_int >> 16) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_int(data, acb_int, acb_float, acb_raw, acb_string, 0x00FF0000, 16);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acb_int >> 8) & 0xff; }, 
        [this](Word nextAddr, DWord data) {
            (void)nextAddr; 
            _process_write_int(data, acb_int, acb_float, acb_raw, acb_string, 0x0000FF00, 8);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (aca_int >> 0) & 0xff; }, 
        [this](Word nextAddr, DWord data) {
            (void)nextAddr; 
            _process_write_int(data, acb_int, acb_float, acb_raw, acb_string, 0x000000FF, 0);       
        },{ "" }}); nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MATH_ACR_POS
    //      Character Position Within the ACR Float String
    /////
    mapped_register.push_back({ "MATH_ACR_POS", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_pos >> 0) & 0xff; }, 
        [this](Word nextAddr, DWord data) { (void)nextAddr; acr_pos = data & 0xff; }, 
        { "(Byte) Character Position Within the ACR Float String",""} }); 
    nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MATH_ACR_DATA
    //      ACR Float String Character Port
    /////
    mapped_register.push_back({ "MATH_ACR_DATA", nextAddr, 
        [this](Word nextAddr) { 
            (void)nextAddr; 
            return _process_read_data(acr_string, acr_pos); 
        }, 
        [this](Word nextAddr, Byte data) { 
            _process_write_data(nextAddr, data, acr_string, acr_pos, acr_float, acr_raw, acr_int);         
        },
        { "(Byte) ACR Float String Character Port"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (4-Bytes) MATH_ACR_RAW
    //      ACR Raw Float Data
    /////
    mapped_register.push_back({ "MATH_ACR_RAW", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_raw >> 24) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acr_raw, acr_float, acr_int, acr_string, 0xFF000000, 24);
        },
        { "(4-Bytes) ACR Raw Float Data"} }); nextAddr++;   
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_raw >> 16) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acr_raw, acr_float, acr_int, acr_string, 0x00FF0000, 16);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_raw >> 8) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acr_raw, acr_float, acr_int, acr_string, 0x0000FF00, 8);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_raw >> 0) & 0xff; },
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_raw(data, acr_raw, acr_float, acr_int, acr_string, 0x000000FF, 0);
        },{ "" }}); nextAddr++;


    ////////////////////////////////////////////////
    // (4-Bytes) MATH_ACR_INT
    //      ACR Integer Data
    /////
    mapped_register.push_back({ "MATH_ACR_INT", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_int >> 24) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_int(data, acr_int, acr_float, acr_raw, acr_string, 0xFF000000, 24);       
        },{ "(4-Bytes) ACR Integer Data",""} }); nextAddr++;   
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_int >> 16) & 0xff; }, 
        [this](Word nextAddr, DWord data) { 
            (void)nextAddr; 
            _process_write_int(data, acr_int, acr_float, acr_raw, acr_string, 0x00FF0000, 16);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_int >> 8) & 0xff; }, 
        [this](Word nextAddr, DWord data) {
            (void)nextAddr; 
            _process_write_int(data, acr_int, acr_float, acr_raw, acr_string, 0x0000FF00, 8);
        },{ "" }}); nextAddr++;
    mapped_register.push_back({ "", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return (acr_int >> 0) & 0xff; }, 
        [this](Word nextAddr, DWord data) {
            (void)nextAddr; 
            _process_write_int(data, acr_int, acr_float, acr_raw, acr_string, 0x000000FF, 0);
        },{ "" }}); nextAddr++;


    ////////////////////////////////////////////////
    // (Byte) MATH_OPERATION
    //      The 'Math Operation' (MOP) to be Issued
    /////
    mapped_register.push_back({ "MATH_OPERATION", nextAddr, 
        [this](Word nextAddr) { (void)nextAddr; return math_operation; },
        [this](Word nextAddr, Byte data) { 
            (void)nextAddr;
            _process_command(data);
        },
        { "(Byte) ACA Float String Character Port   (On Write)"} });
    nextAddr++;
    // Define the MOP's
    int enum_value = 0;
    mapped_register.push_back({ "MOP_BEGIN",        enum_value++, nullptr, nullptr, {"  BEGIN Math Operation Enumeration:"}});
    mapped_register.push_back({ "MOP_RANDOM",       enum_value++, nullptr, nullptr, {"    ACA, ACB, and ACR are set to randomized values"}});
    mapped_register.push_back({ "MOP_RND_SEED",     enum_value++, nullptr, nullptr, {"    MATH_ACA_INT seeds the pseudo-random number generator"}});
    mapped_register.push_back({ "MOP_IS_EQUAL",     enum_value++, nullptr, nullptr, {"    (bool)ACR = (ACA == ACB)"}});
    mapped_register.push_back({ "MOP_IS_NOT_EQUAL", enum_value++, nullptr, nullptr, {"    (bool)ACR = (ACA != ACB)"}});
    mapped_register.push_back({ "MOP_IS_LESS",      enum_value++, nullptr, nullptr, {"    (bool)ACR = std::isless(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_IS_GREATER",   enum_value++, nullptr, nullptr, {"    (bool)ACR = std::isgreater(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_IS_LTE",       enum_value++, nullptr, nullptr, {"    (bool)ACR = std::islessequal(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_IS_GTE",       enum_value++, nullptr, nullptr, {"    (bool)ACR = std::islessgreater(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_IS_FINITE",    enum_value++, nullptr, nullptr, {"    (bool)ACR = std::isfinite(ACA)"}});
    mapped_register.push_back({ "MOP_IS_INF",       enum_value++, nullptr, nullptr, {"    (bool)ACR = std::isinf(ACA)"}});
    mapped_register.push_back({ "MOP_IS_NAN",       enum_value++, nullptr, nullptr, {"    (bool)ACR = std::isnan(ACA)"}});
    mapped_register.push_back({ "MOP_IS_NORMAL",    enum_value++, nullptr, nullptr, {"    (bool)ACR = std::isnormal(ACA)"}});
    mapped_register.push_back({ "MOP_SIGNBIT",      enum_value++, nullptr, nullptr, {"    (bool)ACR = std::signbit(ACA)"}});
    mapped_register.push_back({ "MOP_SUBTRACT",     enum_value++, nullptr, nullptr, {"    ACR = ACA - ACB"}});
    mapped_register.push_back({ "MOP_ADD",          enum_value++, nullptr, nullptr, {"    ACR = ACA + ACB"}});
    mapped_register.push_back({ "MOP_MULTIPLY",     enum_value++, nullptr, nullptr, {"    ACR = ACA * ACB"}});
    mapped_register.push_back({ "MOP_DIVIDE",       enum_value++, nullptr, nullptr, {"    ACR = ACA / ACB"}});
    mapped_register.push_back({ "MOP_FMOD",         enum_value++, nullptr, nullptr, {"    ACR = std::fmod(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_REMAINDER",    enum_value++, nullptr, nullptr, {"    ACR = std::remainder(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_FMAX",         enum_value++, nullptr, nullptr, {"    ACR = std::fmax(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_FMIN",         enum_value++, nullptr, nullptr, {"    ACR = std::fmin(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_FDIM",         enum_value++, nullptr, nullptr, {"    ACR = std::fdim(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_EXP",          enum_value++, nullptr, nullptr, {"    ACR = std::exp(ACA)"}});
    mapped_register.push_back({ "MOP_EXP2",         enum_value++, nullptr, nullptr, {"    ACR = std::exp2(ACA)"}});
    mapped_register.push_back({ "MOP_EXPM1",        enum_value++, nullptr, nullptr, {"    ACR = std::expm1(ACA)"}});
    mapped_register.push_back({ "MOP_LOG",          enum_value++, nullptr, nullptr, {"    ACR = std::log(ACA)"}});
    mapped_register.push_back({ "MOP_LOG10",        enum_value++, nullptr, nullptr, {"    ACR = std::log10(ACA)"}});
    mapped_register.push_back({ "MOP_LOG2",         enum_value++, nullptr, nullptr, {"    ACR = std::log2(ACA)"}});
    mapped_register.push_back({ "MOP_LOG1P",        enum_value++, nullptr, nullptr, {"    ACR = std::log1p(ACA)"}});
    mapped_register.push_back({ "MOP_SQRT",         enum_value++, nullptr, nullptr, {"    ACR = std::sqrt(ACA)"}});
    mapped_register.push_back({ "MOP_CBRT",         enum_value++, nullptr, nullptr, {"    ACR = std::cbrt(ACA)"}});
    mapped_register.push_back({ "MOP_HYPOT",        enum_value++, nullptr, nullptr, {"    ACR = std::hypot(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_POW",          enum_value++, nullptr, nullptr, {"    ACR = std::pow(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_SIN",          enum_value++, nullptr, nullptr, {"    ACR = std::sin(ACA)"}});
    mapped_register.push_back({ "MOP_COS",          enum_value++, nullptr, nullptr, {"    ACR = std::cos(ACA)"}});
    mapped_register.push_back({ "MOP_TAN",          enum_value++, nullptr, nullptr, {"    ACR = std::tan(ACA)"}});
    mapped_register.push_back({ "MOP_ASIN",         enum_value++, nullptr, nullptr, {"    ACR = std::asin(ACA)"}});
    mapped_register.push_back({ "MOP_ACOS",         enum_value++, nullptr, nullptr, {"    ACR = std::acos(ACA)"}});
    mapped_register.push_back({ "MOP_ATAN",         enum_value++, nullptr, nullptr, {"    ACR = std::atan(ACA)"}});
    mapped_register.push_back({ "MOP_ATAN2",        enum_value++, nullptr, nullptr, {"    ACR = std::atan2(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_SINH",         enum_value++, nullptr, nullptr, {"    ACR = std::sinh(ACA)"}});
    mapped_register.push_back({ "MOP_COSH",         enum_value++, nullptr, nullptr, {"    ACR = std::acosh(ACA)"}});
    mapped_register.push_back({ "MOP_ATANH",        enum_value++, nullptr, nullptr, {"    ACR = std::atanh(ACA)"}});
    mapped_register.push_back({ "MOP_ERF",          enum_value++, nullptr, nullptr, {"    ACR = std::erf(ACA)"}});
    mapped_register.push_back({ "MOP_ERFC",         enum_value++, nullptr, nullptr, {"    ACR = std::erfc(ACA)"}});
    mapped_register.push_back({ "MOP_LGAMMA",       enum_value++, nullptr, nullptr, {"    ACR = std::lgamma(ACA)"}});
    mapped_register.push_back({ "MOP_TGAMMA",       enum_value++, nullptr, nullptr, {"    ACR = std::tgamma(ACA)"}});
    mapped_register.push_back({ "MOP_CEIL",         enum_value++, nullptr, nullptr, {"    ACR = std::ceil(ACA)"}});
    mapped_register.push_back({ "MOP_FLOOR",        enum_value++, nullptr, nullptr, {"    ACR = std::floor(ACA)"}});
    mapped_register.push_back({ "MOP_TRUNC",        enum_value++, nullptr, nullptr, {"    ACR = std::trunc(ACA)"}});
    mapped_register.push_back({ "MOP_ROUND",        enum_value++, nullptr, nullptr, {"    ACR = std::round(ACA)"}});
    mapped_register.push_back({ "MOP_LROUND",       enum_value++, nullptr, nullptr, {"    ACR = std::lround(ACA)"}});
    mapped_register.push_back({ "MOP_NEARBYINT",    enum_value++, nullptr, nullptr, {"    ACR = std::nearbyint(ACA)"}});
    mapped_register.push_back({ "MOP_ILOGB",        enum_value++, nullptr, nullptr, {"    ACR = std::ilogb(ACA)"}});
    mapped_register.push_back({ "MOP_LOGB",         enum_value++, nullptr, nullptr, {"    ACR = std::logb(ACA)"}});
    mapped_register.push_back({ "MOP_NEXTAFTER",    enum_value++, nullptr, nullptr, {"    ACR = std::nextafter(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_COPYSIGN",     enum_value++, nullptr, nullptr, {"    ACR = std::copysign(ACA, ACB)"}});
    mapped_register.push_back({ "MOP_LASTOP",       enum_value++, nullptr, nullptr, {"  END Math Operation Enumeration"}});


    ////////////////////////////////////////////////
    // (Constant) MATH_END
    //      End of Math Co-Processor Register Space
    /////
    nextAddr--;
    mapped_register.push_back({ "MATH_END", nextAddr, 
        nullptr, nullptr,  { "End of Math Co-Processor Register Space"} });
    nextAddr++;


    ////////////////////////////////////////////////
    // (Constant) MATH_TOP
    //      Top of Math Co-Processor Register Space
    //      (start of the next device)
    /////
    mapped_register.push_back({ "MATH_TOP", nextAddr, 
    nullptr, nullptr,  { "Top of Math Co-Processor Register Space", "---"}});

    return nextAddr - old_address;
}



void Math::OnInit()
{
    math_random_seed = (DWord)std::time(NULL); 
}



Byte Math::_process_read_data(std::string& str, Byte& pos) {
    Byte data = 0;
    if (!str.empty()) {
        if (pos < str.size()) {
            data = static_cast<Byte>(str[pos++]);  // Read byte and increment position
        } else {
            str.clear();  // Reset the string when end is reached
            data = 0;
        }
    }
    return data;
}


void Math::_process_write_data(Word nextAddr, Byte data, 
                 std::string& pos_string, 
                 Byte& pos, 
                 float& float_val, 
                 DWord& raw_val, 
                 DWord& int_val) 
{
    (void)nextAddr;
    std::string valid_chars = ".-+0123456789eE";
    int found = valid_chars.find((char)data);
    if (found < 0)
        return;

    if (pos == 0)  
    {
        if (data == 'e' || data == 'E' || data == '+')
            return;

        pos_string = "";
        pos = 0;

        if (data == '-')
        {
            pos_string += data;
            pos_string += "0";
            pos++;
        }
        else
        {
            pos_string += data;
            pos++;
        }
    }
    else
    {
        pos_string = pos_string.substr(0, pos);
        pos_string += data;
        pos++;
    }

    float_val = std::stof(pos_string);
    std::memcpy(&raw_val, &float_val, sizeof(float));
    int_val = (DWord)float_val;
    if (data != 'e' && data != 'E' && data != '-')
        pos_string = std::to_string(float_val);
}


void Math::_process_write_raw(DWord data, 
                         DWord& raw_val, 
                         float& float_val, 
                         DWord& int_val, 
                         std::string& string_val, 
                         DWord mask, 
                         DWord shift)
{
    // Update the raw value with the provided data
    raw_val = (raw_val & ~mask) | (data << shift);

    // Update the float value
    std::memcpy(&float_val, &raw_val, sizeof(float));

    // Update the integer value
    int_val = (DWord)float_val;

    // Update the string representation of the float
    string_val = std::to_string(float_val);
}


void Math::_process_write_int(DWord data, unsigned int& int_val, 
                    float& float_val, DWord& raw_val, 
                    std::string& string_val, 
                    unsigned int mask, unsigned int shift)
{
    // Update the integer value with the provided data
    int_val = (int_val & ~mask) | (data << shift);

    // Update the float value from the integer
    float_val = (float)int_val;

    // Update the raw value using memcpy
    std::memcpy(&raw_val, &float_val, sizeof(float));

    // Update the string representation of the float
    string_val = std::to_string(float_val);
}















// Byte Math::_read_acc(Word offset, Word reg, Byte& _pos, 
//         float& _float, std::string& _string, DWord& _raw, DWord& _int)
// {
//     Byte data = 0xCC;
//     // bounds checking
//     if (offset < reg || offset > reg + 9)   return data;

//     if (offset == reg)              // MATH_ACx_POS
//     {
//         data = _pos;
//         if (data >= _string.size())
//             data = _string.size() - 1;
//         if (_string.size() == 0)
//             data = 0;
//     }
//     else if (offset == reg + 1)     // MATH_ACx_DATA
//     {
//         data = 0;
//         if (_string.size() > 0)
//         {
//             if (_pos < _string.size())
//             {
//                 data = (Byte)_string.substr(_pos, 1).at(0);
//                 if (_pos <= _string.size())
//                     _pos++;
//             }
//             else
//             {
//                 _string = "";
//                 data = 0;
//             }
//         }
//     }
//     // MATH_ACx_RAW
//     else if (offset == reg + 2) { data = (_raw >> 24) & 0xff; }
//     else if (offset == reg + 3) { data = (_raw >> 16) & 0xff; }
//     else if (offset == reg + 4) { data = (_raw >>  8) & 0xff; }
//     else if (offset == reg + 5) { data = (_raw >>  0) & 0xff; }
//     // MATH_ACx_INT
//     else if (offset == reg + 6) { data = (_int >> 24) & 0xff; }
//     else if (offset == reg + 7) { data = (_int >> 16) & 0xff; }
//     else if (offset == reg + 8) { data = (_int >>  8) & 0xff; }
//     else if (offset == reg + 9) { data = (_int >>  0) & 0xff; }

//     return data;
// }   // END Math::_read_acc()


// Byte Math::_write_acc(Word offset, Byte data, Word reg, Byte& _pos, 
//         float& _float, std::string& _string, DWord& _raw, DWord& _int)
// {
//     // position within the string
//     if (offset == reg + 0)              // MATH_ACx_POS
//     {
//         if (data > _string.size())
//             data = _string.size() - 1;
//         _pos = data;
//     }
//     // update the string itself
//     if (offset == reg + 1)              // MATH_ACx_DATA
//     {
//         std::string valid_chars = ".-+0123456789eE";
//         int found = valid_chars.find((char)data);
//         if (found < 0)
//             return Memory::Read(offset);

//         // at the beginning of the string?
//         if (_pos == 0)  
//         {
//             if (data == 'e' || data == 'E' || data == '+')
//                 return Memory::Read(offset);
//             _string = "";
//             _pos = 0;
//             if (data == '-')
//             {
//                 _string += data;
//                 _string += "0";
//                 _pos++;
//             }
//             else
//             {
//                 _string += data;
//                 _pos++;
//             }
//         }
//         else // within the string
//         {
//             _string = _string.substr(0, _pos);
//             _string += data;
//             _pos++;
//         }
//         // convert string to float
//         _float = std::stof(_string);    
//         // update MATH_ACx_RAW
//         _raw = *reinterpret_cast<DWord*>(&_float);
//         // update MATH_ACx_INT
//         _int = (DWord)_float;
//         // update the string
//         if (data != 'e' && data != 'E' && data != '-')
//             _string = std::to_string(_float);
//     }
//     // MATH_ACx_RAW
//     if (offset >= reg + 2 && offset <= reg + 5)
//     {
//         if (offset == reg + 2)
//             _raw = (_raw & ~0xff000000) | (data << 24);
//         if (offset == reg + 3)
//             _raw = (_raw & ~0x00ff0000) | (data << 16);
//         if (offset == reg + 4)
//             _raw = (_raw & ~0x0000ff00) | (data <<  8);
//         if (offset == reg + 5)
//             _raw = (_raw & ~0x000000ff) | (data <<  0);
//         // update the internal float
//         _float = *reinterpret_cast<float*>(&_raw);
//         // update the internal int
//         _int = (DWord)_float;
//         // update the string
//         _string = std::to_string(_float);
//     }
//     // MATH_ACx_INT
//     if (offset >= reg + 6 && offset <= reg + 9)
//     {
//         if (offset == reg + 6)
//             _int = (_int & ~0xff000000) | (data << 24);
//         if (offset == reg + 7)
//             _int = (_int & ~0x00ff0000) | (data << 16);
//         if (offset == reg + 8)
//             _int = (_int & ~0x0000ff00) | (data << 8);
//         if (offset == reg + 9)
//             _int = (_int & ~0x000000ff) | (data << 0);
//         // update the internal float
//         _float = (float)_int;
//         // update the internal raw
//         _raw = *reinterpret_cast<DWord*>(&_float);
//         // update the string
//         _string = std::to_string(_float);
//     }
//     // printf("MATH_ACx_RAW ---> _float:%f    _raw: $%08X    _int: $%08X    _string:%s\n", _float, _raw, _int, _string.c_str());            

//     return data;
// }


void Math::_process_command(Byte data)
{
    //data = math_operation;
    if (data == MAP(MOP_RANDOM))
    {
        srand(math_random_seed);
        math_random_seed = rand();

        _update_regs_from_float(((float)rand() / (RAND_MAX)),
            aca_float, aca_pos, aca_string, aca_raw, aca_int);
        aca_int = rand() << 16 | rand();
        _update_regs_from_float(((float)rand() / (RAND_MAX)),
            acb_float, acb_pos, acb_string, acb_raw, acb_int);
        acb_int = rand() << 16 | rand();
        _update_regs_from_float(((float)rand() / (RAND_MAX)),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);

        acr_int = rand() << 16 | rand();
    }
    else if (data == MAP(MOP_RND_SEED))
    {
        math_random_seed = aca_int;
        srand(math_random_seed);
    }
    else if (data == MAP(MOP_IS_EQUAL))
    {
        acr_int = (bool)(aca_float == acb_float);
    }
    else if (data == MAP(MOP_IS_NOT_EQUAL))
    {
        acr_int = (bool)(aca_float != acb_float);
    }
    else if (data == MAP(MOP_IS_LESS))
    {
        acr_int = (bool)(aca_float < acb_float);
    }
    else if (data == MAP(MOP_IS_GREATER))
    {
        acr_int = (bool)(aca_float > acb_float);
    }
    else if (data == MAP(MOP_IS_LTE))
    {
        acr_int = (bool)(aca_float <= acb_float);
    }
    else if (data == MAP(MOP_IS_GTE))
    {
        acr_int = (bool)(aca_float >= acb_float);
    }
    else if (data == MAP(MOP_IS_FINITE))
    {
        acr_int = (bool)(std::isfinite(aca_float));
    }
    else if (data == MAP(MOP_IS_INF))
    {
        acr_int = (bool)(std::isinf(aca_float));
    }
    else if (data == MAP(MOP_IS_NAN))
    {
        acr_int = (bool)(std::isnan(aca_float));
    }
    else if (data == MAP(MOP_IS_NORMAL))
    {
        acr_int = (bool)(std::isnormal(aca_float));
    }
    else if (data == MAP(MOP_SIGNBIT))
    {
        acr_int = (bool)(std::signbit(aca_float));
    }
    else if (data == MAP(MOP_SUBTRACT))
    {
        _update_regs_from_float((aca_float - acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ADD))
    {
        _update_regs_from_float((aca_float + acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_MULTIPLY))
    {
        _update_regs_from_float((aca_float * acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_DIVIDE))
    {
        _update_regs_from_float((aca_float / acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_FMOD))
    {
        _update_regs_from_float(std::fmod(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_REMAINDER))
    {
        _update_regs_from_float(std::remainder(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_FMAX))
    {
        _update_regs_from_float(std::fmax(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_FMIN))
    {
        _update_regs_from_float(std::fmin(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_FDIM))
    {
        _update_regs_from_float(std::fdim(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_EXP))
    {
        _update_regs_from_float(std::exp(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_EXP2))
    {
        _update_regs_from_float(std::exp2(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_EXPM1))
    {
        _update_regs_from_float(std::expm1(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LOG))
    {
        _update_regs_from_float(std::log(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LOG10))
    {
        _update_regs_from_float(std::log10(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LOG2))
    {
        _update_regs_from_float(std::log2(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LOG1P))
    {
        _update_regs_from_float(std::log1p(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_SQRT))
    {
        _update_regs_from_float(std::sqrt(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_CBRT))
    {
        _update_regs_from_float(std::cbrt(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_HYPOT))
    {
        _update_regs_from_float(std::hypot(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_POW))
    {
        _update_regs_from_float(std::pow(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_SIN))
    {
        _update_regs_from_float(std::sin(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_COS))
    {
        _update_regs_from_float(std::cos(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_TAN))
    {
        _update_regs_from_float(std::tan(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ASIN))
    {
        _update_regs_from_float(std::asin(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ACOS))
    {
        _update_regs_from_float(std::acos(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ATAN))
    {
        _update_regs_from_float(std::atan(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ATAN2))
    {
        _update_regs_from_float(std::atan2(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_COSH))
    {
        _update_regs_from_float(std::acosh(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ATANH))
    {
        _update_regs_from_float(std::atanh(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ERF))
    {
        _update_regs_from_float(std::erf(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ERFC))
    {
        _update_regs_from_float(std::erfc(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LGAMMA))
    {
        _update_regs_from_float(std::lgamma(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_TGAMMA))
    {
        _update_regs_from_float(std::tgamma(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_CEIL))
    {
        _update_regs_from_float(std::ceil(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_FLOOR))
    {
        _update_regs_from_float(std::floor(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_TRUNC))
    {
        _update_regs_from_float(std::trunc(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ROUND))
    {
        _update_regs_from_float(std::round(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LROUND))
    {
        _update_regs_from_float(std::lround(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_NEARBYINT))
    {
        _update_regs_from_float(std::nearbyint(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ILOGB))
    {
        _update_regs_from_float(std::ilogb(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_LOGB))
    {
        _update_regs_from_float(std::logb(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_NEXTAFTER))
    {
        _update_regs_from_float(std::nextafter(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_COPYSIGN))
    {
        _update_regs_from_float(std::copysign(aca_float, acb_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
}



void Math::_update_regs_from_float(float f_data, float& _float,
    Byte& _pos, std::string& _string, DWord& _raw, DWord& _int)
{
    // Reset position
    _pos = 0;
    
    // Update floating-point value
    _float = f_data;
    
    // Convert to string with controlled precision
    std::ostringstream stream;
    stream.precision(7);  // For example, to limit precision of the string output
    stream << std::fixed << _float;  // Fixed-point representation
    _string = stream.str();

    // Convert to raw memory (reinterpret as raw bits)
    std::memcpy(&_raw, &_float, sizeof(float));

    // Store integer value (possible truncation of float to int)
    _int = static_cast<DWord>(_float);    
}


// END: Math.cpp
