#ifndef COSTEATETABLE_H
#define COSTEATETABLE_H

#include <limits>
#include <vector>

#include "rate.h"
#include "common/calendar.h"
namespace mpxjpp {

class CostRateTableEntry final {
    private:
        Rate m_standardRate{0, TimeUnit::HOURS};
        Rate m_overtimeRate{0, TimeUnit::HOURS};
        TimeUnit m_standardRateFormat = TimeUnit::HOURS;
        TimeUnit m_overtimeRateFormat = TimeUnit::HOURS;
        double m_costPerUse = 0;
        common::DateTime m_endDate = common::DateTime::max();
    public:
        constexpr CostRateTableEntry() = default;
        constexpr CostRateTableEntry(Rate standardRate, TimeUnit standardRateFormat,
                           Rate overtimeRate, TimeUnit overtimeRateFormat,
                           double costPerUse, common::DateTime endDate) :
            m_standardRate(standardRate), m_overtimeRate(overtimeRate),
            m_standardRateFormat(standardRateFormat), m_overtimeRateFormat(overtimeRateFormat),
            m_costPerUse(costPerUse), m_endDate(endDate)
        {}

        constexpr MPXJPP_GETTER(standardRate, Rate)
        constexpr MPXJPP_GETTER(standardRateFormat, TimeUnit)
        constexpr MPXJPP_GETTER(overtimeRate, Rate)
        constexpr MPXJPP_GETTER(overtimeRateFormat, TimeUnit)
        constexpr MPXJPP_GETTER(costPerUse, double)
        constexpr MPXJPP_GETTER(endDate, common::DateTime)

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
        int getIndexByDate(common::DateTime date) {
            int index = 0;
            for (const auto &entry : *this) {
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
        CostRateTableEntry *getEntryByDate(common::DateTime date) {
            int index = getIndexByDate(date);
            return index != -1 ? &(this->at(index)) : nullptr;
        }
};

}

#endif // COSTEATETABLE_H
