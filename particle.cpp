#include "particle.hpp"

#include <cmath>

Particle::Particle(float y0, float theta0, float velocity)
    : position_{0, y0}, angle_{theta0}, velocity_{velocity} {}

void Particle::move(const Billiard& billiard, float deltaTime) {
  float x = position_.x;
  float y = position_.y;

  float dx = velocity_ * std::cos(angle_) * deltaTime;
  float dy = velocity_ * std::sin(angle_) * deltaTime;

  x += dx;
  y += dy;

  // Controllo se la particella colpisce il bordo superiore
  float expected_y_upper =
      billiard.upper_slope() * x + billiard.upper_segment()[0].y;
  if (y >= expected_y_upper&&angle_>0) {
    float alpha = static_cast<float>(atan(billiard.upper_slope()));
    angle_ = -(angle_- 2.f * alpha);
  }

  // Controllo se la particella colpisce il bordo inferiore
  float expected_y_lower =
      billiard.lower_slope() * x + billiard.lower_segment()[0].y;
  if (y <= expected_y_lower&&angle_<0) {
    float alpha = static_cast<float>(atan(billiard.lower_slope()));
    angle_ = -(angle_- 2.f / alpha);
  }
  position_ = {x, y};
}

void Particle::print_state() const {
  std::cout << "Particle position: (" << position_.x << ", " << position_.y
            << ")\n"
            << "Direction (theta): " << angle_ << " rad\n";
}
