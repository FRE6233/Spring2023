// xll_bootstrap.cpp - Bootstrap a piecewise flat forward curve
#ifndef CATEGORY
#define CATEGORY "FI"
#define _CATEGORY "\\" CATEGORY
#endif
#include "xai.h"
#include "../cpp/fms_bootstrap.h"
#include "../cpp/fms_fixed_income.h"

using namespace xll;
using namespace fms;

#ifdef _DEBUG
int test_extend = bootstrap::extend_test();
#endif // _DEBUG

AddIn xai_pwflat_(
	Function(XLL_HANDLE, "xll_pwflat_", _CATEGORY ".CURVE")
	.Arguments({
		Arg(XLL_FPX, "Time", "is an array of positive increasing times."),
		Arg(XLL_FPX, "Rate", "is an array of forward rates."),
		Arg(XLL_DOUBLE, "extrapolate", "is an optional extrapolation value. Default is 0."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat forward curve.")
);
HANDLEX WINAPI xll_pwflat_(const _FPX* pt, const _FPX* pf, double _f)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		ensure (size(*pt) == size(*pf));

		if (size(*pt) == 1 and pt->array[0] == 0) {
			handle<pwflat::curve<>> c(new pwflat::curve<>());
			ensure(c);
			
			h = c.get();
		}
		else {
			handle<pwflat::curve<>> c(new pwflat::curve<>(size(*pt), pt->array, pf->array, _f));
			ensure(c);

			h = c.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_pwflat(
	Function(XLL_FPX, "xll_pwflat", CATEGORY ".CURVE")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " CATEGORY ".CURVE"),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return times and rates of curve as a two row array.")
);
_FPX* WINAPI xll_pwflat(HANDLEX curve)
{
#pragma XLLEXPORT
	static FPX result; 

	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		result.resize(2, (int)c->size());
		for (int i = 0; i < result.columns(); ++i) {
			result(0, i) = c->time()[i];
			result(1, i) = c->rate()[i];
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return result.get();
}

AddIn xai_pwflat_value(
	Function(XLL_FPX, "xll_pwflat_value", CATEGORY ".CURVE.VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE"),
		Arg(XLL_FPX, "Time", "is an array of times."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return values of piece-wise flat forward curve at given times.")
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
	Function(XLL_FPX, "xll_pwflat_spot", CATEGORY ".CURVE.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE."),
		Arg(XLL_FPX, "Time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return spot rates of piece-wise flat forward curve at given times.")
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
	Function(XLL_FPX, "xll_pwflat_discount", CATEGORY ".CURVE.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE."),
		Arg(XLL_FPX, "Time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return discount of a piecewise flat forward curve at given times.")
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
	Function(XLL_HANDLEX, "xll_fixed_income_instrument_", _CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_FPX, "Time", "is an array of times."),
		Arg(XLL_FPX, "Cash", "is an array of cash flows."),
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

AddIn xai_fixed_income_instrument(
	Function(XLL_FPX, "xll_fixed_income_instrument", CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, "Instrument", "is handle returned by " _CATEGORY ".INSTRUMENT."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a two row array of times and cash flows.")
);
_FPX* WINAPI xll_fixed_income_instrument(HANDLEX inst)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		handle<fixed_income::instrument<>> i(inst);
		ensure(i);
		result.resize(2, (int)i->size());
		for (int j = 0; j < result.columns(); ++j) {
			result(0, j) = i->time()[j];
			result(1, j) = i->cash()[j];
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return result.get();
}

AddIn xai_cash_deposit_(
	Function(XLL_HANDLEX, "xll_cash_deposit_", _CATEGORY ".CD")
	.Arguments({
		Arg(XLL_DOUBLE, "Maturity", "is the time in years to maturity."),
		Arg(XLL_DOUBLE, "Rate", "is the continuously compounded rate."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a cash deposit instrument.")
);
HANDLEX WINAPI xll_cash_deposit_(double mat, double rate)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		handle<fixed_income::instrument<>> i(new fixed_income::cash_deposit(mat, rate));
		ensure(i);

		h = i.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_forward_rate_agreement_(
	Function(XLL_HANDLEX, "xll_forward_rate_agreement_", _CATEGORY ".FRA")
	.Arguments({
		Arg(XLL_DOUBLE, "Effective", "is the time in years to effective date."),
		Arg(XLL_DOUBLE, "Termination", "is the time in years to termination date."),
		Arg(XLL_DOUBLE, "Rate", "is the continuously compounded rate."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a cash deposit instrument.")
);
HANDLEX WINAPI xll_forward_rate_agreement_(double eff, double term, double rate)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		handle<fixed_income::instrument<>> i(new fixed_income::forward_rate_agreement(eff, term, rate));
		ensure(i);

		h = i.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_bootstrap_extend(
	Function(XLL_HANDLE, "xll_bootstrap_extend", CATEGORY ".BOOTSTRAP.EXTEND")
	.Arguments({
		Arg(XLL_HANDLE, "Instrument", "is a handle returned by " _CATEGORY ".INSTRUMENT"),
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Extend curve by instrument.")
);
HANDLEX WINAPI xll_bootstrap_extend(HANDLEX inst, HANDLEX curve)
{
#pragma XLLEXPORT
	try {
		handle<fixed_income::instrument<>> i(inst);
		handle<pwflat::curve<>> c(curve);

		bootstrap::extend(*i, *c);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		curve = INVALID_HANDLEX;
	}

	return curve;
}

AddIn xai_bootstrap_(
	Function(XLL_HANDLE, "xll_bootstrap_", _CATEGORY ".BOOTSTRAP")
	.Arguments({
		Arg(XLL_FPX, "Instruments", "is an array of instrument handles."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a curve bootstrapped from instruments.")
);
HANDLEX WINAPI xll_bootstrap_(const _FPX* pi)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		handle<pwflat::curve<>> c(new pwflat::curve<>{});

		for (unsigned j = 0; j < size(*pi); ++j) {
			handle<fixed_income::instrument<>> i(pi->array[j]);
			ensure(i);
			bootstrap::extend(*i, *c);
		}

		result = c.get();

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = INVALID_HANDLEX;
	}

	return result;
}