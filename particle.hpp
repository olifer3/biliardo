#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "billiard.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

class Particle {
 private:
  sf::Vector2f position_;
  float angle_;  // Angolo in radianti
  float velocity_;

 public:
  Particle(float y0, float theta0, float velocity);

  sf::Vector2f move(Billiard const& billiard);
  sf::Vector2f getPosition() const { return sf::Vector2f(position_.x, position_.y); }
  float angle() const { return angle_; }



  void print_state() const;
};

#endif
