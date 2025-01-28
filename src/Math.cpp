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
#include <string>
#include <cstring>
#include <sstream>

#include "Bus.hpp"
#include "Math.hpp"
#include "UnitTest.hpp"



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
        { "(Byte) Character Position Within the ACB Float String"} }); 
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
        { "(Byte) Character Position Within the ACR Float String"} }); 
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
    mapped_register.push_back({ "MOP_BEGIN",        enum_value  , nullptr, nullptr, {"  BEGIN Math Operation Enumeration:"}});
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
    mapped_register.push_back({ "MOP_COSH",         enum_value++, nullptr, nullptr, {"    ACR = std::cosh(ACA)"}});
    mapped_register.push_back({ "MOP_ASINH",        enum_value++, nullptr, nullptr, {"    ACR = std::asinh(ACA)"}});
    mapped_register.push_back({ "MOP_ACOSH",        enum_value++, nullptr, nullptr, {"    ACR = std::acosh(ACA)"}});
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
    _size = nextAddr - old_address;
    return _size;
}


bool Math::OnTest() 
{ 
    bool test_results = true;

    UnitTest::Log(this, "Testing ...");

    // Check the number of mapped registers
    size_t expectedRegisters = 94; // Number of interrupt vectors
    std::string err = _device_name + ": Incorrect number of mapped registers. Was " + std::to_string(expectedRegisters) + 
                                    " should be " + std::to_string(mapped_register.size()) + ".";
    ASSERT(mapped_register.size() == expectedRegisters, err);

    // Unit Tests
    if (!_test_accumilators()) {
        test_results = false;
    }

    if (!_test_math_operations()) {
        test_results = false;
    }

    // display the result of the tests
    if (test_results)
        UnitTest::Log(this, "Unit Tests PASSED");
    else
        UnitTest::Log(this, clr::RED + "Unit Tests FAILED");
    return test_results;
}

bool Math::_test_accumilators()
{
    // Vectors for the three sets of accumulators
    std::vector<Word> math_acx_pos  = { MAP(MATH_ACA_POS), MAP(MATH_ACB_POS), MAP(MATH_ACR_POS) };
    std::vector<Word> math_acx_data = { MAP(MATH_ACA_DATA), MAP(MATH_ACB_DATA), MAP(MATH_ACR_DATA) };
    std::vector<Word> math_acx_raw  = { MAP(MATH_ACA_RAW), MAP(MATH_ACB_RAW), MAP(MATH_ACR_RAW) };
    std::vector<Word> math_acx_int  = { MAP(MATH_ACA_INT), MAP(MATH_ACB_INT), MAP(MATH_ACR_INT) };
    std::vector<char> reg_char = { 'A', 'B', 'R' };

    // Test string to be written to MATH_ACx_DATA
    std::string input_str = "123.45";  // Example string value

    bool all_tests_passed = true;  // Flag to track if all tests pass

    // Loop through each of the accumulators (ACA, ACB, ACR)
    for (size_t i = 0; i < math_acx_pos.size(); ++i) {
        Word math_aca_pos = math_acx_pos[i];
        Word math_aca_data = math_acx_data[i];
        Word math_aca_raw = math_acx_raw[i];
        Word math_aca_int = math_acx_int[i];

        // Step 1: Clear MATH_ACx_POS to reset position
        Memory::Write(math_aca_pos, (Byte)0);

        // Step 2: Write each character of the string to MATH_ACx_DATA
        for (char c : input_str) {
            Memory::Write(math_aca_data, static_cast<Byte>(c)); // Write character one by one
        }

        // Step 3: Write 0 after the last character to indicate null-terminator
        Memory::Write(math_aca_data, (Byte)0);

        // Step 4: Verify the output in MATH_ACx_RAW and MATH_ACx_INT
        // The raw value (MATH_ACx_RAW) should represent the string "123.45" as a float in IEEE 754 format
        float expected_raw_value = std::stof(input_str); // Convert string to float
        DWord raw_result = Memory::Read_DWord(math_aca_raw); // Read raw float value from MATH_ACx_RAW register

        // Correct conversion of float to DWord using memcpy
        DWord expected_raw_value_dword;
        memcpy(&expected_raw_value_dword, &expected_raw_value, sizeof(float)); // Convert float to raw DWord representation

        // Log mismatch with expected vs actual value
        if (!ASSERT_TRUE(raw_result == expected_raw_value_dword)) {
            all_tests_passed = false;
            UnitTest::Log(this, clr::RED + "register MATH_AC" + reg_char[i] + "_RAW value mismatch: Expected $" +
                clr::hex(expected_raw_value_dword,8) + ", Got $" + clr::hex(raw_result,8));
        }
        else {
            UnitTest::Log(this, "register " + clr::YELLOW + "MATH_AC" + reg_char[i] + "_RAW" + clr::GREEN + " Success with $" + clr::hex(raw_result,8));
        }

        // The integer value (MATH_ACx_INT) should be the integer representation of the float (e.g., 123 from 123.45)
        DWord expected_int_value = static_cast<DWord>(expected_raw_value); // Convert float to DWord (as integer part)
        DWord int_result = Memory::Read_DWord(math_aca_int); // Read integer value from MATH_ACx_INT register

        // Log mismatch with expected vs actual value
        if (!ASSERT_TRUE(int_result == expected_int_value)) {
            all_tests_passed = false;
            UnitTest::Log(this, clr::RED + "register MATH_AC" + reg_char[i] + "_INT value mismatch: Expected " +
                clr::hex(expected_int_value,8) + ", Got $" + clr::hex(int_result,8));
        }
        else {
            UnitTest::Log(this, "register " + clr::YELLOW + "MATH_AC" + reg_char[i] + "_INT" + clr::GREEN + " Success with $" + clr::hex(int_result,8));
        }
    }

    return all_tests_passed;
}

