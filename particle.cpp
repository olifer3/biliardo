#include "particle.hpp"
#include <cmath>

Particle::Particle(double y0, double theta0, double velocity)
    : position_{0, y0}, angle_{theta0}, velocity_{velocity} {}

void Particle::move(Billiard const& billiard, float deltaTime) {
  double x = position_[0];
  double y = position_[1];

  double dx = velocity_ * std::cos(angle_) * deltaTime;
  double dy = velocity_ * std::sin(angle_)* deltaTime;

  //while (x <= billiard.upper_segment()[1][0]) {  // Continua finché non arriva a x = ℓ
    x += dx;
    y += dy;

    // Controllo se la particella colpisce il bordo superiore
    double expected_y_upper = billiard.upper_slope() * x + billiard.upper_segment()[0].y;
    if (y >= expected_y_upper) {
      auto normal = billiard.upper_normal();
      double dot_product = dx * normal[0] + dy * normal[1];

      dx -= 2 * dot_product * normal[0];
      dy -= 2 * dot_product * normal[1];

      angle_ = std::atan2(dy, dx);
    }

    // Controllo se la particella colpisce il bordo inferiore
    double expected_y_lower = billiard.lower_slope() * x + billiard.lower_segment()[0].y;
    if (y <= expected_y_lower) {
      auto normal = billiard.lower_normal();
      double dot_product = dx * normal[0] + dy * normal[1];

      dx -= 2 * dot_product * normal[0];
      dy -= 2 * dot_product * normal[1];

      angle_ = std::atan2(dy, dx);
    }
  //}

  position_ = {x, y};
}

void Particle::print_state() const {
  std::cout << "Particle position: (" << position_[0] << ", " << position_[1] << ")\n"
            << "Direction (theta): " << angle_ << " rad\n";
}
