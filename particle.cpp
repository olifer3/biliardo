#include "particle.hpp"
#include <iostream>
#include <cmath>

/*Particle::Particle(float y0, float theta0, float velocity)
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
    angle_ = +(abs(angle_) - 2.f * alpha);
  }
  position_ = {x, y};
}*/

/*Particle::Particle(float y0, float theta0, float velocity)
    : position_{0, y0}, angle_{theta0}, velocity_{velocity}, last_x(0), last_y(y0), n(0), s(0) {}

void Particle::move(const Billiard& billiard, float deltaTime) {
  float x = position_.x;
  float y = position_.y;

  float dx = velocity_ * std::cos(angle_) * deltaTime;
  float dy = velocity_ * std::sin(angle_) * deltaTime;

  x += dx;
  y += dy;

  float expected_y_upper = billiard.upper_slope() * x + billiard.upper_segment()[0].y;
  float expected_y_lower = billiard.lower_slope() * x + billiard.lower_segment()[0].y;
  // If s has not been initialized yet, initialize it based on the first bounce
  if (s == 0) {
      // Check if the particle hits the upper wall
      
      if (y >= expected_y_upper) { // Upper wall hit
          s = 1;
          n = s; // Set n to s after the first bounce
      }

      // Check if the particle hits the lower wall
      
      if (y <= expected_y_lower) { // Lower wall hit
          s = 2;
          n = s; // Set n to s after the first bounce
      }
      std::cout << "First bounce set: s = " << s << '\n';
  }

  // If the particle hits a wall, calculate the new angle
  
  
  if (y >= expected_y_upper || y <= expected_y_lower) {
      // Calculate the slope angle of the upper wall
      float alpha = static_cast<float>(atan(billiard.upper_slope()));

      x = (std::pow(-1, n+1) * billiard.left_height() - last_y + tan(angle_) * last_x) / 
            (tan(angle_) + std::pow(-1, n) * billiard.upper_slope());
        y = std::pow(-1, n+1) * billiard.upper_slope() * x + std::pow(-1, n+1) * billiard.left_height();
        last_x = x;
        last_y = y;
      
      angle_ = -(angle_ + (std::pow((-1), n) * 2 * alpha));
      
      // Increment n for the next collision
      n++;
      std::cout << n << '\n';
  }

  // Update the position of the particle
  position_ = {x, y};
}*/

Particle::Particle(float y0, float theta0, float velocity)
    : position_{0, y0}, angle_{theta0}, velocity_{velocity}, last_x(0), last_y(y0) {}

void Particle::move(const Billiard& billiard, float deltaTime) {
  float x = position_.x;
  float y = position_.y;

  float dx = velocity_ * std::cos(angle_) * deltaTime;
  float dy = velocity_ * std::sin(angle_) * deltaTime;

  x += dx;
  y += dy;

  float expected_y_upper = billiard.upper_slope() * x + billiard.upper_surface_intercept();
  float expected_y_lower = billiard.lower_slope() * x + billiard.lower_surface_intercept();
  
  //position_.y < expected_y_upper && 
  if (y >= expected_y_upper) {
      // Calculate the slope angle of the upper wall
      float alpha = static_cast<float>(atan(billiard.upper_slope()));

      x = (billiard.upper_surface_intercept() - last_y + tan(angle_) * last_x) / 
            (tan(angle_) - billiard.upper_slope());
        y = billiard.upper_slope() * x + billiard.upper_surface_intercept();
        last_x = x;
        last_y = y;
      
      angle_ = -(angle_ -  2 * alpha);
      std::cout<<"New angle = "<<angle_<<" ;  (x,y) of impact = ( "<<x<<" , "<<y<<" )\n";
  }
  //position_.y > expected_y_lower && 
  if (y <= expected_y_lower) {
    // Calculate the slope angle of the lower wall
    float alpha = static_cast<float>(atan(billiard.lower_slope()));

    x = (billiard.lower_surface_intercept() - last_y + tan(angle_) * last_x) / 
          (tan(angle_) - billiard.lower_slope());
      y = billiard.lower_slope() * x + billiard.lower_surface_intercept();
      last_x = x;
      last_y = y;
    
    angle_ = -(angle_ -  2 * alpha);
    std::cout<<"New angle = "<<angle_<<" ;  (x,y) of impact = ( "<<x<<" , "<<y<<" )\n";
}

  // Update the position of the particle
  position_ = {x, y};
}


void Particle::print_state() const {
  std::cout << "Particle position: (" << position_.x << ", " << position_.y
            << ")\n"
            << "Direction (theta): " << angle_ << " rad\n";
}
