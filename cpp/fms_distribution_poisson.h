// fms_distribution_poisson.h - Poisson distribution
// P(X = k) = exp(-lambda) lambda^k/k!, k = 0, 1, 2, ...
// E[exp(sX)] = sum_k exp(s k) exp(-lambda) lambda^k/k!
//            = sum_k exp(-lambda) (lambda exp(s))^k/k!
//            = exp(lambda exp(s)) exp(-lambda)
//            = exp(lambda (exp(s) - 1))
// kappa(s) = lambda (exp(s) - 1) = sum_{k > 0} lambda s^k/k!, so kappa_k = lambda for k > 0
// E[X] = lambda, Var(X) = lambda
// Z = (X - lambda)/sqrt(lambda)
#pragma once
#include <cmath>
#include "fms_distribution.h"

namespace fms::distribution {

	// E[e^(s X - kappa(s)) 1(X <= x)]
	template<class X = double, class S = double>
	inline X share_cdf(X lambda, X x, S s)
	{
		X v = 0;
		X kappas = lambda * (std::exp(s) - 1);
		X exp_l = std::exp(-lambda);
		for (long k = 0, k_ = 1; k <= x; ++k, k_ *= k) {
			v += std::exp(s * k - kappas) * exp_l * std::pow(lambda, k) / k_;
		}

		return v;
	}

	// Z = X/sqrt(lambda) - sqrt(lambda) so E[Z] = 0, Var(Z) = 1
	template<class X = double, class S = double>
	struct poisson : public fms::distribution::standard<X, S> {
		X lambda;
		poisson(const X& lambda)
			: lambda(lambda)
		{
		}
		X _pdf(const X&, const S&) const override
		{
			return 0;
		}

		// Psi(x, s) = E[e^{s(X - mu)/sigma) - kappa_{(X - mu)/sigma}(s)} 1((X - mu)/sigma <= x)] 
		//           = E[e^(sX/sigma) - kappa(s/sigma)} 1(X <= mu + sigma x)]
		//           = Phi(mu + sigma x, s/sigma)
		X _cdf(const X& x, const S& s = 0) const override
		{
			X sigma = std::sqrt(lambda);

			return share_cdf(lambda, lambda + sigma*x, s/sigma);
		}
		X _mgf(const S&) const override
		{
			return 0;
		}
		X _cgf(const S&) const override
		{
			return 0;
		}
	};

} // namespace fms
