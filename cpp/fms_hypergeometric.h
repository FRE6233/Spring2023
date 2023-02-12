// fms_hypergeometric.h
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <functional>
#include <limits>

namespace fms {

	// Confluent hypergeometric function
	// pFq(p0, ...; q0, ...; x) = sum_0^infty (p0)_n ... /(q0)_n ... x^n/n!
	// (q)_n = q(q + 1)...(q + n - 1), (q)_0 = 1
	template<class C = double, class X = double>
	inline X pFq(unsigned np, const C* p, unsigned nq, const C* q, X x, const auto& eps) 
	{
		C p_qn = 1; // (p0)_n .../(q0)_n ...
		X x_n = 1;  // x^n/n!
		X s = 1;    // n = 0

		X ds = 1;
		for (unsigned n = 1; eps(ds) || eps(x_n); ++n) {
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
	template<class C = double, class X = double>
	inline X _1F1(C p, C q, X x, const auto& eps)
	{
		return pFq(1, &p, 1, &q, x, eps);
	}
	template<class C = double, class X = double>
	inline X _2F1(C a, C b, C c, X x, const auto& eps)
	{
		C _p[2] = { a, b };
		return pFq(2, _p, 1, &c, x, eps);
	}

	// (lower) incomplete gamma
	// int_0^x t^{a - 1} e^{-t} dt 
	template<class A, class X>
	inline X incomplete_gamma(A a, const X& x, auto eps)
	{
		return (pow(x, a) * exp(x) / a) * _1F1(1, 1 + a, x, eps);
	}

#ifdef _DEBUG
	template<class X = double>
	inline int test_2F1()
	{
		double eps = sqrt(std::numeric_limits<X>::epsilon());
		const auto Eps = [eps](const double& x) { return fabs(x) > eps; };
		{
			// _2F1(1, 1, 2, -x) = log(1 + x)/x;
			X one = 1;
			X two = 2;
			double xs[] = { -.1, .1, .9 };

			for (X x : xs) {
				X F = _2F1(one, one, two, -x, Eps);
				X F_ = log(1 + x) / x;
				X dF = F - F_;
				assert(fabs(dF) < eps);
				X one_[] = { 1, 1 };
				X two_[] = { 2 };
				X _F = pFq(2, one_, 1, two_, -x, Eps);
				X Fd = _F - F;
				assert(fabs(_F - F) < std::numeric_limits<X>::epsilon());
			}
		}
		{
			// erf(x) = 2x/sqrt(pi) 1F1(1/2, 3/2, -x^2)
			X xs[] = { -2, -1, -0.5, 0, 0.5, 1, 2 };
			for (X x : xs) {
				X e = erf(x);
				X _e = M_2_SQRTPI * x * _1F1(0.5, 1.5, -x * x, Eps);
				X de = e - _e;
				assert(fabs(de) < eps);
			}
		}

		return 0;
	}
#endif // _DEBUG
}