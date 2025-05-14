#include "statistics.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "particle.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <algorithm>

void show_histograms(const std::vector<float> &y_data, const std::vector<float> &theta_data)
{
  const int width = 800;
  const int height = 600;
  sf::RenderWindow window(sf::VideoMode(width, height), "Histograms");

  // Set common parameters
  const int bin_count = 30;
  const int padding = 50;

  auto create_histogram = [](const std::vector<float> &data, int bin_count, std::map<int, int> &histogram, float &min_val, float &max_val)
  {
    if (data.empty())
      return;

    min_val = *std::min_element(data.begin(), data.end());
    max_val = *std::max_element(data.begin(), data.end());
    float bin_size = (max_val - min_val) / bin_count;

    for (float val : data)
    {
      int bin_index = std::min(static_cast<int>((val - min_val) / bin_size), bin_count - 1);
      ++histogram[bin_index];
    }
  };

  std::map<int, int> hist_y, hist_theta;
  float min_y, max_y, min_theta, max_theta;
  create_histogram(y_data, bin_count, hist_y, min_y, max_y);
  create_histogram(theta_data, bin_count, hist_theta, min_theta, max_theta);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::White);

    // Draw histogram for y
    int graph_height = height / 2 - padding * 2;
    int graph_width = width - 2 * padding;
    int max_count_y = std::max_element(hist_y.begin(), hist_y.end(),
                                       [](const auto &a, const auto &b)
                                       { return a.second < b.second; })
                          ->second;

    for (int i = 0; i < bin_count; ++i)
    {
      float bin_width = graph_width / static_cast<float>(bin_count);
      float bin_height = (hist_y[i] / static_cast<float>(max_count_y)) * graph_height;

      sf::RectangleShape bar(sf::Vector2f(bin_width - 2, bin_height));
      bar.setFillColor(sf::Color::Blue);
      bar.setPosition(padding + i * bin_width, padding + graph_height - bin_height);
      window.draw(bar);
    }

    // Draw histogram for theta
    int offset_y = height / 2;
    int max_count_theta = std::max_element(hist_theta.begin(), hist_theta.end(),
                                           [](const auto &a, const auto &b)
                                           { return a.second < b.second; })
                              ->second;

    for (int i = 0; i < bin_count; ++i)
    {
      float bin_width = graph_width / static_cast<float>(bin_count);
      float bin_height = (hist_theta[i] / static_cast<float>(max_count_theta)) * graph_height;

      sf::RectangleShape bar(sf::Vector2f(bin_width - 2, bin_height));
      bar.setFillColor(sf::Color::Red);
      bar.setPosition(padding + i * bin_width, offset_y + padding + graph_height - bin_height);
      window.draw(bar);
    }

    window.display();
  }
}

// Funzioni statistiche
float mean(const std::vector<float> &values)
{
  float sum = 0.0f;
  for (float v : values)
  {
    sum = sum + v;
  }
  return sum / static_cast<float>(values.size());
}

float stddev(const std::vector<float> &values, float mean_val)
{
  float sum_sq = 0.0f;
  for (float v : values)
  {
    sum_sq += (v - mean_val) * (v - mean_val);
  }
  return std::sqrt(sum_sq / static_cast<float>(values.size()));
}
// skewness: simmetria
float skewness(const std::vector<float> &values, float mean_val,
               float std_val)
{
  float skew_sum = 0.0f;
  for (float v : values)
    skew_sum += static_cast<float>(std::powf((v - mean_val) / std_val, 3));
  return skew_sum / static_cast<float>(values.size());
}
// kurtosis: appiattimento
float kurtosis(const std::vector<float> &values, float mean_val,
               float std_val)
{
  float kurt_sum = 0.0f;
  for (float v : values)
    kurt_sum += static_cast<float>(std::powf((v - mean_val) / std_val, 4));
  return kurt_sum / static_cast<float>(values.size());
}

