// fms_bootstrap.h - Bootstrap a forward curve
#pragma once
#include "ensure.h"
#include "fms_fixed_income.h"
#include "fms_pwflat.h"
#include "fms_root1d.h"

namespace fms::bootstrap {

	template<class U = double, class C = double, class T = double, class F = double>
	inline double present_value(size_t m, const U* u, const C* c, // instrument
		size_t n, const T* t, const F* f, // piecewise flat curve
		F _f = std::numeric_limits<F>::quiet_NaN()) // extrapolate
	{
		double pv = 0;

		for (size_t i = 0; i < m; ++i) {
			pv += c[i] * pwflat::discount(u[i], n, t, f, _f);
		}

		return pv;
	}

	// contant forward extrapolation repricing instrument
	template<class U = double, class C = double, class T = double, class F = double>
	inline std::pair<T, F> extend(size_t m, const U* u, const C* c,
		size_t n, const T* t, const F* f, F p = 0, F _f = 0)
	{
		ensure(m > 0);

		// end of curve 
		auto t_ = n == 0 ? 0 : t[n - 1];
		// last cash flow time
		auto u_ = u[m - 1];
		// instrument maturity must be past end of curve
		ensure(u_ > t_);
		// last cash flow
		auto c_ = c[m - 1];
		// discount to end of current curve
		auto D_ = pwflat::discount(t_, n, t, f);

		// If only one cash flow occurs past the end of the curve there is a closed form solution:
		// We have p = pv + c D e^{-f(u - t)}, where pv is the present value of all but the last
		// cash flow, u is the last cash flow time, and c is the last cash flow.
		if (m == 1 || m == 2 && u[0] <= t_) { // ??? m > 2 ???
			auto pv = present_value(m - 1, u, c, n, t, f);

			return { u_, log((p - pv) / (c_ * D_)) / (t_ - u_) };
		}

		// If exactly two cash flows and price is 0, then we know u[0] > t_ or else
		// the previous case would hold.
		// 0 = c0 D exp(-f(u0 - t)) + c1 D exp(-f(u1 - t)) so
		// f = - log(-c0/c1)/(u1 - u0).
		if (p == 0 && m == 2) {
			return { u_, log(-c[0] / c[1]) / (u[0] - u[1]) };
		}

		/*
		size_t m_ = std::lower_bound(u, u + m, t_) - u;
		double pv_ = present_value(m_, u, c, n, t, f);
		*/

		auto pv = [m, u, c, n, t, f, p](double _f) {
			return -p + present_value(m, u, c, n, t, f, _f);
		};

		// use last forward as initial guess
		if (_f == 0) {
			_f = (n == 0) ? 0.01 : f[n - 1];
		}

		auto sec = root::secant(pv, _f, _f + 0.001);


		return { u_, sec.solve() };
	}

	template<class U = double, class C = double, class T = double, class F = double>
	inline pwflat::curve<T, F>& extend(const fixed_income::instrument<U, C>& i, pwflat::curve<T, F>& f,
		F p = 0, F _f = 0)
	{
		auto [u_, f_] = extend(i.size(), i.time(), i.cash(), f.size(), f.time(), f.forward(), p, _f);

		return f.extend(u_, f_);
	}

#ifdef _DEBUG
	inline int extend_test()
	{
		using inst = fixed_income::instrument_value<>;
		constexpr double eps = std::numeric_limits<double>::epsilon();
		double c = log(2.);

		{
			pwflat::curve<> f;
			inst i0({ 0, 1 }, { -1, 2 });
			f = extend(i0, f);
			ensure(f.size() == 1);
			ensure(f.back().first == 1);
			ensure(fabs(f(0) - c) <= eps);
		}
		{
			pwflat::curve<> f;
			inst i0({ 1 }, { 2 });
			f = extend(i0, f, 1.);
			ensure(f.size() == 1);
			ensure(f.back().first == 1);
			ensure(fabs(f(0) - c) <= eps);
		}
		{
			pwflat::curve<> f;
			inst i0({ 1, 2 }, { -1, 2 });
			f = extend(i0, f);
			ensure(f.size() == 1);
			ensure(f.back().first == 2);
			ensure(fabs(f(0) - c) <= eps);
		}
		{
			pwflat::curve<> f;
			inst i0({ 0, 1 }, { -1, 2 });
			f = extend(i0, f);

			inst i1({ 1, 2 }, { -1, 2 });
			f = extend(i1, f);
			ensure(f.size() == 2);
			ensure(f.back().first == 2);
			ensure(fabs(f(0.5) - c) <= eps);
			ensure(fabs(f(1.5) - c) <= eps);
		}
		{
			pwflat::curve<> f;
			inst i0({ 0, 1 }, { -1, 2 });
			f = extend(i0, f);

			inst i1({ .9, 1.9 }, { -1, 2 });
			f = extend(i1, f);
			ensure(f.size() == 2);
			ensure(f.back().first == 1.9);
			ensure(fabs(f(0.5) - c) <= eps);
			ensure(fabs(f(1.5) - c) <= eps);
		}
		{
			pwflat::curve<> f;
			inst i0({ 0, 1 }, { -1, 2 });
			f = extend(i0, f);

			inst i1({ 1.1, 2.1 }, { -1, 2 });
			f = extend(i1, f);
			ensure(f.size() == 2);
			ensure(f.back().first == 2.1);
			ensure(fabs(f(0.5) - c) <= eps);
			ensure(fabs(f(1.5) - c) <= eps);
		}
		{
			pwflat::curve<> f;
			inst i0({ 0, 1 }, { -1, 2 });
			f = extend(i0, f);

			inst i1({ 1.1, 2.1 }, { -1, 2 });
			f = extend(i1, f);

			inst i2({ 0,1,2,3 }, { -1, 1, 1, 2 });
			f = extend(i2, f);
			ensure(f.size() == 3);
			ensure(f.back().first == 3);
			ensure(fabs(f(3) - c) <= sqrt(eps));
		}

		return 0;
	}
#endif // _DEBUG

	template<class I, class T = double, class F = double>
	inline pwflat::curve<T, F> build(const I& is)
	{
		pwflat::curve<T, F> f;

		for (const auto& i : is) {
			f = extend(*i, f);
		}

		return f;
	}

	// Build from two lists of instruments.
	// Take from first while termination less than initial second effective.
	template<class I, class J, class T = double, class F = double>
	inline pwflat::curve<T, F> build2(const I& is, const J& js)
	{
		pwflat::curve<T, F> f;

		T eff = std::numeric_limits<T>::max();
		if (js.begin() != js.end()) {
			eff = (*js.begin())->effective();
		}

		for (const auto& i : is) {
			if (i->termination() < eff) {
				f = extend(*i, f);
			}
			else {
				break;
			}
		}
		for (const auto& j : js) {
			f = extend(*j, f);
		}

		return f;
	}

} // namespace fms::bootstrap