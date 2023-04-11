// fms_fixed_income.h - Fixed Income instrument interface class
#pragma once
#include "ensure.h"
#include <algorithm>
#include <map>
#include <vector>

namespace fms::fixed_income {

	// fixed time and cash flows
	template<class U = double, class C = double>
	struct instrument {
		virtual ~instrument() { }

		size_t size() const
		{
			return _size();
		}
		const U* time() const
		{
			return _time();
		}
		const C* cash() const
		{
			return _cash();
		}

		// strictly monontonic cash flows
		bool ok() const
		{
			size_t n = size();
			const U* b = time();
			const U* e = b + n;

			return n == 0 || b[0] >= 0 && e == std::adjacent_find(b, e, std::greater_equal<U>{});
		}

		U effective() const
		{
			ensure(size() > 0);

			return time()[0];
		}
		U termination() const
		{
			size_t n = size();
			ensure(n > 0);

			return time()[n - 1];
		}
	private:
		virtual size_t _size() const = 0;
		virtual const U* _time() const = 0;
		virtual const C* _cash() const = 0;
	};

	// instrument given non-owning pointers
	template<class U = double, class C = double>
	class instrument_ptr : public instrument<U, C>
	{
		size_t n;
		const U* u;
		const C* c;
	public:
		instrument_ptr(size_t n, const U* u, const C* c)
			: n(n), u(u), c(c)
		{ }
		instrument_ptr(const instrument_ptr&) = default;
		instrument_ptr& operator=(const instrument_ptr&) = default;
		~instrument_ptr()
		{ }

		size_t _size() const override
		{
			return n;
		}
		const U* _time() const override
		{
			return u;
		}
		const C* _cash() const override
		{
			return c;
		}
	};

	// instrument value type
	template<class U = double, class C = double>
	class instrument_value : public instrument<U, C>
	{
		std::vector<U> u;
		std::vector<C> c;
	public:
		instrument_value()
		{ }
		instrument_value(size_t n, const U* u, const C* c)
			: u(u, u + n), c(c, c + n)
		{ }
		instrument_value(const std::initializer_list<U>& u, const std::initializer_list<C>& c)
			: u(u.begin(), u.end()), c(c.begin(), c.end())
		{
			//??? replace with ok. ensure(u.size() == c.size());
		}
		instrument_value(const instrument_value&) = default;
		instrument_value& operator=(const instrument_value&) = default;
		instrument_value(instrument_value&&) = default;
		instrument_value& operator=(instrument_value&&) = default;
		~instrument_value()
		{ }

		bool ok() const
		{
			return true; //!!! for now
		}

		size_t _size() const override
		{
			return u.size();
		}
		const U* _time() const override
		{
			return u.data();
		}
		const C* _cash() const override
		{
			return c.data();
		}

		instrument<U, C>& extend(U u_, C c_)
		{
			ensure(u_ > u.back());

			u.push_back(u_);
			c.push_back(c_);

			return *this;
		}
	};

	template<class U = double, class C = double>
	class forward_rate_agreement : public instrument_value<U, C>
	{
	public:
		forward_rate_agreement(U effective, U termination, C rate)
			: instrument_value<U,C>({ effective, termination }, { -1, exp(rate * (termination - effective)) })
		{ }
	};

	template<class U = double, class C = double>
	class cash_deposit : public forward_rate_agreement<U, C>
	{
	public:
		cash_deposit(U maturity, C rate)
			: forward_rate_agreement<U,C>(0, maturity, rate)
		{ }
	};

	// Collection of weighted instruments with cash flows sorted by time
	// If n == 1 this sorts by increasing time.
	template<class U = double, class C = double>
	inline instrument_value<U, C> portfolio(size_t n, const C* w, const instrument<U, C>* i)
	{
		std::map<U, C> wi;

		for (size_t j = 0; j < n; ++j) {
			const auto ij = i[j];
			const U* iju = ij.time();
			const C* ijc = ij.cash();
			for (size_t k = 0; k < ij.size(); ++k) {
				wi[iju[k]] += w[j] * ijc[k];
			}
		}

		instrument_value<U, C> iv;
		for (const auto& [u, c] : wi) {
			iv.extend(u, c);
		}

		return iv;
	}


} // namespace fms::fixed_income