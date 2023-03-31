// xll_bootstrap.cpp - Bootstrap a piecewise flat forward curve
#include "xai.h"
#include "../cpp/fms_bootstrap.h"

using namespace xll;
using namespace fms;

AddIn xai_pwflat(
	Function(XLL_HANDLE, "xll_pwflat", "\\XLL.CURVE")
	.Arguments({
		Arg(XLL_FPX, "time", "is an array of times."),
		Arg(XLL_FPX, "forward", "is an array of forwards."),
		Arg(XLL_DOUBLE, "extrapolate", "is an optional extrapolatiom value."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat curve.")
);
HANDLEX WINAPI xll_pwflat(const _FPX* pt, const _FPX* pf, double _f)
{
#pragma XLLEXPORT
	// !ensure (size(*pt) = size(*pf));

	handle<pwflat::curve<>> c(new pwflat::curve<>(size(*pt), pt->array, pf->array, _f));
	// ensure(c);

	return c.get();
}

AddIn xai_pwflat_value(
	Function(XLL_FPX, "xll_pwflat_value", "XLL.CURVE.VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle returned by \\XLL.CURVE."),
		Arg(XLL_FPX, "time", "is an array of times."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat curve.")
);
_FPX* WINAPI xll_pwflat_value(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	handle<pwflat::curve<>> c(curve);
	// ensure(c);
	for (unsigned i = 0; i < size(*pt); ++i) {
		pt->array[i] = c->value(pt->array[i]);
	}

	return pt;
}