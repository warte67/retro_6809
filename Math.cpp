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

Byte Math::read(Word offset, bool debug)
{
	Byte data = 0xCC;

    // read ACA
    if (offset >= MATH_ACA_POS && offset < MATH_ACB_POS)
    {
        data = _read_acc(offset, MATH_ACA_POS,
            aca_pos, aca_data, aca_raw, aca_int);
    }

    // read ACB
    if (offset >= MATH_ACB_POS && offset < MATH_ACR_POS)
    {
        data = _read_acc(offset, MATH_ACB_POS,
            acb_pos, acb_data, acb_raw, acb_int);
    }

    // read ACR
    if (offset >= MATH_ACR_POS && offset < MATH_OPERATION)
    {
        data = _read_acc(offset, MATH_ACR_POS,
            acr_pos, acr_data, acr_raw, acr_int);
    }
    
    // read the last math operation used
    if (offset == MATH_OPERATION)
        data = math_operation;

	Bus::Write(offset, data, true);
	return data;
} // END: Math::read()

Byte Math::_read_acc(Word offset, Word reg,
    Byte& _pos, std::string& _data, DWord& _raw, DWord& _int)
{
    Byte data = 0xCC;
    // bounds checking
    if (offset < reg || offset > reg + 9)   return data;

    if (offset == reg)              // MATH_ACx_POS
    {
        data = _pos;
        if (data >= _data.size())
            data = _data.size() - 1;
        if (_data.size() == 0)
            data = 0;
    }
    else if (offset == reg + 1)     // MATH_ACx_DATA
    {
        data = 0;
        if (_data.size() > 0)
        {
            if (_pos < _data.size())
            {
                data = (Byte)_data.substr(_pos, 1).at(0);
                if (_pos <= _data.size())
                    _pos++;
            }
            else
            {
                _data = "";
                data = 0;
            }
        }
    }
    // MATH_ACx_RAW
    else if (offset == reg + 2) { data = (_raw >> 24) & 0xff; }
    else if (offset == reg + 3) { data = (_raw >> 16) & 0xff; }
    else if (offset == reg + 4) { data = (_raw >> 8) & 0xff; }
    else if (offset == reg + 5) { data = (_raw >> 0) & 0xff; }
    // MATH_ACx_INT
    else if (offset == reg + 6) { data = (_int >> 24) & 0xff; }
    else if (offset == reg + 7) { data = (_int >> 16) & 0xff; }
    else if (offset == reg + 8) { data = (_int >> 8) & 0xff; }
    else if (offset == reg + 9) { data = (_int >> 0) & 0xff; }

    return data;
}   // END Math::_read_acc()


void Math::write(Word offset, Byte data, bool debug)
{


	Bus::Write(offset, data, true);
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
    DisplayEnum("MOP_NAN",          enumID++, "       (bool)ACR = std::isnan(ACA);");
    DisplayEnum("MOP_NORMAL",       enumID++, "       (bool)ACR = std::isnormal(ACA);");
    DisplayEnum("MOP_SIGNBIT",      enumID++, "       (bool)ACR = std::signbit(ACA);");
    DisplayEnum("MOP_SUBTRACT",     enumID++, "       ACR = ACA - ACB");
    DisplayEnum("MOP_ADD",          enumID++, "       ACR = ACA + ACB");
    DisplayEnum("MOP_MULTIPLY",     enumID++, "       ACR = ACA * ACB");
    DisplayEnum("MOP_DIVIDE",       enumID++, "       ACR = ACA / ACB");
    DisplayEnum("MOP_FMOD",         enumID++, "       ACR = std::fmod(ACA, ACB);");
    DisplayEnum("MOP_REMAINDER",    enumID++, "       ACR = std::remainder(ACA, ACB);");
    DisplayEnum("MOP_FMAX",         enumID++, "       ACR = std::std::fmax(ACA, ACB);");
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
    DisplayEnum("MOP_NETAFTER",     enumID++, "       ACR = std::nextafter(ACA, ACB);");
    DisplayEnum("MOP_COPYSIGN",     enumID++, "       ACR = std::copysign(ACA, ACB);");
    DisplayEnum("MOP_LASTOP",       enumID-1, "       last implemented math operation ");

    DisplayEnum("", 0, "End MATH_OPERATION's (MOPS)");
    DisplayEnum("MATH_END", nextAddr, "end of math co-processor registers");
    return nextAddr - old_addr;
}

void Math::OnInit()
{
    printf("Math::OnInit()\n");
}

void Math::OnQuit()
{
    printf("Math::OnQuit()\n");
}



// void Math::OnActivate() {}
// void Math::OnDeactivate() {}
// void Math::OnEvent(SDL_Event* evnt) {}
// void Math::OnUpdate(float fElapsedTime) {}
// void Math::OnRender() {}
