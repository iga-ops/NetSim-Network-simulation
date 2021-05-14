#include "factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

#ifndef SIMNET_SIMULATION_HPP
#define SIMNET_SIMULATION_HPP

void simulate(Factory  &factory, TimeOffset d, std::function<void(Factory&,Time)> func);

#endif //SIMNET_SIMULATION_HPP
