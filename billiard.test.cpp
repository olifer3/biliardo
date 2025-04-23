#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "billiard.hpp"
#include "particle.hpp"
#include "statistics.hpp"
#include <cmath>

TEST_CASE("Billiard initialization and geometry") {
  Billiard b{100.f, 50.f, 30.f};
  CHECK(b.getLength() == doctest::Approx(100.f));
  CHECK(b.upper_slope() == doctest::Approx((30.f - 50.f) / 100.f));
  CHECK(b.lower_slope() == doctest::Approx((-30.f + 50.f) / 100.f));
}

TEST_CASE("Particle bounce logic - simple upper bounce") {
  Billiard b{100.f, 50.f, 50.f};
  Particle p{45.f, M_PI / 4, 100.f}; // verso l'alto
  p.move(b, 0.1f);
  CHECK(p.getPosition().x > 0.f);
}

TEST_CASE("Particle bounce logic - extreme lower bounce") {
  Billiard b{200.f, 60.f, 60.f};
  Particle p{-55.f, -M_PI / 4, 150.f};
  p.move(b, 0.1f);
  CHECK(p.getPosition().x > 0.f);
}

TEST_CASE("Statistical simulation - check bounds") {
  Billiard b{400.f, 100.f, 50.f};
  CHECK_NOTHROW(run_statistics(10, 0.f, 10.f, 0.f, 0.1f, b));
}

TEST_CASE("Statistical simulation - y0 or theta0 out of bounds") {
  Billiard b{400.f, 100.f, 50.f};
  CHECK_THROWS(run_statistics(10, 200.f, 10.f, 0.f, 0.1f, b)); // mu_y0 fuori range
}
