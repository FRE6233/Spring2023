// fms_distribution.h - NVI standard distribution interface
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-Virtual_Interface
#pragma once

namespace fms::distribution {

	// Random variable with mean 0, variance 1.
	template<class X = double, class S = X>
	struct standard {

		virtual ~standard()
		{ }

		// Probability share density function f_s(x) = e^{sX - κ(s)} f(x)
		X pdf(const X& x, const S& s = 0) const
		{
			return _pdf(x, s);
		}

		// Cumulative share distrbution function P_s(X <= x) = E[e^{sX - κ(s)}1(X <= x)]
		X cdf(const X& x, const S& s = 0) const
		{
			return _cdf(x, s);
		}

		// Moment generating function.
		// μ(s) = E[e^{sX}]
		X mgf(const S& s) const
		{
			return _mgf(s);
		}
		
		// Cumulant generating function.
		// κ(s) = log E[e^{sX}]
		X cgf(const S& s) const
		{
			return _cgf(s);
		}

	private:
		virtual X _pdf(const X&, const S&) const = 0; 
		virtual X _cdf(const X&, const S&) const = 0;
		virtual X _mgf(const S&) const = 0;
		virtual X _cgf(const S&) const = 0;
	};

} // namespace fms::distribution

