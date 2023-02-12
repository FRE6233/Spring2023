// test.cpp - test C++ code
#include "fms_distribution.h"
#include "fms_hypergeometric.h"

using namespace fms;

double machine_epsilon()
{
	double x = 1;
	for (x = 1; x != 0; x /= 2) {
		if (1 + x == 1) {
			break;
		}
	}

	return x;
}

int main()
{
	double x = machine_epsilon();
	assert(x != 0);
	assert(x < std::numeric_limits<double>::epsilon());

	int test_2F1 = fms::test_2F1<>();
	int test_normal = distribution::test_normal();

	return 0;
}