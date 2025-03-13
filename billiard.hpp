#ifndef BILLIARD_HPP
#define BILLIARD_HPP

#include <array>

class Billiard {
 private:
  double length_;
  double r1_;
  double r2_;

 public:
  Billiard(double length, double r1, double r2);

  std::array<std::array<double, 2>, 2> upper_segment() const;
  std::array<std::array<double, 2>, 2> lower_segment() const;

  double upper_slope() const;
  double lower_slope() const;

  std::array<double, 2> upper_normal() const;
  std::array<double, 2> lower_normal() const;

  void print_info() /*const*/;
};

#endif