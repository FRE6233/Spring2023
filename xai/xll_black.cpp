// xll_black.cpp - Fischer Black model
#include "xai.h"

using namespace xll;

AddIn xai_black_value(
	Function(XLL_DOUBLE, "xll_black_value", "BLACK.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
	.FunctionHelp("Return the Fischer Black value of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_black_value(double f, double s, double k)
{
#pragma XLLEXPORT
	return f * s * k;
}