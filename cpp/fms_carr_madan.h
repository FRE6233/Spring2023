// fms_carr_madan.h - Carr-Madan formula
// f(x) = f(a) + f'(a)(x - a) + int_{k < a} (k - x)^+ f''(k) dk + int_{k >= a} (x - k)^+ f''(k) dk
// Strikes k[i] and values f[i], 0 <= i < n, determine a piece-wise linear and continuous curve f_(x) for all x.
// 
// Let m[i] = (f[i + 1] - f[i])/(k[i + 1] - k[i]), 0 <= i < n - 1, be the slopes over (k[i], k[i + 1]].
// Let mm[i] = m[i] - m[i - 1] be the jumps at t[i], 1 <= i < n - 1.

// The Carr-Madan formula is determined from data (f(a), mm[0], ..., mm[n - 2], f'(a)) by
// f_(x) = f(a) + f'(a)(x - a) + sum_{k[i] <= a} (k[i] - x)^+ mm[i] + sum_{k[i] > a} (x - k[i])^+ mm[i]
#pragma once
#include "ensure.h"
#include <span>

namespace fms::carr_madan {

	template<class X>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

	// First index of xs where x > xs[i] is false so xs[i - 1] < x <= xs[i]
	template<class X = double, size_t N>
	inline constexpr auto index(const X& x, const std::span<X, N>& xs)
	{
		return std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();
	}

#ifdef _DEBUG
	inline int test_index()
	{
		{
			double xs[] = { 1, 2, 3 };
			for (double x : {0., 1., 1.1, 2.9, 3., 3.1}) {
				auto i = index(x, std::span(xs));
				if (x <= xs[0]) {
					ensure(i == 0);
				}
				else if (x > xs[2]) {
					ensure(i == 3);
				}
				else {
					ensure(xs[i - 1] < x and x <= xs[i]);
				}
			}
		}

		return 0;
	}
#endif // _DEBUG

	// value and slope at x in (k[i - 1], k[i]]
	template<class X, size_t N>
	inline constexpr std::pair<X, X> tangent(X x, const std::span<X,N>& k, const std::span<X,N>& f)
	{
		if (k.size() != f.size()) return { NaN<X>, NaN<X> };

		X fx, mx;
		auto n = k.size();

		switch (n) {
		case 0:
			fx = NaN<X>;
			mx = NaN<X>;
			break;
		case 1: // constant
			fx = f[0];
			mx = 0;
			break;
		case 2: // line
			mx = (f[1] - f[0]) / (k[1] - k[0]);
			fx = f[0] + mx * (x - k[0]);
			break;
		default:
			auto i = index(x, k);
			if (i == 0) { // extrapolate low
				mx = (f[1] - f[0]) / (k[1] - k[0]);
				fx = f[0] + mx * (x - k[0]);
			}
			else if (i == n) { // extrapolate high
				mx = (f[n - 1] - f[n - 2]) / (k[n - 1] - k[n - 2]);
				fx = f[n - 1] + mx * (x - k[n - 1]);
			}
			else {
				mx = (f[i] - f[i - 1]) / (k[i] - k[i - 1]);
				fx = f[i] + mx * (x - k[i]);
			}
		}

		return { fx, mx };
	}

#ifdef _DEBUG
	inline int test_tangent()
	{
		{
			double ks[] = { 0,1,2,3,4 }, fs[] = { 0, 0, 1, 0, 0 };
			{
				auto [f, m] = tangent(0., std::span(ks), std::span(fs));
				ensure(f == 0 and m == 0);
			}
			{
				auto [f, m] = tangent(.5, std::span(ks), std::span(fs));
				ensure(f == 0 and m == 0);
			}
			{
				auto [f, m] = tangent(1., std::span(ks), std::span(fs));
				ensure(f == 0 and m == 0);
			}
			{
				auto [f, m] = tangent(1.5, std::span(ks), std::span(fs));
				ensure(f == 0.5 and m == 1);
			}
			{
				auto [f, m] = tangent(2., std::span(ks), std::span(fs));
				ensure(f == 1 and m == 1);
			}
			{
				auto [f, m] = tangent(2.5, std::span(ks), std::span(fs));
				ensure(f == 0.5 and m == -1);
			}
			{
				auto [f, m] = tangent(3., std::span(ks), std::span(fs));
				ensure(f == 0 and m == -1);
			}
			{
				auto [f, m] = tangent(3.5, std::span(ks), std::span(fs));
				ensure(f == 0 and m == 0);
			}
			{
				auto [f, m] = tangent(4.1, std::span(ks), std::span(fs));
				ensure(f == 0 and m == 0);
			}

		}

		return 0;
	}
#endif // _DEBUG

	// Replace f with (f[0], (f[1] - f[0])/(k[1] - k[0]), f''(k[1]), ..., f''(k[n - 2]))
	template<class X, size_t N>
	inline constexpr void fit(const std::span<X,N>& k, std::span<X,N>& f)
	{
		auto n = k.size();
		ensure(n == f.size());

		// (f[0], m[0], m[1], ...), m[i] = (f[i] - f[i - 1])/(k[i] - k[i - 1])
		for (size_t i = n - 1; i > 0; --i) {
			f[i] = (f[i] - f[i - 1]) / (k[i] - k[i - 1]);
		}
		// (f[0], m[0], m[1] - m[0], ...)
		for (size_t i = n - 1; i > 1; --i) {
			f[i] = f[i] - f[i - 1];
		}
	}
	
	// unfit(fit(k, f)) leaves f unchanged
	template<class X, size_t N>
	inline constexpr void unfit(const std::span<X,N>& k, std::span<X,N>& f)
	{
		auto n = k.size();
		ensure(n == f.size());

		if (n > 1) {
			X m = f[1];
			f[1] = f[0] + m * (k[1] - k[0]);
			for (size_t i = 2; i < n; ++i) {
				m = f[i] + m; // m[i]
				f[i] = f[i - 1] + m * (k[i] - k[i - 1]);
			}
		}
	}

#ifdef _DEBUG
	template<class X>
	inline int test_fit()
	{
		auto eq = [](const std::span<X>& a, const std::initializer_list<X>& b) {
			if (a.size() != b.size()) {
				return false;
			}
			for (size_t i = 0; i < a.size(); ++i) {
				if (a[i] != *(b.begin() + i)) {
					return false;
				}
			}
			return true;
		};
		{
			X k[] = { 0, 1, 2, 3 }, f[] = { 0, 1, 1, 0 };
			auto fs = std::span(f); // l-value
			fit(std::span(k), fs);
			ensure(eq(f, {0, 1, -1, -1}));
			unfit(std::span(k), fs);
			ensure(eq(f, { 0, 1, 1, 0 }));
		}

		return 0;
	}

#endif // _DEBUG
	
	// value pwlinear k, f, using forward x, puts below a, and calls above a
	template<class X>
	inline constexpr X value(const X& x, const X& a, const std::span<X>& p, const std::span<X>& c,
		const std::span<X>& k, std::span<X>& f)
	{
		size_t n = p.size();
		ensure(n == c.size());
		ensure(n == k.size());
		ensure(n == f.size());

		auto [fa, ma] = tangent(a, k, f);
		X v = fa + ma * (x - a);

		fit(k, f);
		size_t i = 1;
		for (; i < n - 1 and k[i] < a; ++i) {
			v += p[i] * f[i + 1];
		}
		for (; i < n - 1; ++i) {
			v += c[i] * f[i + 1];
		}
		unfit(k, f);

		return v;
	}
	

} // namespace fms