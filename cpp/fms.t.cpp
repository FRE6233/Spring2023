// test.cpp - test C++ code
#include <cassert>
#include "fms_distribution_normal.h"
#include "fms_hypergeometric.h"

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

	int test_2F1 = fms::test_2F1<>();
	int test_normal = distribution::normal<>::test();

	return 0;
}