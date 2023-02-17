// test.cpp - test C++ code
#include <cassert>
#include <iostream>
#include "fms_distribution_normal.h"
#include "fms_hypergeometric.h"
#include "fms_black_normal.h"

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
	try {
		double x = machine_epsilon();
		ensure(x != 0);
		ensure(1 + x == 1);
		ensure(x >= std::numeric_limits<double>::epsilon() / 2);

		int test_2F1 = fms::test_2F1<>();
		int test_normal = distribution::normal<>::test();
		int test_black_normal = black::put::test();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;;
	}

	return 0;
}