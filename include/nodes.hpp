#ifndef SIMNET_NODES_HPP
#define SIMNET_NODES_HPP


#include <map>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <random>
#include <memory>

#include "types.hpp"
#include "package.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"

enum class ReceiverType {
    WORKER, STOREHOUSE
};


class IPackageReceiver {
public:
    virtual ElementID get_id() const = 0;
    virtual void receive_package(Package&& p) = 0;
    virtual ReceiverType get_receiver_type() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator random_function = probability_generator) {rand_func = random_function;}
    IPackageReceiver* choose_receiver();
    void add_receiver(IPackageReceiver* receiver);
    void remove_receiver(IPackageReceiver* receiver);
    inline const preferences_t& get_preferences() const { return preferences_; }
    inline void set_preferences(preferences_t pref) { preferences_ = pref;};

    inline const_iterator cbegin() const { return preferences_.cbegin(); }
    inline const_iterator cend() const { return preferences_.cend(); }
    inline const_iterator begin() const { return preferences_.cbegin(); }
    inline const_iterator end() const { return preferences_.cend(); }
private:
    std::map<IPackageReceiver*,double> preferences_;
    ProbabilityGenerator  rand_func;
};

class PackageSender {
public:
    inline PackageSender() {bufer = std::nullopt;}
    PackageSender(PackageSender&&)=default;
    void send_package();
    std::optional<Package>& get_sending_buffer();
    ReceiverPreferences receiver_preferences_;
protected:
    void push_package(Package&& package);
    std::optional<Package> bufer ;
};

class Ramp: public PackageSender {
public:

    Ramp(ElementID id_, TimeOffset di): PackageSender(), id(id_), delivery_interval(di) {}
    void deliver_goods(Time t);
    inline TimeOffset get_delivery_interval() { return delivery_interval; }
    inline ElementID get_id() const { return id; }
private:
    ElementID id;
    TimeOffset delivery_interval;
};

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID ID, std::unique_ptr<PackageQueue> ptr = std::make_unique<PackageQueue>()) : id(ID), stockpile(std::move(ptr)) {}
    void receive_package(Package&& aPackage) override;
    inline ElementID get_id() const override { return id; };
    inline ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; };

    inline IPackageStockpile::const_iterator cbegin() const override { return stockpile->cbegin();};
    inline IPackageStockpile::const_iterator cend() const override { return stockpile->cend();};
    inline IPackageStockpile::const_iterator begin() const override { return stockpile->cbegin();};
    inline IPackageStockpile::const_iterator end() const override { return stockpile->cend();};
private:
    ElementID id;
    std::unique_ptr<IPackageStockpile> stockpile;
};



class Worker: public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> ptr) : PackageSender(), processing_durationWorker(pd), idWorker(id), ptrWorker(std::move(ptr)) {bufor = std::nullopt;};
    void do_work(Time timePackage);
    TimeOffset get_processing_duration() const;
    Time get_package_processing_start_time() const;
    inline ElementID get_id() const override { return idWorker; };
    void receive_package(Package&& aPackage) override;

    inline ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; };
    inline IPackageStockpile::const_iterator cbegin() const override { return ptrWorker->cbegin();};
    inline IPackageStockpile::const_iterator cend() const override { return ptrWorker->cend();};
    inline IPackageStockpile::const_iterator begin() const override { return ptrWorker->cbegin();};
    inline IPackageStockpile::const_iterator end() const override { return ptrWorker->cend();};

private:
    TimeOffset processing_durationWorker;
    ElementID idWorker;
    std::unique_ptr <PackageQueue> ptrWorker;
    std::optional <Package> bufor;
    Time timePackage = 0;
};



#endif //SIMNET_NODES_HPP
