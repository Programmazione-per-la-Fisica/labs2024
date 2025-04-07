#include <cstdlib>
#include <exception>
#include <iostream>

#include "regression.hpp"

int main() {
  try {
    pf::Regression data{};

    // ...

  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}
