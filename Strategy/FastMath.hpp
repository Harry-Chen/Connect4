#ifndef FASTMATH_HPP
#define FASTMATH_HPP

#include <stdint.h>

static inline float fastLog(float x) {
	union { float f; uint32_t i; } vx = { x };
	float y = vx.i;
	y *= 8.2629582881927490e-8f;
	return y - 87.989971088f;
}

static inline float fastSqrt(float x) {
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86 - (i >> 1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x * (1.5f - xhalf * x*x); // Newton step, repeating increases accuracy
	x = x * (1.5f - xhalf * x*x); // Newton step, repeating increases accuracy
	x = x * (1.5f - xhalf * x*x); // Newton step, repeating increases accuracy

	return 1 / x;
}

#endif