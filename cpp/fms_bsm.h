// fms_bsm.h  - Generalized Black-Scholes/Merton option valuation
// S_t = S_0 exp(r t + sX_t - κ(s)t)
#pragma once
#include "fms_black.h"

// Black-Scholes/Merton model
namespace fms::bsm {

	// D(t) = exp(-rate*t)
	template<class R>
	struct bond {
		R rate;
	};

	// S(t) D(t) = spot * exp(s X_t, - kappa(s)), s = sigma sqrt(t)
	template<class S, class V>
	struct stock {
		S spot;
		V sigma;
		const fms::distribution::base<S, V>* model;
	};

	template<class K, class T>
	struct option {
		K strike;
		T expiration; // in years
	};
	template<class K = double, class T = double>
	struct put : public option<K, T> { };
	template<class K = double, class T = double>
	struct call : public option<K, T> { };

	// S_t = s exp(rt + σB_t - σ^2t/2), B_t standard Brownian motion
	// F = f exp(sZ - 
	// f = s exp(rt), s = σ sqrt(t)
	inline auto Dfs(auto r, auto S_0, auto sigma, auto t)
	{
		using X = decltype(r* t);
		auto D = std::exp(-r * t);
		auto f = S_0 / D;
		auto s = sigma * sqrt(t);

		return std::make_tuple(D, f, s);
	}
#ifdef _DEBUG
	inline int test_Dfs()
	{
		{
			double r = 0, S_0 = 100, sigma = 0.2, t = 0.25;
			auto [D, f, s] = Dfs(r, S_0, sigma, t);
			ensure(1 == D);
			ensure(S_0 == f);
			ensure(sigma * sqrt(t) == s);
		}

		return 0;
	}
#endif // _DEBUG

	// exp(-rt) E[max{k - S_T, 0]
	template<class R, class S, class V, class K, class T>
	inline auto value(const bond<R>& B, const stock<S, V>& S_, const put<K, T>& O)
	{
		auto [D, f, s] = Dfs(B.rate, S_.spot, S_.sigma, O.expiration);

		return D * black::put::value(f, s, O.strike);
	}
	/*
	template<class R = double, class S, class V, class K, class T>
	inline auto value(const R& rate, const stock<S, V>& s, const call<K, T>& o, const auto& m)
	{
		auto [D, f, s] = Dfs(rate, s.spot, s.sigma, o.expiration);

		return D * black::call::value(f, s, o.strike);
	}
	*/

} // namespace fms::bsm
