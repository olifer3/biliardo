#include <iostream>

#include "billiard.hpp"

int main() {
  double length, r1, r2;
  std::cout << "Enter billiard length: ";
  std::cin >> length;

  std::cout << "Enter left height (r1): ";
  std::cin >> r1;

  std::cout << "Enter right height (r2): ";
  std::cin >> r2;

  // Create a Billiard object with user-defined parameters
  Billiard billiard(length, r1, r2);

  // Print billiard properties
  billiard.print_info();
}
