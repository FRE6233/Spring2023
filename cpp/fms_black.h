// fms_black.h - Fischer Black model.
#pragma once
#include <cmath>
#include <stdexcept>
#include "fms_distribution_normal.h"

namespace fms::black {

	// F = f exp(sZ - s^2/2 <= k  if and only if  Z <= z(k; f, s) = (log(k/f) + s^2/2)/s
	inline double moneyness(double f, double s, double k)
	{
		if (f <= 0 or s <= 0 or k <= 0) {
			throw std::runtime_error(__FUNCTION__ ": arguments must be positive");
		}

		return log(k / f) / s + s / 2;
	}

	// E[max{k - F, 0}] = k P(F <= k) - f P_s(F <= k)
	inline double value_put(double f, double s, double k)
	{
		double z = moneyness(f, s, k);

		return k * distribution::normal(z) - f * distribution::normal(z - s);
	}

	// -P_s(F <= k)
	inline double delta_put(double f, double s, double k)
	{
		double z = moneyness(f, s, k);

		return -distribution::normal(z - s);
	}

	//!!! implement value_call
	// E[max{F - k, 0}] = ...

} // namespace fms::black
