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
	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		for (unsigned i = 0; i < size(*pt); ++i) {
			pt->array[i] = c->value(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return pt;
}

AddIn xai_pwflat_spot(
	Function(XLL_FPX, "xll_pwflat_spot", "XLL.CURVE.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle returned by \\XLL.CURVE."),
		Arg(XLL_FPX, "time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return spot rate of flat curve at given times.")
);
_FPX* WINAPI xll_pwflat_spot(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		for (unsigned i = 0; i < size(*pt); ++i) {
			pt->array[i] = c->spot(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return pt;
}

AddIn xai_pwflat_discount(
	Function(XLL_FPX, "xll_pwflat_discount", "XLL.CURVE.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle returned by \\XLL.CURVE."),
		Arg(XLL_FPX, "time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return discount of flat curve at given times.")
);
_FPX* WINAPI xll_pwflat_discount(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		for (unsigned i = 0; i < size(*pt); ++i) {
			pt->array[i] = c->discount(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return pt;
}

AddIn xai_fixed_income_instrument_(
	Function(XLL_HANDLEX, "xll_fixed_income_instrument_", "\\XLL.FIXED_INCOME.INSTRUMENT")
	.Arguments({
		Arg(XLL_FPX, "time", "is an array of times."),
		Arg(XLL_FPX, "cash", "is an array of cash flows."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat curve.")
);
HANDLEX WINAPI xll_fixed_income_instrument_(const _FPX* pu, const _FPX* pc)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		ensure(size(*pu) == size(*pc));

		handle<fixed_income::instrument<>> i(new fixed_income::instrument_value(size(*pu), pu->array, pc->array));
		ensure(i);

		h = i.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_bootstrap_extend(
	Function(XLL_HANDLE, "xll_bootstrap_extend", "BOOTSTRAP.EXTEND")
	.Arguments({
		Arg(XLL_HANDLE, "inst", "instrument"),
		Arg(XLL_HANDLE, "curve", "curve"),
		})
		.Category(CATEGORY)
	.FunctionHelp("Extend curve by instrument.")
);
HANDLEX WINAPI xll_bootstrap_extend(HANDLEX inst, HANDLEX curve)
{
#pragma XLLEXPORT
	handle<fixed_income::instrument<>> i(inst);
	handle<pwflat::curve<>> c(curve);

	bootstrap::extend(*i, *c);

	return curve;
}