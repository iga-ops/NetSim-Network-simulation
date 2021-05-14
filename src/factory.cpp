//
// Created by Nadia on 17.01.2020.
//

#include "factory.hpp"
#include "types.hpp"



bool storehouse_is_avaliable(const PackageSender* packsender, std::map<const PackageSender*, NodeColor>& color){
    if(color[packsender] == NodeColor::VERIFIED)
        return true;
    color[packsender] = NodeColor::VISITED;

    if(packsender->receiver_preferences_.get_preferences().empty())
        return false;

    bool other_receiver = false;
    bool reachable_storehouse = false;
    int bad_nodes = 0;

    for(auto receiver : packsender->receiver_preferences_.get_preferences()) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            other_receiver = true;
            reachable_storehouse = true;
        }

        else {
            auto worker_ptr = dynamic_cast<Worker*>(receiver.first);
            if(worker_ptr == packsender)
                continue;

            other_receiver = true;
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(color[sendrecv_ptr] == NodeColor::UNVISITED)
                reachable_storehouse = storehouse_is_avaliable(sendrecv_ptr, color);
        }
        if(!reachable_storehouse || !other_receiver) {
            bad_nodes++;
        }
    }

    color[packsender] = NodeColor::VERIFIED;

    if(reachable_storehouse && !bad_nodes && other_receiver)
        return true;
    else return false;
}


bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> node_colors;
    node_colors.clear();
    for(auto& rampy: ramps) {
        Ramp *rptr = const_cast<Ramp *>(&rampy);
        node_colors.insert({dynamic_cast<PackageSender *>(rptr), NodeColor::UNVISITED});
    }
    for(auto& workerzy: workers) {
        Worker *worker_ptr = const_cast<Worker *>(&workerzy);
        node_colors.insert({dynamic_cast<PackageSender *>(worker_ptr), NodeColor::UNVISITED});
    }

    for(auto& rampy: ramps) {
        try {

            if(!storehouse_is_avaliable(&rampy,node_colors))
                return false;
        } catch (std::logic_error())
        { return false; }
    }
    return true;
}

void Factory::do_deliveries(Time t) {
    for(auto& rampy: ramps)
    {
        rampy.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for(auto& rampy: ramps)
    {
        if(rampy.get_sending_buffer().has_value())
        {
            rampy.send_package();
        }
    }
    for(auto& workerzy: workers)
    {
        if(workerzy.get_sending_buffer().has_value())
        {
            workerzy.send_package();
        }
    }
}

void Factory::do_work(Time t) {
    for(auto& workerzy: workers)
    {
        workerzy.do_work(t);
    }
}