#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cmath>

#include "doctest.h"

double taylor_opt(double x, int N) {
  if (N < 0) {
    return -1.0;
  }

  double sum{1};
  int fact{1};
  int sign{1};

  for (int n{1}; n <= N; ++n) {
    const int nt2{n * 2};
    fact *= nt2 * (nt2 - 1);
    sign = -sign;
    sum += sign * std::pow(x, nt2) / fact;
  }

  return sum;
}

TEST_CASE("Testing the optimized taylor expansion function") {
  CHECK(taylor_opt(0.5, 0) == doctest::Approx(1.0));
  CHECK(taylor_opt(0.5, 1) == doctest::Approx(0.875));
  CHECK(taylor_opt(0.5, 2) == doctest::Approx(0.877).epsilon(0.001));
  CHECK(taylor_opt(0.5, 3) == doctest::Approx(0.877).epsilon(0.001));
  CHECK(taylor_opt(0.5, -9) == doctest::Approx(-1.0));
}
