#include "recurringdata.h"

using namespace mpxjpp;
using namespace mpxjpp::common;

void RecurringData::getDailyDates(Calendar &calendar, std::vector<Date> &dates) const {
    for (size_t index = 0; index < dates.size(); ++index) {
        dates[index] = calendar;
        calendar += days(m_frequency);
    }
}

void RecurringData::getWeeklyDates(Calendar &calendar, std::vector<Date> &dates) const {
    unsigned currentDay = static_cast<unsigned>(weekday(calendar)); // [0-6]
    unsigned occurrenceIndex = 0;

    while (occurrenceIndex < dates.size()) {
        unsigned offset = 0;
        for (unsigned dayIndex = 0; dayIndex < 7; dayIndex++) {
            if (weeklyDay(static_cast<Day>(currentDay + 1))) {
                if (offset != 0) {
                    calendar += days(offset);
                }
                dates[occurrenceIndex] = calendar;
                ++occurrenceIndex;
                if (occurrenceIndex == dates.size())
                   break;
            }

            ++offset;
            ++currentDay;

            if (currentDay > 6)
               currentDay = 0;
        }

        if (m_frequency > 1)
           offset += (7 * (m_frequency - 1));
        calendar += days(offset);
    }
}

void RecurringData::getMonthlyRelativeDates(Calendar &calendar, std::vector<Date> &dates) const {
    (void)calendar;
    (void)dates;
}

void RecurringData::getMonthlyAbsoluteDates(RecurringData::Calendar &calendar, std::vector<Date> &dates) const {
    // TODO: bad algorithm
    Date cal(calendar);
    const unsigned currentDayNumber = static_cast<unsigned>(cal.day());
    cal = cal.year() / cal.month() / 1_d;
    unsigned requiredDayNumber = m_dayNumber;
    if (requiredDayNumber < currentDayNumber)
        cal += months(1);

    const months diff(m_frequency);
    for (unsigned occurrenceIndex = 0; occurrenceIndex < dates.size(); ++occurrenceIndex) {
        cal = normalize(cal);
        month mon = cal.month();
        cal = normalize(cal.year() / cal.month() / day(requiredDayNumber));
        if (cal.month() != mon)
            cal = normalize(cal.year() / cal.month() / day(0));
        dates[occurrenceIndex] = cal;
        cal = cal.year() / cal.month() / 1_d;
        cal += months(m_frequency);
    }
}
