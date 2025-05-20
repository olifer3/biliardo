#include "particle.hpp"

#include <iostream>
#include <cmath>

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
      std::cout<<"New angle = "<<((angle_*180.0f)/static_cast<float>(M_PI))<<"° ;  (x,y) of impact = ( "<<x<<" , "<<y<<" )\n";
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
    std::cout<<"New angle = "<<((angle_*180.0f)/static_cast<float>(M_PI))<<"° ;  (x,y) of impact = ( "<<x<<" , "<<y<<" )\n";
}

  // Update the position of the particle
  position_ = {x, y};
}


void Particle::print_state(const Billiard& billiard) const {
  float x;
  float y;
  if(position_.x<0){
x=0;
y=tan(angle_)*(x-last_x)+last_y;
  }
  else if (position_.x>=billiard.getLength()){
    x=billiard.getLength();
    y=tan(angle_)*(x-last_x)+last_y;
  }
  else {
    x=position_.x;
    y=position_.y;
  }
  std::cout << "Particle position: (" << x << ", " << y
            << ")\n"
            << "Direction (theta): " << ((angle_*180.0f)/static_cast<float>(M_PI)) << "°\n";
}

