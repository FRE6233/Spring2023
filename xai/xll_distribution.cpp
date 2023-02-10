// xll_distribution.cpp - Probability distributions
#include "../cpp/fms_distribution_normal.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_distribution_normal(
	Function(XLL_DOUBLE, "xll_distribution_normal", "NORMAL.CDF")
	.Arguments({
		Arg(XLL_DOUBLE, "z", "is the value at which to evaluate the standard normal cdf.")
		})
	.FunctionHelp("Return the standard normal cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_normal(double z)
{
#pragma XLLEXPORT
	return distribution::normal(z);
}