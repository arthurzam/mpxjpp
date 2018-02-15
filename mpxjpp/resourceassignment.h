#ifndef RESOURCEASSIGNMENT_H
#define RESOURCEASSIGNMENT_H

#include <ctime>
#include <string>
#include <vector>

#include "assignmentfield.h"
#include "fieldtype.h"
#include "mpxjpp-gens.h"
#include "projectentity.h"
#include "timephased.h"
#include "enums.h"

namespace mpxjpp {

class ProjectFile;
class Task;

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

enum class ResourceType {
    MATERIAL,
    WORK,
    COST
};

class ResourceAssignment final : public ProjectEntity, public ProjectEntityWithUniqueID, public FieldContainer
{
private:
    std::shared_ptr<Task> m_task;

    bool m_eventsEnabled = true;
//  DefaultTimephasedWorkContainer m_timephasedWork;
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
    // Task getTask(); #449
    // Resource getResource(); #464
    // void remove(); #492
    // List<TimephasedWork> getTimephasedActualWork(); #543
    // void setTimephasedActualWork(TimephasedWorkContainer data); #554

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
    Date start() const;
    Date finish() const;

    MPXJPP_FIELD_GETTER_SETTER(delay, Duration, ASSIGNMENT_DELAY)
    MPXJPP_FIELD_GETTER_SETTER(resourceUniqueID, int, RESOURCE_UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(workContour, WorkContour, WORK_CONTOUR)
    MPXJPP_FIELD_GETTER_SETTER(remainingWork, Duration, REMAINING_WORK)
    MPXJPP_FIELD_GETTER_SETTER(levelingDelay, Duration, LEVELING_DELAY)

#undef FIELDTYPE_CLASS
};

}

#endif // RESOURCEASSIGNMENT_H
