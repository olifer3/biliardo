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

sf::VertexArray Billiard::createThickLine(sf::Vector2f p0, sf::Vector2f p1, float thickness, sf::Color color) const {
  sf::Vector2f direction = p1 - p0;
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length == 0) length = 1;

  sf::Vector2f normal(-direction.y / length, direction.x / length);
  sf::Vector2f offset = normal * (thickness / 2.f);

  sf::Vector2f v0 = p0 + offset;
  sf::Vector2f v1 = p1 + offset;
  sf::Vector2f v2 = p1 - offset;
  sf::Vector2f v3 = p0 - offset;

  sf::VertexArray quad(sf::Triangles, 6);
  quad[0].position = v0;
  quad[1].position = v1;
  quad[2].position = v2;
  quad[3].position = v0;
  quad[4].position = v2;
  quad[5].position = v3;

  for (std::size_t i = 0; i < 6; ++i)
      quad[i].color = color;

  return quad;
}



void Billiard::print_info() /*const*/
{
  std::cout << "Billiard properties:\n"
            << "Lenght =" << length_ << "\n"
            << " Left height = " << r1_ << "\n"
            << " Right height = " << r2_ << "\n"
            << " Upper wall slope = " << upper_slope() << "\n"
            << " Lower wall slope = " << lower_slope() << "\n";
}