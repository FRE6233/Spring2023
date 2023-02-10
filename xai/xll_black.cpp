// xll_black.cpp - Fischer Black model
#include "../cpp/fms_black.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_black_value_put(
	Function(XLL_DOUBLE, "xll_black_value_put", "BLACK.VALUE.PUT")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
	.FunctionHelp("Return the Fischer Black value of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_black_value_put(double f, double s, double k)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		result = black::value_put(f, s, k);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}