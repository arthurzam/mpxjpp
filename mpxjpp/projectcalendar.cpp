#include "projectcalendar.h"
#include "projectproperties.h"
#include "projectfile.h"

#include "common/strutils.h"

using namespace mpxjpp;
namespace strutils = mpxjpp::common::strutils;

void ProjectCalendarHours::set_day(Day d) {
    if (d == Day::__UNINITIALIZED__)
        m_parentCalendar.removeHoursFromDay(*this);
    m_day = d;
    m_parentCalendar.attachHoursToDay(this);
}

constexpr DateRange ProjectCalendarWeek::DEFAULT_WORKING_MORNING;
constexpr DateRange ProjectCalendarWeek::DEFAULT_WORKING_AFTERNOON;

const ProjectCalendarHours *ProjectCalendarWeek::hours(Day day) {
    const ProjectCalendarHours *res = calendarHours(day);
    if (res)
        return res;
    if (m_parent)
        return m_parent->hours(day);
    addDefaultCalendarHours(day);
    return calendarHours(day);
}

ProjectCalendar::ProjectCalendar(ProjectFile &file) :
    m_projectFile(file) {
    if (m_projectFile.projectConfig().autoCalendarUniqueID())
        set_uniqueID(m_projectFile.projectConfig().getNextCalendarUniqueID());
}

void ProjectCalendar::set_parent(ProjectCalendar *calendar) {
    if (calendar != this) {
//      if (ProjectCalendarWeek *par = ProjectCalendarWeek::parent())
//          static_cast<ProjectCalendar *>(par)->removeDerivedCalendar(this);
//      ProjectCalendarWeek::set_parent(calendar);
//      if (calendar != nullptr)
//          calendar->addDerivedCalendar(this);
//      clearWorkingDateCache();
    }
}

bool ProjectCalendar::isWorkingDay(Day day) const {
    // TODO:
}

void ProjectCalendar::clearWorkingDateCache() {
    m_workingDateCache.clear();
    m_startTimeCache.clear();
    m_getDateLastResult = common::DateTime::zero();
    for (auto &iter : m_derivedCalendars)
        iter->clearWorkingDateCache();
}

template<typename T>
static bool cmp_unique_ptr(const std::unique_ptr<T>& a, const std::unique_ptr<T>& b) {
    return *a < *b;
}

void ProjectCalendar::sortExceptions() {
    if (!m_exceptionsSorted) {
        std::sort(m_exceptions.begin(), m_exceptions.end(), cmp_unique_ptr<ProjectCalendarException>); // CHECK: is using the right compare
        m_exceptionsSorted = true;
    }
}

void ProjectCalendar::sortWorkWeeks() {
    if (!m_weeksSorted) {
        std::sort(m_workWeeks.begin(), m_workWeeks.end(), cmp_unique_ptr<ProjectCalendarWeek>); // CHECK: is using the right compare
        m_weeksSorted = true;
    }
}

void ProjectCalendarContainer::removed(const ProjectCalendarPtr &cal) {
//  Resource resource = cal->getResource();
//  if (resource != null)
//     resource.setResourceCalendar(null);
    cal->set_parent(nullptr);
}

ProjectCalendarPtr ProjectCalendarContainer::addDefaultBaseCalendar() {
    auto cal = create();
    cal->set_name(ProjectCalendar::DEFAULT_BASE_CALENDAR_NAME);
    cal->addDefaultCalendarHours();

    cal->set_workingDay(Day::SUNDAY, false);
    cal->set_workingDay(Day::MONDAY, true);
    cal->set_workingDay(Day::TUESDAY, true);
    cal->set_workingDay(Day::WEDNESDAY, true);
    cal->set_workingDay(Day::THURSDAY, true);
    cal->set_workingDay(Day::FRIDAY, true);
    cal->set_workingDay(Day::SATURDAY, false);
    return (cal);
}

ProjectCalendarPtr ProjectCalendarContainer::addDefaultDerivedCalendar() {
    auto cal = create();
    cal->set_name(ProjectCalendar::DEFAULT_BASE_CALENDAR_NAME);
    cal->set_workingDay(Day::SUNDAY, DayType::DEFAULT);
    cal->set_workingDay(Day::MONDAY, DayType::DEFAULT);
    cal->set_workingDay(Day::TUESDAY, DayType::DEFAULT);
    cal->set_workingDay(Day::WEDNESDAY, DayType::DEFAULT);
    cal->set_workingDay(Day::THURSDAY, DayType::DEFAULT);
    cal->set_workingDay(Day::FRIDAY, DayType::DEFAULT);
    cal->set_workingDay(Day::SATURDAY, DayType::DEFAULT);
    return (cal);
}

ProjectCalendarPtr ProjectCalendarContainer::getByName(const std::string &calendarName) const {
    if (!calendarName.empty()) {
        const std::string calName = strutils::str_toupper(calendarName);
        for (const auto &calendar : *this) {
            const std::string &name = calendar->name();
            if (!name.empty() && strutils::str_toupper(name) == calName)
                return calendar;
        }
    }
    return std::shared_ptr<ProjectCalendar>();
}

// TODO: continue
