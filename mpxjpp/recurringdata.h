#ifndef RECURRINGDATA_H
#define RECURRINGDATA_H

#include <ctime>
#include <vector>

#include "mpxjpp-gens.h"
#include "enums.h"
#include "duration.h"

#include "common/calendar.h"

namespace mpxjpp {

class RecurringData
{
    using Calendar = common::sys_days;
private:
    enum Flags {
        Flags_relative = 0x1,
        Flags_workingDaysOnly = 0x2,
        Flags_useEndDate = 0x4,
    };
    static constexpr int maskOfDay(Day d) {
        return 0x1 << (2 + static_cast<int>(d));
    }

    common::Date m_startDate;
    common::Date m_finishDate;
    int m_occurrences;
    RecurrenceType m_recurrenceType;
    unsigned m_frequency; // CHECK: if negative
    int m_dayNumber;
    int m_monthNumber;
    unsigned m_flags;
public:
    MPXJPP_GETTER_SETTER(startDate, common::Date)
    MPXJPP_GETTER_SETTER(finishDate, common::Date)
    MPXJPP_GETTER_SETTER(occurrences, int)
    MPXJPP_GETTER_SETTER(recurrenceType, RecurrenceType)
    MPXJPP_GETTER_SETTER(frequency, int)
    MPXJPP_GETTER_SETTER(dayNumber, int)
    MPXJPP_GETTER_SETTER(monthNumber, int)

    MPXJPP_GETTER_SETTER_BITS(useEndDate, m_flags, Flags_useEndDate)
    MPXJPP_GETTER_SETTER_BITS(workingDaysOnly, m_flags, Flags_workingDaysOnly)
    MPXJPP_GETTER_SETTER_BITS(relative, m_flags, Flags_relative)

    bool weeklyDay(Day d) const {
        return (m_flags & maskOfDay(d)) != 0;
    }

    void set_weeklyDay(Day day, bool value) {
        if (value)
            m_flags |= maskOfDay(day);
        else
            m_flags &= ~maskOfDay(day);
    }

    /**
     * Converts from a bitmap to individual day flags for a weekly recurrence,
     * using the array of masks.
     *
     * @param days bitmap
     * @param masks array of mask values
     */
    void set_weeklyDaysFromBitmap(int days, int masks[7]) {
        for(int i = 0; i < 7; i++)
            set_weeklyDay(static_cast<Day>(i + 1), (days & masks[i]) != 0);
    }

    /**
     * Retrieves the monthly or yearly relative day of the week.
     *
     * @return day of the week
     */
    Day dayOfWeek() const {
        for(Day day = Day::SUNDAY; day <= Day::SATURDAY; ++day)
            if (weeklyDay(day))
                return day;
        return Day::__UNINITIALIZED__;
    }

    /**
     * Sets the monthly or yearly relative day of the week.
     *
     * @param day day of the week
     */
    void set_dayOfWeek(Day day) {
        m_flags &= ~(0b1111111000);
        m_flags |= maskOfDay(day);
    }

    std::vector<common::Date> dates() const {
        std::vector<common::Date> dates;
        dates.resize(std::max(m_occurrences, 1));

        Calendar calendar(m_startDate);
        switch(m_recurrenceType) {
            case RecurrenceType::DAILY:
                getDailyDates(calendar, dates);
                break;
            case RecurrenceType::WEEKLY:
                getWeeklyDates(calendar, dates);
                break;
            case RecurrenceType::MONTHLY:
                getMonthlyDates(calendar, dates);
                break;
            case RecurrenceType::YEARLY:
                getYearlyDates(calendar, dates);
                break;
        }
        return dates;
    }
private:

    void getDailyDates (Calendar &calendar, std::vector<common::Date> &dates) const;
    void getWeeklyDates(Calendar &calendar, std::vector<common::Date> &dates) const;

    void getMonthlyRelativeDates(Calendar &calendar, std::vector<common::Date> &dates) const;
    void getMonthlyAbsoluteDates(Calendar &calendar, std::vector<common::Date> &dates) const;
    void getMonthlyDates(Calendar &calendar, std::vector<common::Date> &dates) const {
        if (relative())
           getMonthlyRelativeDates(calendar, dates);
        else
           getMonthlyAbsoluteDates(calendar, dates);
    }

    void getYearlyRelativeDates(Calendar &calendar, std::vector<common::Date> &dates) const;
    void getYearlyAbsoluteDates(Calendar &calendar, std::vector<common::Date> &dates) const;
    void getYearlyDates(Calendar &calendar, std::vector<common::Date> &dates) const {
        if (relative())
           getYearlyRelativeDates(calendar, dates);
        else
           getYearlyAbsoluteDates(calendar, dates);
    }

    // TODO: continue

};

class RecurringTask final : public RecurringData {
    private:
        Duration m_duration;
    public:
        MPXJPP_GETTER_SETTER(duration, Duration)
};

}

#endif // RECURRINGDATA_H
