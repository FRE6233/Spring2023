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
		static constexpr X beta = M_SQRT2;

		// e^{sz - κ(s)} f(z)
		X _pdf(const X& z, const S& s) const override
		{
			return 0*z*s; //!!! implement
		}
		// (1 − s/β)e^{(s + β)x}/2, x <= 0
		// 1 − (1 + s/β)e^{(s − β)x}/2, x >= 0
		X _cdf(const X& z, const S& s) const override
		{
			return 0*z*s; //!!! implement
		}
		// E[e^{sX}] = β^2/(β + s)(β − s), s < β.
		X _mgf(const S& s) const override
		{
			return 0*s; //!!! implement
		}
		X _cgf(const S& s) const override
		{
			return log(_mgf(s));
		}
#ifdef _DEBUG
		static int test()
		{
			double_exponential<> DE;

			return 0;
		}
#endif
	};

} // namespace fms::distribution

