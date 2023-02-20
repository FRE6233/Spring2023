// xll_distribution_double_exponential.cpp - Standard double_exponential distribution.
#include "../cpp/fms_distribution_double_exponential.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_distribution_double_exponential(
	Function(XLL_HANDLEX, "xll_distribution_double_exponential", "DISTRIBUTION.DOUBLE_EXPONENTIAL")
	.Arguments({ })
	.Uncalced()
	.FunctionHelp("Return a handle to a standard double exponential cumulative distribution.")
	.Category(CATEGORY)
);
double WINAPI xll_distribution_double_exponential()
{
#pragma XLLEXPORT
	static distribution::double_exponential<> de;

	return safe_handle<distribution::standard<>>(&de);
}
