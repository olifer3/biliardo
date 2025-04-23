#include "billiard.hpp"

#include <array>
#include <cmath>
#include <iostream>

Billiard::Billiard(float length, float r1, float r2)
    : length_{length}, r1_{r1}, r2_{r2} {}

std::vector<sf::Vector2f> Billiard::upper_segment() const
{
  // return {{{0, r1_}, {length_, r2_}}};
  return std::vector<sf::Vector2f>{
      sf::Vector2f(0, r1_),
      sf::Vector2f(length_, r2_)};
}

std::vector<sf::Vector2f> Billiard::lower_segment() const
{
  // return {{{0, -r1_}, {length_, -r2_}}};
  return std::vector<sf::Vector2f>{
    sf::Vector2f(0, -r1_),
    sf::Vector2f(length_, -r2_)};
}

float Billiard::upper_slope() const { return ((r2_ - r1_) / length_); }
float Billiard::lower_slope() const { return ((-r2_ + r1_) / length_); }
float Billiard::upper_surface_intercept() const { return (r1_); }
float Billiard::lower_surface_intercept() const { return (-r1_); }
float Billiard::upper_right_height() const { return (r2_); }
float Billiard::lower_right_height() const { return (-r2_); }

void Billiard::print_info() /*const*/
{
  std::cout << "Billiard properties:\n"
            << "Lenght =" << length_ << "\n"
            << " Left height = " << r1_ << "\n"
            << " Right height = " << r2_ << "\n"
            << " Upper wall slope = " << upper_slope() << "\n"
            << " Lower wall slope = " << lower_slope() << "\n";
}