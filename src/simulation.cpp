#include "simulation.hpp"

void simulate(Factory &factory, TimeOffset d, std::function<void(Factory &, Time)> func){
    for(TimeOffset time_ = 1; time_ <= d; time_++){
        factory.do_deliveries(time_);
        factory.do_package_passing();
        factory.do_work(time_);

        func(factory, time_);
    }
}