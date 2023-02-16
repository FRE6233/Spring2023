// xll_distribution.cpp - Probability distributions
#include "../cpp/fms_distribution_normal.h"
#include "xai.h"

using namespace fms;
using namespace xll;

static distribution::normal<> N;

AddIn xai_distribution_pdf(
	Function(XLL_DOUBLE, "xll_distribution_pdf", "DISTRIBUTION.PDF")
	.Arguments({ 
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution."),
		Arg(XLL_DOUBLE, "x", "is the value at which to calculate the pdf."),
		Arg(XLL_DOUBLE, "s", "is the optional share measure parameter. Default is 0.")
		})
	.Uncalced()
	.FunctionHelp("Return the probability density.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_pdf(HANDLEX h, double z, double s)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		distribution::base<>* p = h ? safe_pointer<distribution::base<>>(h) : &N;
		ensure(p);

		result = p->pdf(z, s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_distribution_cdf(
	Function(XLL_DOUBLE, "xll_distribution_cdf", "DISTRIBUTION.CDF")
	.Arguments({
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution."),
		Arg(XLL_DOUBLE, "x", "is the value at which to calculate the cdf."),
		Arg(XLL_DOUBLE, "s", "is the optional share measure parameter. Default is 0.")
		})
	.Uncalced()
	.FunctionHelp("Return the cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_normal(HANDLEX h, double z, double s)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		distribution::base<>* p = h ? safe_pointer<distribution::base<>>(h) : &N;
		ensure(p);

		result = p->cdf(z, s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

distribution::normal<> normal;

AddIn xai_distribution_normal(
	Function(XLL_DOUBLE, "xll_distribution_normal", "DISTRIBUTION.NORMAL")
	.Arguments({ })
	.Uncalced()
	.FunctionHelp("Return a handle to a standard normal cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_normal()
{
#pragma XLLEXPORT
	return safe_handle<distribution::base<>>(&normal);
}
