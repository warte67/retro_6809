/* Math.h ***************************
 *
 *   Acts to emulate a Raspberry PI Pico
 *		Implementing Math Coprocessor 
 *      functions.
 *
 * Copyright (C) 2023 by Jay Faries
 ************************************/

#pragma once
#ifndef __MATH_H__
#define __MATH_H__

#include "Device.h"

class Math : public Device
{
public:
    Math() { _deviceName = "Math"; }
    virtual ~Math() {}

    // register access
    Byte read(Word offset, bool debug = false) override;
    void write(Word offset, Byte data, bool debug = false) override;
    // virtual overrides
    Word OnAttach(Word nextAddr) override;
    void OnInit() override;
    void OnQuit() override;
    // void OnActivate() override;
    // void OnDeactivate() override;
    // void OnEvent(SDL_Event* evnt) override;
    // void OnUpdate(float fElapsedTime) override;
    // void OnRender() override;

protected:
private:
};

#endif // __MATH_H__



/*****************************************
 *
 * 	Float A Register:
 * 		- (Byte) position within the float string
 * 		- (Byte) Port to string float
 * 		- (4-Bytes) raw float data
 * 		- (4-Bytes) real integer
 *
 * 	Float B Register:
 * 		- (Byte) position within the float string
 * 		- (Byte) Port to string float
 * 		- (4-Bytes) raw float data
 * 		- (4-Bytes) real integer
 *
 * 	Float C Register (result):
 * 		- (Byte) position within the float string
 * 		- (Byte) Port to string float
 * 		- (4-Bytes) raw float data
 * 		- (4-Bytes) real integer
 *
 * Operator:
 * 		- MOP_RANDOM	- FAR, FBR, and FRR are set to randomized values
 *
 * 		- MOP_IS_EQUAL		(bool)ACR = (ACA == ACB)
 * 		- MOP_IS_NOT_EQUAL	(bool)ACR = (ACA != ACB)
 * 		- MOP_IS_LESS		(bool)ACR = isless(half, half);
 * 		- MOP_IS_GREATER	(bool)ACR = isgreater(half, half);
 * 		- MOP_IS_LTE		(bool)ACR = islessequal(half, half);
 * 		- MOP_IS_GTE		(bool)ACR = islessgreater(half, half);
 
 *		- MOP_IS_FINITE		bool isfinite(half);
 *		- MOP_IS_INF		bool isinf(half);
 *		- MOP_IS_NAN		bool isnan(half);
 *		- MOP_IS_NORMAL		bool isnormal(half);
 *		- MOP_SIGNBIT		bool signbit(half);

 * 		- MOP_SUBTRACT
 * 		- MOP_ADD
 * 		- MOP_MULTIPLY
 * 		- MOP_DIVIDE

		MOP_FMOD = fmod(half, half);
		friend = std::remainder(half, half);
		friend = std::fmax(half, half);
		friend = std::fmin(half, half);
		friend = std::fdim(half, half);
		friend = std::exp(half);
		friend = std::exp2(half);
		friend = std::expm1(half);
		friend = std::log(half);
		friend = std::log10(half);
		friend = std::log2(half);
		friend = std::log1p(half);
		friend = std::sqrt(half);
		friend = std::cbrt(half);
		friend = std::hypot(half, half);
		friend = std::pow(half, half);
		friend = std::sin(half);
		friend = std::cos(half);
		friend = std::tan(half);
		friend = std::asin(half);
		friend = std::acos(half);
		friend = std::atan(half);
		friend = std::atan2(half, half);
		friend = std::sinh(half);
		friend = std::cosh(half);
		friend = std::tanh(half);
		friend = std::asinh(half);
		friend = std::acosh(half);
		friend = std::atanh(half);
		friend = std::erf(half);
		friend = std::erfc(half);
		friend = std::lgamma(half);
		friend = std::tgamma(half);
		friend = std::ceil(half);
		friend = std::floor(half);
		friend = std::trunc(half);
		friend = std::round(half);
		friend = std::lround(half);
		friend = std::rint(half);
		friend = std::lrint(half);
		friend = std::nearbyint(half);
		friend = std::ilogb(half);
		friend = std::logb(half);
		friend = std::nextafter(half, half);
		friend = std::copysign(half, half);

 *****************************************/
