#include "input.hpp"

#include <sstream>

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

float getSigmaWithWarning(const std::string &prompt, float mu, float max_val, float critical_threshold)
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