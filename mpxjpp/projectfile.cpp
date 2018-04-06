#include "projectfile.h"

using namespace mpxjpp;

ProjectCalendarPtr ProjectFile::defaultCalendar() {
    ProjectCalendarPtr calendar = getCalendarByName(m_properties.defaultCalendarName());
    if(!calendar) {
        return m_calendars.empty() ? addDefaultBaseCalendar() : m_calendars[0];
    }
    return calendar;
}
