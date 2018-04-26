#ifndef AVAILABILITY_H
#define AVAILABILITY_H

#include <vector>

#include "mpxjpp-gens.h"
#include "daterange.h"
#include "common/calendar.h"

namespace mpxjpp {

struct Availability final {
    DateRange range;
    double units;

//    constexpr bool operator ==(const Availability &other) {
//        return range.compareTo(other.range) == 0;
//    }
//    constexpr bool operator <(const Availability &other) {
//        return range.compareTo(other.range) < 0;
//    }
};

class AvailabilityTable final : public std::vector<Availability> {
    public:
        Availability *getEntryByDate(common::DateTime date) {
            for (auto &entry : *this) {
                const int c = entry.range.compareTo(date);
                if (c == 0)
                    return &entry;
                else if (c < 0)
                    return nullptr;
            }
            return nullptr;
        }
};

}

#endif // AVAILABILITY_H
