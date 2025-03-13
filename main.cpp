#include "billiard.hpp"
#include "particle.hpp"
#include <iostream>

int main()
{
  double length, r1, r2;
  std::cout << "Enter billiard length: ";
  std::cin >> length;

  std::cout << "Enter left height (r1): ";
  std::cin >> r1;

  std::cout << "Enter right height (r2): ";
  std::cin >> r2;

  // Create a Billiard object with user-defined parameters
  Billiard billiard(length, r1, r2);

  // Print billiard properties
  billiard.print_info();

  auto upper_norm = billiard.upper_normal();
  auto lower_norm = billiard.lower_normal();

  std::cout << "Upper normal: (" << upper_norm[0] << ", " << upper_norm[1] << ")\n";
  std::cout << "Lower normal: (" << lower_norm[0] << ", " << lower_norm[1] << ")\n";

  // Chiedi i parametri della particella
  double y0, theta0, velocity;
  std::cout << "Enter initial y position of particle: ";
  std::cin >> y0;

  std::cout << "Enter initial angle (in radians): ";
  std::cin >> theta0;

  std::cout << "Enter initial velocity: ";
  std::cin >> velocity;

  // Crea la particella con i parametri inseriti
  Particle particle(y0, theta0, velocity);

  // Stampa lo stato iniziale della particella
  std::cout << "\nInitial state of the particle:\n";
  particle.print_state();

  // Muovi la particella all'interno del biliardo
  particle.move(billiard);

  // Stampa lo stato finale della particella
  std::cout << "\nFinal state of the particle:\n";
  particle.print_state();

  return 0;
}
