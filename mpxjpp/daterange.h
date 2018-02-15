#ifndef DATERANGE_H
#define DATERANGE_H

#include <ctime>

#include "mpxjpp-gens.h"

#include "common/calendar.h"

namespace mpxjpp {

struct DateRange final {
    common::DateTime start;
    common::DateTime end;

    /**
     * This method compares a target date with a date range. The method will
     * return 0 if the date is within the range, less than zero if the date
     * is before the range starts, and greater than zero if the date is after
     * the range ends.
     *
     * @param date target date
     * @return comparison result
     */
    constexpr int compareTo(common::DateTime date) {
        if (date < start)
            return -1;
        if (date > end)
            return 1;
        return 0;
    }

    constexpr int compareTo(const DateRange &other) {
        int result = (start - other.start).count();
        if (result == 0)
            result = (other.end - end).count();
        return result;
    }
};
static_assert(std::is_pod<DateRange>::value, "DateRange should be POD");

}

namespace std {

template<>
struct hash<mpxjpp::DateRange> {
    constexpr size_t operator()(const mpxjpp::DateRange &val) const {
        auto start = val.start.count();
        auto end = val.end.count();
        return (static_cast<size_t>(start) * 2654435761U) ^ end;
    }
};

}
#endif // DATERANGE_H
