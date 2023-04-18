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
#include "fms_carr_madan.h"

using namespace fms;

double machine_epsilon()
{
	double x = 1;

	while (1 + x != 1) {
		x /= 2;
	}

	return x;
}

int test_carr_madan()
{
	// strikes
	double lo = 1, hi = 200;
	std::vector<double> ks;
	for (double k = lo; k <= hi; k += 1) {
		ks.push_back(k);
	}
	size_t n = ks.size();

	double f = 100, s = 0.1;
	std::vector<double> p(n), c(n);
	for (size_t i = 0; i < n; ++i) {
		p[i] = black::normal::put::value(f, s, ks[i]);
		c[i] = black::normal::call::value(f, s, ks[i]);
	}

	{
		// variance swap payoff -2 log(k/f);
		std::vector<double> fs(n);
		for (size_t i = 0; i < n; ++i) {
			fs[i] = -2 * log(ks[i] / f);
		}

		std::span<double> f_(fs);
		for (double a : {-20., -10., 0., 10., 20.}) {
			auto s2 = carr_madan::value(f, a, std::span(p), std::span(c), std::span(ks), f_);
			double z = sqrt(s2) - s;
			ensure(fabs(z) < 1e-4);
		}
	}

	return 0;
}
int carr_madan_test = test_carr_madan();

int main()
{
	double x = machine_epsilon();
	ensure(x != 0);
	ensure(1 + x == 1);
	ensure(x >= std::numeric_limits<double>::epsilon() / 2);
	try {
		fms::test_hypergeometric<>();
		distribution::normal<>::test();
		distribution::double_exponential<>::test();
		black::normal::put::test();
		bachelier::put::test();
		bsm::test_Dfs();
		carr_madan::test_index();
		carr_madan::test_tangent();
		carr_madan::test_fit<double>();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;;
	}

	return 0;
}