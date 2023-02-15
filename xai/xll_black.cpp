// xll_black.cpp - Fischer Black model
#include "../cpp/fms_distribution_normal.h"
#include "../cpp/fms_black.h"
#include "xai.h"

using namespace fms;
using namespace xll;

static distribution::normal<> N;
inline distribution::base<>* model_pointer(HANDLEX m)
{
	return m ? safe_pointer<fms::distribution::base<>>(m) : &N;
}

AddIn xai_black_value_put(
	Function(XLL_DOUBLE, "xll_black_value_put", "BLACK.VALUE.PUT")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward value."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_HANDLEX, "m", "is a handle to a distribution. Default is normal."),
		})
	.FunctionHelp("Return the Fischer Black value of a put.")
	.Category(CATEGORY)
);
double WINAPI xll_black_value_put(double f, double s, double k, HANDLEX m)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		fms::distribution::base<>* pm = model_pointer(m);
		ensure(pm);

		result = black::value_put(f, s, k, *pm);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

//!!! implement BLACK.VALUE.DELTA and BLACK.VALUE.CALL
