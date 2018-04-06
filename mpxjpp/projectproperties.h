#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include "duration.h"
#include "enums.h"
#include "fieldtype.h"
#include "projectentity.h"
#include "projectfield.h"
#include "rate.h"

#include "common/calendar.h"

#include <ctime>
namespace mpxjpp {

enum class ProjectTimeFormat {
    TWELVE_HOUR = 0,
    TWENTY_FOUR_HOUR = 1
};

/**
 * Instances of this class represent enumerated date format values.
 */
enum class ProjectDateFormat {
    DD_MM_YY_HH_MM = 0, ///< 25/12/98 12:56.
    DD_MM_YY, ///< 25/05/98.
    DD_MMMMM_YYYY_HH_MM, ///< 13 December 2002 12:56.
    DD_MMMMM_YYYY, ///< 13 December 2002.
    DD_MMM_HH_MM, ///< 24 Nov 12:56.
    DD_MMM_YY, ///< 25 Aug '98.
    DD_MMMMM, ///< 25 September.
    DD_MMM, ///< 25 Aug.
    EEE_DD_MM_YY_HH_MM, ///< Thu 25/05/98 12:56.
    EEE_DD_MM_YY, ///< Wed 25/05/98.
    EEE_DD_MMM_YY, ///< Wed 25 Mar '98.
    EEE_HH_MM, ///< Wed 12:56.
    DD_MM, ///< 25/5.
    DD, ///< 23.
    HH_MM, ///< 12:56.
    EEE_DD_MMM, ///< Wed 23 Mar.
    EEE_DD_MM, ///< Wed 25/5.
    EEE_DD, ///< Wed 05.
    DD_WWW, ///< 5/W25.
    DD_WWW_YY_HH_MM, ///< 5/W25/98 12:56.
    DD_MM_YYYY ///< 25/05/1998.
};

class ProjectProperties final : public ProjectEntity, public FieldContainer {
    using CustomProperties = std::vector<std::pair<std::string, common::any>>;
private:
    CustomProperties m_customProperties;
public:
    ProjectProperties(ProjectFile &mpx);

    virtual common::any &getCurrentValue(const FieldType &field) override {
        return FieldContainer::getCachedValue(field);
    }

