#ifndef DATERANGE_H
#define DATERANGE_H

#include <ctime>

#include "mpxjpp-gens.h"

#include "common/calendar.h"

namespace mpxjpp {

class DateRange final {
private:
	std::time_t m_start = 0, m_end = 0;
public:
	constexpr DateRange() = default;

	/**
	 * Constructor.
	 *
	 * @param startDate start date
	 * @param endDate end date
	 */
	constexpr DateRange(std::time_t start, std::time_t end) :
		m_start(start), m_end(end)
	{ }

	constexpr DateRange(common::Time start, common::Time end) :
		m_start(static_cast<std::time_t>(start)), m_end(static_cast<std::time_t>(end))
	{ }

	constexpr MPXJPP_GETTER(start, std::time_t)
	constexpr MPXJPP_GETTER(end, std::time_t)

	/**
	 * This method compares a target date with a date range. The method will
	 * return 0 if the date is within the range, less than zero if the date
	 * is before the range starts, and greater than zero if the date is after
	 * the range ends.
	 *
	 * @param date target date
	 * @return comparison result
	 */
	constexpr int compareTo(std::time_t date) {
		if (date < m_start)
			return -1;
		if (date < m_end)
			return 0;
		return 1;
	}

	constexpr int compareTo(const DateRange &other) {
		int result = other.m_start - this->m_start;
		if (result == 0)
			result = other.m_end - this->m_end;
		return result;
	}

	constexpr bool operator <(const DateRange &other) {
		return compareTo(other) < 0;
	}

	constexpr bool operator ==(const DateRange &other) {
		return compareTo(other) == 0;
	}

	static constexpr DateRange EMPTY_RANGE() {
		return {0, 0};
	}
};

}

namespace std {

template<>
struct hash<mpxjpp::DateRange> {
	constexpr size_t operator()(const mpxjpp::DateRange &val) const {
		const std::time_t start = val.start();
		const std::time_t end = val.end();
		return static_cast<size_t>(((int) start ^ (int) (start >> 32)) ^ ((int) end ^ (int) (end >> 32)));
	}
};

}
#endif // DATERANGE_H
