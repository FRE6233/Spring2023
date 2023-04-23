// fms_ho_lee.h - Ho-Lee model f_t = r + σ B_t
// D_t(u) = E_t[D_u/D_t] = exp(-r (u - t) + σ^2 (u - t)^3/6 + σ (u - t) B_t)
#pragma once
#include <cmath>
#include "../cpp/fms_bachelier.h"

namespace fms {

	template<class R = double, class S = double, class T = double>
	struct ho_lee {
		R r;
		S σ;
		ho_lee(const R& r, const S& s)
			: r{ r }, σ{ s }
		{ }

		// E[log D_t(u)]
		auto E_logD(T t, T u) const
		{
			return -r * (u - t) + σ * σ * pow(u - t, 3) / 6;
		}

		// Var[log D_t(u)]
		auto Var_logD(T t, T u) const
		{
			return σ * σ * (u - t) * (u - t) * t;
		}

		// Cov(log D_t(u), log D_t(v)) = Cov(-σ(u - t)B_t, -σ(v - u)B_t)
		auto Cov_logD(T t, T u, T v) const
		{
			return σ * σ * (u - t) * (v - t) * t;
		}

		// E[D_t(u)]
		auto ED(T t, T u) const
		{
			return exp(E_logD(t, u) + Var_logD(t, u) / 2);
		}

		// Difference of futures and forward.
		auto convexity(T t) const
		{
			return σ * σ * t * t / 2;
		}

		// Forward f(t)
		auto f(T t) const
		{
			return r - convexity(t);
		}
	};

	// E[max{k - f_t,0} D_t] = ???
	template<class R, class S, class T>
	inline auto floorlet(const ho_lee<R, S, T>& m, R k, T t)
	{
		R k_ = k + m.convexity(t);
		R f = m.r;
		R s = m.σ * sqrt(t);

		return bachelier::put::value(f, s, k) * m.ED(0, t);
	}

} // namespace fms