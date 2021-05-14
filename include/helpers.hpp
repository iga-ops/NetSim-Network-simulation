#ifndef SIMNET_HELPERS_HPP
#define SIMNET_HELPERS_HPP


#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;


#endif //SIMNET_HELPERS_HPP
