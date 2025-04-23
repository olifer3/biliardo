#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

#include "billiard.hpp"
#include "particle.hpp"

// Risultati statistici
struct StatsResult
{
  float mean;
  float stddev;
  float skewness;
  float kurtosis;
};

void save_to_file(
    const std::string &filename,
    const std::vector<float> &data); // funzione per scrivere su testo

// Funzione principale di simulazione
void run_statistics(int N, float mu_y0, float sigma_y0, float mu_theta0_deg,
                    float sigma_theta0_deg, const Billiard &billiard);

void show_histograms(const std::vector<float> &y_data, const std::vector<float> &theta_data);

#endif
