#include "randomizer.hpp"

#include <algorithm>
#include <sstream>
#include <random>


/**
 * @brief Generates a random boolean value based on a given probability.
 *
 * This function returns `true` with a probability of `trueChance` and `false` otherwise.
 * The probability value should be between `0.0f` and `1.0f`, inclusive.
 *
 * @param trueChance The probability (between 0.0 and 1.0) of returning `true`.
 * @return `true` with a probability of `trueChance`, otherwise `false`.
 *
 * @note Uses `std::random_device` and `std::mt19937` for better randomness.
 * @warning If `trueChance` is outside the range [0.0, 1.0], behavior is undefined.
 */
bool randomBoolean(float trueChance) {
  // Use random_device and mt19937 for a better random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  // Generate a random float between 0 and 1
  float randVal = dis(gen);

  // Return true if randVal is less than or equal to trueChance
  return randVal <= trueChance;
}
