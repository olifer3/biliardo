#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "billiard.hpp"
#include "particle.hpp"
#include "statistics.hpp"
#include <cmath>

TEST_CASE("Billiard initialization and geometry")
{
  Billiard b{100.f, 50.f, 30.f};
  CHECK(b.getLength() == doctest::Approx(100.f));
  CHECK(b.upper_slope() == doctest::Approx((30.f - 50.f) / 100.f));
  CHECK(b.lower_slope() == doctest::Approx((-30.f + 50.f) / 100.f));
}

TEST_CASE("Particle bounce logic - simple upper bounce")
{
  Billiard b{100.f, 50.f, 50.f};
  Particle p{45.f, static_cast<float>(M_PI) / 4, 100.f}; // verso l'alto
  p.move(b, 0.1f);
  CHECK(p.getPosition().x > 0.f);
}

TEST_CASE("Particle bounce logic - extreme lower bounce")
{
  Billiard b{200.f, 60.f, 60.f};
  Particle p{-55.f, static_cast<float>(-M_PI) / 4, 150.f};
  p.move(b, 0.1f);
  CHECK(p.getPosition().x > 0.f);
}

TEST_CASE("Statistical simulation - check bounds")
{
  Billiard b{400.f, 100.f, 50.f};
  CHECK_NOTHROW(run_statistics(10, 0.f, 10.f, 0.f, 0.1f, b));
}

TEST_CASE("Statistics functions compute correct results on known data")
{
  std::vector<float> data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

  float m = mean(data);
  float s = stddev(data, m);
  float sk = skewness(data, m, s);
  float ku = kurtosis(data, m, s);

  CHECK(m == doctest::Approx(3.0f));
  CHECK(s == doctest::Approx(std::sqrt(2.0f)));     // stddev = sqrt(2)
  CHECK(sk == doctest::Approx(0.0f));               // simmetrica
  CHECK(ku == doctest::Approx(1.7f).epsilon(0.01)); // ≈ 1.7 con tolleranza
}