// Salvataggio dei dati su file
void save_to_file(const std::string &filename, const std::vector<float> &data)
{
  std::ofstream file(filename);
  if (!file)
  {
    std::cerr << "Error: cannot open file " << filename << " for writing.\n";
    return;
  }
  for (float val : data)
    file << val << '\n';
  file.close();
}

void run_statistics(int N, float mu_y0, float sigma_y0, float mu_theta0,
                    float sigma_theta0, const Billiard &billiard)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<float> dist_y0(mu_y0, sigma_y0);

  std::normal_distribution<float> dist_theta0(mu_theta0, sigma_theta0);

  std::vector<float> y_finals;
  std::vector<float> theta_finals;
  int discarded_generated_balls = 0;
  int discarded_left_balls = 0;

  for (int i = 0; i < N; ++i)
  {
    float y0 = dist_y0(gen);
    float theta0 = dist_theta0(gen);
    // Controllo che y0 sia tra upper e lower surface intercept
    if (y0 > billiard.upper_surface_intercept() || y0 < billiard.lower_surface_intercept())
    {
      std::cout << "BALL N. " << i + 1 << " DISCARDED: y out of range, y = " << y0 << "\n"
                << "\n";
      ++discarded_generated_balls;
      continue; // Scarta il ciclo corrente
    }

    // Controllo che theta sia tra -90° e 90°
    float theta0_deg = (theta0 * 180.f) / static_cast<float>(M_PI);
    if (theta0_deg > 90.f || theta0_deg < -90.f)
    {
      std::cout << "BALL N. " << i + 1 << " DISCARDED: theta out of range, theta = " << theta0_deg << "\n"
                << "\n";
      ++discarded_generated_balls;
      continue; // Scarta il ciclo corrente
    }
    //std::cout << "BALL N. " << i + 1 << " SHOT\n"
              //<< "GENERATED VALUES: theta = " << (theta0 * 180.f) / (M_PI)
              //<< "   y = " << y0 << "\n";
    float x0 = 0;
    int r = 0;
    // Caso speciale: nessun rimbalzo, va direttamente alla destra
    float y_direct = tan(theta0) * billiard.getLength() + y0;
    if (y_direct <= billiard.upper_right_height() && y_direct >= billiard.lower_right_height())
    {
      y_finals.push_back(y_direct);
      theta_finals.push_back(theta0);
      //std::cout << "BALL N. " << i + 1 << " SHOT\n"
                //<< "GENERATED VALUES: theta = " << (theta0 * 180.f) / (M_PI)
                //<< "   y = " << y0 << "\n";
      //std::cout << "NO BOUNCE - DIRECT TO EXIT: theta = " << (theta0 * 180.f) / M_PI
                //<< "  y = " << y_direct << "   x = " << billiard.getLength() << "\n\n";
      continue;
    }

    float x1 = (y0 - billiard.upper_surface_intercept()) /
               (billiard.upper_slope() - tan(theta0));
    float x2 = (y0 - billiard.lower_surface_intercept()) /
               (billiard.lower_slope() - tan(theta0));
    if (x1 > x2)
    {
      r = 1;
      x0 = x1;
      y0 = tan(theta0) * x0 + y0;
      theta0 = -(theta0 - 2.f * atan(billiard.upper_slope()));
    }
    else
    {
      r = 2;
      x0 = x2;
      y0 = tan(theta0) * x0 + y0;
      theta0 = -(theta0 - 2.f * atan(billiard.lower_slope()));
    }

    do
    {
      //std::cout << "theta = " << (theta0 * 180.f) / (M_PI) << "  y = " << y0
                //<< "   x0 = " << x0 << "\n ";
      float a = (((powf(-1, static_cast<float>(r))) * billiard.upper_surface_intercept()) - y0 +
                 tanf(theta0) * x0) /
                (tanf(theta0) + (powf(static_cast<float>(-1), static_cast<float>(r + 1))) * billiard.upper_slope());
      if (a <= billiard.getLength() && a >= 0)
      {
        x0 = (powf(-1, static_cast<float>(r)))* billiard.upper_surface_intercept() - y0 +
        (tanf(theta0)) * x0 /
             (tanf(theta0) + (powf(-1, static_cast<float>(r + 1))) * billiard.upper_slope());
        y0 =(powf(-1, static_cast<float>(r)))* billiard.upper_slope() * x0 +
        powf(-1,static_cast<float>(r)) * billiard.upper_surface_intercept();
        theta0 = -(theta0 - 2.f * (powf(-1, static_cast<float>(r))) * atanf(billiard.upper_slope()));
        theta0_deg = (theta0 * 180.f) / static_cast<float>(M_PI);

        r++;
      }
      else
      {
        break;
      }

    } while (x0 <= billiard.getLength() && x0 >= 0);

    float y_final =
        tanf(theta0) * (billiard.getLength() - x0) + y0;
    if (theta0_deg < 90 && theta0_deg > -90 && y_final <= billiard.upper_right_height() && y_final >= billiard.lower_right_height())
    {

      float x_final = (y_final - y0) / tanf(theta0) + x0;
      y_finals.push_back(y_final);
      theta_finals.push_back(theta0);
      /*std::cout << "FINAL: theta = " << (theta0 * 180.f) / (M_PI)
                << "  y = " << y_final << "   x = " << x_final << "\n " << "\n";*/
    }
    else if (y_final > billiard.upper_right_height() || y_final < billiard.lower_right_height() || theta0_deg > 90 || theta0_deg < -90)
    {
      float y_left =
          tanf(theta0) * (0 - x0) + y0;
      //std::cout << "FINAL: theta = " << (theta0 * 180.f) / (M_PI)
                //<< "  y = " << y_left << "   x = " << 0 << "\n ";
      //std::cout << "DISCARDED: DIDN'T GET OUT FROM THE RIGHT SIDE \n \n";
      ++discarded_left_balls;
    }
  }
  if (discarded_generated_balls + discarded_left_balls != 0)
  {
    std::cout << "\n"
              << discarded_generated_balls + discarded_left_balls << " BALLS OUT OF " << N
              << " WERE DISCARDED:\n";
  }
  if (discarded_generated_balls != 0)
  {
    std::cout << discarded_generated_balls << " DISCARDED BECAUSE GENERATED OUT OF THE LIMITS" << "\n ";
  }
  if (discarded_left_balls != 0)
  {
    std::cout << discarded_left_balls << " DISCARDED BECAUSE DIDN'T GET OUT FROM THE RIGHT SIDE\n";
  }

  // Calcolo delle statistiche
  float my = mean(y_finals);
  float sy = stddev(y_finals, my);
  float sk_y = skewness(y_finals, my, sy);
  float ku_y = kurtosis(y_finals, my, sy);

  float mt = mean(theta_finals);
  float st = stddev(theta_finals, mt);
  float sk_t = skewness(theta_finals, mt, st);
  float ku_t = kurtosis(theta_finals, mt, st);

  // Output delle statistiche
  std::cout << "\n--- Final Statistics ---\n";
  std::cout << "Final y:\n";
  std::cout << "Mean = " << my << ", StdDev = " << sy << ", Skewness = " << sk_y
            << ", Kurtosis = " << ku_y << "\n";

  std::cout << "Final theta:\n";
  std::cout << "Mean = " << mt << ", StdDev = " << st << ", Skewness = " << sk_t
            << ", Kurtosis = " << ku_t << "\n";

  // Salvataggio su file
  save_to_file("theta_finals", theta_finals);
  save_to_file("y_finals", y_finals);

  std::cout << "\nData saved to theta_finals.txt, y_finals.txt\n";
  // show_histograms(y_finals, theta_finals);
}
