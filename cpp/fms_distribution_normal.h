// fms_distribution_normal.h - Standard normal distribution.
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "ensure.h"
#include "fms_distribution.h"

namespace fms::distribution {

	template<class X = double, class S = X>
	struct normal : public base<X, S> {
		// P_s(Z <= z) = P(Z <= z - s)
		X _cdf(const X& z, const S& s) const override
		{
			X z_ = z - s;

			return 0.5 * (1 + erf(z_ / M_SQRT2));
		}
		X _mgf(const S& s) const override
		{
			return exp(s*s/2);
		}
		X _cgf(const S& s) const override
		{
			return s*s/2;
		}
		// X _inv(const X& x, const S& s) const override
#ifdef _DEBUG
		static int test()
		{
			normal<> N;
			{
				ensure(0.5 == N.cdf(0));
				ensure(1 == N.cdf(1) + N.cdf(-1));
				// https://www.wolframalpha.com/input?i=normal+cdf+calculator
				ensure(fabs(N.cdf(-1) - 0.1587) <= 1e-4);
				ensure(fabs(N.cdf(1) - 0.8413) <= 1e-4);
			}
			{
				ensure(1 == N.mgf(0));
				ensure(0 == N.cgf(0));
			}

			return 0;
		}
#endif
	};

} // namespace fms::distribution
