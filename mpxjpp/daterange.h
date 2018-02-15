#ifndef DATERANGE_H
#define DATERANGE_H

#include <ctime>

#include "mpxjpp-gens.h"

#include "common/calendar.h"

namespace mpxjpp {

class DateRange final {
    using Date = common::DateTime;
private:
    Date m_start = Date::zero(), m_end = Date::zero();
public:
    constexpr DateRange() = default;

    /**
     * Constructor.
     *
     * @param startDate start date
     * @param endDate end date
     */
    constexpr DateRange(Date start, Date end) :
        m_start(start), m_end(end)
    { }

    constexpr MPXJPP_GETTER(start, Date)
    constexpr MPXJPP_GETTER(end, Date)

    /**
     * This method compares a target date with a date range. The method will
     * return 0 if the date is within the range, less than zero if the date
     * is before the range starts, and greater than zero if the date is after
     * the range ends.
     *
     * @param date target date
     * @return comparison result
     */
    constexpr int compareTo(Date date) {
        if (date < m_start)
            return -1;
        if (date < m_end)
            return 0;
        return 1;
    }

    constexpr int compareTo(const DateRange &other) {
        int result = (m_start - other.m_start).count();
        if (result == 0)
            result = (m_end - other.m_end).count();
        return result;
    }

//  bool operator <(const DateRange &other) {
//      return compareTo(other) < 0;
//  }

//  bool operator ==(const DateRange &other) {
//      return compareTo(other) == 0;
//  }
};

}

namespace std {

template<>
struct hash<mpxjpp::DateRange> {
    constexpr size_t operator()(const mpxjpp::DateRange &val) const {
        auto start = val.start().count();
        auto end = val.end().count();
        return (static_cast<size_t>(start) * 2654435761U) ^ end;
    }
};

}
#endif // DATERANGE_H
