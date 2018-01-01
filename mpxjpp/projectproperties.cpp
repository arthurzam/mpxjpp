#include "projectproperties.h"

using namespace mpxjpp;

static constexpr char DEFAULT_TIME_SEPARATOR = ':'; ///< Default time separator character.
static constexpr char DEFAULT_DATE_SEPARATOR = '/'; ///< Default date separator character.
static constexpr char DEFAULT_THOUSANDS_SEPARATOR = ','; ///< Default thousands separator character.
static constexpr char DEFAULT_DECIMAL_SEPARATOR = '.'; ///< Default decimal separator character.
static constexpr const char *DEFAULT_CURRENCY_SYMBOL = "$"; ///< Default currency symbol.
static constexpr double DEFAULT_COST = 0.0; ///< Default cost value.
static constexpr char DEFAULT_MPX_DELIMITER = ','; ///< Default MPX delimiter.
static constexpr int DEFAULT_CRITICAL_SLACK_LIMIT = 0; ///< Default critical slack limit.
static constexpr int DEFAULT_BASELINE_FOR_EARNED_VALUE = 0; ///< Default baseline for earned value.
static constexpr int DEFAULT_FISCAL_YEAR_START_MONTH = 1; ///< Default fiscal year start month.
static constexpr Day DEFAULT_WEEK_START_DAY = Day::MONDAY; ///< Default week start day.
static constexpr Duration DEFAULT_WORK = Duration(0, TimeUnit::HOURS); ///< Default work value.
static constexpr double DEFAULT_WORK2 = 0.0; ///< Default work 2 value.
static constexpr Duration DEFAULT_DURATION = Duration(0, TimeUnit::DAYS); ///< Default duration value.
static constexpr ScheduleFrom DEFAULT_SCHEDULE_FROM = ScheduleFrom::START; ///< Default schedule from value.
static constexpr double DEFAULT_PERCENT_COMPLETE = 0; ///< Default percent complete value.
static constexpr const char *DEFAULT_CALENDAR_NAME = "Standard"; ///< Default calendar name.
static constexpr int DEFAULT_MINUTES_PER_DAY = 480; ///< Default minutes per day.
static constexpr int DEFAULT_DAYS_PER_MONTH = 20; ///< Default days per month.
static constexpr int DEFAULT_MINUTES_PER_WEEK = 2400; ///< Default minutes per week.

ProjectProperties::ProjectProperties(ProjectFile &mpx) :
	ProjectEntity(mpx), FieldContainer(ProjectField::FINISH_DATE + 1) {

}

void ProjectProperties::set_defaultCalendarName(const std::string &calendarName) {
	set(ProjectField(ProjectField::DEFAULT_CALENDAR_NAME),
		std::move(!calendarName.empty() ? common::any(calendarName) : common::any(DEFAULT_CALENDAR_NAME)));
}

void ProjectProperties::set_currencySymbol(const std::string &currencySymbol) {
	set(ProjectField(ProjectField::CURRENCY_SYMBOL), currencySymbol.empty() ? DEFAULT_CURRENCY_SYMBOL : currencySymbol);
}
