#include "TGMath/MathFunctions.h"

#include <cmath>
#include <stdlib.h>

Real32 tg_math::random() {
	return rand() / static_cast<Real32>(RAND_MAX);
}

Real32 tg_math::max(Real32 a, Real32 b) {
	return (a >= b) ? a : b;
}

Real32 tg_math::min(Real32 a, Real32 b) {
	return (a <= b) ? a : b;
}

Real32 tg_math::degree2Rad(Real32 a) {
	return a / 180.0 * tg_math::Pi;
}

Real32 tg_math::rad2Degree(Real32 a) {
	return a / tg_math::Pi * 180.0;
}

Real32 tg_math::pow(Real32 a, Real32 b) {
	return std::pow(a, b);
}

Real32 tg_math::sqrt(Real32 a) {
	return std::sqrt(a);
}

Real32 tg_math::sgn(Real32 a) {
	if (a < 0) {
		return -1;
	} else if (a > 0) {
		return 1;
	}
	return 0;
}

Real32 tg_math::abs(Real32 a) {
	return (a < 0) ? -a : a;
}

Real32 tg_math::cos(Real32 a) {
	return std::cos(a);
}

Real32 tg_math::acos(Real32 a) {
	return std::acos(a);
}

Real32 tg_math::sin(Real32 a) {
	return std::sin(a);
}

Real32 tg_math::asin(Real32 a) {
	return std::asin(a);
}

Real32 tg_math::tan(Real32 a) {
	return std::tan(a);
}

Real32 tg_math::atan(Real32 a) {
	return std::atan(a);
}

Real32 tg_math::atan2(Real32 a, Real32 b) {
	return std::atan2(a, b);
}