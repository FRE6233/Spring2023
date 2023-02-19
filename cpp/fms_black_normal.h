// fms_black_normal.h - Self-contained Black forward model for put options.
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

namespace fms::black {

	constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

	// Stanard normal cumulative distribution.
	inline double normal(double z, double s = 0)
	{
		return 0.5 * (1 + erf((z - s) / M_SQRT2));
	}

	// F = f exp(sZ - s^2/2) <= k iff Z <= log(k/f)/2 + s/2
	inline double moneyness(double f, double s, double k)
	{
		if (f <= 0 or s <= 0 or k <= 0) {
			return NaN;
		}

		return log(k / f)/s + s/2;
	}

	namespace put {
		
		// E[max{k - F}, 0]
		inline double value(double f, double s, double k)
		{
			double z = moneyness(f, s, k);

			return k * normal(z) - f * normal(z, s);
		}

		// (d/df)E[max{k - F}, 0] = -P_s(F <= k)
		inline double delta(double f, double s, double k)
		{
			double z = moneyness(f, s, k);

			return -normal(z, s);
		}

		// Find s with p = put::value(f, s, k).
		inline double implied(double f, double p, double k, double s = 0.1, double eps = sqrt(std::numeric_limits<double>::epsilon()))
		{
			if (f <= 0 or p <= 0 or k <= 0) {
				return NaN;
			}
			if (p <= k - f) {
				return p == k - f ? 0 : NaN;
			}
			if (s <= 0) {
				s = 0.1;
			}
			if (eps <= 0) {
				eps = sqrt(std::numeric_limits<double>::epsilon());
			}

			double v = value(f, s, k) - p;
			double s_ = s + 2*eps;
			double v_ = value(f, s_, k) - p;

			while(fabs(s_ - s) > eps) {
				s = (s * v_ - s_ * v) / (v_ - v); // secant
				if (s <= 0) {
					s = s_ / 2;
				}
				v = value(f, s, k) - p;

				std::swap(s, s_);
				std::swap(v, v_);
			}

			return s_;
		}

#ifdef _DEBUG
#include <cassert>
		inline int test()
		{
			{
				double eps = sqrt(std::numeric_limits<double>::epsilon());
				double f = 100, s = 0.1, k = 100;

				assert(s / 2 == moneyness(f, s, k));
				assert(fabs(3.988 - value(f, s, k)) <= 1e-3);
				assert(fabs(-0.480 - delta(f, s, k)) <= 1e-3);

				double p = value(f, s, k);
				double s_ = implied(f, p, k, 0.2, eps);
				assert(fabs(s_ - s) <= eps);
			}

			return 0;
		}
#endif // _DEBUG

	} // namespace put

	namespace call {

		inline double value(double f, double s, double k)
		{
			return put::value(f, s, k) + f - k;
		}

		inline double delta(double f, double s, double k)
		{
			return put::delta(f, s, k) + 1;
		}

		// c = call::value(f, s, k)
		inline double implied(double f, double c, double k, double s = 0.1, double eps = sqrt(std::numeric_limits<double>::epsilon()))
		{
			return put::implied(f, c + f - k, k, s, eps);
		}

#ifdef _DEBUG
#include <cassert>
		inline int test()
		{
			{
				double eps = sqrt(std::numeric_limits<double>::epsilon());
				double f = 100, s = 0.1, k = 100;

				assert(s / 2 == moneyness(f, s, k));
				assert(fabs(3.988 - value(f, s, k)) <= 1e-3);
				assert(fabs(0.520 - delta(f, s, k)) <= 1e-3);

				double v = value(f, s, k);
				double s_ = implied(f, v, k, 0.2, eps);
				assert(fabs(s_ - s) <= eps);
			}

			return 0;
		}
#endif // _DEBUG


	} // namespace call
}
