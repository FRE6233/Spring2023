// fms_secant.h - 1-d root using secant method
#pragma once
#include <cmath>
#include <limits>

namespace fms::secant {

	inline static constexpr double epsilon = std::numeric_limits<double>::epsilon();
	inline static constexpr double tolerance = 1e-8; // sqrt(epsilon);

	template<class F>
	inline double solve(const F& f, double x0, double x1,
		double tol = tolerance, int iter = 100)
	{
		double y0 = f(x0);

		while (iter && fabs(y0) > tol) {
			double y1 = f(x1);
			x0 = (x0 * y1 - x1 * y0) / (y1 - y0);
			std::swap(x0, x1);
			std::swap(y0, y1);
			--iter;
		}

		return iter ? x0 : std::numeric_limits<double>::quiet_NaN();
	}

} // namespace fms::secant