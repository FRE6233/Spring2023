// fms_distribution_double_exponential.h
// f(x) = exp(−β∣x∣) β/2, −∞ < x < ∞, where β > 0.
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "ensure.h"
#include "fms_distribution.h"

namespace fms::distribution {

	template<class X = double, class S = X>
	struct double_exponential : public standard<X, S> {
		static constexpr X β = M_SQRT2;

		// e^{sz - κ(s)} f(z)
		X _pdf(const X& z, const S& s) const override
		{
			return exp(s * z - _cgf(s)) * exp(-β * fabs(z)) / β;
		}
		// (1 − s/β)e^{(s + β)x}/2, x <= 0
		// 1 − (1 + s/β)e^{(s − β)x}/2, x >= 0
		X _cdf(const X& z, const S& s) const override
		{
			return z <= 0 ? (1 - s / β) * exp((s + β) * z) / 2 : 1 - (1 + s / β) * exp((s - β) * z) / 2;
		}

		// E[e^{sX}] = β^2/(β^2 - s^2), s < β.
		X _mgf(const S& s) const override
		{
			return β * β / (β * β - s * s);
		}
		X _cgf(const S& s) const override
		{
			return log(_mgf(s));
		}
#ifdef _DEBUG
		static int test()
		{
			double_exponential<> DE;

			double z = 0;
			double ss[] = { -0.5, 0, 0.5 };
			// !!! test pdf, cdf, mgf, and cgf at z = 0, s = -0.5, 0, 0.5

			return 0;
		}
#endif
	};

} // namespace fms::distribution

