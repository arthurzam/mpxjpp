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
	DD_MM_YY_HH_MM = 0, ///< This format represents dates in the form 25/12/98 12:56.
	DD_MM_YY, ///< This format represents dates in the form 25/05/98.
	DD_MMMMM_YYYY_HH_MM, ///< This format represents dates in the form 13 December 2002 12:56.
	DD_MMMMM_YYYY, ///< This format represents dates in the form 13 December 2002.
	DD_MMM_HH_MM, ///< This format represents dates in the form 24 Nov 12:56.
	DD_MMM_YY, ///< This format represents dates in the form 25 Aug '98.
	DD_MMMMM, ///< This format represents dates in the form 25 September.
	DD_MMM, ///< This format represents dates in the form 25 Aug.
	EEE_DD_MM_YY_HH_MM, ///< This format represents dates in the form Thu 25/05/98 12:56.
	EEE_DD_MM_YY, ///< This format represents dates in the form Wed 25/05/98.
	EEE_DD_MMM_YY, ///< This format represents dates in the form Wed 25 Mar '98.
	EEE_HH_MM, ///< This format represents dates in the form Wed 12:56.
	DD_MM, ///< This format represents dates in the form 25/5.
	DD, ///< This format represents dates in the form 23.
	HH_MM, ///< This format represents dates in the form 12:56.
	EEE_DD_MMM, ///< This format represents dates in the form Wed 23 Mar.
	EEE_DD_MM, ///< This format represents dates in the form Wed 25/5.
	EEE_DD, ///< This format represents dates in the form Wed 05.
	DD_WWW, ///< This format represents dates in the form 5/W25.
	DD_WWW_YY_HH_MM, ///< This format represents dates in the form 5/W25/98 12:56.
	DD_MM_YYYY ///< This format represents dates in the form 25/05/1998.
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

	using Date = common::DateTime;