bool Math::_test_math_operations()
{
    bool all_tests_passed = true;

    struct mop_node
    {
        Word mop_value;
        std::string name;   
        std::function<float(float, float)> func;
    };

    // Define the vector of MOP operations using mop_node structs with lambda functions
    std::vector<mop_node> mop_operations = {
        { MAP(MOP_RANDOM),       "MOP_RANDOM",      [](float, float) { return true; }},  // Placeholder for MOP_RANDOM (it does not need params)
        { MAP(MOP_RND_SEED),     "MOP_RND_SEED",    [](float, float) { return true; }},  // Placeholder for MOP_RANDOM (it does not need params)
        { MAP(MOP_IS_EQUAL),     "MOP_IS_EQUAL",    [](float aca, float acb)    { return aca == acb; }},
        { MAP(MOP_IS_NOT_EQUAL), "MOP_IS_NOT_EQUAL",[](float aca, float acb)    { return aca != acb; }},
        { MAP(MOP_IS_LESS),      "MOP_IS_LESS",     [](float aca, float acb)    { return aca < acb; }},
        { MAP(MOP_IS_GREATER),   "MOP_IS_GREATER",  [](float aca, float acb)    { return aca > acb; }},
        { MAP(MOP_IS_LTE),       "MOP_IS_LTE",      [](float aca, float acb)    { return aca <= acb; }},
        { MAP(MOP_IS_GTE),       "MOP_IS_GTE",      [](float aca, float acb)    { return aca >= acb; }},
        { MAP(MOP_IS_FINITE),    "MOP_IS_FINITE",   [](float aca, float)        { return std::isfinite(aca); }},
        { MAP(MOP_IS_INF),       "MOP_IS_INF",      [](float aca, float)        { return std::isinf(aca); }},
        { MAP(MOP_IS_NAN),       "MOP_IS_NAN",      [](float aca, float)        { return std::isnan(aca); }},
        { MAP(MOP_IS_NORMAL),    "MOP_IS_NORMAL",   [](float aca, float)        { return std::isnormal(aca); }},
        { MAP(MOP_SIGNBIT),      "MOP_SIGNBIT",     [](float aca, float)        { return std::signbit(aca); }},
        { MAP(MOP_SUBTRACT),     "MOP_SUBTRACT",    [](float aca, float acb)    { return aca - acb; }},
        { MAP(MOP_ADD),          "MOP_ADD",         [](float aca, float acb)    { return aca + acb; }},
        { MAP(MOP_MULTIPLY),     "MOP_MULTIPLY",    [](float aca, float acb)    { return aca * acb; }},
        { MAP(MOP_DIVIDE),       "MOP_DIVIDE",      [](float aca, float acb)    { return aca / acb; }},
        { MAP(MOP_FMOD),         "MOP_FMOD",        [](float aca, float acb)    { return std::fmod(aca, acb); }},
        { MAP(MOP_REMAINDER),    "MOP_REMAINDER",   [](float aca, float acb)    { return std::remainder(aca, acb); }},
        { MAP(MOP_FMAX),         "MOP_FMAX",        [](float aca, float acb)    { return std::fmax(aca, acb); }},
        { MAP(MOP_FMIN),         "MOP_FMIN",        [](float aca, float acb)    { return std::fmin(aca, acb); }},
        { MAP(MOP_FDIM),         "MOP_FDIM",        [](float aca, float acb)    { return std::fdim(aca, acb); }},
        { MAP(MOP_EXP),          "MOP_EXP",         [](float aca, float)        { return std::exp(aca); }},
        { MAP(MOP_EXP2),         "MOP_EXP2",        [](float aca, float)        { return std::exp2(aca); }},
        { MAP(MOP_EXPM1),        "MOP_EXPM1",       [](float aca, float)        { return std::expm1(aca); }},
        { MAP(MOP_LOG),          "MOP_LOG",         [](float aca, float)        { return std::log(aca); }},
        { MAP(MOP_LOG10),        "MOP_LOG10",       [](float aca, float)        { return std::log10(aca); }},
        { MAP(MOP_LOG2),         "MOP_LOG2",        [](float aca, float)        { return std::log2(aca); }},
        { MAP(MOP_LOG1P),        "MOP_LOG1P",       [](float aca, float)        { return std::log1p(aca); }},
        { MAP(MOP_SQRT),         "MOP_SQRT",        [](float aca, float)        { return std::sqrt(aca); }},
        { MAP(MOP_CBRT),         "MOP_CBRT",        [](float aca, float)        { return std::cbrt(aca); }},
        { MAP(MOP_HYPOT),        "MOP_HYPOT",       [](float aca, float acb)    { return std::hypot(aca, acb); }},
        { MAP(MOP_POW),          "MOP_POW",         [](float aca, float acb)    { return std::pow(aca, acb); }},
        { MAP(MOP_SIN),          "MOP_SIN",         [](float aca, float)        { return std::sin(aca); }},
        { MAP(MOP_COS),          "MOP_COS",         [](float aca, float)        { return std::cos(aca); }},
        { MAP(MOP_TAN),          "MOP_TAN",         [](float aca, float)        { return std::tan(aca); }},
        { MAP(MOP_ASIN),         "MOP_ASIN",        [](float aca, float)        { return std::asin(aca); }},
        { MAP(MOP_ACOS),         "MOP_ACOS",        [](float aca, float)        { return std::acos(aca); }},
        { MAP(MOP_ATAN),         "MOP_ATAN",        [](float aca, float)        { return std::atan(aca); }},
        { MAP(MOP_ATAN2),        "MOP_ATAN2",       [](float aca, float acb)    { return std::atan2(aca, acb); }},
        { MAP(MOP_SINH),         "MOP_SINH",        [](float aca, float)        { return std::sinh(aca); }},
        { MAP(MOP_COSH),         "MOP_COSH",        [](float aca, float)        { return std::cosh(aca); }},
        { MAP(MOP_ASINH),        "MOP_ASINH",       [](float aca, float)        { return std::asinh(aca); }},
        { MAP(MOP_ACOSH),        "MOP_ACOSH",       [](float aca, float)        { return std::acosh(aca); }},
        { MAP(MOP_ATANH),        "MOP_ATANH",       [](float aca, float)        { return std::atanh(aca); }},
        { MAP(MOP_ERF),          "MOP_ERF",         [](float aca, float)        { return std::erf(aca); }},
        { MAP(MOP_ERFC),         "MOP_ERFC",        [](float aca, float)        { return std::erfc(aca); }},
        { MAP(MOP_LGAMMA),       "MOP_LGAMMA",      [](float aca, float)        { return std::lgamma(aca); }},
        { MAP(MOP_TGAMMA),       "MOP_TGAMMA",      [](float aca, float)        { return std::tgamma(aca); }},
        { MAP(MOP_CEIL),         "MOP_CEIL",        [](float aca, float)        { return std::ceil(aca); }},
        { MAP(MOP_FLOOR),        "MOP_FLOOR",       [](float aca, float)        { return std::floor(aca); }},
        { MAP(MOP_TRUNC),        "MOP_TRUNC",       [](float aca, float)        { return std::trunc(aca); }},
        { MAP(MOP_ROUND),        "MOP_ROUND",       [](float aca, float)        { return std::round(aca); }},
        { MAP(MOP_LROUND),       "MOP_LROUND",      [](float aca, float)        { return std::lround(aca); }},
        { MAP(MOP_NEARBYINT),    "MOP_NEARBYINT",   [](float aca, float)        { return std::nearbyint(aca); }},
        { MAP(MOP_ILOGB),        "MOP_ILOGB",       [](float aca, float)        { return std::ilogb(aca); }},
        { MAP(MOP_LOGB),         "MOP_LOGB",        [](float aca, float)        { return std::logb(aca); }},
        { MAP(MOP_NEXTAFTER),    "MOP_NEXTAFTER",   [](float aca, float acb)    { return std::nextafter(aca, acb); }},
        { MAP(MOP_COPYSIGN),     "MOP_COPYSIGN",    [](float aca, float acb)    { return std::copysign(aca, acb); }},
    };    

    // Iterate through each MOP and perform the corresponding test
    // for (DWord mop : mop_values) 
    for (mop_node op : mop_operations) 
    {
        DWord mop = op.mop_value;

        // Prepare input values based on the operation type
        float aca_value = 0.0f, acb_value = 0.0f;

        // Handle different MOP types based on the type of operation (unary vs binary)
        if (mop == MAP(MOP_RANDOM)) 
        {
            int iterations = 10;
            int fail_count = 0;
            for (int i = 0; i < iterations; i++) 
            {
                Memory::Write(MAP(MATH_OPERATION), (Byte)MAP(MOP_RANDOM)); // call the MOP_RANDOM operation
                // all three accumilators should now contain random values
                DWord rnd_aca_raw = Memory::Read_DWord(MAP(MATH_ACA_RAW));
                DWord rnd_aca_int = Memory::Read_DWord(MAP(MATH_ACA_INT));
                DWord rnd_acb_raw = Memory::Read_DWord(MAP(MATH_ACB_RAW));
                DWord rnd_acb_int = Memory::Read_DWord(MAP(MATH_ACB_INT));
                DWord rnd_acr_raw = Memory::Read_DWord(MAP(MATH_ACR_RAW));
                DWord rnd_acr_int = Memory::Read_DWord(MAP(MATH_ACR_INT));

                Memory::Write(MAP(MATH_OPERATION), (Byte)MAP(MOP_RANDOM)); // call the MOP_RANDOM operation, again
                // all three accumilators should now contain new random values
                if (rnd_aca_raw == Memory::Read_DWord(MAP(MATH_ACA_RAW)) || 
                    rnd_aca_int == Memory::Read_DWord(MAP(MATH_ACA_INT)) || 
                    rnd_acb_raw == Memory::Read_DWord(MAP(MATH_ACB_RAW)) || 
                    rnd_acb_int == Memory::Read_DWord(MAP(MATH_ACB_INT)) || 
                    rnd_acr_raw == Memory::Read_DWord(MAP(MATH_ACR_RAW)) || 
                    rnd_acr_int == Memory::Read_DWord(MAP(MATH_ACR_INT)))
                {
                    fail_count++;
                }    
            }
            if (fail_count == iterations) 
            {
                all_tests_passed = false;
                UnitTest::Log(this, clr::RED + "operation " + op.name + " test FAILED: " + std::to_string(fail_count));
            }
            else 
            {
                UnitTest::Log(this, "operation " + clr::YELLOW + op.name + clr::GREEN + " PASSED!");
            }
        } 
        else if (mop == MAP(MOP_RND_SEED)) 
        {
            int iterations = 10;
            int fail_count = 0;
            for (int i = 0; i < iterations; i++) 
            { 
                // randomize the seed by calling the MOP_RANDOM operation
                Memory::Write(MAP(MATH_OPERATION), (Byte)MAP(MOP_RANDOM)); // call the MOP_RANDOM operation
                DWord aca_int = Memory::Read_DWord(MAP(MATH_ACA_INT));
                // MOP_RND_SEED is set to the value in the ACA register (randomized at this point)
                Memory::Write(MAP(MATH_OPERATION), (Byte)MAP(MOP_RND_SEED)); // call the MOP_RND_SEED operation
                // the seed should now have been randomized
                DWord acr_result = Memory::Read_DWord(MAP(MATH_ACR_INT));
                if (aca_int == acr_result)
                {
                    fail_count++;
                }
                if (fail_count == iterations) 
                {
                    all_tests_passed = false;
                    UnitTest::Log(this, clr::RED + "operation " + op.name + " test FAILED. Expected: " + 
                        std::to_string(aca_int) + ", Got: " + std::to_string(acr_result));
                }         
            }
            if (fail_count == 0) {
                UnitTest::Log(this, "operation " + clr::YELLOW + op.name + clr::GREEN + " PASSED!");
            }
        } 
        else 
        {
            if (mop != MAP(MOP_ACOSH))
            {
                // Arithmetic operations (binary)
                aca_value = 0.5f;
                acb_value = 0.3f;
                // Write the operation to MATH_OPERATION
                aca_float = aca_value; // directly assign the internal aca_float
                acb_float = acb_value; // directly assign the internal acb_float
                Memory::Write(MAP(MATH_OPERATION), (Byte)mop);           
                // Read the result from MATH_ACR_FLOAT as a boolean
                float expected_result = op.func(aca_value, acb_value);
                float actual_result = acr_float; //(bool)Memory::Read_DWord(MAP(MATH_ACR_INT));
                if (mop == MAP(MOP_IS_EQUAL) || 
                    mop == MAP(MOP_IS_NOT_EQUAL) || 
                    mop == MAP(MOP_IS_LESS) || 
                    mop == MAP(MOP_IS_GREATER) || 
                    mop == MAP(MOP_IS_LTE) || 
                    mop == MAP(MOP_IS_GTE) ||
                    mop == MAP(MOP_IS_INF) || 
                    mop == MAP(MOP_IS_NAN) || 
                    mop == MAP(MOP_SIGNBIT) ||
                    mop == MAP(MOP_IS_FINITE) ||
                    mop == MAP(MOP_IS_NORMAL))
                { // Read the result from MATH_ACR_INT as a boolean
                    actual_result = (bool)Memory::Read_DWord(MAP(MATH_ACR_INT));
                }           
                // Compare the expected and actual results
                if (actual_result != expected_result) {
                    all_tests_passed = false;
                    UnitTest::Log(this, clr::RED + "operation " + op.name + " FAILED. Expected: " + 
                        std::to_string(expected_result) + ", Got: " + std::to_string(actual_result));
                }
                else {
                    UnitTest::Log(this, "operation " + clr::YELLOW + op.name + clr::GREEN + " PASSED!");
                }
            }
            else // MOP_ACOSH
            {
                // Logical operations (binary)
                aca_value = 10.0f;
                acb_value = 0.0f;
                // Write the operation to MATH_OPERATION
                aca_float = aca_value; // directly assign the internal aca_float
                acb_float = acb_value; // directly assign the internal acb_float
                Memory::Write(MAP(MATH_OPERATION), (Byte)mop);           
                // Read the result from MATH_ACR_FLOAT as a boolean
                bool expected_result = op.func(aca_value, acb_value);
                bool actual_result = (bool)Memory::Read_DWord(MAP(MATH_ACR_INT));
                // Compare the expected and actual results
                if (actual_result != expected_result) {
                    all_tests_passed = false;
                    UnitTest::Log(this, clr::RED + "operation " + op.name + " FAILED. Expected: " + 
                        std::to_string(expected_result) + ", Got: " + std::to_string(actual_result));
                }
                else {
                    
                }
            }
        }
    }
    // TEST: Division by zero
    // ...

    // Precision Handling for Floating-Point Comparisons:
    //
    // Floating-point operations (e.g., std::sqrt, std::pow) can result in precision errors. Instead of direct
    // equality checks, consider using:
    //      ASSERT_TRUE(std::fabs(actual_result - expected_result) < epsilon);
    // Set epsilon to a small threshold like 1e-6.

    return all_tests_passed;
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
    else if (data == MAP(MOP_ASINH))
    {
        _update_regs_from_float(std::asinh(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_ACOSH))
    {
        _update_regs_from_float(std::acosh(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_SINH))
    {
        _update_regs_from_float(std::sinh(aca_float),
            acr_float, acr_pos, acr_string, acr_raw, acr_int);
    }
    else if (data == MAP(MOP_COSH))
    {
        _update_regs_from_float(std::cosh(aca_float),
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
    stream.precision(5);  // (7) limit precision of the string output
    stream << std::fixed << _float;  // Fixed-point representation
    _string = stream.str();

    // Convert to raw memory (reinterpret as raw bits)
    std::memcpy(&_raw, &_float, sizeof(float));

    // Store integer value (possible truncation of float to int)
    _int = static_cast<DWord>(_float);    
}


// END: Math.cpp
