#ifndef PROJECTCALENDARDATERANGES_H
#define PROJECTCALENDARDATERANGES_H

#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "projectentity.h"
#include "mpxjpp-gens.h"
#include "daterange.h"
#include "enums.h"
#include "duration.h"

#include "common/calendar.h"

namespace mpxjpp {

class Resource;
using ResourcePtr = std::shared_ptr<Resource>;

/**
 * An abstract class representing a collection of date ranges
 * with supporting methods.
 */
class ProjectCalendarDateRanges : public std::vector<DateRange> {
public:
    /**
     * Retrieve the date range at the specified index.
     * The index is zero based, and this method will return
     * null if the requested date range does not exist.
     *
     * @param index range index
     * @return date range instance
     */
    DateRange get(size_type n) const noexcept {
        if (n < size())
            return (*this)[n];
        else
            return { };
    }
};

class ProjectCalendarException final : public ProjectCalendarDateRanges { // CHECK: maybe private inheritance
    friend class ProjectCalendar;
private:
    common::DateTime m_fromDate, m_toDate;
    std::string m_name;
    RecurrenceType m_recurring;

    ProjectCalendarException(common::DateTime fromDate, common::DateTime toDate) {
        // TODO: call needed functions
       m_fromDate = fromDate; // DateHelper.getDayStartDate(fromDate);
       m_toDate = toDate; // DateHelper.getDayEndDate(toDate);
    }
public:
    MPXJPP_GETTER_SETTER(name, const std::string &)
    MPXJPP_GETTER_SETTER(recurring, RecurrenceType)
    MPXJPP_GETTER(fromDate, common::DateTime)
    MPXJPP_GETTER(toDate, common::DateTime)

    /**
     * Gets working status.
     *
     * @return boolean value
     */
    bool getWorking() {
       return (size() != 0);
    }

    /**
     * This method determines whether the given date falls in the range of
     * dates covered by this exception. Note that this method assumes that both
     * the start and end date of this exception have been set.
     *
     * @param date Date to be tested
     * @return Boolean value
     */
    bool contains(common::DateTime date) {
        return m_fromDate <= date && date <= m_toDate;
    }

    bool operator <(const ProjectCalendarException &o) {
        return m_fromDate < o.m_fromDate;
    }
};

class ProjectCalendarWeek;

class ProjectCalendarHours final : public ProjectCalendarDateRanges {
private:
    ProjectCalendarWeek &m_parentCalendar;
    Day m_day = Day::__UNINITIALIZED__;
public:
    ProjectCalendarHours(ProjectCalendarWeek &parentCalendar) :
        m_parentCalendar(parentCalendar)
    {}
    ProjectCalendarHours(const ProjectCalendarHours &) = delete;
    ProjectCalendarHours(ProjectCalendarHours &&) = default;
    ProjectCalendarHours &operator =(const ProjectCalendarHours &) = delete;
    ProjectCalendarHours &operator =(ProjectCalendarHours &&) = default;

    ProjectCalendarWeek &parentCalendar() {
        return m_parentCalendar;
    }

    MPXJPP_GETTER(day, Day)
    void set_day(Day d);
};

class ProjectCalendarWeek {
public:
    static constexpr DateRange DEFAULT_WORKING_MORNING   {common::hours( 8), common::hours(12)};
    static constexpr DateRange DEFAULT_WORKING_AFTERNOON {common::hours(13), common::hours(17)};
private:
    std::string m_name;
    DateRange m_dateRange;
    ProjectCalendarWeek *m_parent = nullptr;
    std::array<std::unique_ptr<ProjectCalendarHours>, 7> m_hours;
    std::array<DayType, 7> m_days;
public:
    ProjectCalendarWeek() {
        m_days.fill(DayType::__UNINITIALIZED__);
    }

    MPXJPP_GETTER_SETTER(name, const std::string &)
    MPXJPP_GETTER_SETTER(dateRange, DateRange)

    MPXJPP_GETTER(parent, ProjectCalendarWeek *)
    void set_parent(ProjectCalendarWeek *parent) {
        m_parent = parent;
        for(int i = 0; i < 7; i++)
            if (m_days[i] == DayType::__UNINITIALIZED__)
                m_days[i] = DayType::DEFAULT;
    }

    /**
     * Flag indicating if this week is derived from another week.
     *
     * @return true if this week is derived from another
     */
    bool isDerived() const {
        return m_parent != nullptr;
    }

