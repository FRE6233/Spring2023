// fms_black.h - Generalized Fischer Black model.
// F = f exp(sX - κ(s)), κ(s) = log E[exp(sX)], E[X] = 0, Var(X) = 1.
// E[F] = f, Var(log(F)) = s^2.
#pragma once
#include <cmath>
#include <stdexcept>
#include "fms_distribution.h"

namespace fms::black {

	// F = f exp(sZ - κ(s) <= k  if and only if  Z <= z(k; f, s) = (log(k/f) + κ(s))/s
	template<class F = double, class S = double, class K = double>
	inline auto moneyness(const F& f, const S& s, const K& k, const fms::distribution::standard<F, S>* p)
	{
		if (f <= 0 or s <= 0 or k <= 0) {
			throw std::runtime_error(__FUNCTION__ ": arguments must be positive");
		}

		return (log(k / f) + p->cgf(s)) / s;
	}

	namespace put {

		// E[max{k - F, 0}] = k P(F <= k) - f P_s(F <= k)
		template<class F = double, class S = double, class K = double>
		inline auto value(const F& f, const S& s, const K& k, const fms::distribution::standard<F, S>* p)
		{
			double z = moneyness(f, s, k, p);

			return k * p->cdf(z) - f * p->cdf(z, s);
		}

		// (d/df)E[max{k - F, 0}] = E[-exp(sX - κ(s)1(F <= k)] = -P_s(F <= k)
		template<class F = double, class S = double, class K = double>
		inline auto delta(const F& f, const S& s, const K& k, const fms::distribution::standard<F, S>* p)
		{
			double z = moneyness(f, s, k, p);

			return -p->cdf(z, s);
		}

	} // namespace put

	namespace call {

		// max{ F - k, 0 } - max{ k - F, 0 } = F - k
		template<class F = double, class S = double, class K = double>
		inline auto value(const F& f, const S& s, const K& k, const fms::distribution::standard<F, S>* p)
		{
			return put::value(f, s, k, p) + f - k;
		}
		// (d/df) (max{ k - F, 0 } + F - k)
		template<class F = double, class S = double, class K = double>
		inline auto delta(const F& f, const S& s, const K& k, const fms::distribution::standard<F, S>* p)
		{
			return put::delta(f, s, k, p) + 1;
		}

	} // namespace call

} // namespace fms::black
