// fms_black.h - Fischer Black model.
#pragma once
#include <cmath>
#include <stdexcept>
#include "fms_distribution.h"

namespace fms::black {

	// F = f exp(sZ - kappa(s) <= k  if and only if  Z <= z(k; f, s) = (log(k/f) + kappa(s))/s
	inline double moneyness(double f, double s, double k, const fms::distribution::base<>& m)
	{
		if (f <= 0 or s <= 0 or k <= 0) {
			throw std::runtime_error(__FUNCTION__ ": arguments must be positive");
		}

		return (log(k / f) + m.cgf(s))/s;
	}

	// E[max{k - F, 0}] = k P(F <= k) - f P_s(F <= k)
	inline double value_put(double f, double s, double k, const fms::distribution::base<>& m)
	{
		double z = moneyness(f, s, k, m);

		return k * m.cdf(z) - f * m.cdf(z, s);
	}

	// -P_s(F <= k)
	inline double delta_put(double f, double s, double k, const fms::distribution::base<>& m)
	{
		double z = moneyness(f, s, k, m);

		return -m.cdf(z, s);
	}

	//!!! implement value_call using max{F - k, 0} - max{k - F, 0} = F - k
	// E[max{F - k, 0}] = ...

} // namespace fms::black
