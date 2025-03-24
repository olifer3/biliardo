#include "particle.hpp"
#include <cmath>

Particle::Particle(float y0, float theta0, float velocity)
    : position_{0, y0}, angle_{theta0}, velocity_{velocity} {}

sf::Vector2f Particle::move(Billiard const& billiard) {
  float x = position_.x;
  float y = position_.y;

  float dx = velocity_ * std::cos(angle_);
  float dy = velocity_ * std::sin(angle_);

  //while (x <= billiard.upper_segment()[1][0]) {  // Continua finché non arriva a x = ℓ
    x += dx;
    y += dy;

    // Controllo se la particella colpisce il bordo superiore
    float expected_y_upper = billiard.upper_slope() * x + billiard.upper_segment()[0].y;
    if (y >= expected_y_upper) {
      auto normal = billiard.upper_normal();
      float dot_product = dx * normal[0] + dy * normal[1];

      dx -= 2 * dot_product * normal[0];
      dy -= 2 * dot_product * normal[1];

      angle_ = std::atan2(dy, dx);
    }

    // Controllo se la particella colpisce il bordo inferiore
    float expected_y_lower = billiard.lower_slope() * x + billiard.lower_segment()[0].y;
    if (y <= expected_y_lower) {
      auto normal = billiard.lower_normal();
      float dot_product = dx * normal[0] + dy * normal[1];

      dx -= 2 * dot_product * normal[0];
      dy -= 2 * dot_product * normal[1];

      angle_ = std::atan2(dy, dx);

    }
    position_ = {x, y};
    return position_;
}

void Particle::print_state() const {
  std::cout << "Particle position: (" << position_.x << ", " << position_.y << ")\n"
            << "Direction (theta): " << angle_ << " rad\n";
}
