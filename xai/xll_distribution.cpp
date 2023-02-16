// xll_distribution.cpp - Probability distributions
#include "../cpp/fms_distribution_normal.h"
#include "xai.h"

using namespace fms;
using namespace xll;

distribution::normal<> normal;

AddIn xai_distribution_normal(
	Function(XLL_HANDLEX, "xll_distribution_normal", "DISTRIBUTION.NORMAL")
	.Arguments({ })
	.Uncalced()
	.FunctionHelp("Return a handle to a standard normal cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_normal(double z, double s)
{
#pragma XLLEXPORT
	return safe_handle<distribution::base<>>(&normal);
}