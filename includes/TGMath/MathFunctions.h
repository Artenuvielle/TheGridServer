#ifndef _MathFunctions_H_
#define _MathFunctions_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include "TGMath/MathTypes.h"

#undef max
#undef min
namespace tg_math {
	const Real32 Pi = M_PI;
	Real32 random();
	Real32 max(Real32 a, Real32 b);
	Real32 min(Real32 a, Real32 b);
	Real32 degree2Rad(Real32 a);
	Real32 rad2Degree(Real32 a);
	Real32 pow(Real32 a, Real32 b);
	Real32 sqrt(Real32 a);
	Real32 sgn(Real32 a);
	Real32 abs(Real32 a);
	Real32 cos(Real32 a);
	Real32 acos(Real32 a);
	Real32 sin(Real32 a);
	Real32 asin(Real32 a);
	Real32 tan(Real32 a);
	Real32 atan(Real32 a);
	Real32 atan2(Real32 a, Real32 b);
}
#endif