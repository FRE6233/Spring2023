// fms_root1d.h - One dimensional root finding
#pragma once
#include <cmath>
#include <limits>
#include <stdexcept>
#include <tuple>
#include "ensure.h"

namespace fms::root {

	// Solve f(x) = 0 given bound [x0, x1]
	template<class F, class X>
	class secant {
		F f;
		X x0, x1;
		X y0, y1;
		X eps;
	public:
		secant(F _f, X _x0, X _x1, X eps = 0)
			: f{ _f }, x0{ _x0 }, x1{ _x1 }, eps{ eps }
		{
			if (x0 > x1) {
				std::swap(x0, x1);
			}
			y0 = f(x0);
			y1 = f(x1);

			ensure(signbit(y0) != signbit(y1));
		}
		explicit operator bool() const
		{
			return std::max(fabs(y0), fabs(y1)) >= eps;
		}
		X operator*() const
		{
			return fabs(y0) < fabs(y1) ? x0 : x1;
		}
		secant& operator++()
		{
			X x = (x0 * y1 - x1 * y0) / (y1 - y0); // secant
			if (x < x0 or x > x1) {
				x = x0 / 2 + x1 / 2; // bisect
			}
			X y = f(x);
			if (signbit(y) == signbit(y0)) {
				x0 = x;
				y0 = y;
			}
			else {
				x1 = x;
				y1 = y;
			}
		}

		X solve()
		{
			while (operator bool()) {
				operator++();
			}

			return operator*();
		}
	};

} // namespace fms::root