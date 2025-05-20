#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "billiard.hpp"
#include "particle.hpp"
#include "statistics.hpp"
#include "input.hpp"



void normal()
{
  float length = getValidFloat("Enter billiard length (0 - 800): ", 0, 800);
  float r1 = getValidFloat("Enter left height (0 - 300): ", 0, 300);
  float r2 = getValidFloat("Enter right height (0 - 300): ", 0, 300);

  // Create a Billiard object with user-defined parameters
  Billiard billiard(length, r1, r2);

  // Print billiard properties
  // billiard.print_info();
  // Chiedi i parametri della particella
  float y0 = getValidFloat("Enter initial y position of particle: ", -r1, r1);
  float theta0_deg = getValidFloat("Enter initial angle (-90 < theta < 90): ", static_cast<float>(-89.9999), static_cast<float>(89.9999));
  float theta0 = (theta0_deg * static_cast<float>(M_PI)) / 180.0f;
  float velocity = getValidFloat("Enter initial velocity: ", 0, 2000);

  // Crea la particella con i parametri inseriti
  Particle particle(y0, theta0, velocity);

  // Stampa lo stato iniziale della particella
  std::cout << "\nInitial state of the particle:\n";
  particle.print_state(billiard);

  sf::RenderWindow window(sf::VideoMode(800, 600), "Biliardo Triangolare");
  sf::Vector2f windowSize(static_cast<float>(window.getSize().x),
                          static_cast<float>(window.getSize().y));
  sf::View view;
  // view.setSize(windowSize);
  view.setSize(windowSize.x, -windowSize.y); // Flip Y-axis
  view.setCenter(0, windowSize.y / 2);
  window.setView(view);

  window.setVerticalSyncEnabled(false);
  
  // Definisco i Colori
  sf::Color darkGreen(3, 99, 14);
  sf::Color darkRed(153, 9, 19);
  sf::Color Brown(56, 25, 2);

  // Disegna l'asse X (orizzontale)
  sf::RectangleShape xAxis(sf::Vector2f(
      windowSize.x, 1));               // Linea orizzontale lunga quanto la finestra
  xAxis.setFillColor(sf::Color::White); // Colore bianco per l'asse X
  xAxis.setPosition(
      -windowSize.x / 2,
      windowSize.y / 2); // Posiziona la linea al centro in altezza

  // Disegna l'asse Y (verticale)
  sf::RectangleShape yAxis(sf::Vector2f(
      1, windowSize.y));               // Linea verticale alta quanto la finestra
  yAxis.setFillColor(sf::Color::White); // Colore bianco per l'asse Y
  yAxis.setPosition(billiard.PointsUp(windowSize)[0].x,
                    0); // Posiziona la linea lungo il bordo sinistro

  
                    float thickness = 4.f;

                    // Prendi i punti delle due linee (up e low)
                    sf::Vector2f up0 = billiard.PointsUp(windowSize)[0];
                    sf::Vector2f up1 = billiard.PointsUp(windowSize)[1];
                    sf::Vector2f low0 = billiard.PointsLow(windowSize)[0];
                    sf::Vector2f low1 = billiard.PointsLow(windowSize)[1];
                    
                    // Crea i rettangoli
                    sf::VertexArray rectUp = billiard.createThickLine(up0, up1, thickness, Brown);
                    sf::VertexArray rectLow = billiard.createThickLine(low0, low1, thickness, Brown);

                 

  sf::CircleShape particleShape(7);
  sf::Vector2f offset{billiard.PointsUp(windowSize)[0].x, windowSize.y / 2.f};
  particleShape.setFillColor(darkRed);
  particleShape.setOrigin(particleShape.getRadius(),
                          particleShape.getRadius()); // Centering the circle
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

    if (particle.getPosition().x <= billiard.getLength() &&
        particle.getPosition().x >= 0)
    {
      particle.move(billiard, deltaTime);
      // std::cout<<particle.getAngle()<<"  ";
      particleShape.setPosition(particle.getPosition() + offset);
      window.clear(darkGreen); // Clear the window with dark green
      window.draw(xAxis);
      window.draw(yAxis);
      window.draw(rectUp);
      window.draw(rectLow);
      // window.draw(lineUp, 2, sf::Lines);
      // window.draw(lineLow, 2, sf::Lines);
      //window.draw(lineUp, 4, sf::Lines);
      //window.draw(lineLow, 4, sf::Lines);

      window.draw(particleShape);
      window.display();
    }
  }
  std::cout << "\nFinal state of the particle:\n";
  particle.print_state(billiard);
  // giusto per capire
  /*std::cout << "\nFinal (not precise) state of the particle:\n";
  particle.print_state_notprecise();*/
}

void statistics()
{
  // PARTE STATISTICA
  float length = getValidFloat("Enter billiard length (0 - 800): ", 0, 800);
  float r1 = getValidFloat("Enter left height (0 - 300): ", 0, 300);
  float r2 = getValidFloat("Enter right height (0 - 300): ", 0, 300);

  
  Billiard billiard(length, r1, r2);

  // 1. Lettura dei parametri statistici
  float mu_y0 = getValidFloat("Enter mean of y0 (mu_y0): ", -r1, r1);
  
  float sigma_y0 = getSigmaWithWarning(
      "Enter standard deviation of y0 (sigma_y0): ",
      mu_y0, r1, 0.7f // 70% del massimo range utile, soglia di warning
  );

  float mu_theta0_deg = getValidFloat(
      "Enter mean of theta0 in degrees (mu_theta0): ", -90.0f, 90.0f);
  float mu_theta0 = (static_cast<float>(M_PI) * mu_theta0_deg) / 180.f;

  
  float sigma_theta0_deg = getSigmaWithWarning(
      "Enter standard deviation of theta0 in degrees (sigma_theta0): ",
      mu_theta0_deg, 90.f, 0.7f);
  float sigma_theta0 = (static_cast<float>(M_PI) * sigma_theta0_deg) / 180.f;

  int N = static_cast<int>(
      getValidFloat("How many particles to shoot? ", 1, 10000));

  // 2. Chiamata alla funzione statistica vera e propria
  run_statistics(N, mu_y0, sigma_y0, mu_theta0, sigma_theta0, billiard);
}

int main()
{
  int scelta;

  std::cout << "Choose game mode:\n";
  std::cout << "1. Shoot one ball: insert 1\n";
  std::cout << "2. Shoot N balls: insert 2\n";
  std::cout << "Insert a number: ";

  std::cin >> scelta;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (scelta == 1)
  {
    normal();
  }
  else if (scelta == 2)
  {
    statistics();
  }
  else
  {
    std::cout << "Scelta non valida. Esci dal programma.\n";
  }
  return 0;
}
