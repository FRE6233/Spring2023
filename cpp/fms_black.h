// fms_black.h - Fischer Black model.
#pragma once
#include <cmath>
#include <stdexcept>
#include "fms_distribution.h"

namespace fms {
	namespace black {

		// F = f exp(sZ - kappa(s) <= k  if and only if  Z <= z(k; f, s) = (log(k/f) + kappa(s))/s
		template<class F = double, class S = double, class K = double>
		inline auto moneyness(const F& f, const S& s, const K& k, const fms::distribution::base<F, S>& m)
		{
			if (f <= 0 or s <= 0 or k <= 0) {
				throw std::runtime_error(__FUNCTION__ ": arguments must be positive");
			}

			return (log(k / f) + m.cgf(s)) / s;
		}

		// E[max{k - F, 0}] = k P(F <= k) - f P_s(F <= k)
		template<class F = double, class S = double, class K = double>
		inline auto value_put(const F& f, const S& s, const K& k, const fms::distribution::base<F, S>& m)
		{
			double z = moneyness(f, s, k, m);

			return k * m.cdf(z) - f * m.cdf(z, s);
		}

		// -P_s(F <= k)
		template<class F = double, class S = double, class K = double>
		inline auto delta_put(const F& f, const S& s, const K& k, const fms::distribution::base<F, S>& m)
		{
			double z = moneyness(f, s, k, m);

			return -m.cdf(z, s);
		}

		// max{ F - k, 0 } - max{ k - F, 0 } = F - k
		template<class F = double, class S = double, class K = double>
		inline auto value_call(const F& f, const S& s, const K& k, const fms::distribution::base<F, S>& m)
		{
			return value_put(f, s, k, m) + f - k;
		}
	}

	// Black-Scholes/Merton model
	// S_t = s exp(rt + σB_t + σ^2t/2), B_t standard Brownian motion
	// f = s exp(rt), s = σ sqrt(t)
	namespace bsm {

		template<class K, class T>
		struct option {
			K k; // strike
			T t; // expiration in years
		};
		template<class K = double, class T = double>
		struct put : public option<K, T> {
			put(const K& k, const T& t)
				: option{ -k,t }
			{ }
		};
		template<class K = double, class T = double>
		struct call : public option<K, T> {
			call(const K& k, const T& t)
				: option{ k,t }
			{ }
		};
		/*
		template<class S = double, class R = double, class Sigma = double, class T = double>
		inline auto value(const S& s, const R& r, const Sigma& sigma, const T& t, const distribution::base<R, R>& m)
		{
		}
		*/
	}

} // namespace fms::black
