// xll_holee.cpp - Ho-Lee model
#include "../cpp/fms_holee.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_holee_put_value(
	Function(XLL_DOUBLE, "xll_holee_put_value", "HOLEE.FLOORLET")
	.Arguments({
		Arg(XLL_DOUBLE, "r", "is the interest rate."),
		Arg(XLL_DOUBLE, "sigma", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_DOUBLE, "t", "is the time in years to expiration."),
		})
		.FunctionHelp("Return the holee value of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_holee_put_value(double r, double sigma, double k, double t)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		holee<> m(r, sigma);
		result = floorlet(m, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}