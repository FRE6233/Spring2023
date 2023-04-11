// test.cpp - test C++ code
#include <cassert>
#include <iostream>
#include "fms_distribution_normal.h"
#include "fms_distribution_double_exponential.h"
#include "fms_hypergeometric.h"
#include "fms_bachelier.h"
#include "fms_black_normal.h"
#include "fms_black.h"
#include "fms_bsm.h"
#include "fms_pwflat.h"
#include "fms_fixed_income.h"
#include "fms_bootstrap.h"

using namespace fms;

double machine_epsilon()
{
	double x = 1;

	while (1 + x != 1) {
		x /= 2;
	}

	return x;
}

int main()
{
	double x = machine_epsilon();
	assert(x != 0);
	assert(1 + x == 1);
	assert(x >= std::numeric_limits<double>::epsilon() / 2);
	try {
		fms::test_hypergeometric<>();
		distribution::normal<>::test();
		distribution::double_exponential<>::test();
		black::normal::put::test();
		bachelier::put::test();
		bsm::test_Dfs();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;;
	}

	return 0;
}