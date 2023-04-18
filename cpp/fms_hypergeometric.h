// fms_hypergeometric.h
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <functional>
#include <limits>

namespace fms {

	// Default test for convergence.
	template<class X>
	inline const X Eps = sqrt(std::numeric_limits<X>::epsilon());

	// Generalized hypergeometric function and its derivatives.
	// pFq(p0, ...; q0, ...; x) = sum_{n>=0} (p0)_n ... /(q0)_n ... x^n/n!
	// (q)_n = q(q + 1)...(q + n - 1), (q)_0 = 1
	// (d/dx)^n pFq(p, q, x) = (p)_n/(q_n) pFq(p + n, q + n, x)
	template<class X = double>
	inline X pFq(unsigned np, const X* p, unsigned nq, const X* q, X x, X eps = Eps<X>/*, int dn = 0*/) 
	{
		if (np > nq + 1) { // diverges
			return x == 0 ? 0 : std::numeric_limits<X>::infinity();
		}
		if (np == nq + 1 and fabs(x) >= 1) {
			return std::numeric_limits<X>::infinity();
		}

		X p_qn = 1; // (p0)_n .../(q0)_n ...
		X x_n = 1;  // x^n/n!
		X s = 1;    // n = 0

		X ds = 1;
		for (unsigned n = 1; fabs(ds) > eps || fabs(x_n) > eps; ++n) {
			unsigned int i = 0;
			while (i < np && i < nq) {
				p_qn *= (p[i] + n - 1)/(q[i] + n - 1);
				++i;
			}
			while (i < np) {
				p_qn *= p[i] + n - 1;
				++i;
			}
			while (i < nq) {
				p_qn /= q[i] + n - 1;
				++i;
			}
			x_n *= x / n;
			ds = p_qn * x_n;
			s += ds;
		}

		return s;
	}

	//
	// Well known functions
	//
	template<class X = double>
	inline X _1F1(X p, X q, X x, X eps = Eps<X>)
	{
		return pFq(1, &p, 1, &q, x, eps);
	}
	template<class X = double>
	inline X _2F1(X a, X b,/**/ X c, X x, X eps = Eps)
	{
		X _p[2] = { a, b };
		return pFq(2, _p, 1, &c, x, eps);
	}

	// (lower) incomplete gamma function
	// int_0^x t^{a - 1} e^{-t} dt 
	template<class X>
	inline X incomplete_gamma(X a, X x, X eps = Eps<X>)
	{
		return (pow(x, a) * exp(x, eps) / a) * _1F1(1, 1 + a, x, eps);
	}

#ifdef _DEBUG
	template<class X = double>
	inline int test_hypergeometric()
	{
		{
			// (1 - x)^{-a} = 1 + ax + a(a + 1)/2 x^2 + ... = sum_{n>=0}(a)_n x^n/n!
			X as[] = { -0.1, 0, 0.1, 1, 2 };
			X xs[] = { -0.5, 0, 0.5 };
			for (X a : as) {
				for (X x : xs) {
					X F = pow(1 - x, -a); // 1F0(a,x)
					X _F = pFq(1, &a, 0, (const X*)nullptr, x); // 1F0(x)
					X dF = F - _F;
					ensure(fabs(dF) < 2*Eps<X>);
				}
			}
		}
		{
			// _2F1(1, 1, 2, -x) = log(1 + x)/x;
			X one = 1;
			X two = 2;
			double xs[] = { -.1, .1, .9 };

			for (X x : xs) {
				X F = _2F1(one, one, two, -x, Eps<X>);
				X F_ = log(1 + x) / x;
				X dF = F - F_;
				ensure(fabs(dF) < Eps<X>);
				X one_[] = { 1, 1 };
				X two_[] = { 2 };
				X _F = pFq(2, one_, 1, two_, -x);
				X Fd = _F - F;
				ensure(fabs(_F - F) < std::numeric_limits<X>::epsilon());
			}
		}

		return 0;
	}
#endif // _DEBUG
}