#include <iostream>

int main() {
  std::cout << "Provide the first number\n";
  int i;
  std::cin >> i;
  std::cout << "Provide the second number\n";
  int j;
  std::cin >> j;

  int sum{i + j};
  std::cout << "The sum is: " << sum << '\n';

  std::cout << i
            << (i % j != 0 ? " is NOT a multiple of " : " is a multiple of ")
            << j << '\n';
}