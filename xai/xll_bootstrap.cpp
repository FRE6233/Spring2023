// xll_bootstrap.cpp - Bootstrap a piecewise flat forward curve
#include "xai.h"
#include "../cpp/fms_bootstrap.h"
#include "../cpp/fms_fixed_income.h"

using namespace xll;
using namespace fms;

#ifdef _DEBUG
int test_extend = bootstrap::extend_test();
#endif // _DEBUG

AddIn xai_pwflat(
	Function(XLL_HANDLE, "xll_pwflat", "\\XLL.CURVE")
	.Arguments({
		Arg(XLL_FPX, "time", "is an array of times."),
		Arg(XLL_FPX, "rate", "is an array of forward rates."),
		Arg(XLL_DOUBLE, "extrapolate", "is an optional extrapolatiom value. Default is 0."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat curve.")
);
HANDLEX WINAPI xll_pwflat(const _FPX* pt, const _FPX* pf, double _f)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		ensure (size(*pt) == size(*pf));

		handle<pwflat::curve<>> c(new pwflat::curve<>(size(*pt), pt->array, pf->array, _f));
		ensure(c);

		h = c.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_pwflat_value(
	Function(XLL_FPX, "xll_pwflat_value", "XLL.CURVE.VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle returned by \\XLL.CURVE."),
		Arg(XLL_FPX, "time", "is an array of times."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return values of flat curve at given times.")
);
_FPX* WINAPI xll_pwflat_value(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	//!!! Add try-catch for exceptions.
	handle<pwflat::curve<>> c(curve);
	// ensure(c);
	for (unsigned i = 0; i < size(*pt); ++i) {
		pt->array[i] = c->value(pt->array[i]);
	}

	return pt;
}

//!!! Implement XLL.CURVE.DISCOUNT and XLL.CURVE.SPOT
//!!! Add discount and spot to bootstrap.xlsx

//!!! Implement \XLL.FIXED_INCOME.INSTRUMENT
AddIn xai_fixed_income_instrument_(
	Function(XLL_HANDLEX, "xll_fixed_income_instrument_", "\\XLL.FIXED_INCOME.INSTRUMENT")
	.Arguments({
		//!!! Arg time
		//!!! Arg cash
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handles to times and cash flows of a fixed income instrument.")
);
HANDLEX WINAPI xll_fixed_income_instrument_(const _FPX* pu, const _FPX* pc)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	//!!!hint: handle<fixed_income::instrument<>> i(new fixed_income::instrument_value(size(*pu), pu->array, pc->array);

	return h;
}