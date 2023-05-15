// xll_distribution_discrete.cpp - Discrete distribution
#include "../cpp/fms_distribution_discrete.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_distribution_discrete(
	Function(XLL_HANDLEX, "xll_distribution_discrete", "DISTRIBUTION.DISCRETE")
	.Arguments({ 
		Arg(XLL_FPX, "x", "is an array of discrete values."),
		Arg(XLL_FPX, "p", "is an array of discrete probabilites."),
		Arg(XLL_BOOL, "_standardize", "is and optional flag to make the distribution have mean 0 and variance 1."),
		})
	.Uncalced()
	.FunctionHelp("Return a handle to a standard discrete cumulative distribution.")
	.Category(CATEGORY)
);
HANDLEX WINAPI xll_distribution_discrete(_FPX* px, _FPX* pp, bool std)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<distribution::standard<>> h_(new distribution::discrete<>(span(px), span(pp), std));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_distribution_discrete_inv(
	Function(XLL_DOUBLE, "xll_distribution_discrete_inv", "DISTRIBUTION.DISCRETE_INV")
	.Arguments({
		Arg(XLL_HANDLEX, "h", "is a handle to a discrete distribution."),
		Arg(XLL_DOUBLE, "q", "is probability."),
		Arg(XLL_DOUBLE, "s", "is share measure parameter."),
		})
	.FunctionHelp("Return the inverse of a standard discrete cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_discrete_inv(HANDLEX h, double p, double s)
{
#pragma XLLEXPORT
	double F = NaN;

	try {
		handle<distribution::standard<>> h_(h);
		ensure(h_);
		
		F = h_.as<distribution::discrete<>>()->inv(p, s);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return F;
}
