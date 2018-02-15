#ifndef RATE_H
#define RATE_H

#include <string>

#include "mpxjpp-gens.h"
#include "timeunit.h"

namespace mpxjpp {
struct Rate final {
    static constexpr bool IS_MOVABLE = true;

    double amount = 0;
    TimeUnit units = TimeUnit::MINUTES;


    constexpr bool operator ==(const Rate &other) const {
        return units == other.units && (abs(other.amount - amount) < 0.00001);
    }

    constexpr int compareTo(const Rate &other) const {
        if (units == other.units)
            return abs(amount - other.amount) < 0.00001 ? 0 : amount < other.amount ? -1 : 1;
        return static_cast<int>(units) - static_cast<int>(other.units);
    }
};
static_assert(common::anyimpl::is_movable<Rate>, "Rate should be movable");
}

namespace std {
template<>
struct hash<mpxjpp::Rate> {
    constexpr size_t operator()(const mpxjpp::Rate &val) const {
        return (int)val.amount + (int)val.units;
    }
};

//std::string to_string(mpxjpp::Rate val) {
//  return to_string(val.amount()) + to_string(val.units());
//}
}

#endif // RATE_H
