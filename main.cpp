#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

#include "billiard.hpp"
#include "particle.hpp"

int main() {
  float length, r1, r2;
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

  std::cout << "Upper normal: (" << upper_norm[0] << ", " << upper_norm[1]
            << ")\n";
  std::cout << "Lower normal: (" << lower_norm[0] << ", " << lower_norm[1]
            << ")\n";

  // Chiedi i parametri della particella
  float y0, theta0, velocity;
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

  sf::RenderWindow window(sf::VideoMode(800, 600), "Biliardo Triangolare");
  sf::Vector2f windowSize(window.getSize().x, window.getSize().y);
  sf::View view;
  view.setSize(windowSize);
  view.setCenter(0, windowSize.y / 2);
  window.setView(view);

  window.setVerticalSyncEnabled(false);

  // Disegna l'asse X (orizzontale)
  sf::RectangleShape xAxis(sf::Vector2f(
      windowSize.x, 2));  // Linea orizzontale lunga quanto la finestra
  xAxis.setFillColor(sf::Color::Red);  // Colore rosso per l'asse X
  xAxis.setPosition(
      -windowSize.x / 2,
      windowSize.y / 2);  // Posiziona la linea al centro in altezza

  // Disegna l'asse Y (verticale)
  sf::RectangleShape yAxis(sf::Vector2f(
      2, windowSize.y));  // Linea verticale alta quanto la finestra
  yAxis.setFillColor(sf::Color::Blue);  // Colore blu per l'asse Y
  yAxis.setPosition(-windowSize.x / 2,
                    0);  // Posiziona la linea lungo il bordo sinistro

  sf::Vertex lineUp[] = {
      sf::Vertex(billiard.PointsUp(windowSize)[0], sf::Color::White),
      sf::Vertex(billiard.PointsUp(windowSize)[1], sf::Color::White)};
  sf::Vertex lineLow[] = {
      sf::Vertex(billiard.PointsLow(windowSize)[0], sf::Color::Green),
      sf::Vertex(billiard.PointsLow(windowSize)[1], sf::Color::Green)};

  sf::CircleShape particleShape(10);
  particleShape.setFillColor(sf::Color::Red);
  //sf::Clock clock;
  //float deltaTime = clock.restart().asSeconds();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
    //float deltaTime = clock.restart().asSeconds();

    particleShape.setPosition(particle.getPosition().x - windowSize.x / 2,
                              particle.getPosition().y + windowSize.y / 2);

    // Muovi la particella all'interno del biliardo
    while (particle.getPosition().x <= billiard.getLength()) {
      particle.move(billiard);
    window.draw(xAxis);
    window.draw(yAxis);
    window.draw(lineUp, 2, sf::Lines);
    window.draw(lineLow, 2, sf::Lines);
    window.draw(particleShape);
    window.display();
    window.clear();
    }

    
  }

  // Stampa lo stato finale della particella
  std::cout << "\nFinal state of the particle:\n";
  particle.print_state();

  return 0;
}
