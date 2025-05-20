#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

#include "billiard.hpp"
#include "particle.hpp"

float mean(const std::vector<float> &values);
float stddev(const std::vector<float> &values, float mean_val);
float skewness(const std::vector<float> &values, float mean_val,
               float std_val);
float kurtosis(const std::vector<float> &values, float mean_val,
               float std_val);

void save_to_file(
    const std::string &filename,
    const std::vector<float> &data);

void run_statistics(int N, float mu_y0, float sigma_y0, float mu_theta0_deg,
                    float sigma_theta0_deg, const Billiard &billiard);

#endif
