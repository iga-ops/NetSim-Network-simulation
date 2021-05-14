#include "storage_types.hpp"

PackageQueue::PackageQueue(PackageQueueType QueueType): _QueueType(QueueType) {}

void PackageQueue::push(Package &&elem) {
    _list.emplace_back(std::move(elem));
}

bool PackageQueue::empty() const {
    return _list.empty();
}

size_t PackageQueue::size() const {
    return _list.size();
}

Package PackageQueue::pop(){
    switch(_QueueType){
        case PackageQueueType::FIFO: {
            Package first_elem = std::move(_list.front());
            _list.pop_front();
            return first_elem;
        }
        case PackageQueueType::LIFO: {
            Package last_elem = std::move(_list.back());
            _list.pop_back();
            return last_elem;
        }
    }
    return Package();
}

