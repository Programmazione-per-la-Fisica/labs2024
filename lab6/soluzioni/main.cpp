#include "regression.hpp"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <random>

int main() {
  try {
    pf::Regression reg;
    std::random_device r;
    std::default_random_engine eng{r()};
    std::uniform_real_distribution<double> dist_x{0., 10.};
    std::normal_distribution<double> dist_y{1., 0.1};

    const auto N = 1'000'000;
    for (auto i = 0; i != N; ++i) {
      reg.add({dist_x(eng), dist_y(eng)});
    }

    const auto result{reg.fit_algo()};
    std::cout << "- A: " << result.A << '\n';
    std::cout << "- B: " << result.B << '\n';

    const double med_x = reg.median_x_nth();
    const double med_y = reg.median_y_nth();
    std::cout << "- Median x: " << med_x << '\n';
    std::cout << "- Median y: " << med_y << '\n';

    pf::Regression reg_file;
    std::ifstream infile{"../samples/data.txt"};

    if (!infile) {
      throw std::runtime_error{"Impossible to open file!"};
    }
    double x, y;
    while (infile >> x >> y) {
      reg_file.add({x, y});
    }

    const auto result_file{reg_file.fit_algo()};

    std::ofstream outfile{"results.txt"};

    if (!outfile) {
      throw std::runtime_error{"Impossible to open file!"};
    }

    outfile << "- Read data: " << reg_file.size() << '\n';
    outfile << "- A: " << result_file.A << '\n';
    outfile << "- B: " << result_file.B << '\n';

  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}