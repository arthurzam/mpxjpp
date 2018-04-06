#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>
#include <vector>

#include "recurringdata.h"
#include "subproject.h"
#include "mpxjpp-gens.h"
#include "enums.h"
#include "timeunit.h"
#include "duration.h"
#include "daterange.h"
#include "fieldtype.h"
#include "projectentity.h"
#include "taskfield.h"
#include "priority.h"
#include "relation.h"
#include "projectcalendar.h"

#include "common/uuid.h"

namespace mpxjpp {

class ResourceAssignment;

class Task;
using TaskPtr = std::shared_ptr<Task>;

/**
 * Interface implemented by classes which have child tasks.
 */
class ChildTaskContainer {
protected:
    std::vector<TaskPtr> m_childTasks;
public:
    virtual ~ChildTaskContainer() = default;
    /**
     * Retrieve a list of child tasks held by this object.
     *
     * @return list of child tasks
     */
    std::vector<TaskPtr> &childTasks() {
        return m_childTasks;
    }
    const std::vector<TaskPtr> &childTasks() const {
        return m_childTasks;
    }

    /**
     * Creates and adds a task to the list of tasks held by this object.
     *
     * @return newly created task
     */
    virtual TaskPtr addTask() = 0;
};

class Task final : public ProjectEntity, public FieldContainer, public ProjectEntityWithID, public ChildTaskContainer,
        public std::enable_shared_from_this<Task> {
public:
    using RelationList = std::vector<Relation>;

    struct FinderTask {
        const Task *ptr;

        bool operator() (const TaskPtr &p) {
            return p.get() == ptr;
        }
    };
private:
    /**
     * This is a reference to the parent task, as specified by the
     * outline level.
     */
    Task *m_parent;

    /**
     * List of resource assignments for this task.
     */
    std::vector<ResourceAssignment> m_assignments;

    /**
     * Recurring task details associated with this task.
     */
    RecurringTask m_recurringTask;

    //  bool m_eventsEnabled = true;
    bool m_null;
    std::string m_wbsLevel;
    bool m_resumeValid;
    std::string m_externalTaskProject;
    TimeUnit m_levelingDelayFormat{TimeUnit::HOURS};
    Duration m_actualWorkProtected;
    Duration m_actualOvertimeWorkProtected;
    bool m_expanded = true;

    std::vector<DateRange> m_splits;
    common::DateTime m_splitsComplete;
    SubProject m_subProject;
    //  std::vector<FieldListener> m_listeners;

public:
    Task(ProjectFile &file, Task *parent = nullptr);
    Task(const Task &t) = delete;
    Task &operator =(const Task &) const = delete;

    void generateWBS(const Task *parent);
    void generateOutlineNumber(const Task *parent);

    TaskPtr addTask() override;
    void remove();

    std::vector<ResourceAssignment> &resourceAssignments() {
        return m_assignments;
    }

    common::any &getCurrentValue(const FieldType &field) override;

    void addChildTask(const TaskPtr &child, int childOutlineLevel);
    void addChildTask(TaskPtr child);
    void addChildTaskBefore(TaskPtr child, const Task *previousSibling);
    void removeChildTask(Task *child) {
        auto iter = std::find_if(m_childTasks.cbegin(), m_childTasks.cend(), FinderTask{child});
        if (iter == m_childTasks.cend())
            return;
        m_childTasks.erase(iter);
        child->m_parent = nullptr;
        set_summary(!m_childTasks.empty());
    }
    void clearChildTasks() {
        m_childTasks.clear();
        set_summary(false);
    }

    Task *parentTask() const {
        return m_parent;
    }
    RecurringTask &recurringTask() {
        return m_recurringTask;
    }
    MPXJPP_GETTER_SETTER(null, bool)
    MPXJPP_GETTER_SETTER(wbsLevel, const std::string &)
    MPXJPP_GETTER_SETTER(resumeValid, bool)
    MPXJPP_GETTER_SETTER(externalTaskProject, const std::string &)
    MPXJPP_GETTER_SETTER(actualWorkProtected, Duration)
    MPXJPP_GETTER_SETTER(actualOvertimeWorkProtected, Duration)
    MPXJPP_GETTER_SETTER(expanded, bool)
    MPXJPP_GETTER_SETTER(splits, const std::vector<DateRange> &)
    MPXJPP_GETTER_SETTER(splitsComplete, common::DateTime)
    MPXJPP_GETTER_SETTER(subProject, const SubProject &)

    using Date = common::DateTime;

#define FIELDTYPE_CLASS TaskField
    int uniqueID() const override {
        return _field_get<int>(TaskField::UNIQUE_ID);
    }
    void set_uniqueID(int varName) override {
        _field_set<int>(TaskField::UNIQUE_ID, varName);
    }
    int id() const override {
        return _field_get<int>(TaskField::ID);
    }
    void set_id(int val) override;

    const ProjectCalendar *effectiveCalendar();
    MPXJPP_FIELD_GETTER(calendar, const ProjectCalendar *, CALENDAR)
    void set_calendar(const ProjectCalendar *calendar) {
        _field_set<const ProjectCalendar *>(TaskField::CALENDAR, calendar);
        set_calendarUniqueID(calendar == nullptr ? -1 : calendar->uniqueID());
    }

    MPXJPP_FIELD_GETTER_SETTER(notes, const std::string &, NOTES)
    MPXJPP_FIELD_GETTER_SETTER(contact, const std::string &, CONTACT)
    MPXJPP_FIELD_GETTER_SETTER(percentageComplete, double, PERCENT_COMPLETE)
    MPXJPP_FIELD_GETTER_SETTER(percentageWorkComplete, double, PERCENT_WORK_COMPLETE)

    MPXJPP_FIELD_GETTER_SETTER(actualCost, double, ACTUAL_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualDuration, Duration, ACTUAL_DURATION)
    MPXJPP_FIELD_GETTER_SETTER(actualFinish, Date, ACTUAL_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(actualStart, Date, ACTUAL_START)
    MPXJPP_FIELD_GETTER_SETTER(actualWork, Duration, ACTUAL_WORK)

    MPXJPP_FIELD_GETTER_SETTER(baselineCost, double, BASELINE_COST)
    MPXJPP_FIELD_GETTER_SETTER(baselineWork, Duration, BASELINE_WORK)
    Date completeThrough();

    MPXJPP_FIELD_GETTER_SETTER(bcwp, double, BCWP)
    MPXJPP_FIELD_GETTER_SETTER(bcws, double, BCWS)
    MPXJPP_FIELD_GETTER_SETTER(confirmed, bool, CONFIRMED)
    MPXJPP_FIELD_GETTER_SETTER(constraintDate, Date, CONSTRAINT_DATE)
    MPXJPP_FIELD_GETTER_SETTER(constraintType, ConstraintType, CONSTRAINT_TYPE)

    MPXJPP_FIELD_GETTER_SETTER(cost, double, COST)
    MPXJPP_FIELD_SELECT_GETTER_SETTER(cost, double, 1, 10, [] (unsigned pos) {
        return pos <= 3 ? TaskField::COST1 + (pos - 1) : TaskField::COST4 + (pos - 4); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(duration, Duration, 1, 10, [] (unsigned pos) {
        return pos <= 3 ? TaskField::DURATION1 + (pos - 1) : TaskField::DURATION4 + (pos - 4); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(number, double, 1, 20, [] (unsigned pos) {
        return pos <= 5 ? TaskField::NUMBER1 + (pos - 1) : TaskField::NUMBER6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(flag, bool, 1, 20, [] (unsigned pos) {
        return pos <= 10 ? TaskField::FLAG1 + (pos - 1) : TaskField::FLAG11 + (pos - 11); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(date, Date, 1, 10, [] (unsigned pos) {
        return TaskField::DATE1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(text, const std::string &, 1, 30, [] (unsigned pos) {
        return pos <= 5 ? TaskField::TEXT1 + 3 * (pos - 1) :
               pos <= 10 ? TaskField::TEXT6 + (pos - 6) : TaskField::TEXT11 + (pos - 11); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(start, Date, 1, 10, [] (unsigned pos) {
        return pos <= 6 ? TaskField::START1 + 3 * (pos - 1) : TaskField::START6 + 2 * (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(finish, Date, 1, 10, [] (unsigned pos) {
        return pos <= 6 ? TaskField::FINISH1 + 3 * (pos - 1) : TaskField::FINISH6 + 2 * (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(outlineCode, const std::string &, 1, 10, [] (unsigned pos) {
        return TaskField::OUTLINE_CODE1 + (pos - 1); })

    MPXJPP_FIELD_SETTER(costVariance, double, COST_VARIANCE)
    double costVariance() {
        const common::any &variance = getCachedValue(TaskField::COST_VARIANCE);
        if(variance.empty()) {
            const double result = this->cost() - this->baselineCost();
            _field_set<double>(TaskField::COST_VARIANCE, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(critical, bool, CRITICAL)
    bool critical();
    MPXJPP_FIELD_SETTER(cv, double, CV)
    double cv() {
        const common::any &variance = getCachedValue(TaskField::CV);
        if (variance.empty()) {
            const double result = bcwp() - acwp();
            _field_set<double>(TaskField::CV, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(sv, double, CV)
    double sv() {
        const common::any &variance = getCachedValue(TaskField::SV);
        if (variance.empty()) {
            const double result = bcwp() - bcws();
            _field_set<double>(TaskField::SV, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(durationVariance, Duration, DURATION_VARIANCE)
    Duration durationVariance();
    MPXJPP_FIELD_SETTER(finishVariance, Duration, FINISH_VARIANCE)
    Duration finishVariance();
    MPXJPP_FIELD_SETTER(workVariance, Duration, WORK_VARIANCE)
    Duration workVariance();
    MPXJPP_FIELD_SETTER(totalSlack, Duration, TOTAL_SLACK)
    Duration totalSlack();

    MPXJPP_FIELD_GETTER_SETTER(createDate, Date, CREATED)
    MPXJPP_FIELD_GETTER_SETTER(levelingDelay, Duration, LEVELING_DELAY)
    MPXJPP_FIELD_GETTER_SETTER(duration, Duration, DURATION)
    MPXJPP_FIELD_GETTER_SETTER(durationText, const std::string &, DURATION_TEXT)
    MPXJPP_FIELD_GETTER_SETTER(manualDuration, Duration, MANUAL_DURATION)
    MPXJPP_FIELD_GETTER_SETTER(earlyFinish, Date, EARLY_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(earlyStart, Date, EARLY_START)
    MPXJPP_FIELD_GETTER_SETTER(finish, Date, FINISH)
    MPXJPP_FIELD_GETTER_SETTER(finishText, const std::string &, FINISH_TEXT)
    MPXJPP_FIELD_GETTER_SETTER(fixedCost, double, FIXED_COST)
    MPXJPP_FIELD_GETTER_SETTER(freeSlack, Duration, FREE_SLACK)
    MPXJPP_FIELD_GETTER_SETTER(hideBar, bool, HIDE_BAR)
    MPXJPP_FIELD_GETTER_SETTER(lateFinish, Date, LATE_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(lateStart, Date, LATE_START)
    MPXJPP_FIELD_GETTER_SETTER(linkedFields, bool, LINKED_FIELDS)
    MPXJPP_FIELD_GETTER_SETTER(marked, bool, MARKED)
    MPXJPP_FIELD_GETTER_SETTER(milestone, bool, MILESTONE)
    MPXJPP_FIELD_GETTER_SETTER(name, const std::string &, NAME)
    MPXJPP_FIELD_GETTER_SETTER(objects, int, OBJECTS)
    MPXJPP_FIELD_GETTER_SETTER(outlineLevel, int, OUTLINE_LEVEL)
    MPXJPP_FIELD_GETTER_SETTER(outlineNumber, const std::string &, OUTLINE_NUMBER)
    MPXJPP_FIELD_GETTER_SETTER(priority, Priority, PRIORITY)
    MPXJPP_FIELD_GETTER_SETTER(project, const std::string &, PROJECT)
    MPXJPP_FIELD_GETTER_SETTER(remainingCost, double, REMAINING_COST)
    MPXJPP_FIELD_GETTER_SETTER(remainingDuration, Duration, REMAINING_DURATION)
    MPXJPP_FIELD_GETTER_SETTER(remainingWork, Duration, REMAINING_WORK)
    MPXJPP_FIELD_GETTER_SETTER(resourceGroup, const std::string &, RESOURCE_GROUP)
    MPXJPP_FIELD_GETTER_SETTER(resourceInitials, const std::string &, RESOURCE_INITIALS)
    MPXJPP_FIELD_GETTER_SETTER(resourceNames, const std::string &, RESOURCE_NAMES)
    MPXJPP_FIELD_GETTER_SETTER(resume, Date, RESUME)
    MPXJPP_FIELD_GETTER_SETTER(rollup, bool, ROLLUP)
    MPXJPP_FIELD_GETTER_SETTER(start, Date, START)
    MPXJPP_FIELD_GETTER_SETTER(startText, const std::string &, START_TEXT)
    MPXJPP_FIELD_GETTER_SETTER(startVariance, Duration, START_VARIANCE)
    MPXJPP_FIELD_GETTER_SETTER(stop, Date, STOP)
    MPXJPP_FIELD_GETTER_SETTER(subprojectName, const std::string &, SUBPROJECT_FILE)
    MPXJPP_FIELD_GETTER_SETTER(summary, bool, SUMMARY)
    MPXJPP_FIELD_GETTER_SETTER(updateNeeded, bool, UPDATE_NEEDED)
    MPXJPP_FIELD_GETTER_SETTER(wbs, const std::string &, WBS)
    MPXJPP_FIELD_GETTER_SETTER(work, Duration, WORK)

    MPXJPP_FIELD_SETTER(baselineDuration, Duration, BASELINE_DURATION)
    MPXJPP_FIELD_SETTER(baselineDurationText, const std::string &, BASELINE_DURATION)
    Duration baselineDuration() const {
        const common::any &obj = _field_get_nonempty(TaskField::BASELINE_DURATION, TaskField::BASELINE_ESTIMATED_DURATION);
        return obj.isType<Duration>() ? obj.cast<Duration>() : Duration{};
    }
    std::string baselineDurationText() const {
        const common::any &obj = _field_get_nonempty(TaskField::BASELINE_DURATION, TaskField::BASELINE_ESTIMATED_DURATION);
        return obj.isType<std::string>() ? obj.cast<std::string>() : std::string{};
    }
    MPXJPP_FIELD_SETTER(baselineFinish, Date, BASELINE_FINISH)
    MPXJPP_FIELD_SETTER(baselineFinishText, const std::string &, BASELINE_FINISH)
    Date baselineFinish() const {
        const common::any &obj = _field_get_nonempty(TaskField::BASELINE_FINISH, TaskField::BASELINE_ESTIMATED_FINISH);
        return obj.isType<Date>() ? obj.cast<Date>() : Date{};
    }
    std::string baselineFinishText() const {
        const common::any &obj = _field_get_nonempty(TaskField::BASELINE_FINISH, TaskField::BASELINE_ESTIMATED_FINISH);
        return obj.isType<std::string>() ? obj.cast<std::string>() : std::string{};
    }
    MPXJPP_FIELD_SETTER(baselineStart, Date, BASELINE_START)
    MPXJPP_FIELD_SETTER(baselineStartText, const std::string &, BASELINE_START)
    Date baselineStart() const {
        const common::any &obj = _field_get_nonempty(TaskField::BASELINE_START, TaskField::BASELINE_ESTIMATED_START);
        return obj.isType<Date>() ? obj.cast<Date>() : Date{};
    }
    std::string baselineStartText() const {
        const common::any &obj = _field_get_nonempty(TaskField::BASELINE_START, TaskField::BASELINE_ESTIMATED_START);
        return obj.isType<std::string>() ? obj.cast<std::string>() : std::string{};
    }

    MPXJPP_FIELD_GETTER_SETTER(estimated, bool, ESTIMATED)
    MPXJPP_FIELD_GETTER_SETTER(deadline, Date, DEADLINE)
    MPXJPP_FIELD_GETTER_SETTER(type, TaskType, TYPE)
    MPXJPP_FIELD_GETTER_SETTER(recurring, bool, RECURRING)
    MPXJPP_FIELD_GETTER_SETTER(overAllocated, bool, OVERALLOCATED)
    MPXJPP_FIELD_GETTER_SETTER(subprojectTaskUniqueID, int, SUBPROJECT_UNIQUE_TASK_ID)
    MPXJPP_FIELD_GETTER_SETTER(subprojectTaskID, int, SUBPROJECT_TASK_ID)
    MPXJPP_FIELD_GETTER_SETTER(subprojectTasksUniqueIDOffset, int, SUBPROJECT_TASKS_UNIQUEID_OFFSET)
    MPXJPP_FIELD_GETTER_SETTER(subprojectReadOnly, bool, SUBPROJECT_READ_ONLY)
    MPXJPP_FIELD_GETTER_SETTER(externalTask, bool, EXTERNAL_TASK)
    MPXJPP_FIELD_GETTER_SETTER(acwp, double, ACWP)
    MPXJPP_FIELD_GETTER_SETTER(ignoreResourceCalendar, bool, IGNORE_RESOURCE_CALENDAR)
    MPXJPP_FIELD_GETTER_SETTER(physicalPercentComplete, double, PHYSICAL_PERCENT_COMPLETE)
    MPXJPP_FIELD_GETTER_SETTER(earnedValueMethod, EarnedValueMethod, EARNED_VALUE_METHOD)
    MPXJPP_FIELD_GETTER_SETTER(regularWork, Duration, REGULAR_WORK)
    MPXJPP_FIELD_GETTER_SETTER(effortDriven, bool, EFFORT_DRIVEN)
    MPXJPP_FIELD_GETTER_SETTER(overtimeCost, double, OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualOvertimeCost, double, ACTUAL_OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualOvertimeWork, Duration, ACTUAL_OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(fixedCostAccrual, AccrueType, FIXED_COST_ACCRUAL)
    MPXJPP_FIELD_GETTER_SETTER(hyperlink, const std::string &, HYPERLINK)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkAddress, const std::string &, HYPERLINK_ADDRESS)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkSubAddress, const std::string &, HYPERLINK_SUBADDRESS)
    MPXJPP_FIELD_GETTER_SETTER(levelAssignments, bool, LEVEL_ASSIGNMENTS)
    MPXJPP_FIELD_GETTER_SETTER(levelingCanSplit, bool, LEVELING_CAN_SPLIT)
    MPXJPP_FIELD_GETTER_SETTER(overtimeWork, Duration, OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(preleveledStart, Date, PRELEVELED_START)
    MPXJPP_FIELD_GETTER_SETTER(preleveledFinish, Date, PRELEVELED_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(remainingOvertimeWork, Duration, REMAINING_OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(remainingOvertimeCost, double, REMAINING_OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(calendarUniqueID, int, CALENDAR_UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(startSlack, Duration, START_SLACK)
    MPXJPP_FIELD_GETTER_SETTER(finishSlack, Duration, FINISH_SLACK)
    MPXJPP_FIELD_GETTER_SETTER(summaryProgress, Date, SUMMARY_PROGRESS)
    MPXJPP_FIELD_GETTER_SETTER(guid, common::UUID, GUID)
    MPXJPP_FIELD_GETTER_SETTER(taskMode, TaskMode, TASK_MODE)
    MPXJPP_FIELD_GETTER_SETTER(active, bool, ACTIVE)
    MPXJPP_FIELD_GETTER_SETTER(baselineEstimatedDuration, Duration, BASELINE_ESTIMATED_DURATION)
    MPXJPP_FIELD_GETTER_SETTER(baselineEstimatedStart, Date, BASELINE_ESTIMATED_START)
    MPXJPP_FIELD_GETTER_SETTER(baselineEstimatedFinish, Date, BASELINE_ESTIMATED_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(baselineFixedCost, double, BASELINE_FIXED_COST)
    MPXJPP_FIELD_GETTER_SETTER(baselineFixedCostAccrual, AccrueType, BASELINE_FIXED_COST_ACCRUAL)

    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineCost, double, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_COST + 5 * (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineWork, Duration, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_WORK + 5 * (pos - 1); })

    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineEstimatedDuration, Duration, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_ESTIMATED_DURATION + 9 * (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineEstimatedFinish, Date, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_ESTIMATED_FINISH + 9 * (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineEstimatedStart, Date, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_ESTIMATED_START + 9 * (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineFixedCost, double, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_FIXED_COST + 9 * (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineFixedCostAccrual, AccrueType, 1, 10, [] (unsigned pos) {
        return TaskField::BASELINE1_FIXED_COST_ACCRUAL + 9 * (pos - 1); })

    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseCost, double, 1, 10, [] (unsigned pos) {
        return TaskField::ENTERPRISE_COST1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseDate, Date, 1, 30, [] (unsigned pos) {
        return TaskField::ENTERPRISE_DATE1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseDuration, Duration, 1, 10, [] (unsigned pos) {
        return TaskField::ENTERPRISE_DURATION1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseFlag, bool, 1, 20, [] (unsigned pos) {
        return TaskField::ENTERPRISE_FLAG1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseNumber, double, 1, 40, [] (unsigned pos) {
        return TaskField::ENTERPRISE_NUMBER1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseText, const std::string &, 1, 40, [] (unsigned pos) {
        return TaskField::ENTERPRISE_TEXT1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseCustomField, const std::string &, 1, 50, [] (unsigned pos) {
        return TaskField::ENTERPRISE_CUSTOM_FIELD1 + (pos - 1); })

    MPXJPP_FIELD_GETTER(predecessors, const RelationList &, PREDECESSORS)
    MPXJPP_FIELD_GETTER(successors, const RelationList &, SUCCESSORS)
    Relation addPredecessor(TaskPtr targetTask, RelationType type, Duration lag);
    bool removePredecessor(const Task *targetTask, RelationType type, Duration lag);
    bool isPredecessor(const Task *task) const {
        return isRelated(task, predecessors());
    }
    bool isSucessor(const Task *task) const {
        return isRelated(task, successors());
    }
#undef FIELDTYPE_CLASS

    bool operator <(const Task &x) const {
        return this->id() < x.id();
    }

    int compareTo(const Task &o) {
        return this->id() - o.id();
    }
private:
    static bool isRelated(const Task *task, const RelationList &list);
};

class TaskContainer final : public ProjectEntityWithIDContainer<Task> {
protected:
    virtual int firstUniqueID() override {
        auto firstEntity = getByID(0);
        return firstEntity ? 0 : 1;
    }

    void removed(const TaskPtr &task) override;

public:
    TaskContainer(ProjectFile &mpx) :
        ProjectEntityWithIDContainer<Task>(mpx)
    {}

    TaskPtr create();
    void updateStructure();
    void synchronizeTaskIDToHierarchy();
private:
    int synchroizeTaskIDToHierarchy(Task *parentTask, int currentID);

    // ListWithCallbacks interface
};

}

#endif // TASK_H
