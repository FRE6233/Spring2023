// xll_carr_madan.cpp - Price arbitrary payoff
#include "../cpp/fms_carr_madan.h"
#include "xai.h"

using namespace fms;
using namespace xll;

// convert _FPX* to std::span
inline constexpr auto span(_FPX* pa)
{
	return std::span(pa->array, size(*pa));
}

AddIn xai_carr_madan_fit(
	Function(XLL_FPX, "xll_carr_madan_fit", "CARR_MADAN.FIT")
	.Arguments({
		Arg(XLL_FPX, "k", "Strike prices."),
		Arg(XLL_FPX, "f", "Payoff corresponding to strikes."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return (f(k_0), f'(k_0), f''(k_1), ..., f''(k_{n-2}))")
);
_FPX* WINAPI xll_carr_madan_fit(_FPX* pk, _FPX* pf)
{
#pragma XLLEXPORT
	auto f = span(pf); // need l-value
	carr_madan::fit(span(pk), f);

	return pf;
}

AddIn xai_carr_madan_unfit(
	Function(XLL_FPX, "xll_carr_madan_unfit", "CARR_MADAN.UNFIT")
	.Arguments({
		Arg(XLL_FPX, "k", "Strike prices."),
		Arg(XLL_FPX, "f", "Payoff corresponding to strikes."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Inverse of CARR_MADAN.FIT.")
);
_FPX* WINAPI xll_carr_madan_unfit(_FPX* pk, _FPX* pf)
{
#pragma XLLEXPORT
	auto f = span(pf); // need l-value
	carr_madan::unfit(span(pk), f);

	return pf;
}

AddIn xai_carr_madan_value(
	Function(XLL_DOUBLE, "xll_carr_madan_value", "CARR_MADAN.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f0", "Forward price of underlying."),
		Arg(XLL_DOUBLE, "k0", "Put/Call seperator."),
		Arg(XLL_FPX, "p", "Put prices."),
		Arg(XLL_FPX, "c", "Call prices."),
		Arg(XLL_FPX, "k", "Strike prices."),
		Arg(XLL_FPX, "f", "Payoff corresponding to strikes."),
		})
	.Category("XLL")
	.FunctionHelp("Price arbitrary option payoff.")
);
double WINAPI xll_carr_madan_value(double f0, double k0, _FPX* pp, _FPX* pc, _FPX* pk, _FPX* pf)
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