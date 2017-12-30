#include "projectcalendar.h"

using namespace mpxjpp;

void ProjectCalendarHours::set_day(Day d) {
	if (d == Day::__UNINITIALIZED__)
		m_parentCalendar.removeHoursFromDay(*this);
	m_day = d;
	m_parentCalendar.attachHoursToDay(this);
}

constexpr DateRange ProjectCalendarWeek::DEFAULT_WORKING_MORNING;
constexpr DateRange ProjectCalendarWeek::DEFAULT_WORKING_AFTERNOON;

const ProjectCalendarHours *ProjectCalendarWeek::getHours(Day day) {
	const ProjectCalendarHours *res = getCalendarHours(day);
	if (res)
		return res;
	if (m_parent)
		return m_parent->getHours(day);
	addDefaultCalendarHours(day);
	return getCalendarHours(day);
}

void ProjectCalendar::clearWorkingDateCache() {
	m_workingDateCache.clear();
	m_startTimeCache.clear();
	m_getDateLastResult = 0;
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

// TODO: continue

std::shared_ptr<ProjectCalendar> ProjectCalendarContainer::getByName(const std::string &calendarName) const {
	for (const auto &calendar : *this) {
		const std::string &name = calendar->name();
		//name
	}
}
