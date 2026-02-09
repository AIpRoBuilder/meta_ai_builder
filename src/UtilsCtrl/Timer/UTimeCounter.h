/***************************

@File: UTimeCounter.h
@Time: 2023/12/3 17:49
@Desc: 
***************************/

#ifndef DAOGRAPH_UTIMECOUNTER_H
#define DAOGRAPH_UTIMECOUNTER_H

#include <string>
#include <chrono>

#include "../UtilsObject.h"

DAOGRAPH_NAMESPACE_BEGIN

class UTimeCounter : public UtilsObject {
public:
    explicit UTimeCounter() {
        key_ = DAOGRAPH_DEFAULT;
        start_ts_ = std::chrono::steady_clock::now();
    }

    explicit UTimeCounter(const std::string& key) {
        start_ts_ = std::chrono::steady_clock::now();
        key_ = key;
    }

    ~UTimeCounter() override {
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        DaoGraph::DAOGRAPH_ECHO("[%s]: time counter is : [%0.2lf] ms", key_.c_str(), span.count());
    }

private:
    std::chrono::steady_clock::time_point start_ts_;
    std::string key_;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UTIMECOUNTER_H
