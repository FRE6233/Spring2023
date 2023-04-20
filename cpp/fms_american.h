// fms_american.h - American option pricing
#pragma once
#include "ensure.h"
#include <cmath>
#include <array>

namespace fms::american {

	namespace binomial {

		// F_n = f exp(s W_n/sqrt(n))/cosh^n(s/sqrt(n))
		// where W_n is random walk
		// max_tau E[phi(F_tau)]
		template<size_t N, class Phi, class X = double>
		inline X value(X f, X s, Phi phi, int n, std::span<X,N> v)
		{
			ensure(n < N);

			if (n == 0) {
				return v[0];
			}

			// payoff at expiration
			if (n == N - 1) {
				X ds = s / sqrt(1.*n);
				for (int i = 0; i <= n; ++i) {
					X F = f * exp(ds * (n - 2*i)) / pow(cosh(ds), n);
					v[i] = phi(F);
				}
				--n;
			}

			// v has values at step n + 1
			for (size_t i = 0; i <= n; ++i) {
				X ds = s / sqrt(1.*n);
				X F = f * exp(ds * (n - 2*i)) / pow(cosh(ds), n);
				X vi = phi(F);
				X Ev = (v[i] + v[i + 1]) / 2;
				v[i] = std::max(vi, Ev);
			}

			return value(f, s, phi, n - 1, v);
		}

#ifdef _DEBUG
		inline int test()
		{
			{
				double f = 100, s = 0.1, k = 100;
				auto phi = [=](double x) { return std::max(x - k, 0.); }; // call
				constexpr size_t N = 3;
				double v[N + 1];
				double v0 = value(f, s, phi, N, std::span(v));
				ensure(fabs(v0 - 10) <= 1);
			}

			return 0;
		}
#endif // _DEBUT

	} // namespace binomial

} // namespace fms::american