    /**
     * Adds a set of hours to this calendar without assigning them to
     * a particular day.
     *
     * @return calendar hours instance
     */
    virtual std::unique_ptr<ProjectCalendarHours> addCalendarHours() {
        return std::make_unique<ProjectCalendarHours>(*this);
    }

    /**
     * Used to add working hours to the calendar. Note that the MPX file
     * definition allows a maximum of 7 calendar hours records to be added to
     * a single calendar.
     *
     * @param day day number
     * @return new ProjectCalendarHours instance
     */
    ProjectCalendarHours *addCalendarHours(Day day) {
        std::unique_ptr<ProjectCalendarHours> bch = std::make_unique<ProjectCalendarHours>(*this);
        bch->set_day(day);
        m_hours[static_cast<int>(day) - 1] = std::move(bch);
        return m_hours[static_cast<int>(day) - 1].get();
    }

    /**
     * This method retrieves the calendar hours for the specified day.
     * Note that this method only returns the hours specification for the
     * current calendar.If this is a derived calendar, it does not refer to
     * the base calendar.
     *
     * @param day Day instance
     * @return calendar hours
     */
    const ProjectCalendarHours *calendarHours(Day day) const {
        return (m_hours[static_cast<int>(day)- 1].get());
    }

    /**
     * Retrieve an array representing all of the calendar hours defined
     * by this calendar.
     *
     * @return array of calendar hours
     */
    const std::array<std::unique_ptr<ProjectCalendarHours>, 7> &hours() const {
        return m_hours;
    }

    /**
     * This method retrieves the calendar hours for the specified day.
     * Note that if this is a derived calendar, then this method
     * will refer to the base calendar where no hours are specified
     * in the derived calendar.
     *
     * @param day Day instance
     * @return calendar hours
     */
    const ProjectCalendarHours *hours(Day day);

    /**
     * This is a convenience method used to add a default set of calendar
     * hours to a calendar.
     *
     * @param day Day for which to add default hours for
     */
    void addDefaultCalendarHours(Day day) {
       ProjectCalendarHours *hours = addCalendarHours(day);

       if (day != Day::SATURDAY && day != Day::SUNDAY) {
          hours->push_back(DEFAULT_WORKING_MORNING);
          hours->push_back(DEFAULT_WORKING_AFTERNOON);
       }
    }

    /**
     * This is a convenience method used to add a default set of calendar
     * hours to a calendar.
     */
    void addDefaultCalendarHours() {
       for (int i = 1; i <= 7; i++) {
          addDefaultCalendarHours(static_cast<Day>(i));
       }
    }

    /**
     * Attaches a pre-existing set of hours to the correct
     * day within the calendar.
     *
     * @param hours calendar hours instance
     */
    virtual void attachHoursToDay(ProjectCalendarHours *hours) {
        if (&hours->parentCalendar() != this)
            throw std::invalid_argument("bad ownship");
        m_hours[static_cast<int>(hours->day()) - 1].reset(hours);
    }

    /**
     * Removes a set of calendar hours from the day to which they
     * are currently attached.
     *
     * @param hours calendar hours instance
     */
    virtual void removeHoursFromDay(ProjectCalendarHours &hours) {
        if (&hours.parentCalendar() != this)
            throw std::invalid_argument("bad ownship");
        m_hours[static_cast<int>(hours.day()) - 1].reset(nullptr);
    }

    const std::array<DayType, 7> &days() const {
        return m_days;
    }

    DayType workingDay(Day day) {
        return m_days[static_cast<int>(day) - 1];
    }

    void set_workingDay(Day day, DayType working) {
        m_days[static_cast<int>(day) - 1] =
                (working != DayType::__UNINITIALIZED__) ? working :
                isDerived() ? DayType::DEFAULT : DayType::WORKING;
    }

    void set_workingDay(Day day, bool working) {
        set_workingDay(day, (working ? DayType::WORKING : DayType::NON_WORKING));
    }

    bool operator <(const ProjectCalendarWeek &o) {
        return m_dateRange.start < o.m_dateRange.start;
    }
};

class ProjectCalendar final : public ProjectCalendarWeek {
public:
    static constexpr const char *DEFAULT_BASE_CALENDAR_NAME = "Standard";
    struct FinderCalendar {
        const ProjectCalendar *ptr;

