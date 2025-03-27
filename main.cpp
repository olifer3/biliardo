#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

#include "billiard.hpp"
#include "particle.hpp"

int main()
{
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
  sf::Vector2f windowSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
  sf::View view;
  // view.setSize(windowSize);
  view.setSize(windowSize.x, -windowSize.y); // Flip Y-axis
  view.setCenter(0, windowSize.y / 2);
  window.setView(view);

  window.setVerticalSyncEnabled(false);

  sf::Color darkGreen(3, 38, 7);

  // Disegna l'asse X (orizzontale)
  sf::RectangleShape xAxis(sf::Vector2f(
      windowSize.x, 2));               // Linea orizzontale lunga quanto la finestra
  xAxis.setFillColor(sf::Color::Blue); // Colore rosso per l'asse X
  xAxis.setPosition(
      -windowSize.x / 2,
      windowSize.y / 2); // Posiziona la linea al centro in altezza

  // Disegna l'asse Y (verticale)
  sf::RectangleShape yAxis(sf::Vector2f(
      2, windowSize.y));               // Linea verticale alta quanto la finestra
  yAxis.setFillColor(sf::Color::Blue); // Colore blu per l'asse Y
  yAxis.setPosition(billiard.PointsUp(windowSize)[0].x,
                    0); // Posiziona la linea lungo il bordo sinistro

  /*sf::Vertex lineUp[] = {
      sf::Vertex(billiard.PointsUp(windowSize)[0], sf::Color::Black),
      sf::Vertex(billiard.PointsUp(windowSize)[1], sf::Color::Black)};
  sf::Vertex lineLow[] = {
      sf::Vertex(billiard.PointsLow(windowSize)[0], sf::Color::Black),
      sf::Vertex(billiard.PointsLow(windowSize)[1], sf::Color::Black)};*/

  // Define a thicker "line" by drawing multiple close vertices
  sf::Vertex lineUp[] = {
      sf::Vertex(billiard.PointsUp(windowSize)[0] + sf::Vector2f(0, 2), sf::Color::Black), // Offset vertically to simulate thickness
      sf::Vertex(billiard.PointsUp(windowSize)[1] + sf::Vector2f(0, 2), sf::Color::Black),

      sf::Vertex(billiard.PointsUp(windowSize)[0] + sf::Vector2f(0, -2), sf::Color::Black), // Negative offset for the opposite side
      sf::Vertex(billiard.PointsUp(windowSize)[1] + sf::Vector2f(0, -2), sf::Color::Black)};

  sf::Vertex lineLow[] = {
      sf::Vertex(billiard.PointsLow(windowSize)[0] + sf::Vector2f(0, 2), sf::Color::Black),
      sf::Vertex(billiard.PointsLow(windowSize)[1] + sf::Vector2f(0, 2), sf::Color::Black),

      sf::Vertex(billiard.PointsLow(windowSize)[0] + sf::Vector2f(0, -2), sf::Color::Black),
      sf::Vertex(billiard.PointsLow(windowSize)[1] + sf::Vector2f(0, -2), sf::Color::Black)};

  sf::CircleShape particleShape(5);
  sf::Vector2f offset{billiard.PointsUp(windowSize)[0].x, windowSize.y / 2.f};
  particleShape.setFillColor(sf::Color::Red);
  particleShape.setOrigin(particleShape.getRadius(), particleShape.getRadius()); // Centering the circle
  sf::Clock clock;
  // float deltaTime = clock.restart().asSeconds();
  particleShape.setPosition(particle.getPosition() + offset);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    float deltaTime = clock.restart().asSeconds();

    // Muovi la particella all'interno del biliardo

    if (particle.getPosition().x <= billiard.getLength() && particle.getPosition().x >= 0)
    {

      particle.move(billiard, deltaTime);
      // std::cout<<particle.getAngle()<<"  ";
      particleShape.setPosition(particle.getPosition() + offset);
      window.clear(darkGreen); // Clear the window with dark green
      window.draw(xAxis);
      window.draw(yAxis);
      // window.draw(lineUp, 2, sf::Lines);
      // window.draw(lineLow, 2, sf::Lines);
      window.draw(lineUp, 4, sf::Lines);
      window.draw(lineLow, 4, sf::Lines);

      window.draw(particleShape);
      window.display();
    }
  }
  std::cout << "\nFinal state of the particle:\n";
  particle.print_state();
  return 0;
}
