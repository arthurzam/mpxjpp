#ifndef RESOURCEASSIGNMENT_H
#define RESOURCEASSIGNMENT_H

#include <ctime>
#include <string>
#include <vector>

#include "costeatetable.h"
#include "assignmentfield.h"
#include "fieldtype.h"
#include "mpxjpp-gens.h"
#include "projectentity.h"
#include "timephased.h"
#include "enums.h"

#include "common/uuid.h"

namespace mpxjpp {

enum class ResourceRequestType {
    NONE = 0,
    REQUEST,
    DEMAND
};

class ProjectFile;
class Task;
class Resource;

class ResourceAssignmentWorkgroupFields final {
private:
    std::string m_messageUniqueID;
    std::string m_scheduleID;
    common::DateTime m_updateStart;
    common::DateTime m_updateFinish;
    unsigned char m_confirmed:1;
    unsigned char m_responsePending:1;
public:
    MPXJPP_GETTER_SETTER(messageUniqueID, const std::string &)
    MPXJPP_GETTER_SETTER(scheduleID, const std::string &)
    MPXJPP_GETTER_SETTER(updateStart, common::DateTime)
    MPXJPP_GETTER_SETTER(updateFinish, common::DateTime)
    MPXJPP_GETTER_SETTER(confirmed, bool)
    MPXJPP_GETTER_SETTER(responsePending, bool)
};

class ResourceAssignment final : public FieldContainer, public ProjectEntity, public ProjectEntityWithUniqueID,
        public std::enable_shared_from_this<ResourceAssignment> {
public:
    static constexpr double DEFAULT_UNITS = 100;
    struct FinderAssignment {
        const ResourceAssignment *ptr;

        bool operator() (const std::shared_ptr<ResourceAssignment> &p) {
            return p.get() == ptr;
        }
    };
private:
    std::shared_ptr<Task> m_task;

    bool m_eventsEnabled = true;
    std::unique_ptr<DefaultTimephasedWorkContainer> m_timephasedWork;
    std::vector<TimephasedCost> m_timephasedCost;

    std::unique_ptr<TimephasedWorkContainer> m_timephasedActualWork;
    std::vector<TimephasedCost> m_timephasedActualCost;

    std::unique_ptr<TimephasedWorkContainer> m_timephasedOvertimeWork;
    std::unique_ptr<TimephasedWorkContainer> m_timephasedActualOvertimeWork;

    ResourceAssignmentWorkgroupFields m_workgroup;
public:
    ResourceAssignment(ProjectFile &file, std::shared_ptr<Task> task);

    const ResourceAssignmentWorkgroupFields &workgroupAssignment() const {
        return m_workgroup;
    }

    // Resource getResource(); #464

    TimephasedWorkContainer::data_type timephasedActualWork() const {
        return (m_timephasedActualWork ? m_timephasedActualWork->data() : TimephasedWorkContainer::data_type{});
    }
    void set_timephasedActualWork(std::unique_ptr<TimephasedWorkContainer> &&data) {
        m_timephasedActualWork = std::move(data);
    }
    TimephasedWorkContainer::data_type timephasedWork() const {
        return (m_timephasedWork ? m_timephasedWork->data() : TimephasedWorkContainer::data_type{});
    }
    void set_timephasedWork(std::unique_ptr<DefaultTimephasedWorkContainer> &&data) {
        m_timephasedWork = std::move(data);
    }
    TimephasedWorkContainer::data_type timephasedOvertimeWork();


    ResourceAssignmentWorkgroupFields &workgroupAssignment() {
        return m_workgroup;
    }

    Task *task();
    std::shared_ptr<Resource> resource();
    void remove();

    CostRateTable *costRateTable();

    // getTimephasedWork(); #565
    // setTimephasedWork; #576
    // List<TimephasedWork> getTimephasedOvertimeWork(); #587

    void set_timephasedActualOvertimeWork(std::unique_ptr<TimephasedWorkContainer> &&data) {
        m_timephasedActualOvertimeWork = std::move(data);
    }

    using Date = common::DateTime;

#define FIELDTYPE_CLASS AssignmentField
    MPXJPP_FIELD_GETTER_SETTER(uniqueID, int, UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(units, double, ASSIGNMENT_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(work, Duration, WORK)
    MPXJPP_FIELD_GETTER_SETTER(baselineStart, Date, BASELINE_START)
    MPXJPP_FIELD_GETTER_SETTER(actualStart, Date, ACTUAL_START)
    MPXJPP_FIELD_GETTER_SETTER(baselineFinish, Date, BASELINE_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(actualFinish, Date, ACTUAL_FINISH)
    MPXJPP_FIELD_GETTER_SETTER(baselineWork, Duration, BASELINE_WORK)
    MPXJPP_FIELD_GETTER_SETTER(actualWork, Duration, ACTUAL_WORK)
    MPXJPP_FIELD_GETTER_SETTER(overtimeWork, Duration, OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(cost, double, COST)
    MPXJPP_FIELD_GETTER_SETTER(baselineCost, double, BASELINE_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualCost, double, ACTUAL_COST)

    MPXJPP_FIELD_SETTER(start, Date, START)
    MPXJPP_FIELD_SETTER(finish, Date, FINISH)
    Date start();
    Date finish();

    MPXJPP_FIELD_SELECT_GETTER_SETTER(text, const std::string &, 1, 30, [] (unsigned pos) {
        return pos < 11 ? AssignmentField::TEXT1 + (pos - 1) : AssignmentField::TEXT11 + (pos - 11); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(start, Date, 1, 10, [] (unsigned pos) {
        return pos < 6 ? AssignmentField::START1 + (pos - 1) : AssignmentField::START6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(finish, Date, 1, 10, [] (unsigned pos) {
        return pos < 6 ? AssignmentField::FINISH1 + (pos - 1) : AssignmentField::FINISH6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(number, double, 1, 20, [] (unsigned pos) {
        return pos < 6 ? AssignmentField::NUMBER1 + (pos - 1) : AssignmentField::NUMBER6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(duration, Duration, 1, 10, [] (unsigned pos) {
        return pos < 4 ? AssignmentField::DURATION1 + (pos - 1) : AssignmentField::DURATION4 + (pos - 4); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(cost, double, 1, 10, [] (unsigned pos) {
        return pos < 4 ? AssignmentField::COST1 + (pos - 1) : AssignmentField::COST4 + (pos - 4); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(flag, bool, 1, 20, [] (unsigned pos) {
        return pos == 10 ? AssignmentField::FLAG10 + 0 :
               pos <  10 ? AssignmentField::FLAG1 + (pos - 1) : AssignmentField::FLAG11 + (pos - 11); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(date, Date, 1, 10, [] (unsigned pos) {
        return AssignmentField::DATE1 + (pos - 1); })

    MPXJPP_FIELD_GETTER_SETTER(delay, Duration, ASSIGNMENT_DELAY)
    MPXJPP_FIELD_GETTER_SETTER(resourceUniqueID, int, RESOURCE_UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(workContour, WorkContour, WORK_CONTOUR)
    MPXJPP_FIELD_GETTER_SETTER(remainingWork, Duration, REMAINING_WORK)
    MPXJPP_FIELD_GETTER_SETTER(levelingDelay, Duration, LEVELING_DELAY)
    MPXJPP_FIELD_GETTER_SETTER(variableRateUnits, TimeUnit, VARIABLE_RATE_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(taskUniqueID, int, TASK_UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(budgetCost, double, BUDGET_COST)
    MPXJPP_FIELD_GETTER_SETTER(budgetWork, Duration, BUDGET_WORK)
    MPXJPP_FIELD_GETTER_SETTER(baselineBudgetCost, double, BASELINE_BUDGET_COST)
    MPXJPP_FIELD_GETTER_SETTER(baselineBudgetWork, Duration, BASELINE_BUDGET_WORK)
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineCost, double, 1, 10, [] (unsigned pos) {
        return AssignmentField::BASELINE1_COST + (pos - 1); })

    MPXJPP_FIELD_GETTER_SETTER(regularWork, Duration, REGULAR_WORK)
    MPXJPP_FIELD_GETTER_SETTER(actualOvertimeWork, Duration, ACTUAL_OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(remainingOvertimeWork, Duration, REMAINING_OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(remainingCost, double, REMAINING_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualOvertimeCost, double, ACTUAL_OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(remainingOvertimeCost, double, REMAINING_OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(bcwp, double, BCWP)
    MPXJPP_FIELD_GETTER_SETTER(bcws, double, BCWS)
    MPXJPP_FIELD_GETTER_SETTER(acwp, double, ACWP)

    MPXJPP_FIELD_SETTER(overtimeCost, double, OVERTIME_COST)
    double overtimeCost() {
        const common::any &variance = getCachedValue(AssignmentField::OVERTIME_COST);
        if (variance.empty()) {
            const double result = actualOvertimeCost() + remainingOvertimeCost();
            _field_set<double>(AssignmentField::OVERTIME_COST, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(sv, double, SV)
    double sv() {
        const common::any &variance = getCachedValue(AssignmentField::SV);
        if (variance.empty()) {
            const double result = bcwp() - bcws();
            _field_set<double>(AssignmentField::SV, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(cv, double, CV)
    double cv() {
        const common::any &variance = getCachedValue(AssignmentField::CV);
        if (variance.empty()) {
            const double result = bcwp() - acwp();
            _field_set<double>(AssignmentField::CV, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(costVariance, double, COST_VARIANCE)
    double costVariance() {
        const common::any &variance = getCachedValue(AssignmentField::COST_VARIANCE);
        if (variance.empty()) {
            const double result = cost() - baselineCost();
            _field_set<double>(AssignmentField::COST_VARIANCE, result);
            return result;
        }
        return variance.cast<double>();
    }
    MPXJPP_FIELD_SETTER(percentageWorkComplete, double, PERCENT_WORK_COMPLETE)
    double percentageWorkComplete();
    MPXJPP_FIELD_SETTER(workVariance, Duration, WORK_VARIANCE)
    Duration workVariance();

    MPXJPP_FIELD_GETTER_SETTER(notes, const std::string &, NOTES)
    MPXJPP_FIELD_GETTER_SETTER(confirmed, bool, CONFIRMED)
    MPXJPP_FIELD_GETTER_SETTER(updateNeeded, bool, UPDATE_NEEDED)
    MPXJPP_FIELD_GETTER_SETTER(linkedFields, bool, LINKED_FIELDS)
    MPXJPP_FIELD_GETTER_SETTER(hyperlink, const std::string &, HYPERLINK)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkAddress, const std::string &, HYPERLINK_ADDRESS)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkSubAddress, const std::string &, HYPERLINK_SUBADDRESS)
//    MPXJPP_FIELD_GETTER_SETTER(startVariance, Duration, START_VARIANCE)
//    MPXJPP_FIELD_GETTER_SETTER(finishVariance, Duration, FINISH_VARIANCE)
    MPXJPP_FIELD_GETTER_SETTER(createDate, Date, CREATED)
    MPXJPP_FIELD_GETTER_SETTER(guid, common::UUID, GUID)
    MPXJPP_FIELD_GETTER_SETTER(responsePending, bool, RESPONSE_PENDING)
    MPXJPP_FIELD_GETTER_SETTER(teamStatusPending, bool, TEAM_STATUS_PENDING)
    MPXJPP_FIELD_GETTER_SETTER(vac, double, VAC)
    MPXJPP_FIELD_GETTER_SETTER(costRateTableIndex, int, COST_RATE_TABLE)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkScreenTip, const std::string &, HYPERLINK_SCREEN_TIP)
    MPXJPP_FIELD_GETTER_SETTER(resourceRequestType, ResourceRequestType, RESOURCE_REQUEST_TYPE)
    MPXJPP_FIELD_GETTER_SETTER(stop, Date, STOP)
    MPXJPP_FIELD_GETTER_SETTER(resume, Date, RESUME)
#undef FIELDTYPE_CLASS
};

using ResourceAssignmentPtr = std::shared_ptr<ResourceAssignment>;

class ResourceAssignmentContainer final : public ProjectEntityContainer<ResourceAssignment> {
protected:
    void removed(const ResourceAssignmentPtr &assignment) override;

public:
    ResourceAssignmentContainer(ProjectFile &mpx) :
        ProjectEntityContainer<ResourceAssignment>(mpx)
    {}
};


}

#endif // RESOURCEASSIGNMENT_H
