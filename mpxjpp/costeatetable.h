#ifndef COSTEATETABLE_H
#define COSTEATETABLE_H

#include <ctime>
#include <limits>
#include <vector>

#include "rate.h"

namespace mpxjpp {

class CostRateTableEntry final {
	private:
		Rate m_standardRate = Rate(0, TimeUnit::HOURS);
		TimeUnit m_standardRateFormat = TimeUnit::HOURS;
		Rate m_overtimeRate = Rate(0, TimeUnit::HOURS);
		TimeUnit m_overtimeRateFormat = TimeUnit::HOURS;
		double m_costPerUse = 0;
		std::time_t m_endDate = std::numeric_limits<std::time_t>::max();
	public:
		constexpr CostRateTableEntry() = default;
		constexpr CostRateTableEntry(Rate standardRate, TimeUnit standardRateFormat,
						   Rate overtimeRate, TimeUnit overtimeRateFormat,
						   double costPerUse, std::time_t endDate) :
			m_standardRate(standardRate), m_standardRateFormat(standardRateFormat),
			m_overtimeRate(overtimeRate), m_overtimeRateFormat(overtimeRateFormat),
			m_costPerUse(costPerUse), m_endDate(endDate)
		{}

		constexpr MPXJPP_GETTER(standardRate, Rate)
		constexpr MPXJPP_GETTER(standardRateFormat, TimeUnit)
		constexpr MPXJPP_GETTER(overtimeRate, Rate)
		constexpr MPXJPP_GETTER(overtimeRateFormat, TimeUnit)
		constexpr MPXJPP_GETTER(costPerUse, double)
		constexpr MPXJPP_GETTER(endDate, std::time_t)

		constexpr bool operator ==(const CostRateTableEntry &other) {
			return m_endDate == other.m_endDate;
		}
		constexpr bool operator <(const CostRateTableEntry &other) {
			return m_endDate < other.m_endDate;
		}
};

/**
 * This class represents a resource's cost rate table.
 */
class CostRateTable final : public std::vector<CostRateTableEntry> {
	public:
		/**
		 * Retrieve the index of the table entry valid for the supplied date.
		 *
		 * @param date required date
		 * @return cost rate table entry index
		 */
		int getIndexByDate(std::time_t date) {
			int index = 0;
			for (const auto entry : *this) {
				if (date < entry.endDate())
					return index;
				index++;
			}
			return -1;
		}

		/**
		 * Retrieve the table entry valid for the supplied date.
		 *
		 * @param date required date
		 * @return cost rate table entry
		 */
		CostRateTableEntry *getEntryByDate(std::time_t date) {
			int index = getIndexByDate(date);
			return index != -1 ? &(this->at(index)) : nullptr;
		}
};

}

#endif // COSTEATETABLE_H