    MPXJPP_GETTER_SETTER(customProperties, const CustomProperties &)

#define FIELDTYPE_CLASS ProjectField
    MPXJPP_FIELD_GETTER_SETTER(defaultDurationUnits, TimeUnit, DEFAULT_DURATION_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(defaultDurationIsFixed, bool, DEFAULT_DURATION_IS_FIXED)
    MPXJPP_FIELD_GETTER_SETTER(defaultWorkUnits, TimeUnit, DEFAULT_WORK_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(defaultStandardRate, Rate, DEFAULT_STANDARD_RATE)
    MPXJPP_FIELD_GETTER_SETTER(defaultOvertimeRate, Rate, DEFAULT_OVERTIME_RATE)
    MPXJPP_FIELD_GETTER_SETTER(updatingTaskStatusUpdatesResourceStatus, bool, UPDATING_TASK_STATUS_UPDATES_RESOURCE_STATUS)
    MPXJPP_FIELD_GETTER_SETTER(splitInProgressTasks, bool, SPLIT_IN_PROGRESS_TASKS)
    MPXJPP_FIELD_GETTER_SETTER(dateOrder, DateOrder, DATE_ORDER)
    MPXJPP_FIELD_GETTER_SETTER(timeFormat, ProjectTimeFormat, TIME_FORMAT)
    MPXJPP_FIELD_GETTER_SETTER(defaultStartTime, common::DateTime, DEFAULT_START_TIME)
    MPXJPP_FIELD_GETTER_SETTER(dateSeparator, char, DATE_SEPARATOR)
    MPXJPP_FIELD_GETTER_SETTER(timeSeparator, char, TIME_SEPARATOR)
    MPXJPP_FIELD_GETTER_SETTER(amText, const std::string &, AM_TEXT)
    MPXJPP_FIELD_GETTER_SETTER(pmText, const std::string &, PM_TEXT)
    MPXJPP_FIELD_GETTER_SETTER(dateFormat, ProjectDateFormat, DATE_FORMAT)
    MPXJPP_FIELD_GETTER_SETTER(barTextDateFormat, ProjectDateFormat, BAR_TEXT_DATE_FORMAT)
    MPXJPP_FIELD_GETTER_SETTER(defaultEndTime, common::DateTime, DEFAULT_END_TIME)
    MPXJPP_FIELD_GETTER_SETTER(projectTitle, const std::string &, PROJECT_TITLE)
    MPXJPP_FIELD_GETTER_SETTER(company, const std::string &, COMPANY)
    MPXJPP_FIELD_GETTER_SETTER(manager, const std::string &, MANAGER)

    void set_defaultCalendarName(const std::string &calendarName);
    MPXJPP_FIELD_GETTER(defaultCalendarName, const std::string &, DEFAULT_CALENDAR_NAME)
    MPXJPP_FIELD_SETTER(startDate, common::DateTime, START_DATE)
    MPXJPP_FIELD_SETTER(finishDate, common::DateTime, FINISH_DATE)
    common::DateTime startDate() const;
    common::DateTime finishDate() const;

    MPXJPP_FIELD_GETTER_SETTER(scheduleFrom, ScheduleFrom, SCHEDULE_FROM)
    MPXJPP_FIELD_GETTER_SETTER(currentDate, common::DateTime, CURRENT_DATE)
    MPXJPP_FIELD_GETTER_SETTER(comments, const std::string &, COMMENTS)
    MPXJPP_FIELD_GETTER_SETTER(cost, double, COST)
    MPXJPP_FIELD_GETTER_SETTER(baselineCost, double, BASELINE_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualCost, double, ACTUAL_COST)
    MPXJPP_FIELD_GETTER_SETTER(work, Duration, WORK)
    MPXJPP_FIELD_GETTER_SETTER(work2, double, WORK2)
    MPXJPP_FIELD_GETTER_SETTER(baselineWork, Duration, BASELINE_WORK)
    MPXJPP_FIELD_GETTER_SETTER(actualWork, Duration, ACTUAL_WORK)
    MPXJPP_FIELD_GETTER_SETTER(duration, Duration, DURATION)
    MPXJPP_FIELD_GETTER_SETTER(baselineDuration, Duration, BASELINE_DURATION)
    MPXJPP_FIELD_GETTER_SETTER(actualDuration, Duration, ACTUAL_DURATION)
    MPXJPP_FIELD_GETTER_SETTER(percentComplete, double, PERCENTAGE_COMPLETE)
    MPXJPP_FIELD_GETTER_SETTER(baselineStart, common::DateTime, BASELINE_START)
    MPXJPP_FIELD_GETTER_SETTER(baselineFinish, common::DateTime, BASELINE_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(actualStart, common::DateTime, ACTUAL_START)
    MPXJPP_FIELD_GETTER_SETTER(actualFinish, common::DateTime, ACTUAL_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(startVariance, Duration, START_VARIANCE)
    MPXJPP_FIELD_GETTER_SETTER(finishVariance, Duration, FINISH_VARIANCE)

    MPXJPP_FIELD_GETTER_SETTER(subject, const std::string &, SUBJECT)
    MPXJPP_FIELD_GETTER_SETTER(author, const std::string &, AUTHOR)
    MPXJPP_FIELD_GETTER_SETTER(keywords, const std::string &, KEYWORDS)
    MPXJPP_FIELD_GETTER(currencySymbol, const std::string &, CURRENCY_SYMBOL)
    void set_currencySymbol(const std::string &currencySymbol);
    MPXJPP_FIELD_GETTER_SETTER(symbolPosition, CurrencySymbolPosition, CURRENCY_SYMBOL_POSITION)
    MPXJPP_FIELD_GETTER_SETTER(currencyDigits, int, CURRENCY_DIGITS)
    MPXJPP_FIELD_GETTER_SETTER(thousandsSeparator, char, THOUSANDS_SEPARATOR)
    MPXJPP_FIELD_GETTER_SETTER(decimalSeparator, char, DECIMAL_SEPARATOR)
    MPXJPP_FIELD_GETTER_SETTER(projectExternallyEdited, bool, PROJECT_EXTERNALLY_EDITED)
    MPXJPP_FIELD_GETTER_SETTER(category, const std::string &, CATEGORY)

    MPXJPP_FIELD_GETTER_SETTER(daysPerMonth, double, DAYS_PER_MONTH)
    MPXJPP_FIELD_GETTER_SETTER(minutesPerDay, double, MINUTES_PER_DAY)
    MPXJPP_FIELD_GETTER_SETTER(minutesPerWeek, double, MINUTES_PER_WEEK)
    double minutesPerMonth() const {
        return minutesPerDay() * daysPerMonth();
    }
    double minutesPerYear() const {
        return minutesPerDay() * daysPerMonth() * 12;
    }
    MPXJPP_FIELD_GETTER_SETTER(fiscalYearStart, bool, FISCAL_YEAR_START)
    MPXJPP_FIELD_GETTER_SETTER(defaultTaskEarnedValueMethod, EarnedValueMethod, DEFAULT_TASK_EARNED_VALUE_METHOD)
    MPXJPP_FIELD_GETTER_SETTER(removeFileProperties, bool, REMOVE_FILE_PROPERTIES)
    MPXJPP_FIELD_GETTER_SETTER(moveCompletedEndsBack, bool, MOVE_COMPLETED_ENDS_BACK)
    MPXJPP_FIELD_GETTER_SETTER(newTasksEstimated, bool, NEW_TASKS_ESTIMATED)
    MPXJPP_FIELD_GETTER_SETTER(spreadActualCost, bool, SPREAD_ACTUAL_COST)
    MPXJPP_FIELD_GETTER_SETTER(multipleCriticalPaths, bool, MULTIPLE_CRITICAL_PATHS)
    MPXJPP_FIELD_GETTER_SETTER(autoAddNewResourcesAndTasks, bool, AUTO_ADD_NEW_RESOURCES_AND_TASKS)
    MPXJPP_FIELD_GETTER_SETTER(lastSaved, common::DateTime, LAST_SAVED)
    MPXJPP_FIELD_GETTER_SETTER(statusDate, common::DateTime, STATUS_DATE)
    MPXJPP_FIELD_GETTER_SETTER(moveRemainingStartsBack, bool, MOVE_REMAINING_STARTS_BACK)
    MPXJPP_FIELD_GETTER_SETTER(autolink, bool, AUTO_LINK)
    MPXJPP_FIELD_GETTER_SETTER(microsoftProjectServerURL, bool, MICROSOFT_PROJECT_SERVER_URL)
    MPXJPP_FIELD_GETTER_SETTER(honorConstraints, bool, HONOR_CONSTRAINTS)
    MPXJPP_FIELD_GETTER_SETTER(adminProject, bool, ADMIN_PROJECT)
    MPXJPP_FIELD_GETTER_SETTER(insertedProjectsLikeSummary, bool, INSERTED_PROJECTS_LIKE_SUMMARY)
    MPXJPP_FIELD_GETTER_SETTER(name, const std::string &, NAME)
    MPXJPP_FIELD_GETTER_SETTER(spreadPercentComplete, bool, SPREAD_PERCENT_COMPLETE)
    MPXJPP_FIELD_GETTER_SETTER(moveCompletedEndsForward, bool, MOVE_COMPLETED_ENDS_FORWARD)
    MPXJPP_FIELD_GETTER_SETTER(editableActualCosts, bool, EDITABLE_ACTUAL_COSTS)

    MPXJPP_FIELD_GETTER_SETTER(uniqueID, const std::string &, UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(revision, int, REVISION)
    MPXJPP_FIELD_GETTER_SETTER(newTasksEffortDriven, bool, NEW_TASKS_EFFORT_DRIVEN)
    MPXJPP_FIELD_GETTER_SETTER(moveRemainingStartsForward, bool, MOVE_REMAINING_STARTS_FORWARD)
    MPXJPP_FIELD_GETTER_SETTER(actualsInSync, bool, ACTUALS_IN_SYNC)
    MPXJPP_FIELD_GETTER_SETTER(defaultTaskType, TaskType, DEFAULT_TASK_TYPE)
    MPXJPP_FIELD_GETTER_SETTER(earnedValueMethod, EarnedValueMethod, EARNED_VALUE_METHOD)
    MPXJPP_FIELD_GETTER_SETTER(creationDate, common::DateTime, CREATION_DATE)
    MPXJPP_FIELD_GETTER_SETTER(defaultFixedCostAccrual, AccrueType, DEFAULT_FIXED_COST_ACCRUAL)
    MPXJPP_FIELD_GETTER_SETTER(extendedCreationDate, common::DateTime, EXTENDED_CREATION_DATE)
    MPXJPP_FIELD_GETTER_SETTER(criticalSlackLimit, int, CRITICAL_SLACK_LIMIT)
    MPXJPP_FIELD_GETTER_SETTER(baselineForEarnedValue, int, BASELINE_FOR_EARNED_VALUE)
    MPXJPP_FIELD_GETTER_SETTER(fiscalYearStartMonth, int, FISCAL_YEAR_START_MONTH)
    MPXJPP_FIELD_GETTER_SETTER(newTaskStartIsProjectStart, bool, NEW_TASK_START_IS_PROJECT_START)
    MPXJPP_FIELD_GETTER_SETTER(weekStartDay, Day, WEEK_START_DAY)
    MPXJPP_FIELD_GETTER_SETTER(calculateMultipleCriticalPaths, bool, CALCULATE_MULTIPLE_CRITICAL_PATHS)
    MPXJPP_FIELD_GETTER_SETTER(currencyCode, const std::string &, CURRENCY_CODE)

    MPXJPP_FIELD_GETTER_SETTER(hyperlinkBase, const std::string &, HYPERLINK_BASE)
    MPXJPP_FIELD_GETTER_SETTER(showProjectSummaryTask, bool, SHOW_PROJECT_SUMMARY_TASK)

    MPXJPP_FIELD_GETTER_SETTER(baselineDate, common::DateTime, BASELINE_DATE)
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineDate, common::DateTime, 1, 10, [] (unsigned pos) {
        return ProjectField::BASELINE1_DATE + (pos - 1); })

    MPXJPP_FIELD_GETTER_SETTER(fileTemplate, const std::string &, TEMPLATE)
    MPXJPP_FIELD_GETTER_SETTER(lastAuthor, const std::string &, LAST_AUTHOR)
    MPXJPP_FIELD_GETTER_SETTER(lastPrinted, common::DateTime, LAST_PRINTED)
    MPXJPP_FIELD_GETTER_SETTER(shortApplicationName, const std::string &, SHORT_APPLICATION_NAME)
    MPXJPP_FIELD_GETTER_SETTER(editingTime, int, EDITING_TIME)
    MPXJPP_FIELD_GETTER_SETTER(presentationFormat, const std::string &, PRESENTATION_FORMAT)
    MPXJPP_FIELD_GETTER_SETTER(contentType, const std::string &, CONTENT_TYPE)
    MPXJPP_FIELD_GETTER_SETTER(contentStatus, const std::string &, CONTENT_STATUS)
    MPXJPP_FIELD_GETTER_SETTER(language, const std::string &, LANGUAGE)
    MPXJPP_FIELD_GETTER_SETTER(documentVersion, const std::string &, DOCUMENT_VERSION)
    MPXJPP_FIELD_GETTER_SETTER(mpxDelimiter, char, MPX_DELIMITER)
    MPXJPP_FIELD_GETTER_SETTER(mpxProgramName, const std::string &, MPX_PROGRAM_NAME)
    MPXJPP_FIELD_GETTER_SETTER(mpxFileVersion, FileVersion, MPX_FILE_VERSION)
    // TODO: mpxCodePage field - [#2450 .. #2468]
    // MPXJPP_FIELD_GETTER_SETTER(mpxCodePage, CodePage, MPX_CODE_PAGE, CodePage)
    MPXJPP_FIELD_GETTER_SETTER(projectFilePath, const std::string &, PROJECT_FILE_PATH)
    MPXJPP_FIELD_GETTER_SETTER(fullApplicationName, const std::string &, FULL_APPLICATION_NAME)
    MPXJPP_FIELD_GETTER_SETTER(applicationVersion, int, APPLICATION_VERSION)
    MPXJPP_FIELD_GETTER_SETTER(mppFileType, int, MPP_FILE_TYPE)
    MPXJPP_FIELD_GETTER_SETTER(autoFilter, bool, AUTOFILTER)
    MPXJPP_FIELD_GETTER_SETTER(fileApplication, const std::string &, FILE_APPLICATION)
    MPXJPP_FIELD_GETTER_SETTER(fileType, const std::string &, FILE_TYPE)
#undef FIELDTYPE_CLASS
};

}

#endif // PROJECTPROPERTIES_H
