#ifndef AVAILABILITY_H
#define AVAILABILITY_H

#include <vector>

#include "mpxjpp-gens.h"
#include "daterange.h"

namespace mpxjpp {

class Availability final {
	private:
		DateRange m_range;
		double m_units;
	public:
		constexpr Availability(std::time_t startDate, std::time_t endDate, double units) :
			m_range(startDate, endDate), m_units(units)
		{}

		constexpr MPXJPP_GETTER(range, DateRange)
		constexpr MPXJPP_GETTER(units, double)

		constexpr bool operator ==(const Availability &other) {
			return m_range.compareTo(other.m_range) == 0;
		}
		constexpr bool operator <(const Availability &other) {
			return m_range.compareTo(other.m_range) < 0;
		}
};

class AvailabilityTable final : public std::vector<Availability> {
	public:
		Availability *getEntryByDate(std::time_t date) {
			for (auto &entry : *this) {
				const int c = entry.range().compareTo(date);
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