        bool operator() (const std::shared_ptr<ProjectCalendar> &p) {
            return p.get() == ptr;
        }
    };
private:
    static constexpr int MAX_NONWORKING_DAYS = 1000;

    ProjectFile &m_projectFile;
    int m_uniqueID = 0;
    std::vector<std::unique_ptr<ProjectCalendarException>> m_exceptions;
    std::vector<std::unique_ptr<ProjectCalendarException>> m_expandedExceptions;
    bool m_exceptionsSorted;
    bool m_weeksSorted;
    ResourcePtr m_resource;
    std::vector<std::unique_ptr<ProjectCalendar>> m_derivedCalendars;

    std::map<DateRange, common::DateTime> m_workingDateCache;
    std::map<common::DateTime, common::DateTime> m_startTimeCache;
    common::DateTime m_getDateLastStartDate;
    double m_getDateLastRemainingMinutes;
    common::DateTime m_getDateLastResult;

    std::vector<std::unique_ptr<ProjectCalendarWeek>> m_workWeeks;

    int m_minutesPerDay;
    int m_minutesPerWeek;
    int m_minutesPerMonth;
    int m_minutesPerYear;
public:
    ProjectCalendar(ProjectFile &file);

    int minutesPerDay() const;
    int minutesPerWeek() const;
    int minutesPerMonth() const;
    int minutesPerYear() const;

    MPXJPP_SETTER(minutesPerDay, int)
    MPXJPP_SETTER(minutesPerWeek, int)
    MPXJPP_SETTER(minutesPerMonth, int)
    MPXJPP_SETTER(minutesPerYear, int)
    MPXJPP_GETTER_SETTER(uniqueID, int)

    ResourcePtr resource() {
        return m_resource;
    }
    void set_resource(ResourcePtr resource);

    ProjectCalendarWeek *addWorkWeek();
    void clearWorkWeeks() {
        m_workWeeks.clear();
        m_weeksSorted = false;
        clearWorkingDateCache();
    }
    MPXJPP_GETTER(workWeeks, const std::vector<std::unique_ptr<ProjectCalendarWeek>> &)

    ProjectCalendarException *addCalendarException(common::DateTime fromDate, common::DateTime toDate);
    void clearCalendarExceptions() {
        m_exceptions.clear();
        m_expandedExceptions.clear();
        m_exceptionsSorted = false;
        clearWorkingDateCache();
    }
    const std::vector<std::unique_ptr<ProjectCalendarException>> &getCalendarExceptions() {
        sortExceptions();
        return m_exceptions;
    }

    std::unique_ptr<ProjectCalendarHours> addCalendarHours() override {
        clearWorkingDateCache();
        return ProjectCalendarWeek::addCalendarHours();
    }

    void attachHoursToDay(ProjectCalendarHours *hours) override {
        clearWorkingDateCache();
        return ProjectCalendarWeek::attachHoursToDay(hours);
    }

    void removeHoursFromDay(ProjectCalendarHours &hours) override {
        clearWorkingDateCache();
        return ProjectCalendarWeek::removeHoursFromDay(hours);
    }

    ProjectCalendar *parent() const {
        return static_cast<ProjectCalendar *>(ProjectCalendarWeek::parent());
    }
    void set_parent(ProjectCalendar *calendar);

    bool isWorkingDay(Day day) const;
    Duration getDuration(common::DateTime startDate, common::DateTime endDate);


    // TODO: more functions
private:
    void clearWorkingDateCache();
    ProjectCalendarDateRanges *getRanges(common::DateTime date, common::DateTime cal, Day day);
    void sortExceptions();
    void populateExpandedExceptions();
    void sortWorkWeeks();
};
using ProjectCalendarPtr = std::shared_ptr<ProjectCalendar>;

class ProjectCalendarContainer final : public ProjectEntityContainer<ProjectCalendar> {
protected:
    virtual void removed(const ProjectCalendarPtr &cal) override;
public:
    ProjectCalendarContainer(ProjectFile &file) :
        ProjectEntityContainer<ProjectCalendar>(file)
    {}

    ProjectCalendarPtr create() {
        auto r = std::make_shared<ProjectCalendar>(m_mpx);
        add(r);
        return r;
    }

    ProjectCalendarPtr addDefaultBaseCalendar();
    ProjectCalendarPtr addDefaultDerivedCalendar();
    ProjectCalendarPtr getByName(const std::string &calendarName) const;
};

}

#endif // PROJECTCALENDARDATERANGES_H
