// xai.h - Common header file for Excel add-ins.
#pragma once
#include <span>
#include "../xll/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY "FRE6233"
#endif

namespace xll {
	constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

	inline auto span(_FPX* pa)
	{
		return std::span(pa->array, size(*pa));
	}
}