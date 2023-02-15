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
HANDLEX WINAPI xll_distribution_normal(double z, double s)
{
#pragma XLLEXPORT
	return safe_handle<distribution::base<>>(&normal);
}

AddIn xai_distribution_cdf(
	Function(XLL_DOUBLE, "xll_distribution_cdf", "DISTRIBUTION.CDF")
	.Arguments({ 
		Arg(XLL_HANDLEX, "m", "is a handle to a distribution."),
		Arg(XLL_DOUBLE, "z", "is the value at which to calculate the cdf."),
		Arg(XLL_DOUBLE, "s", "is the share measure parameter. Default is 0."),
		})
	.FunctionHelp("Return the standard normal cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_cdf(HANDLEX m, double z, double s)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		distribution::base<>* pm = safe_pointer<distribution::base<>>(m);
		ensure(pm);

		result = pm->cdf(z, s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}