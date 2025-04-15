#include "statistics.hpp"
#include "particle.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <string>

// Funzioni statistiche
float mean(const std::vector<float>& values) {
    float sum = 0.0f;
    for (float v : values) sum += v;
    return sum / static_cast<float>(values.size());
}

float stddev(const std::vector<float>& values, float mean_val) {
    float sum_sq = 0.0f;
    for (float v : values)
        sum_sq += (v - mean_val) * (v - mean_val);
    return std::sqrt(sum_sq / static_cast<float>(values.size()));
}

float skewness(const std::vector<float>& values, float mean_val, float std_val) {
    float skew_sum = 0.0f;
    for (float v : values)
        skew_sum += std::pow((v - mean_val) / std_val, 3);
    return skew_sum / static_cast<float>(values.size());
}

float kurtosis(const std::vector<float>& values, float mean_val, float std_val) {
    float kurt_sum = 0.0f;
    for (float v : values)
        kurt_sum += std::pow((v - mean_val) / std_val, 4);
    return kurt_sum / static_cast<float>(values.size());
}

// Salvataggio dei dati su file
void save_to_file(const std::string& filename, const std::vector<float>& data) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: cannot open file " << filename << " for writing.\n";
        return;
    }
    for (float val : data)
        file << val << '\n';
    file.close();
}

void run_statistics(
    int N,
    float mu_y0,
    float sigma_y0,
    float mu_theta0_deg,
    float sigma_theta0_deg,
    float velocity,
    const Billiard& billiard
) {
    std::vector<float> y_finals;
    std::vector<float> theta_finals;
    std::vector<float> theta_inputs;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist_y0(mu_y0, sigma_y0);
    std::normal_distribution<float> dist_theta0(mu_theta0_deg, sigma_theta0_deg);

    for (int i = 0; i < N; ++i) {
        float y0 = dist_y0(gen);
        if (y0 < billiard.upper_surface_intercept() || y0 > billiard.lower_surface_intercept()) {
            --i;
            continue;
        }

        float theta0_deg = dist_theta0(gen);
        if (theta0_deg >= -90.0f || theta0_deg <= 90.0f) {
            --i;
            continue;
        }

        float theta0 = theta0_deg * static_cast<float>(M_PI) / 180.0f;
        theta_inputs.push_back(theta0_deg);

        Particle particle(y0, theta0, velocity);

        while (particle.getPosition().x >= 0 && particle.getPosition().x <= billiard.getLength()) {
            particle.move(billiard, 0.01f);
        }

        sf::Vector2f pos = particle.getPosition();
        y_finals.push_back(pos.y);
        theta_finals.push_back(particle.getAngle() * 180.0f / static_cast<float>(M_PI));
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
    std::cout << "y_f (output position):\n";
    std::cout << "Mean = " << my << ", StdDev = " << sy << ", Skewness = " << sk_y << ", Kurtosis = " << ku_y << "\n";

    std::cout << "theta_f (output angle):\n";
    std::cout << "Mean = " << mt << ", StdDev = " << st << ", Skewness = " << sk_t << ", Kurtosis = " << ku_t << "\n";

    // Salvataggio su file
    save_to_file("theta_input.txt", theta_inputs);
    save_to_file("theta_output.txt", theta_finals);
    save_to_file("y_output.txt", y_finals);

    std::cout << "\nData saved to theta_input.txt, theta_output.txt, y_output.txt\n";
}
