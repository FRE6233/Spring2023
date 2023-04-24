// fms_american.h - American option pricing
// F_k = f exp(s W_k/sqrt(n))/cosh^k(s/sqrt(n)) where W_k is random walk
// E[F_n] = f, Var(log F_n) = s^2
#pragma once
#include "ensure.h"
#include <cmath>
#include <algorithm>
#include <array>
#include <span>

namespace fms::binomial {

	template<class X>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

	// F_k(j)
	template<class X>
	constexpr X forward(X f, X s, size_t n, size_t k, size_t j)
	{
		ensure(n >= k and k >= j);

		if (n == 0) {
			return f;
		}

		X sn = s / sqrt(n);
		X cn = cosh(sn);
		
		return f * exp(sn * (2.0 * j - k)) / pow(cn, k);
	}

	// Populate F_n(j) at step n = F.size() - 1
	template<class X, size_t N>
	constexpr size_t fill(X f, X s, std::span<X, N> F)
	{
		ensure(F.size() > 0);
		size_t n = F.size() - 1;

		if (n == 0) {
			F[0] = f;
		}
		else {
			X sn = s / sqrt(n);
			X cn = cosh(sn);
			for (size_t j = 0; j <= n; ++j) {
				F[j] = forward(f, s, n, n, j);
			}
		}

		return n;
	}
#ifdef _DEBUG
	static int fill_test()
	{
		double f = 100, s = 0.1;
		int n;
		double F[10];
		{
			n = 0;
			fill(f, s, std::span(F, n + 1));
			ensure(F[0] == f);
		}
		{
			n = 1;
			double sn = s / sqrt(n);
			double cn = cosh(sn);

			fill(f, s, std::span(F, n + 1));
			ensure(F[0] == f * exp(-sn) / cn);
			ensure(F[1] == f * exp(sn) / cn);
		}
		{
			n = 2;
			double sn = s / sqrt(n);
			double cn = cosh(sn);

			fill(f, s, std::span(F, n + 1));
			ensure(F[0] == f * exp(-2 * sn) / (cn * cn));
			ensure(F[1] == f * exp(0*sn) / (cn * cn));
			ensure(F[2] == f * exp(2*sn) / (cn * cn));
		}

		return 0;
	}
#endif // _DEBUG

	// Skewed Random Walk
	// Let P(X_j = 1) = p, P(X_j = -1) = 1 - p be independent and Wp_n = a_n sum_j X_j - b_n
	// Note E[X_j] = 2p - 1 and Var(X_j) = 4p(1 - p).
	// Define Fp_n = f exp(s Wp_n)/cp_n where cp_n = E[s Wp_n].
	// !!! Find a_n and b_n such that E[Fp_n] = f and Var(log Fp_n) = s^2
	// Fp_k(j)
	template<class X>
	constexpr X forwardp(X f, X s, X p, size_t n, size_t k, size_t j)
	{
		ensure(p > 0 and 1 - p > 0);

		return 0; //!!! Implement
	}

	// Populate Fp_n(j) at step n = F.size() - 1
	template<class X, size_t N>
	constexpr size_t fillp(X f, X s, X p, std::span<X, N> F)
	{
		return 0; // !!! Implement
	}

	namespace european {

		// { v[0], ..., v[n-1] } => { (v[0] + v[1])/2, ..., (v[n-2] + v[n-1])/2 }
		template<class X, size_t N>
		constexpr auto step(std::span<X, N> v)
		{
			for (size_t i = 0; i < v.size() - 1; ++i) {
				v[i] = (v[i] + v[i + 1]) / 2;
			}

			return std::span<X>(v.begin(), v.size() - 1);
		}

		// { v[0], ..., v[n-1] } => { (v[0] (1 - p) + v[1] p, ..., v[n-2] (1 - p) + v[n-1] p }
		// !!! Implement stepp(X p, std::span<X, N> v)
		template<class X, size_t N>
		constexpr auto stepp(X p, std::span<X, N> v)
		{
			return v; // !!! Implement
		}

		template<class Phi, class X, size_t N>
		X value(X f, X s, Phi phi, std::span<X, N> v)
		{
			fill(f, s, v);
			// Apply phi to F
			std::transform(v.begin(), v.end(), v.begin(), phi);
			// Expected value
			while (v.size() > 1) {
				v = step(v);
			}

			return v[0];
		}

		// !!! Implement XLL.BINOMIAL.EUROPEANP in xll_binomial.cpp
		template<class Phi, class X, size_t N>
		X valuep(X f, X s, X p, Phi phi, std::span<X, N> v)
		{
			fillp(f, s, p, v);
			// Apply phi to F
			std::transform(v.begin(), v.end(), v.begin(), phi);
			// Expected value
			while (v.size() > 1) {
				v = step(v);
			}

			return v[0];
		}


#ifdef _DEBUG
		inline int test()
		{
			double eps = std::numeric_limits<double>::epsilon();
			double f = 100;
			double s = 0.1;
			double k = 100;
			double v[101];
			{
				double v0 = value(f, s, [](double x) { return x; }, std::span(v, 101));
				double err = v0 - f;
				ensure(fabs(err) < 1e-12);
			}
			{
				double v0 = value(f, s, [=](double x) { return std::max(x - k, 0.); }, std::span(v, 101));
				double err = v0 - 3.99;
				ensure(fabs(err) < 3e-3);
			}
			{
				double v0 = value(f, s, [=](double x) { return std::max(k - x, 0.); }, std::span(v, 101));
				double err = v0 - 3.99;
				ensure(fabs(err) < 3e-3);
			}

			return 0;
		}
#endif // _DEBUG

	}

	namespace american {

		template<class Phi, class X = double>
		constexpr auto step(X f, X s, size_t n, Phi phi, std::span<X> v)
		{
			size_t k = v.size() - 1;

			for (size_t j = 0; j < k; ++j) {
				v[j] = (v[j] + v[j + 1]) / 2;
				X vj = phi(forward(f, s, n, k, j));
				if (vj > v[j]) {
					v[j] = vj;
				}
			}

			return std::span<X>(v.begin(), v.size() - 1);
		}

		// Return max_tau E[phi(F_tau)]
		template<class Phi, class X = double>
		inline X value(X f, X s, Phi phi, std::span<X> v)
		{
			size_t n = fill(f, s, v);
			// Apply phi to F
			std::transform(v.begin(), v.end(), v.begin(), phi);
			// Expected value
			while (v.size() > 1) {
				v = step(f, s, n, phi, v);
			}

			return v[0];
		}

#ifdef _DEBUG
		inline int test()
		{
			double f = 100, s = 0.1, k = 100;
			double v[101];
			auto phi = [=](double x) { return std::max(x - k, 0.); }; // call
			{
				auto id = [](double x) { return x; };
				double v0;
				v0 = value(f, s, id, std::span(v, 1));
				ensure(v0 == f);
				v0 = value(f, s, id, std::span(v, 2));
				ensure(v0 == f);
				v0 = value(f, s, id, std::span(v, 3));
				ensure(v0 == f);
			}
			{
				auto c = [=](double x) { return std::max(x - k, 0.); };
				double va = value(f, s, c, std::span(v, 101));
				double ve = european::value(f, s, c, std::span(v, 101));
				ensure(va == ve);
			}
			{
				auto p = [=](double x) { return std::max(k - x, 0.); };
				double va = value(f, s, p, std::span(v, 101));
				double ve = european::value(f, s, p, std::span(v, 101));
				ensure(va > ve);
			}

			return 0;
		}
#endif // _DEBUG

	} // namespace american
} // namespace fms::binomial
