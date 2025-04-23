#ifndef BILLIARD_HPP
#define BILLIARD_HPP

#include <vector>
#include <SFML/Graphics.hpp>

class Billiard {
 private:
  float length_;
  float r1_;
  float r2_;

 public:
  Billiard(float length, float r1, float r2);

  std::vector<sf::Vector2f> upper_segment() const;
  std::vector<sf::Vector2f> lower_segment() const;

  float upper_slope() const;
  float lower_slope() const;
  float upper_surface_intercept() const;
  float lower_surface_intercept() const;
  float upper_right_height() const;
  float lower_right_height() const;

  std::vector<sf::Vector2f> PointsUp(sf::Vector2f windowSize) const {
    return std::vector<sf::Vector2f>{
      sf::Vector2f((windowSize.x / 2 - length_ / 2) - windowSize.x / 2, +r1_ + windowSize.y / 2),
      sf::Vector2f((windowSize.x / 2 + length_ / 2) - windowSize.x / 2, +r2_ + windowSize.y / 2)
  };
  
  }

  std::vector<sf::Vector2f> PointsLow(sf::Vector2f windowSize) const {
      return std::vector<sf::Vector2f>{
      sf::Vector2f((windowSize.x / 2 - length_ / 2) - windowSize.x / 2, -r1_ + windowSize.y / 2),
      sf::Vector2f((windowSize.x / 2 + length_ / 2) - windowSize.x / 2, -r2_ + windowSize.y / 2)
  };
  
  }
 
  float getLength() const {return length_;}
  void print_info() /*const*/;
};

#endif