#ifndef SIMNET_STORAGE_TYPES_HPP
#define SIMNET_STORAGE_TYPES_HPP


#include <iostream>
#include <list>
#include <deque>
#include <functional>

#include "types.hpp"
#include "package.hpp"


enum class PackageQueueType {
    FIFO, LIFO
};


class IPackageStockpile {
public:
    virtual void push(Package &&elem) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0 ;

    using const_iterator = std::list<Package>::const_iterator;

    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

private:

};


class IPackageQueue : public IPackageStockpile {
public:
    virtual PackageQueueType get_queue_type() const = 0;
    virtual Package pop() = 0;
private:

};


class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType);
    PackageQueue() = default;
    void push(Package &&elem) override;
    bool empty() const override;
    size_t size() const override;
    Package pop() override;
    inline PackageQueueType get_queue_type() const override { return _QueueType; };

    inline const_iterator cbegin() const override {return _list.cbegin();};
    inline const_iterator cend() const override {return _list.cend();};
    inline const_iterator begin() const override {return _list.begin();};
    inline const_iterator end() const override {return _list.end();};
private:
    PackageQueueType _QueueType;
    std::list<Package> _list;

};


#endif //SIMNET_STORAGE_TYPES_HPP
