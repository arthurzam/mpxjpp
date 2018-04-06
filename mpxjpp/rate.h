#ifndef RATE_H
#define RATE_H

#include <string>

#include "mpxjpp-gens.h"
#include "timeunit.h"

namespace mpxjpp {
struct Rate final {

    double amount = 0;
    TimeUnit units = TimeUnit::MINUTES;


    constexpr bool operator ==(const Rate &other) const {
        return units == other.units && (std::abs(other.amount - amount) < 0.00001);
    }
};
}

namespace std {
template<>
struct hash<mpxjpp::Rate> {
    constexpr size_t operator()(const mpxjpp::Rate &val) const {
        return static_cast<size_t>(val.amount) + static_cast<unsigned>(val.units);
    }
};
}

#endif // RATE_H