#define FIELDTYPE_CLASS ProjectField
	MPXJPP_FIELD_GETTER_SETTER(defaultDurationUnits, TimeUnit, ProjectField::DEFAULT_DURATION_UNITS)
	MPXJPP_FIELD_GETTER_SETTER(defaultDurationIsFixed, bool, ProjectField::DEFAULT_DURATION_IS_FIXED)
	MPXJPP_FIELD_GETTER_SETTER(defaultWorkUnits, TimeUnit, ProjectField::DEFAULT_WORK_UNITS)
	MPXJPP_FIELD_GETTER_SETTER(defaultStandardRate, Rate, ProjectField::DEFAULT_STANDARD_RATE)
	MPXJPP_FIELD_GETTER_SETTER(defaultOvertimeRate, Rate, ProjectField::DEFAULT_OVERTIME_RATE)
	MPXJPP_FIELD_GETTER_SETTER(updatingTaskStatusUpdatesResourceStatus, bool, ProjectField::UPDATING_TASK_STATUS_UPDATES_RESOURCE_STATUS)
	MPXJPP_FIELD_GETTER_SETTER(splitInProgressTasks, bool, ProjectField::SPLIT_IN_PROGRESS_TASKS)
	MPXJPP_FIELD_GETTER_SETTER(dateOrder, DateOrder, ProjectField::DATE_ORDER)
	MPXJPP_FIELD_GETTER_SETTER(timeFormat, ProjectTimeFormat, ProjectField::TIME_FORMAT)
	MPXJPP_FIELD_GETTER_SETTER(defaultStartTime, Date, ProjectField::DEFAULT_START_TIME)
	MPXJPP_FIELD_GETTER_SETTER(dateSeparator, char, ProjectField::DATE_SEPARATOR)
	MPXJPP_FIELD_GETTER_SETTER(timeSeparator, char, ProjectField::TIME_SEPARATOR)
	MPXJPP_FIELD_GETTER_SETTER(amText, const std::string &, ProjectField::AM_TEXT)
	MPXJPP_FIELD_GETTER_SETTER(pmText, const std::string &, ProjectField::PM_TEXT)
	MPXJPP_FIELD_GETTER_SETTER(dateFormat, ProjectDateFormat, ProjectField::DATE_FORMAT)
	MPXJPP_FIELD_GETTER_SETTER(barTextDateFormat, ProjectDateFormat, ProjectField::BAR_TEXT_DATE_FORMAT)
	MPXJPP_FIELD_GETTER_SETTER(defaultEndTime, Date, ProjectField::DEFAULT_END_TIME)
	MPXJPP_FIELD_GETTER_SETTER(projectTitle, const std::string &, ProjectField::PROJECT_TITLE)
	MPXJPP_FIELD_GETTER_SETTER(company, const std::string &, ProjectField::COMPANY)
	MPXJPP_FIELD_GETTER_SETTER(manager, const std::string &, ProjectField::MANAGER)

	void set_defaultCalendarName(const std::string &calendarName);
	MPXJPP_FIELD_GETTER(defaultCalendarName, const std::string &, ProjectField::DEFAULT_CALENDAR_NAME)
	MPXJPP_FIELD_SETTER(startDate, Date, ProjectField::START_DATE)
	MPXJPP_FIELD_SETTER(finishDate, Date, ProjectField::FINISH_DATE)
	Date startDate() const;
	Date finishDate() const;

	MPXJPP_FIELD_GETTER_SETTER(scheduleFrom, ScheduleFrom, ProjectField::SCHEDULE_FROM)
	MPXJPP_FIELD_GETTER_SETTER(currentDate, Date, ProjectField::CURRENT_DATE)
	MPXJPP_FIELD_GETTER_SETTER(comments, const std::string &, ProjectField::COMMENTS)
	MPXJPP_FIELD_GETTER_SETTER(cost, double, ProjectField::COST)
	MPXJPP_FIELD_GETTER_SETTER(baselineCost, double, ProjectField::BASELINE_COST)
	MPXJPP_FIELD_GETTER_SETTER(actualCost, double, ProjectField::ACTUAL_COST)
	MPXJPP_FIELD_GETTER_SETTER(work, Duration, ProjectField::WORK)
	MPXJPP_FIELD_GETTER_SETTER(work2, double, ProjectField::WORK2)
	MPXJPP_FIELD_GETTER_SETTER(baselineWork, Duration, ProjectField::BASELINE_WORK)
	MPXJPP_FIELD_GETTER_SETTER(actualWork, Duration, ProjectField::ACTUAL_WORK)
	MPXJPP_FIELD_GETTER_SETTER(duration, Duration, ProjectField::DURATION)
	MPXJPP_FIELD_GETTER_SETTER(baselineDuration, Duration, ProjectField::BASELINE_DURATION)
	MPXJPP_FIELD_GETTER_SETTER(actualDuration, Duration, ProjectField::ACTUAL_DURATION)
	MPXJPP_FIELD_GETTER_SETTER(percentComplete, double, ProjectField::PERCENTAGE_COMPLETE)
	MPXJPP_FIELD_GETTER_SETTER(baselineStart, Date, ProjectField::BASELINE_START)
	MPXJPP_FIELD_GETTER_SETTER(baselineFinish, Date, ProjectField::BASELINE_FINISH)
	MPXJPP_FIELD_GETTER_SETTER(actualStart, Date, ProjectField::ACTUAL_START)
	MPXJPP_FIELD_GETTER_SETTER(actualFinish, Date, ProjectField::ACTUAL_FINISH)
	MPXJPP_FIELD_GETTER_SETTER(startVariance, Duration, ProjectField::START_VARIANCE)
	MPXJPP_FIELD_GETTER_SETTER(finishVariance, Duration, ProjectField::FINISH_VARIANCE)

	MPXJPP_FIELD_GETTER_SETTER(subject, const std::string &, ProjectField::SUBJECT)
	MPXJPP_FIELD_GETTER_SETTER(author, const std::string &, ProjectField::AUTHOR)
	MPXJPP_FIELD_GETTER_SETTER(keywords, const std::string &, ProjectField::KEYWORDS)
	MPXJPP_FIELD_GETTER(currencySymbol, const std::string &, ProjectField::CURRENCY_SYMBOL)
	void set_currencySymbol(const std::string &currencySymbol);
	MPXJPP_FIELD_GETTER_SETTER(symbolPosition, CurrencySymbolPosition, ProjectField::CURRENCY_SYMBOL_POSITION)
	MPXJPP_FIELD_GETTER_SETTER(currencyDigits, int, ProjectField::CURRENCY_DIGITS)
	MPXJPP_FIELD_GETTER_SETTER(thousandsSeparator, char, ProjectField::THOUSANDS_SEPARATOR)
	MPXJPP_FIELD_GETTER_SETTER(decimalSeparator, char, ProjectField::DECIMAL_SEPARATOR)
	MPXJPP_FIELD_GETTER_SETTER(projectExternallyEdited, bool, ProjectField::PROJECT_EXTERNALLY_EDITED)
	MPXJPP_FIELD_GETTER_SETTER(category, const std::string &, ProjectField::CATEGORY)

	MPXJPP_FIELD_GETTER_SETTER(daysPerMonth, double, ProjectField::DAYS_PER_MONTH)
	MPXJPP_FIELD_GETTER_SETTER(minutesPerDay, double, ProjectField::MINUTES_PER_DAY)
	MPXJPP_FIELD_GETTER_SETTER(minutesPerWeek, double, ProjectField::MINUTES_PER_WEEK)
	double minutesPerMonth() const {
		return minutesPerDay() * daysPerMonth();
	}
	double minutesPerYear() const {
		return minutesPerDay() * daysPerMonth() * 12;
	}
	MPXJPP_FIELD_GETTER_SETTER(fiscalYearStart, bool, ProjectField::FISCAL_YEAR_START)
	MPXJPP_FIELD_GETTER_SETTER(defaultTaskEarnedValueMethod, EarnedValueMethod, ProjectField::DEFAULT_TASK_EARNED_VALUE_METHOD)
	MPXJPP_FIELD_GETTER_SETTER(removeFileProperties, bool, ProjectField::REMOVE_FILE_PROPERTIES)
	MPXJPP_FIELD_GETTER_SETTER(moveCompletedEndsBack, bool, ProjectField::MOVE_COMPLETED_ENDS_BACK)
	MPXJPP_FIELD_GETTER_SETTER(newTasksEstimated, bool, ProjectField::NEW_TASKS_ESTIMATED)
	MPXJPP_FIELD_GETTER_SETTER(spreadActualCost, bool, ProjectField::SPREAD_ACTUAL_COST)
	MPXJPP_FIELD_GETTER_SETTER(multipleCriticalPaths, bool, ProjectField::MULTIPLE_CRITICAL_PATHS)
	MPXJPP_FIELD_GETTER_SETTER(autoAddNewResourcesAndTasks, bool, ProjectField::AUTO_ADD_NEW_RESOURCES_AND_TASKS)
	MPXJPP_FIELD_GETTER_SETTER(lastSaved, Date, ProjectField::LAST_SAVED)
	MPXJPP_FIELD_GETTER_SETTER(statusDate, Date, ProjectField::STATUS_DATE)
	MPXJPP_FIELD_GETTER_SETTER(moveRemainingStartsBack, bool, ProjectField::MOVE_REMAINING_STARTS_BACK)
	MPXJPP_FIELD_GETTER_SETTER(autolink, bool, ProjectField::AUTO_LINK)
	MPXJPP_FIELD_GETTER_SETTER(microsoftProjectServerURL, bool, ProjectField::MICROSOFT_PROJECT_SERVER_URL)
	MPXJPP_FIELD_GETTER_SETTER(honorConstraints, bool, ProjectField::HONOR_CONSTRAINTS)
	MPXJPP_FIELD_GETTER_SETTER(adminProject, bool, ProjectField::ADMIN_PROJECT)
	MPXJPP_FIELD_GETTER_SETTER(insertedProjectsLikeSummary, bool, ProjectField::INSERTED_PROJECTS_LIKE_SUMMARY)
	MPXJPP_FIELD_GETTER_SETTER(name, const std::string &, ProjectField::NAME)
	MPXJPP_FIELD_GETTER_SETTER(spreadPercentComplete, bool, ProjectField::SPREAD_PERCENT_COMPLETE)
	MPXJPP_FIELD_GETTER_SETTER(moveCompletedEndsForward, bool, ProjectField::MOVE_COMPLETED_ENDS_FORWARD)
	MPXJPP_FIELD_GETTER_SETTER(editableActualCosts, bool, ProjectField::EDITABLE_ACTUAL_COSTS)

	MPXJPP_FIELD_GETTER_SETTER(uniqueID, const std::string &, ProjectField::UNIQUE_ID)
	MPXJPP_FIELD_GETTER_SETTER(revision, int, ProjectField::REVISION)
	MPXJPP_FIELD_GETTER_SETTER(newTasksEffortDriven, bool, ProjectField::NEW_TASKS_EFFORT_DRIVEN)
	MPXJPP_FIELD_GETTER_SETTER(moveRemainingStartsForward, bool, ProjectField::MOVE_REMAINING_STARTS_FORWARD)
	MPXJPP_FIELD_GETTER_SETTER(actualsInSync, bool, ProjectField::ACTUALS_IN_SYNC)
	MPXJPP_FIELD_GETTER_SETTER(defaultTaskType, TaskType, ProjectField::DEFAULT_TASK_TYPE)
	MPXJPP_FIELD_GETTER_SETTER(earnedValueMethod, EarnedValueMethod, ProjectField::EARNED_VALUE_METHOD)
	MPXJPP_FIELD_GETTER_SETTER(creationDate, Date, ProjectField::CREATION_DATE)
	MPXJPP_FIELD_GETTER_SETTER(defaultFixedCostAccrual, AccrueType, ProjectField::DEFAULT_FIXED_COST_ACCRUAL)
	MPXJPP_FIELD_GETTER_SETTER(extendedCreationDate, Date, ProjectField::EXTENDED_CREATION_DATE)
	MPXJPP_FIELD_GETTER_SETTER(criticalSlackLimit, int, ProjectField::CRITICAL_SLACK_LIMIT)
	MPXJPP_FIELD_GETTER_SETTER(baselineForEarnedValue, int, ProjectField::BASELINE_FOR_EARNED_VALUE)
	MPXJPP_FIELD_GETTER_SETTER(fiscalYearStartMonth, int, ProjectField::FISCAL_YEAR_START_MONTH)
	MPXJPP_FIELD_GETTER_SETTER(newTaskStartIsProjectStart, bool, ProjectField::NEW_TASK_START_IS_PROJECT_START)
	MPXJPP_FIELD_GETTER_SETTER(weekStartDay, Day, ProjectField::WEEK_START_DAY)
	MPXJPP_FIELD_GETTER_SETTER(calculateMultipleCriticalPaths, bool, ProjectField::CALCULATE_MULTIPLE_CRITICAL_PATHS)
	MPXJPP_FIELD_GETTER_SETTER(currencyCode, const std::string &, ProjectField::CURRENCY_CODE)

	MPXJPP_FIELD_GETTER_SETTER(hyperlinkBase, const std::string &, ProjectField::HYPERLINK_BASE)
	MPXJPP_FIELD_GETTER_SETTER(showProjectSummaryTask, bool, ProjectField::SHOW_PROJECT_SUMMARY_TASK)

	MPXJPP_FIELD_GETTER_SETTER(baselineDate, Date, ProjectField::BASELINE_DATE)
	Date baselineDate(unsigned baselineNumber) {
		if (baselineNumber < 1 || baselineNumber > 10)
			throw std::invalid_argument("baselineNumber should be int range [1,10]");
		return getCachedValue(ProjectField(ProjectField::BASELINE1_DATE + (baselineNumber - 1))).cast<Date>();
	}
	void set_baselineDate(int baselineNumber, Date value) {
		if (baselineNumber < 1 || baselineNumber > 10)
			throw std::invalid_argument("baselineNumber should be int range [1,10]");
		set(ProjectField(ProjectField::BASELINE1_DATE + (baselineNumber - 1)), value);
	}

	MPXJPP_FIELD_GETTER_SETTER(fileTemplate, const std::string &, ProjectField::TEMPLATE)
	MPXJPP_FIELD_GETTER_SETTER(lastAuthor, const std::string &, ProjectField::LAST_AUTHOR)
	MPXJPP_FIELD_GETTER_SETTER(lastPrinted, Date, ProjectField::LAST_PRINTED)
	MPXJPP_FIELD_GETTER_SETTER(shortApplicationName, const std::string &, ProjectField::SHORT_APPLICATION_NAME)
	MPXJPP_FIELD_GETTER_SETTER(editingTime, int, ProjectField::EDITING_TIME)
	MPXJPP_FIELD_GETTER_SETTER(presentationFormat, const std::string &, ProjectField::PRESENTATION_FORMAT)
	MPXJPP_FIELD_GETTER_SETTER(contentType, const std::string &, ProjectField::CONTENT_TYPE)
	MPXJPP_FIELD_GETTER_SETTER(contentStatus, const std::string &, ProjectField::CONTENT_STATUS)
	MPXJPP_FIELD_GETTER_SETTER(language, const std::string &, ProjectField::LANGUAGE)
	MPXJPP_FIELD_GETTER_SETTER(documentVersion, const std::string &, ProjectField::DOCUMENT_VERSION)
	MPXJPP_FIELD_GETTER_SETTER(mpxDelimiter, char, ProjectField::MPX_DELIMITER)
	MPXJPP_FIELD_GETTER_SETTER(mpxProgramName, const std::string &, ProjectField::MPX_PROGRAM_NAME)
	MPXJPP_FIELD_GETTER_SETTER(mpxFileVersion, FileVersion, ProjectField::MPX_FILE_VERSION)
	// TODO: mpxCodePage field - [#2450 .. #2468]
	// MPXJPP_FIELD_GETTER_SETTER(mpxCodePage, CodePage, ProjectField::MPX_CODE_PAGE, CodePage)
	MPXJPP_FIELD_GETTER_SETTER(projectFilePath, const std::string &, ProjectField::PROJECT_FILE_PATH)
	MPXJPP_FIELD_GETTER_SETTER(fullApplicationName, const std::string &, ProjectField::FULL_APPLICATION_NAME)
	MPXJPP_FIELD_GETTER_SETTER(applicationVersion, int, ProjectField::APPLICATION_VERSION)
	MPXJPP_FIELD_GETTER_SETTER(mppFileType, int, ProjectField::MPP_FILE_TYPE)
	MPXJPP_FIELD_GETTER_SETTER(autoFilter, bool, ProjectField::AUTOFILTER)
	MPXJPP_FIELD_GETTER_SETTER(fileApplication, const std::string &, ProjectField::FILE_APPLICATION)
	MPXJPP_FIELD_GETTER_SETTER(fileType, const std::string &, ProjectField::FILE_TYPE)
#undef FIELDTYPE_CLASS
};

}

#endif // PROJECTPROPERTIES_H
