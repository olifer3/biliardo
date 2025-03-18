#include "billiard.hpp"

#include <array>
#include <cmath>
#include <iostream>

Billiard::Billiard(float length, float r1, float r2)
    : length_{length}, r1_{r1}, r2_{r2} {}

std::vector<sf::Vector2f> Billiard::upper_segment() const {
  return {{{0, r1_}, {length_, r2_}}};
}

std::vector<sf::Vector2f> Billiard::lower_segment() const {
  return {{{0, -r1_}, {length_, -r2_}}};
}

float Billiard::upper_slope() const { return (r2_ - r1_) / length_; }
float Billiard::lower_slope() const { return (-r2_ + r1_) / length_; }

std::array<double, 2> Billiard::upper_normal() const {
  double norm_x = -(upper_slope());
  double norm_y = 1;
  double norm_lenght = std::sqrt(norm_x * norm_x + norm_y * norm_y);
  return { norm_x / norm_lenght, norm_y / norm_lenght };
}

std::array<double, 2> Billiard::lower_normal() const {
  double norm_x = -(lower_slope());
  double norm_y = 1;
  double norm_lenght = std::sqrt(norm_x * norm_x + norm_y * norm_y);
  return { norm_x / norm_lenght, norm_y / norm_lenght };
}
void Billiard::print_info() /*const*/ {
  std::cout << "Billiard properties:\n"
            << "Lenght =" << length_ << "\n"
            << " Left height = " << r1_ << "\n"
            << " Right height = " << r2_ << "\n"
            << " Upper wall slope = " << upper_slope() << "\n"
            << " Lower wall slope = " << lower_slope() << "\n";

            
            }