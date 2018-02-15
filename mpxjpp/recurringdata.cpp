#include "recurringdata.h"

using namespace mpxjpp;

void RecurringData::getDailyDates(Calendar &calendar, std::vector<common::Date> &dates) const {
    for (size_t index = 0; index < dates.size(); ++index) {
        dates[index] = calendar;
        calendar += common::days(m_frequency);
    }
}

void RecurringData::getWeeklyDates(Calendar &calendar, std::vector<common::Date> &dates) const {
    unsigned currentDay = static_cast<unsigned>(common::weekday(calendar)); // [0-6]
    unsigned occurrenceIndex = 0;

    while (occurrenceIndex < dates.size()) {
        unsigned offset = 0;
        for (unsigned dayIndex = 0; dayIndex < 7; dayIndex++) {
            if (weeklyDay(static_cast<Day>(currentDay + 1))) {
                if (offset != 0) {
                    calendar += common::days(offset);
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
        calendar += common::days(offset);
    }
}

void RecurringData::getMonthlyRelativeDates(Calendar &calendar, std::vector<common::Date> &dates) const {
//  unsigned occurrenceIndex = 0;
//  common::DateTime startDate = static_cast<std::common::DateTime>(calendar);
//  calendar.set(Calendar.DAY_OF_MONTH, 1);

//  while (occurrenceIndex < dates.size())
//  {
//     if (m_dayNumber > 4)
//        setCalendarToLastRelativeDay(calendar);
//     else
//        setCalendarToOrdinalRelativeDay(calendar, m_dayNumber);
//     if (calendar.compareTo(startDate) > 0) {
//        dates[occurrenceIndex] = static_cast<common::DateTime>(calendar);
//        ++occurrenceIndex;
//        if (occurrenceIndex == dates.size())
//           break;
//     }
//     calendar.set(Calendar.DAY_OF_MONTH, 1);
//     calendar.add(Calendar.MONTH, frequency);
//  }
}
