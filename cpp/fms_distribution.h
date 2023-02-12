#pragma once
#ifdef _DEBUG
#include <cassert>
#endif
#include <cmath>

namespace fms::distribution {

	// Standard normal cumulative distribution function.
	inline double normal(double z)
	{
		return 0.5 * (1 + erf(z / sqrt(2)));
	}
#ifdef _DEBUG
	inline int test_normal()
	{
		{
			assert(0.5 == normal(0));
			assert(1 == normal(1) + normal(-1));
			// https://www.wolframalpha.com/input?i=normal+cdf+calculator
			assert(fabs(normal(-1) - 0.1587) <= 1e-4);
			assert(fabs(normal(1) - 0.8413) <= 1e-4);
		}

		return 0;
	}
#endif
} // namespace fms::distribution

