// xll_bachelier.cpp - General Fischer bachelier model
#include "../cpp/fms_bachelier.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_bachelier_put_value(
	Function(XLL_DOUBLE, "xll_bachelier_put_value", "BACHELIER.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
		.FunctionHelp("Return the Bachelier value of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_bachelier_put_value(double f, double s, double k)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		result = bachelier::put::value(f, s, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_bachelier_put_delta(
	Function(XLL_DOUBLE, "xll_bachelier_put_delta", "BACHELIER.PUT.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
		.FunctionHelp("Return the Bachelier delta of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_bachelier_put_delta(double f, double s, double k)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		result = bachelier::put::delta(f, s, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

// Note this uses the normal distribution.
// bachelier implied volatility is a proxy for price.
AddIn xai_bachelier_put_implied(
	Function(XLL_DOUBLE, "xll_bachelier_put_implied", "BACHELIER.PUT.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "p", "is the put price."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
		.FunctionHelp("Return the implied Bachelier volatility of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_bachelier_put_implied(double f, double p, double k)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		result = bachelier::put::implied(f, p, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_bachelier_call_implied(
	Function(XLL_DOUBLE, "xll_bachelier_call_implied", "BACHELIER.CALL.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "p", "is the call price."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
	.FunctionHelp("Return the implied Bachelier volatility of a call.")
	.Category(CATEGORY)
);
double WINAPI xll_bachelier_call_implied(double f, double c, double k)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		result = bachelier::call::implied(f, c, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
