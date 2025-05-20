#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

float getValidFloat(const std::string &prompt, float min, float max);
float getSigmaWithWarning(const std::string &prompt, float mu, float max_val, float critical_threshold);

#endif 