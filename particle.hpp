#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "billiard.hpp"
#include <array>
#include <iostream>

class Particle {
 private:
  std::array<double, 2> position_;
  double angle_;  // Angolo in radianti
  double velocity_;

 public:
  Particle(double y0, double theta0, double velocity);

  void move(Billiard const& billiard);
  std::array<double, 2> position() const { return position_; }
  double angle() const { return angle_; }

  void print_state() const;
};

#endif
