// xll_carr_madan.cpp - Price arbitrary payoff
#include "../cpp/fms_carr_madan.h"
#include "xai.h"

using namespace fms;
using namespace xll;

AddIn xai_carr_madan_value(
	Function(XLL_DOUBLE, "xll_carr_madan_value", "CARR_MADAN.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f0", "Forward price of underlying."),
		Arg(XLL_FPX, "p", "Put prices."),
		Arg(XLL_FPX, "c", "Call prices."),
		Arg(XLL_FPX, "k", "Strike prices."),
		Arg(XLL_FPX, "f", "Payoff corresponding to strikes."),
		})
	.Category("XLL")
	.FunctionHelp("Price arbitrary option payoff.")
);
double WINAPI xll_carr_madan_value(double f0, const _FPX* pp, const _FPX* pc, const _FPX* pk, _FPX* pf)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		//!!!Implement
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}