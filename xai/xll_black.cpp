// xll_black.cpp - General Fischer Black model
#include "../cpp/fms_distribution_normal.h"
#include "../cpp/fms_black_normal.h"
#include "../cpp/fms_black.h"
#include "xai.h"

using namespace fms;
using namespace xll;

inline const distribution::standard<>* distribution_pointer(HANDLEX h)
{
	static distribution::normal<> N;

	return h == 0 ? &N : safe_pointer<distribution::standard<>>(h);
}

AddIn xai_black_put_value(
	Function(XLL_DOUBLE, "xll_black_put_value", "BLACK.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution. Default is normal."),
		})
	.FunctionHelp("Return the Fischer Black value of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_black_put_value(double f, double s, double k, HANDLEX h)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		const fms::distribution::standard<>* p = distribution_pointer(h);
		ensure(p);

		result = black::put::value(f, s, k, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_black_put_delta(
	Function(XLL_DOUBLE, "xll_black_put_delta", "BLACK.PUT.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution. Default is normal."),
		})
	.FunctionHelp("Return the Fischer Black delta of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_black_put_delta(double f, double s, double k, HANDLEX h)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		const fms::distribution::standard<>* p = distribution_pointer(h);
		ensure(p);

		result = black::put::delta(f, s, k, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

// Note this uses the normal distribution.
// Black implied volatility is a proxy for price.
AddIn xai_black_put_implied(
	Function(XLL_DOUBLE, "xll_black_put_implied", "BLACK.PUT.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "p", "is the put price."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
	.FunctionHelp("Return the implied Black volatility of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_black_put_implied(double f, double p, double k)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		result = black::normal::put::implied(f, p, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_black_call_implied(
	Function(XLL_DOUBLE, "xll_black_call_implied", "BLACK.CALL.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "p", "is the call price."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
		.FunctionHelp("Return the implied volatility of a call.")
	.Category(CATEGORY)
);
double WINAPI xll_black_call_implied(double f, double c, double k)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		result = black::normal::call::implied(f, c, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
