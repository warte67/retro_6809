/* Math.cpp *************************
 *
 *   Acts to emulate a Raspberry PI Pico
 *		Implementing Math Coprocessor
 *      functions.
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#include "Bus.h"
#include "Math.h"
#include <cfloat>
#include <cmath>
#include <time.h>		// for srand()

Byte Math::read(Word offset, bool debug)
{
	Byte data = 0xCC;

    // read ACA
    if (offset >= MATH_ACA_POS && offset < MATH_ACB_POS)
    {
        data = _read_acc(offset, MATH_ACA_POS,
            aca_pos, aca_float, aca_string, aca_raw, aca_int);
    }

    // read ACB
    if (offset >= MATH_ACB_POS && offset < MATH_ACR_POS)
    {
        data = _read_acc(offset, MATH_ACB_POS,
            acb_pos, acb_float, acb_string, acb_raw, acb_int);
    }

    // read ACR
    if (offset >= MATH_ACR_POS && offset < MATH_OPERATION)
    {
        data = _read_acc(offset, MATH_ACR_POS,
            acr_pos, acr_float, acr_string, acr_raw, acr_int);
    }
    
    // read the last math operation used
    if (offset == MATH_OPERATION)
        data = math_operation;

	Bus::Write(offset, data, true);
	return data;
} // END: Math::read()

Byte Math::_read_acc(Word offset, Word reg, Byte& _pos, 
        float& _float, std::string& _string, DWord& _raw, DWord& _int)
{
    Byte data = 0xCC;
    // bounds checking
    if (offset < reg || offset > reg + 9)   return data;

    if (offset == reg)              // MATH_ACx_POS
    {
        data = _pos;
        if (data >= _string.size())
            data = _string.size() - 1;
        if (_string.size() == 0)
            data = 0;
    }
    else if (offset == reg + 1)     // MATH_ACx_DATA
    {
        data = 0;
        if (_string.size() > 0)
        {
            if (_pos < _string.size())
            {
                data = (Byte)_string.substr(_pos, 1).at(0);
                if (_pos <= _string.size())
                    _pos++;
            }
            else
            {
                _string = "";
                data = 0;
            }
        }
    }
    // MATH_ACx_RAW
    else if (offset == reg + 2) { data = (_raw >> 24) & 0xff; }
    else if (offset == reg + 3) { data = (_raw >> 16) & 0xff; }
    else if (offset == reg + 4) { data = (_raw >>  8) & 0xff; }
    else if (offset == reg + 5) { data = (_raw >>  0) & 0xff; }
    // MATH_ACx_INT
    else if (offset == reg + 6) { data = (_int >> 24) & 0xff; }
    else if (offset == reg + 7) { data = (_int >> 16) & 0xff; }
    else if (offset == reg + 8) { data = (_int >>  8) & 0xff; }
    else if (offset == reg + 9) { data = (_int >>  0) & 0xff; }

    return data;
}   // END Math::_read_acc()


void Math::write(Word offset, Byte data, bool debug)
{
    // write ACB
    if (offset >= MATH_ACA_POS && offset < MATH_ACB_POS)
    {
        data = _write_acc(offset, data, MATH_ACA_POS, 
            aca_pos, aca_float, aca_string, aca_raw, aca_int);
    }

    // write ACB
    if (offset >= MATH_ACB_POS && offset < MATH_ACR_POS)
    {
        data = _write_acc(offset, data, MATH_ACB_POS,
            acb_pos, acb_float, acb_string, acb_raw, acb_int);
    }

    // write ACR
    if (offset >= MATH_ACR_POS && offset < MATH_OPERATION)
    {
        data = _write_acc(offset, data, MATH_ACR_POS,
            acr_pos, acr_float, acr_string, acr_raw, acr_int);
    }

    // write and execute the math operation
    if (offset == MATH_OPERATION)
    {
        //data = math_operation;
        switch (data)
        {
            case MOP_RANDOM:
            {
                _update_regs_from_float(((float)rand() / (RAND_MAX)),
                    aca_float, aca_pos, aca_string, aca_raw, aca_int);
                aca_int = rand() << 16 | rand();
                _update_regs_from_float(((float)rand() / (RAND_MAX)),
                    acb_float, acb_pos, acb_string, acb_raw, acb_int);
                acb_int = rand() << 16 | rand();
                _update_regs_from_float(((float)rand() / (RAND_MAX)),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int);
                acr_int = rand() << 16 | rand();
                break;
            }
            case MOP_IS_EQUAL:      { acr_int = (bool)(aca_float == acb_float); break; }
            case MOP_IS_NOT_EQUAL:  { acr_int = (bool)(aca_float != acb_float); break; }
            case MOP_IS_LESS:       { acr_int = (bool)(aca_float < acb_float); break; }
            case MOP_IS_GREATER:    { acr_int = (bool)(aca_float > acb_float); break; }
            case MOP_IS_LTE:        { acr_int = (bool)(aca_float <= acb_float); break; }
            case MOP_IS_GTE:        { acr_int = (bool)(aca_float >= acb_float); break; }

            case MOP_IS_FINITE:     { acr_int = (bool)(std::isfinite(aca_float)); break; }
            case MOP_IS_INF:        { acr_int = (bool)(std::isinf(aca_float)); break; }
            case MOP_IS_NAN:        { acr_int = (bool)(std::isnan(aca_float)); break; }
            case MOP_IS_NORMAL:     { acr_int = (bool)(std::isnormal(aca_float)); break; }
            case MOP_SIGNBIT:       { acr_int = (bool)(std::signbit(aca_float)); break; }

            case MOP_SUBTRACT: {
                _update_regs_from_float((aca_float - acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ADD: {
                _update_regs_from_float((aca_float + acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_MULTIPLY: {
                _update_regs_from_float((aca_float * acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int);  break; }
            case MOP_DIVIDE: {
                _update_regs_from_float((aca_float / acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_FMOD: {
                _update_regs_from_float(std::fmod(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_REMAINDER: {
                _update_regs_from_float(std::remainder(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_FMAX: {
                _update_regs_from_float(std::fmax(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_FMIN: {
                _update_regs_from_float(std::fmin(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_FDIM: {
                _update_regs_from_float(std::fdim(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_EXP: {
                _update_regs_from_float(std::exp(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_EXP2: {
                _update_regs_from_float(std::exp2(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_EXPM1: {
                _update_regs_from_float(std::expm1(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LOG: {
                _update_regs_from_float(std::log(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LOG10: {
                _update_regs_from_float(std::log10(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LOG2: {
                _update_regs_from_float(std::log2(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LOG1P: {
                _update_regs_from_float(std::log1p(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_SQRT: {
                _update_regs_from_float(std::sqrt(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_CBRT: {
                _update_regs_from_float(std::cbrt(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_HYPOT: {
                _update_regs_from_float(std::hypot(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_POW: {
                _update_regs_from_float(std::pow(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_SIN: {
                _update_regs_from_float(std::sin(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_COS: {
                _update_regs_from_float(std::cos(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_TAN: {
                _update_regs_from_float(std::tan(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ASIN: {
                _update_regs_from_float(std::asin(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ACOS: {
                _update_regs_from_float(std::acos(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ATAN: {
                _update_regs_from_float(std::atan(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ATAN2: {
                _update_regs_from_float(std::atan2(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_COSH: {
                _update_regs_from_float(std::acosh(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ATANH: {
                _update_regs_from_float(std::atanh(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ERF: {
                _update_regs_from_float(std::erf(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ERFC: {
                _update_regs_from_float(std::erfc(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LGAMMA: {
                _update_regs_from_float(std::lgamma(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_TGAMMA: {
                _update_regs_from_float(std::tgamma(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_CEIL: {
                _update_regs_from_float(std::ceil(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_FLOOR: {
                _update_regs_from_float(std::floor(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_TRUNC: {
                _update_regs_from_float(std::trunc(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ROUND: {
                _update_regs_from_float(std::round(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LROUND: {
                _update_regs_from_float(std::lround(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_NEARBYINT: {
                _update_regs_from_float(std::nearbyint(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_ILOGB: {
                _update_regs_from_float(std::ilogb(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_LOGB: {
                _update_regs_from_float(std::logb(aca_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_NEXTAFTER: {
                _update_regs_from_float(std::nextafter(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
            case MOP_COPYSIGN: {
                _update_regs_from_float(std::copysign(aca_float, acb_float),
                    acr_float, acr_pos, acr_string, acr_raw, acr_int); break; }
        }
    }
	Bus::Write(offset, data, true);
}

Byte Math::_write_acc(Word offset, Byte data, Word reg, Byte& _pos, 
        float& _float, std::string& _string, DWord& _raw, DWord& _int)
{
    // position within the string
    if (offset == reg + 0)              // MATH_ACx_POS
    {
        if (data > _string.size())
            data = _string.size() - 1;
        _pos = data;
    }
    // update the string itself
    if (offset == reg + 1)              // MATH_ACx_DATA
    {
        // at the beginning of the string?
        if (_pos == 0)  
        {
            _string = "";
            _pos = 0;
            _string += data;
            _pos++;
        }
        else // within the string
        {
            _string = _string.substr(0, _pos);
            _string += data;
            _pos++;
        }
        // convert string to float
        _float = std::stof(_string);    
        // update MATH_ACx_RAW
        _raw = *reinterpret_cast<DWord*>(&_float);
        // update MATH_ACx_INT
        _int = (DWord)_float;
        // update the string
        _string = std::to_string(_float);

        //printf("_write_acc(): _float: %f   _raw: $%08X   _int: $%08X\n", _float, _raw, _int);
    }
    // MATH_ACx_RAW
    if (offset >= reg + 2 && offset <= reg + 5)
    {
        if (offset == reg + 2)
            _raw = (_raw & ~0xff000000) | (data << 24);
        if (offset == reg + 3)
            _raw = (_raw & ~0x00ff0000) | (data << 16);
        if (offset == reg + 4)
            _raw = (_raw & ~0x0000ff00) | (data <<  8);
        if (offset == reg + 5)
            _raw = (_raw & ~0x000000ff) | (data <<  0);
        // update the internal float
        _float = *reinterpret_cast<float*>(&_raw);
        // update the internal int
        _int = (DWord)_float;
        // update the string
        _string = std::to_string(_float);

        //printf("MATH_ACx_RAW ---> _float:%f    _raw: $%08X    _int: $%08X    _string:%s\n", _float, _raw, _int, _string.c_str());
    }
    // MATH_ACx_INT
    if (offset >= reg + 6 && offset <= reg + 9)
    {
        if (offset == reg + 6)
            _int = (_int & ~0xff000000) | (data << 24);
        if (offset == reg + 7)
            _int = (_int & ~0x00ff0000) | (data << 16);
        if (offset == reg + 8)
            _int = (_int & ~0x0000ff00) | (data << 8);
        if (offset == reg + 9)
            _int = (_int & ~0x000000ff) | (data << 0);
        // update the internal float
        _float = (float)_int;
        // update the internal raw
        _raw = *reinterpret_cast<DWord*>(&_float);
        // update the string
        _string = std::to_string(_float);

        //printf("MATH_ACx_INT ---> _float:%f    _raw: $%08X    _int: $%08X    _string:%s\n", _float, _raw, _int, _string.c_str());
    }
    return data;
}

void Math::_update_regs_from_float(float f_data, float& _float,
    Byte& _pos, std::string& _string, DWord _raw, DWord _int)
{
    _pos = 0;
    _float = f_data;
    _string = std::to_string(_float);
    _raw = *reinterpret_cast<DWord*>(&_float);
    _int = (DWord)_float;
}



Word Math::OnAttach(Word nextAddr)
{
    int size = 0;
    Word old_addr = nextAddr;

    DisplayEnum("", 0, "");
    DisplayEnum("", 0, "Math Co-Processor Hardware Registers:");
    DisplayEnum("MATH_BEGIN", nextAddr, " start of math co-processor  hardware registers");

    DisplayEnum("MATH_ACA_POS", nextAddr, " (Byte) character position within the ACA float string");
    nextAddr += 1;
    DisplayEnum("MATH_ACA_DATA", nextAddr, " (Byte) ACA float string character port");
    nextAddr += 1;
    DisplayEnum("MATH_ACA_RAW", nextAddr, " (4-Bytes) ACA raw float data");
    nextAddr += 4;
    DisplayEnum("MATH_ACA_INT", nextAddr, " (4-Bytes) ACA integer data");
    nextAddr += 4;

    DisplayEnum("MATH_ACB_POS", nextAddr, " (Byte) character position within the ACB float string");
    nextAddr += 1;
    DisplayEnum("MATH_ACB_DATA", nextAddr, " (Byte) ACB float string character port");
    nextAddr += 1;
    DisplayEnum("MATH_ACB_RAW", nextAddr, " (4-Bytes) ACB raw float data");
    nextAddr += 4;
    DisplayEnum("MATH_ACB_INT", nextAddr, " (4-Bytes) ACB integer data");
    nextAddr += 4;

    DisplayEnum("MATH_ACR_POS", nextAddr, " (Byte) character position within the ACR float string");
    nextAddr += 1;
    DisplayEnum("MATH_ACR_DATA", nextAddr, " (Byte) ACR float string character port");
    nextAddr += 1;
    DisplayEnum("MATH_ACR_RAW", nextAddr, " (4-Bytes) ACR raw float data");
    nextAddr += 4;
    DisplayEnum("MATH_ACR_INT", nextAddr, " (4-Bytes) ACR integer data");
    nextAddr += 4;

    DisplayEnum("MATH_OPERATION", nextAddr, " (Byte) Operation 'command' to be issued");
    nextAddr += 1;
    Byte enumID = 0;
    DisplayEnum("", 0, "Begin MATH_OPERATION's (MOPS)");
    DisplayEnum("MOP_RANDOM",       enumID++, "       ACA, ACB, and ACR are set to randomized values");
    DisplayEnum("MOP_IS_EQUAL",     enumID++, "       (bool)ACR = (ACA == ACB)");
    DisplayEnum("MOP_IS_NOT_EQUAL", enumID++, "       (bool)ACR = (ACA != ACB)");
    DisplayEnum("MOP_IS_LESS",      enumID++, "       (bool)ACR = std::isless(ACA, ACB);");
    DisplayEnum("MOP_IS_GREATER",   enumID++, "       (bool)ACR = std::isgreater(ACA, ACB);");
    DisplayEnum("MOP_IS_LTE",       enumID++, "       (bool)ACR = std::islessequal(ACA, ACB);");
    DisplayEnum("MOP_IS_GTE",       enumID++, "       (bool)ACR = std::islessgreater(ACA, ACB);");
    DisplayEnum("MOP_IS_FINITE",    enumID++, "       (bool)ACR = std::isfinite(ACA);");
    DisplayEnum("MOP_IS_INF",       enumID++, "       (bool)ACR = std::isinf(ACA);");
    DisplayEnum("MOP_IS_NAN",       enumID++, "       (bool)ACR = std::isnan(ACA);");
    DisplayEnum("MOP_IS_NORMAL",    enumID++, "       (bool)ACR = std::isnormal(ACA);");
    DisplayEnum("MOP_SIGNBIT",      enumID++, "       (bool)ACR = std::signbit(ACA);");
    DisplayEnum("MOP_SUBTRACT",     enumID++, "       ACR = ACA - ACB");
    DisplayEnum("MOP_ADD",          enumID++, "       ACR = ACA + ACB");
    DisplayEnum("MOP_MULTIPLY",     enumID++, "       ACR = ACA * ACB");
    DisplayEnum("MOP_DIVIDE",       enumID++, "       ACR = ACA / ACB");
    DisplayEnum("MOP_FMOD",         enumID++, "       ACR = std::fmod(ACA, ACB);");
    DisplayEnum("MOP_REMAINDER",    enumID++, "       ACR = std::remainder(ACA, ACB);");
    DisplayEnum("MOP_FMAX",         enumID++, "       ACR = std::fmax(ACA, ACB);");
    DisplayEnum("MOP_FMIN",         enumID++, "       ACR = std::fmin(ACA, ACB);");
    DisplayEnum("MOP_FDIM",         enumID++, "       ACR = std::fdim(ACA, ACB);");
    DisplayEnum("MOP_EXP",          enumID++, "       ACR = std::exp(ACA);");
    DisplayEnum("MOP_EXP2",         enumID++, "       ACR = std::exp2(ACA);");
    DisplayEnum("MOP_EXPM1",        enumID++, "       ACR = std::expm1(ACA);");
    DisplayEnum("MOP_LOG",          enumID++, "       ACR = std::log(ACA);");
    DisplayEnum("MOP_LOG10",        enumID++, "       ACR = std::log10(ACA);");
    DisplayEnum("MOP_LOG2",         enumID++, "       ACR = std::log2(ACA);");
    DisplayEnum("MOP_LOG1P",        enumID++, "       ACR = std::log1p(ACA);");
    DisplayEnum("MOP_SQRT",         enumID++, "       ACR = std::sqrt(ACA);");
    DisplayEnum("MOP_CBRT",         enumID++, "       ACR = std::cbrt(ACA);");
    DisplayEnum("MOP_HYPOT",        enumID++, "       ACR = std::hypot(ACA, ACB);");
    DisplayEnum("MOP_POW",          enumID++, "       ACR = std::pow(ACA, ACB);");
    DisplayEnum("MOP_SIN",          enumID++, "       ACR = std::sin(ACA);");
    DisplayEnum("MOP_COS",          enumID++, "       ACR = std::cos(ACA);");
    DisplayEnum("MOP_TAN",          enumID++, "       ACR = std::tan(ACA);");
    DisplayEnum("MOP_ASIN",         enumID++, "       ACR = std::asin(ACA);");
    DisplayEnum("MOP_ACOS",         enumID++, "       ACR = std::acos(ACA);");
    DisplayEnum("MOP_ATAN",         enumID++, "       ACR = std::atan(ACA);");
    DisplayEnum("MOP_ATAN2",        enumID++, "       ACR = std::atan2(ACA, ACB);");
    DisplayEnum("MOP_SINH",         enumID++, "       ACR = std::sinh(ACA);");
    DisplayEnum("MOP_COSH",         enumID++, "       ACR = std::acosh(ACA);");
    DisplayEnum("MOP_ATANH",        enumID++, "       ACR = std::atanh(ACA);");
    DisplayEnum("MOP_ERF",          enumID++, "       ACR = std::erf(ACA);");
    DisplayEnum("MOP_ERFC",         enumID++, "       ACR = std::erfc(ACA);");
    DisplayEnum("MOP_LGAMMA",       enumID++, "       ACR = std::lgamma(ACA);");
    DisplayEnum("MOP_TGAMMA",       enumID++, "       ACR = std::tgamma(ACA);");
    DisplayEnum("MOP_CEIL",         enumID++, "       ACR = std::ceil(ACA);");
    DisplayEnum("MOP_FLOOR",        enumID++, "       ACR = std::floor(ACA);");
    DisplayEnum("MOP_TRUNC",        enumID++, "       ACR = std::trunc(ACA);");
    DisplayEnum("MOP_ROUND",        enumID++, "       ACR = std::round(ACA);");
    DisplayEnum("MOP_LROUND",       enumID++, "       ACR = std::lround(ACA);");
    DisplayEnum("MOP_NEARBYINT",    enumID++, "       ACR = std::nearbyint(ACA);");
    DisplayEnum("MOP_ILOGB",        enumID++, "       ACR = std::ilogb(ACA);");
    DisplayEnum("MOP_LOGB",         enumID++, "       ACR = std::logb(ACA);");
    DisplayEnum("MOP_NEXTAFTER",    enumID++, "       ACR = std::nextafter(ACA, ACB);");
    DisplayEnum("MOP_COPYSIGN",     enumID++, "       ACR = std::copysign(ACA, ACB);");
    DisplayEnum("MOP_LASTOP",       enumID-1, "       last implemented math operation ");

    DisplayEnum("", 0, "End MATH_OPERATION's (MOPS)");
    DisplayEnum("MATH_END", nextAddr, "end of math co-processor registers");
    return nextAddr - old_addr;
}

void Math::OnInit()
{
    //printf("Math::OnInit()\n");
    srand(time(NULL));
}

void Math::OnQuit()
{
    // printf("Math::OnQuit()\n");
}



// void Math::OnActivate() {}
// void Math::OnDeactivate() {}
// void Math::OnEvent(SDL_Event* evnt) {}
// void Math::OnUpdate(float fElapsedTime) {}
// void Math::OnRender() {}
