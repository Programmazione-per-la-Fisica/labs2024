#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>

#include "doctest.h"

int count_multiples(std::vector<int> values, int M) {
  if (M == 0) {
    return -1;
  }

  int count{0};
  for (int i{0}, n = values.size(); i != n; ++i) {
    if (values[i] % M == 0) {
      ++count;
    }
  }
  return count;
}

TEST_CASE("Testing the count_multiples function") {
  std::vector<int> values_empty;                // empty vector
  std::vector<int> values_even = {2, 4, 6, 8};  // only even values
  std::vector<int> values_zeros = {0, 0, 0};    // vector of zeros

  CHECK(count_multiples(values_empty, 2) == 0);
  CHECK(count_multiples(values_empty, -3) == 0);
  CHECK(count_multiples(values_empty, 0) == -1);

  CHECK(count_multiples(values_even, 2) == 4);
  CHECK(count_multiples(values_even, 3) == 1);
  CHECK(count_multiples(values_even, -2) == 4);
  CHECK(count_multiples(values_even, -3) == 1);
  CHECK(count_multiples(values_even, -5) == 0);
  CHECK(count_multiples(values_even, 0) == -1);

  CHECK(count_multiples({-12, -24, -36, -4}, 2) ==
        4);  // can also build std::vector directly in the argurment
  CHECK(count_multiples({-12, -24, -36, -4}, 3) == 3);
  CHECK(count_multiples({-12, -24, -36, -4}, -2) == 4);
  CHECK(count_multiples({-12, -24, -36, -4}, -3) == 3);
  CHECK(count_multiples({-12, -24, -36, -4}, 5) == 0);
  CHECK(count_multiples({-12, -24, -36, -4}, -7) == 0);

  CHECK(count_multiples({3, 6, 27}, 2) == 1);
  CHECK(count_multiples({3, 6, 27}, -2) == 1);
  CHECK(count_multiples({3, 6, 27}, 3) == 3);
  CHECK(count_multiples({3, 6, 27}, -3) == 3);
  CHECK(count_multiples({3, 6, 27}, 9) == 1);

  CHECK(count_multiples(values_zeros, 1) == 3);
  CHECK(count_multiples(values_zeros, 3) == 3);
  CHECK(count_multiples(values_zeros, -11) == 3);
  CHECK(count_multiples(values_zeros, 0) == -1);
}
