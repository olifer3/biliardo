/*#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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
}*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "billiard.hpp"

#include <cmath>
#include <cstdio>  // per std::removeF
#include <fstream>

#include "doctest.h"
#include "particle.hpp"
#include "statistics.hpp"

TEST_CASE("Billiard initialization and geometry") {
  Billiard b{100.f, 50.f, 30.f};
  CHECK(b.getLength() == doctest::Approx(100.f));
  CHECK(b.upper_slope() == doctest::Approx((30.f - 50.f) / 100.f));
  CHECK(b.lower_slope() == doctest::Approx((-30.f + 50.f) / 100.f));
}

TEST_CASE("Particle bounce logic - simple upper bounce") {
  Billiard b{100.f, 50.f, 50.f};
  Particle p{45.f, static_cast<float>(M_PI) / 4, 100.f};  // verso l'alto
  p.move(b, 0.1f);
  CHECK(p.getPosition().x > 0.f);
}

TEST_CASE("Particle bounce logic - extreme lower bounce") {
  Billiard b{200.f, 60.f, 60.f};
  Particle p{-55.f, static_cast<float>(-M_PI) / 4, 150.f};
  p.move(b, 0.1f);
  CHECK(p.getPosition().x > 0.f);
}

TEST_CASE("Statistical simulation - check bounds") {
  Billiard b{400.f, 100.f, 50.f};
  CHECK_NOTHROW(run_statistics(10, 0.f, 10.f, 0.f, 0.1f, b));
}

TEST_CASE("Statistics functions compute correct results on known data") {
  std::vector<float> data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

  float m = mean(data);
  float s = stddev(data, m);
  float sk = skewness(data, m, s);
  float ku = kurtosis(data, m, s);

  CHECK(m == doctest::Approx(3.0f));
  CHECK(s == doctest::Approx(std::sqrt(2.0f)));      // stddev = sqrt(2)
  CHECK(sk == doctest::Approx(0.0f));                // simmetrica
  CHECK(ku == doctest::Approx(1.7f).epsilon(0.01));  // ≈ 1.7 con tolleranza
}
TEST_CASE("Due rimbalzi + uscita a sinistra  (confronto con Desmos)") {
  Billiard billiard(10.f, 8.f, 4.f);
  float theta0_rad = 0.6f;
  Particle p(1.4f, theta0_rad, 5.f);
  float initial_angle = p.getAngle();
  int bounce_count = 0;

  sf::Vector2f first_hit, second_hit;

  // per trovare primo e secondo rimbalzo
  for (int i = 0; i < 5000; ++i) {
    p.move(billiard, 0.01f);
    if (p.getAngle() != initial_angle && bounce_count == 0) {
      first_hit = p.getPosition();
      ++bounce_count;
      initial_angle = p.getAngle();  // aggiorna per rilevare prossimo rimbalzo
    } else if (p.getAngle() != initial_angle && bounce_count == 1) {
      second_hit = p.getPosition();
      ++bounce_count;
      break;
    }
  }

  CHECK(first_hit.x == doctest::Approx(6.08779f).epsilon(0.002));
  CHECK(first_hit.y == doctest::Approx(5.56488f).epsilon(0.002));
  CHECK(second_hit.x == doctest::Approx(8.27152f).epsilon(0.002));
  CHECK(second_hit.y == doctest::Approx(-4.69139f).epsilon(0.002));

  // Verifica uscita a sinistra
  for (int i = 0; i < 3000; ++i) {
    p.move(billiard, 0.01f);
    if (p.getPosition().x <= 0.f) break;
  }

  auto final_pos = p.getPosition();
  CHECK(final_pos.x <= 0.f);
  CHECK(final_pos.y == doctest::Approx(7.58684f).epsilon(0.002));
}

TEST_CASE("due rimbalzi (bordo sotto) + uscita a sinistra  (valori Desmos)") {
  Billiard billiard(10.f, 10.f, 6.f);
  float theta0_rad = -0.5f;
  Particle p(2.f, theta0_rad, 5.f);
  float initial_angle = p.getAngle();
  int bounce_count = 0;

  sf::Vector2f first_hit, second_hit;

  // rilevazione primo e secondo rimbalzo
  for (int i = 0; i < 6000; ++i) {
    p.move(billiard, 0.01f);
    if (p.getAngle() != initial_angle && bounce_count == 0) {
      first_hit = p.getPosition();
      ++bounce_count;
      initial_angle = p.getAngle();
    } else if (p.getAngle() != initial_angle && bounce_count == 1) {
      second_hit = p.getPosition();
      ++bounce_count;
      break;
    }
  }

  CHECK(first_hit.x == doctest::Approx(12.68093f).epsilon(0.002));
  CHECK(first_hit.y == doctest::Approx(-4.92763f).epsilon(0.002));
  CHECK(second_hit.x == doctest::Approx(15.47744f).epsilon(0.002));
  CHECK(second_hit.y == doctest::Approx(3.80902f).epsilon(0.002));

  // Verifica uscita a sinistra
  for (int i = 0; i < 3000; ++i) {
    p.move(billiard, 0.01f);
    if (p.getPosition().x <= 0.f) break;
  }

  auto final_pos = p.getPosition();
  CHECK(final_pos.x <= 0.f);
  CHECK(final_pos.y == doctest::Approx(-1.55643f).epsilon(0.002));
}

TEST_CASE("Salvataggio file statistici – verifica esistenza e righe") {
  // 1. Setup: definisco un biliardo e parametri stretti per avere poche
  // particelle valide
  Billiard billiard(20.f, 5.f, 5.f);
  int N = 10;
  float mu_y0 = 0.f;
  float sigma_y0 = 0.1f;
  float mu_theta0 = 0.f;
  float sigma_theta0 = 0.01f;

  // 2. Eseguo la simulazione
  run_statistics(N, mu_y0, sigma_y0, mu_theta0, sigma_theta0, billiard);

  // 3. Verifica esistenza dei file
  std::ifstream f1("theta_finals");
  std::ifstream f2("y_finals");
  CHECK(f1.good());
  CHECK(f2.good());

  // 4. Conta le righe nei file
  int lines_theta = 0;
  std::string line;
  while (std::getline(f1, line)) ++lines_theta;
  f1.close();

  int lines_y = 0;
  while (std::getline(f2, line)) ++lines_y;
  f2.close();

  CHECK(lines_theta > 0);
  CHECK(lines_y == lines_theta);  // Devono avere lo stesso numero di righe

  // 5. (Opzionale) Elimina i file per non sporcare il filesystem
  std::remove("theta_finals");
  std::remove("y_finals");
}

TEST_CASE("Uscita a destra senza rimbalzi – traiettoria quasi diretta") {
  Billiard billiard(10.f, 5.f, 5.f);  // Pareti parallele: y = ±5
  float theta0_rad = 0.2f;            // ≈ 11.5°
  float y0 = 0.f;
  float velocity = 5.f;

  Particle p(y0, theta0_rad, velocity);

  // Simula finché non esce dal lato destro
  for (int i = 0; i < 3000; ++i) {
    p.move(billiard, 0.01f);
    if (p.getPosition().x >= billiard.getLength()) break;
  }

  auto final_pos = p.getPosition();
  float y_expected = std::tan(theta0_rad) * billiard.getLength() + y0;

  CHECK(final_pos.x >= billiard.getLength());
  CHECK(final_pos.y == doctest::Approx(y_expected)
                           .epsilon(0.01));  // tolleranza più larga dovuta ad
                                             // accumuli numerici di deltaTime
}
TEST_CASE("Uscita a destra con θ = 0 – orizzontale") {
  Billiard billiard(10.f, 5.f, 5.f);  // pareti parallele
  float theta0_rad = 0.f;
  float y0 = 0.f;
  float velocity = 5.f;

  Particle p(y0, theta0_rad, velocity);

  for (int i = 0; i < 3000; ++i) {
    p.move(billiard, 0.01f);
    if (p.getPosition().x >= billiard.getLength()) break;
  }

  auto final_pos = p.getPosition();

  CHECK(final_pos.x >= billiard.getLength());
  CHECK(final_pos.y == doctest::Approx(y0).epsilon(0.002));  // y costante
}
