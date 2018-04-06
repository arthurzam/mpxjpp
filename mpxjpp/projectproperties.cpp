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
    // Configure MPX File Creation Record Settings
    set_mpxDelimiter(DEFAULT_MPX_DELIMITER);
    set_mpxProgramName("Microsoft Project for Windows");
    set_mpxFileVersion(FileVersion::VERSION_4_0);
    // TODO: setMpxCodePage(CodePage.ANSI);

    // Configure MPX Date Time Settings and Currency Settings Records
    set_currencySymbol(DEFAULT_CURRENCY_SYMBOL);
    set_symbolPosition(CurrencySymbolPosition::BEFORE);
    set_currencyDigits(2);
    set_thousandsSeparator(DEFAULT_THOUSANDS_SEPARATOR);
    set_decimalSeparator(DEFAULT_DECIMAL_SEPARATOR);

    set_dateOrder(DateOrder::DMY);
    set_timeFormat(ProjectTimeFormat::TWELVE_HOUR);
    set_defaultStartTime(common::hours(8));
    set_dateSeparator(DEFAULT_DATE_SEPARATOR);
    set_timeSeparator(DEFAULT_TIME_SEPARATOR);
    set_amText("am");
    set_pmText("pm");
    set_dateFormat(ProjectDateFormat::DD_MM_YYYY);
    set_barTextDateFormat(ProjectDateFormat::DD_MM_YYYY);

    // Configure MPX Default Settings Record
    set_defaultDurationUnits(TimeUnit::DAYS);
    set_defaultDurationIsFixed(false);
    set_defaultWorkUnits(TimeUnit::HOURS);
    set_minutesPerDay(480);
    set_minutesPerWeek(2400);
    set_defaultStandardRate(Rate{10, TimeUnit::HOURS});
    set_defaultOvertimeRate(Rate{15, TimeUnit::HOURS});
    set_updatingTaskStatusUpdatesResourceStatus(true);
    set_splitInProgressTasks(false);

    // Configure MPX Project Header Record
    set_projectTitle("Project1");
    set_company({});
    set_manager({});
    set_defaultCalendarName(DEFAULT_CALENDAR_NAME);
    set_startDate({});
    set_finishDate({});
    set_scheduleFrom(DEFAULT_SCHEDULE_FROM);
    set_currentDate({});
    set_comments({});
    set_cost(DEFAULT_COST);
    set_baselineCost(DEFAULT_COST);
    set_actualCost(DEFAULT_COST);
    set_work(DEFAULT_WORK);
    set_baselineWork(DEFAULT_WORK);
    set_actualWork(DEFAULT_WORK);
    set_work2(DEFAULT_WORK2);
    set_duration(DEFAULT_DURATION);
    set_baselineDuration(DEFAULT_DURATION);
    set_actualDuration(DEFAULT_DURATION);
    set_percentComplete(DEFAULT_PERCENT_COMPLETE);
    set_baselineStart({});
    set_baselineFinish({});
    set_actualStart({});
    set_actualFinish({});
    set_startVariance(DEFAULT_DURATION);
    set_finishVariance(DEFAULT_DURATION);
    set_subject({});
    set_author({});
    set_keywords({});

    // Configure non-MPX attributes
    set_projectExternallyEdited(false);
    set_minutesPerDay(DEFAULT_MINUTES_PER_DAY);
    set_daysPerMonth(DEFAULT_DAYS_PER_MONTH);
    set_minutesPerWeek(DEFAULT_MINUTES_PER_WEEK);
    set_fiscalYearStart(false);
    set_defaultTaskEarnedValueMethod(EarnedValueMethod::PERCENT_COMPLETE);
    set_newTasksEstimated(true);
    set_autoAddNewResourcesAndTasks(true);
    set_autolink(true);
    set_microsoftProjectServerURL(true);
    set_defaultTaskType(TaskType::FIXED_UNITS);
    set_defaultFixedCostAccrual(AccrueType::END);
    set_criticalSlackLimit(DEFAULT_CRITICAL_SLACK_LIMIT);
    set_baselineForEarnedValue(DEFAULT_BASELINE_FOR_EARNED_VALUE);
    set_fiscalYearStartMonth(DEFAULT_FISCAL_YEAR_START_MONTH);
    set_newTaskStartIsProjectStart(true);
    set_weekStartDay(DEFAULT_WEEK_START_DAY);
}

void ProjectProperties::set_defaultCalendarName(const std::string &calendarName) {
    _field_set<std::string>(ProjectField::DEFAULT_CALENDAR_NAME, calendarName.empty() ? DEFAULT_CALENDAR_NAME : calendarName);
}

void ProjectProperties::set_currencySymbol(const std::string &currencySymbol) {
    _field_set<std::string>(ProjectField::CURRENCY_SYMBOL, currencySymbol.empty() ? DEFAULT_CURRENCY_SYMBOL : currencySymbol);
}
