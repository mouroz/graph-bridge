#ifndef RANDOMIZER_H
#define RANDOMIZER_H


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
bool randomBoolean(float trueChance);

#endif