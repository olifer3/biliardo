#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "billiard.hpp"
#include "particle.hpp"
#include "statistics.hpp"

float getValidFloat(const std::string &prompt, float min, float max)
{
  float value;
  std::string input;

  while (true)
  {
    std::cout << prompt;
    std::getline(std::cin, input); // Legge tutta la riga

    std::stringstream ss(input);
    if (ss >> value)
    {
      // Controlla che non ci siano caratteri residui
      char c;
      if (ss >> c)
      {
        std::cout << "Error: you must insert only a number without extra "
                     "characters.\n";
      }
      else if (value < min || value > max)
      {
        std::cout << "Error: the value must be between " << min << " and "
                  << max << ".\n";
      }
      else
      {
        return value;
      }
    }
    else
    {
      std::cout << "Error: invalid input, you must insert a number.\n";
    }
  }
}

float getSigmaWithWarning(const std::string &prompt, float mu, float min_val, float max_val, float critical_threshold)
{
  while (true)
  {
    float sigma = getValidFloat(prompt, 0.0f, max_val);

    // Calcolo distanza minima dai bordi
    float max_sigma = std::min(max_val - mu, max_val + mu);
    if (max_sigma < 0.f)
    {
      max_sigma = 0.f;
    }
    // Se la sigma è molto grande rispetto all’intervallo utile
    if (sigma > critical_threshold * max_sigma)
    {
      std::cout << "WARNING: the standard deviation is quite large compared to the allowed range.\n"
                << "Many particles may be discarded. Do you want to proceed anyway? (y/n): ";
      std::string response;
      std::getline(std::cin, response);
      if (response == "y" || response == "Y")
      {
        return sigma;
      }
      else
      {
        std::cout << "Please enter a new value for the standard deviation.\n";
        continue;
      }
    }

    return sigma;
  }
}

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
  float theta0_deg = getValidFloat("Enter initial angle (-90 < theta < 90): ", -89.9999, 89.9999);
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

  sf::Color darkGreen(3, 99, 14);

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

  // Define a thicker "line" by drawing multiple close vertices
  sf::Vertex lineUp[] = {
      sf::Vertex(billiard.PointsUp(windowSize)[0] + sf::Vector2f(0, 2),
                 sf::Color::Black), // Offset vertically to simulate thickness
      sf::Vertex(billiard.PointsUp(windowSize)[1] + sf::Vector2f(0, 2),
                 sf::Color::Black),

      sf::Vertex(billiard.PointsUp(windowSize)[0] + sf::Vector2f(0, -2),
                 sf::Color::Black), // Negative offset for the opposite side
      sf::Vertex(billiard.PointsUp(windowSize)[1] + sf::Vector2f(0, -2),
                 sf::Color::Black)};

  sf::Vertex lineLow[] = {
      sf::Vertex(billiard.PointsLow(windowSize)[0] + sf::Vector2f(0, 2),
                 sf::Color::Black),
      sf::Vertex(billiard.PointsLow(windowSize)[1] + sf::Vector2f(0, 2),
                 sf::Color::Black),

      sf::Vertex(billiard.PointsLow(windowSize)[0] + sf::Vector2f(0, -2),
                 sf::Color::Black),
      sf::Vertex(billiard.PointsLow(windowSize)[1] + sf::Vector2f(0, -2),
                 sf::Color::Black)};

  sf::CircleShape particleShape(5);
  sf::Vector2f offset{billiard.PointsUp(windowSize)[0].x, windowSize.y / 2.f};
  particleShape.setFillColor(sf::Color::Red);
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
      // window.draw(lineUp, 2, sf::Lines);
      // window.draw(lineLow, 2, sf::Lines);
      window.draw(lineUp, 4, sf::Lines);
      window.draw(lineLow, 4, sf::Lines);

      window.draw(particleShape);
      window.display();
    }
  }
  std::cout << "\nFinal state of the particle:\n";
  particle.print_state(billiard);
  // giusto per capire
  std::cout << "\nFinal (not precise) state of the particle:\n";
  particle.print_state_notprecise();
}

void statistics()
{
  // PARTE STATISTICA
  float length = getValidFloat("Enter billiard length (0 - 800): ", 0, 800);
  float r1 = getValidFloat("Enter left height (0 - 300): ", 0, 300);
  float r2 = getValidFloat("Enter right height (0 - 300): ", 0, 300);

  // Create a Billiard object with user-defined parameters
  Billiard billiard(length, r1, r2);

  // 1. Lettura da tastiera dei parametri statistici
  float mu_y0 = getValidFloat("Enter mean of y0 (mu_y0): ", -r1, r1);
  
  float sigma_y0 = getSigmaWithWarning(
      "Enter standard deviation of y0 (sigma_y0): ",
      mu_y0, -r1, r1, 0.7f // 70% del massimo range utile, soglia di warning
  );

  float mu_theta0_deg = getValidFloat(
      "Enter mean of theta0 in degrees (mu_theta0): ", -90.0f, 90.0f);
  float mu_theta0 = (M_PI * mu_theta0_deg) / 180.f;

  
  float sigma_theta0_deg = getSigmaWithWarning(
      "Enter standard deviation of theta0 in degrees (sigma_theta0): ",
      mu_theta0_deg, -90.f, 90.f, 0.7f);
  float sigma_theta0 = (M_PI * sigma_theta0_deg) / 180.f;

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
