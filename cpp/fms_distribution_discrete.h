// fms_distribution_discrete.h - Discrete distribution.
#pragma once
#include <algorithm>
#include <span>
#include <valarray>
#include <vector>
#include "ensure.h"
#include "fms_distribution.h"

namespace fms::distribution {

	template<class X = double, class S = double>
	struct discrete : public standard<X, S> {
		std::valarray<X> x, p;
		discrete()
		{ }
		discrete(size_t n, const X* _x, const X* _p, bool std = false)
			: x(_x, n), p(_p, n)
		{
			// p is a probability measure
			ensure(p.min() >= 0);
			ensure(std::fabs(p.sum() - 1) <= 100*std::numeric_limits<X>::epsilon());
			if (std) {
				standardize();
			}
		}
		discrete(const std::span<X>& _x, const std::span<X>& _p, bool std = false)
			: discrete(_x.size(), _x.data(), _p.data(), std)
		{
			ensure(_x.size() == _p.size());
		}
		discrete(const discrete&) = default;
		discrete& operator=(const discrete&) = default;
		~discrete() = default;

		// convert to mean 0 variance 1
		discrete& standardize()
		{
			X mu = (x*p).sum();
			x -= mu;
			X sigma2 = (x * x * p).sum();
			x /= sqrt(sigma2);

			return *this;
		}

		// e^{sx_j)/E[e^{sX}] P(X = x_j)
		X _pdf(const X& z, const S& s) const override
		{
			return 0; // stub
		}

		// E[e^{sX}/E[e^{sX}] 1(X <= z)]
		X _cdf(const X& z, const S& s) const override
		{
			//auto esx = std::exp(s * x);
			//esx[x > z] = 0;

			//return (esx*p).sum()/_mgf(s);
			
			return 0; //!!! implement your algorithm
		}

		// E[e^{s X}]
		X _mgf(const S& s) const override
		{
			return (std::exp(s * x) * p).sum();
		}

		// log E[e^{s X}]
		X _cgf(const S& s) const override
		{
			return std::log(_mgf(s));
		}

		// Find x for which q = E[e^{sX}/E[e^{sX}] 1(X <= x)]
		// Note the graph contains the points (e^{s x_i}/E[e^{s X}], p_i)
		X inv(const X& q, const S& s)
		{
			ensure(q >= 0);
			auto mgfs = _mgf(s);

			if (q <= p[0]) {
				return std::exp(s * x[0]) / mgfs;
			}
			ensure(q <= 1);
			//!!! Hint: find largest i with q <= p[0] + ... + p[i]

			return 0;
		}

#ifdef _DEBUG
		static int test()
		{
			{
				X x[] = { 0, 1 };
				X p[] = { 0.5, 0.5 };
				discrete D(2, x, p);
				D.normalize();
				ensure(D.x[0] == -1);
				ensure(D.x[1] == 1);
				for (auto s : { X(-0.1), X(0.), X(0.1) }) {
					ensure(D._mgf(s) == std::cosh(s));
				}
			}
			{
				X x[] = { 0, 1 };
				X p[] = { 0.5, 0.5 };
				discrete D(2, x, p);
				ensure(D.inv(0) == 0);
				ensure(D.inv(0.25) == 0);
				ensure(D.inv(0.5) == 1);
				ensure(D.inv(1) == 1);
			}

			return 0;
		}
#endif
	};

} // namespace fms::distribution
