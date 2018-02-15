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
        /**
         * Retrieve a list of child tasks held by this object.
         *
         * @return list of child tasks
         */
        std::vector<TaskPtr> &childTasks() {
            return m_childTasks;
        }

        /**
         * Creates and adds a task to the list of tasks held by this object.
         *
         * @return newly created task
         */
        virtual TaskPtr addTask() = 0;
};

class Task final : public FieldContainer, public ProjectEntityWithID, public ChildTaskContainer {
    private:
        /**
         * This is a reference to the parent task, as specified by the
         * outline level.
         */
        Task *m_parent;

        /**
         * This list holds references to all tasks that are children of the
         * current task as specified by the outline level.
         */
        std::vector<Task> m_children;

        /**
         * List of resource assignments for this task.
         */

        std::vector<ResourceAssignment> m_assignments;

        /**
         * Recurring task details associated with this task.
         */
        RecurringTask m_recurringTask;

        bool m_eventsEnabled = true;
        bool m_null;
        std::string m_wbsLevel;
        bool m_resumeValid;
        std::string m_externalTaskProject;
        TimeUnit m_levelingDelayFormat;
        Duration m_actualWorkProtected;
        Duration m_actualOvertimeWorkProtected;
        bool m_expanded = true;

        std::vector<DateRange> m_splits;
        common::DateTime m_splitsComplete;
        SubProject m_subProject;
//      std::vector<FieldListener> m_listeners;
    public:
        Task(ProjectFile &file, TaskPtr parent = TaskPtr());
        Task(const Task &t) = delete;
        Task &operator =(const Task &) const = delete;

        /**
         * This method is used to automatically generate a value
         * for the WBS field of this task.
         *
         * @param parent Parent Task
         */
        void generateWBS(const TaskPtr &parent);

        virtual TaskPtr addTask() override;

        using Date = common::DateTime;

#define FIELDTYPE_CLASS TaskField
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
        MPXJPP_FIELD_GETTER_SETTER(baselineDuration, Duration, BASELINE_DURATION)
        MPXJPP_FIELD_GETTER_SETTER(baselineFinish, Date, BASELINE_FINISH)
        MPXJPP_FIELD_GETTER_SETTER(baselineStart, Date, BASELINE_START)
        MPXJPP_FIELD_GETTER_SETTER(baselineWork, Duration, BASELINE_WORK)

        MPXJPP_FIELD_GETTER_SETTER(bcwp, double, BCWP)
        MPXJPP_FIELD_GETTER_SETTER(bcws, double, BCWS)
        MPXJPP_FIELD_GETTER_SETTER(confirmed, bool, CONFIRMED)
        MPXJPP_FIELD_GETTER_SETTER(constraintDate, Date, CONSTRAINT_DATE)
        MPXJPP_FIELD_GETTER_SETTER(constraintType, ConstraintType, CONSTRAINT_TYPE)

        MPXJPP_FIELD_GETTER_SETTER(cost, double, COST)
        double cost(unsigned baselineNumber) {
            if (baselineNumber < 1 || baselineNumber > 10)
                throw std::invalid_argument("baselineNumber should be int range [1,10]");
            return getCachedValue(TaskField(TaskField::COST1 + (baselineNumber - 1))).cast<double>();
        }
        void set_cost(int baselineNumber, Date value) {
            if (baselineNumber < 1 || baselineNumber > 10)
                throw std::invalid_argument("baselineNumber should be int range [1,10]");
            set(TaskField(TaskField::COST1 + (baselineNumber - 1)), value);
        }


        MPXJPP_FIELD_GETTER_SETTER(summary, bool, SUMMARY)
#undef FIELDTYPE_CLASS

        bool operator <(const Task &x) const {
            return this->id() < x.id();
        }
};



class TaskContainer final : public ProjectEntityWithIDContainer<Task> {
protected:
    virtual int firstUniqueID() override {
        auto firstEntity = getByID(0);
        return firstEntity ? 0 : 1;
    }
public:
    TaskContainer(ProjectFile &mpx) :
        ProjectEntityWithIDContainer<Task>(mpx)
    {}

    TaskPtr add() {
        TaskPtr task ; //=  std::make_shared<Task>(m_mpx);
        ListWithCallbacks<TaskPtr>::add(task);
//      m_projectFile.getChildTasks().add(task);
        return task;
    }
};

}

#endif // TASK_H
