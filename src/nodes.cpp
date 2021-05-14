#include "nodes.hpp"


//--------------------------------------------ReceiverPreferences------------------------------------------------------

void ReceiverPreferences::add_receiver(IPackageReceiver* receiver){
    preferences_.emplace(std::make_pair(receiver, 1));


    double sum = 0;
    for(const auto &item: preferences_) {
        sum+=item.second;
    }

    for( auto &item: preferences_) {
        item.second /= sum;
    }

    /*double new_probability = rand_func();
    preferences_.insert(std::make_pair(receiver, new_probability));
    double sum_probabilities = 1 + new_probability;
    for (auto& [key, value]: preferences_) {
        value = value/sum_probabilities;
    }*/
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* receiver) {
    preferences_.erase(receiver);
    double sum_probabilities = 0;
    for (auto& [key, value]: preferences_) {
        sum_probabilities += value;
    }
    for (auto& [key, value]: preferences_) {
        value = value/sum_probabilities;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double distributio = rand_func();
    double sum_probabilities = 0;
    for (auto&[key, value]: preferences_) {
        sum_probabilities += value;
        if (sum_probabilities >= distributio) {
            return key;
        }
    }

    return preferences_.begin()->first;
}

//--------------------------------------------PackageSender------------------------------------------------------


void PackageSender::push_package(Package&& package)
{
    bufer.emplace(std::move(package));
}

std::optional<Package>& PackageSender::get_sending_buffer()
{
    return bufer;
}

void PackageSender::send_package()
{
    if (bufer) {
        auto receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*bufer));
        bufer.reset();
    }
}


//--------------------------------------------Ramp------------------------------------------------------


void Ramp::deliver_goods(Time t)
{
    if(t % delivery_interval == 1)
    {
        Package pkc;
        push_package(std::move(pkc));
    }
}


//--------------------------------------------Worker------------------------------------------------------

void Worker::do_work(Time t){

    if (!(ptrWorker -> empty()) && !bufor) {
        bufor = ptrWorker -> pop();
        timePackage = t;
    }
    if(t-timePackage == processing_durationWorker-1 && bufor) {
        push_package(std::move(*bufor));
        bufor.reset();
    }

}

TimeOffset Worker::get_processing_duration() const {
    return processing_durationWorker;
}

Time Worker::get_package_processing_start_time() const {
    return timePackage;
}

void Worker::receive_package(Package&& aPackage) {
    ptrWorker->push(std::move(aPackage));
}


//--------------------------------------------Storehouse------------------------------------------------------

void Storehouse::receive_package(Package&& aPackage) {
    stockpile->push(std::move(aPackage));
}
// 39 numer