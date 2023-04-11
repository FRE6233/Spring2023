// fms_pwflat.h - piecewise flat curve
/*
	       { f[i] if t[i-1] < t <= t[i];
	f(t) = { _f   if t > t[n-1];
	       { NaN  if t < 0
	F                                   _f
	|        f[1]             f[n-1] o--------
	| f[0] o------          o--------x
	x------x      ... ------x
	|
	0-----t[0]--- ... ---t[n-2]---t[n-1]--- T
*/
#pragma once
#include <cmath>
#include <algorithm>
#include <limits>
#include <numeric>
#include <iterator>
#include <vector>
#include "ensure.h"

namespace fms {
	namespace pwflat {

		template<class X>
		inline static constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

		// strictly increasing values
		template<class I>
		inline bool monotonic(const I b, const I e)
		{
			using T = std::iterator_traits<I>::value_type;

			return e == std::adjacent_find(b, e, std::greater_equal<T>{});
		}
		template<class T>
		inline bool monotonic(size_t n, const T* t)
		{
			return monotonic(t, t + n);
		}

		// f(u) assuming t[i] monotonically increasing
		template<class T = double, class F = double>
		inline F value(T u, size_t n, const T* t, const F* f, F _f = NaN<F>)
		{
			if (u < 0)
				return NaN<F>;
			if (n == 0)
				return _f;

			auto ti = std::lower_bound(t, t + n, u);

			return ti == t + n ? _f : f[ti - t];
		}

		// int_0^u f(t) dt
		template<class T, class F>
		inline F integral(T u, size_t n, const T* t, const F* f, F _f = NaN<F>)
		{
			if (u < 0)
				return NaN<F>;
			if (u == 0)
				return 0;
			if (n == 0)
				return u * _f;

			F I = 0;
			T t_ = 0;

			size_t i;
			for (i = 0; i < n && t[i] <= u; ++i) {
				I += f[i] * (t[i] - t_);
				t_ = t[i];
			}
			if (fabs(u - t_) > std::numeric_limits<T>::epsilon()) {
				I += (i == n ? _f : f[i]) * (u - t_);
			}

			return I;
		}

		// discount D(u) = exp(-int_0^u f(t) dt)
		template<class T, class F>
		inline F discount(T u, size_t n, const T* t, const F* f, F _f = NaN<F>)
		{
			return exp(-integral(u, n, t, f, _f));
		}

		// spot r(u) = (int_0^u f(t) dt)/u
		// r(u) = f(u) if u <= t[0]
		template<class T, class F>
		inline F spot(T u, size_t n, const T* t, const F* f, F _f = NaN<F>)
		{
			return n == 0 ? _f
				: u <= t[0] ? value(u, n, t, f, _f) : integral(u, n, t, f, _f) / u;
		}

		template<class T = double, class F = double>
		class curve {
			std::vector<T> t;
			std::vector<F> f;
			F _f;
		public:
			curve()
				: _f(NaN<F>)
			{ }
			curve(size_t n, const T* t_, const F* f_, F _f = NaN<F>)
				: t(t_, t_ + n), f(f_, f_ + n), _f(_f)
			{
				ensure(ok());
			}
			curve(const std::vector<T>& t, const std::vector<F>& f, F _f = NaN<F>)
				: t(t), f(f), _f(_f)
			{
				ensure(ok());
			}
			curve(const curve&) = default;
			curve& operator=(const curve&) = default;
			curve(curve&&) = default;
			curve& operator=(curve&&) = default;
			~curve()
			{ }

			bool ok() const
			{
				return t.size() == f.size() && (t.size() == 0 || t[0] > 0 && monotonic(t.begin(), t.end()));
			}

			size_t size() const
			{
				return t.size();
			}
			const T* time() const
			{
				return t.data();
			}
			const F* forward() const
			{
				return f.data();
			}
			const F* rate() const
			{
				return f.data();
			}
			std::pair<T, F> back() const
			{
				return { t.back(), f.back() };
			}

			// add point
			curve& extend(T t_, F f_)
			{
				ensure(size() == 0 || t_ > t.back());

				t.push_back(t_);
				f.push_back(f_);

				return *this;
			}
			// Get extrapolated value.
			F extrapolate() const
			{
				return _f;
			}
			// Set extrapolated value.
			curve& extrapolate(F f_)
			{
				_f = f_;
			}

			F value(T u) const
			{
				return pwflat::value(u, t.size(), t.data(), f.data(), _f);
			}
			F operator()(T u) const
			{
				return value(u);
			}
			F integral(T u) const
			{
				return pwflat::integral(u, t.size(), t.data(), f.data(), _f);
			}
			F discount(T u) const
			{
				return pwflat::discount(u, t.size(), t.data(), f.data(), _f);
			}
			F spot(T u) const
			{
				return pwflat::spot(u, t.size(), t.data(), f.data(), _f);
			}
		};

	} // namespace pwflat
} // namespace fms