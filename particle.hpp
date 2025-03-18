#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "billiard.hpp"
#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>

class Particle {
 private:
  std::array<double, 2> position_;
  double angle_;  // Angolo in radianti
  double velocity_;

 public:
  Particle(double y0, double theta0, double velocity);

  void move(Billiard const& billiard, float deltaTime);
  sf::Vector2f getPosition() const { return sf::Vector2f(position_[0], position_[1]); }
  double angle() const { return angle_; }


  void print_state() const;
};

#endif
