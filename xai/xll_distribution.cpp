// xll_distribution.cpp - Probability distributions
#include "../cpp/fms_distribution.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_distribution_pdf(
	Function(XLL_DOUBLE, "xll_distribution_pdf", "DISTRIBUTION.PDF")
	.Arguments({ 
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution."),
		Arg(XLL_DOUBLE, "x", "is the value at which to calculate the pdf"),
		Arg(XLL_DOUBLE, "s", "is the optional share measure parameter. Default is 0."),
		})
	.FunctionHelp("Return the standard probability density function.")
	.Category(CATEGORY)
);

double WINAPI xll_distribution_pdf(HANDLEX h, double x, double s)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		const distribution::standard<>* ph = safe_pointer<distribution::standard<>>(h);
		ensure(ph);

		result = ph->pdf(x, s);
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
		Arg(XLL_DOUBLE, "x", "is the value at which to calculate the cdf"),
		Arg(XLL_DOUBLE, "s", "is the optional share measure parameter. Default is 0."),
		})
		.FunctionHelp("Return the standard cumulative distribtion function.")
	.Category(CATEGORY)
);

double WINAPI xll_distribution_cdf(HANDLEX h, double x, double s)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		const distribution::standard<>* ph = safe_pointer<distribution::standard<>>(h);
		ensure(ph);

		result = ph->cdf(x, s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
AddIn xai_distribution_mgf(
	Function(XLL_DOUBLE, "xll_distribution_mgf", "DISTRIBUTION.MGF")
	.Arguments({
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution."),
		Arg(XLL_DOUBLE, "s", "is distribution parameter."),
		})
		.FunctionHelp("Return the moment generating function.")
	.Category(CATEGORY)
);

double WINAPI xll_distribution_mgf(HANDLEX h, double s)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		const distribution::standard<>* ph = safe_pointer<distribution::standard<>>(h);
		ensure(ph);

		result = ph->mgf(s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
AddIn xai_distribution_cgf(
	Function(XLL_DOUBLE, "xll_distribution_cgf", "DISTRIBUTION.CGF")
	.Arguments({
		Arg(XLL_HANDLEX, "h", "is a handle to a distribution."),
		Arg(XLL_DOUBLE, "s", "is distribution parameter."),
		})
		.FunctionHelp("Return the cumulant generating function.")
	.Category(CATEGORY)
);

double WINAPI xll_distribution_cgf(HANDLEX h, double s)
{
#pragma XLLEXPORT
	double result = NaN;

	try {
		const distribution::standard<>* ph = safe_pointer<distribution::standard<>>(h);
		ensure(ph);

		result = ph->cgf(s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}