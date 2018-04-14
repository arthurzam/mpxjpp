#include "resourceassignment.h"
#include "task.h"
#include "projectfile.h"

using namespace mpxjpp;

ResourceAssignment::ResourceAssignment(ProjectFile &file, std::shared_ptr<mpxjpp::Task> task) :
    FieldContainer(AssignmentField::FINISH + 1), ProjectEntity(file),
    m_task(std::move(task)) {
    if(file.projectConfig().autoAssignmentUniqueID())
        set_uniqueID(file.projectConfig().getNextAssignmentUniqueID());
}

TimephasedWorkContainer::data_type ResourceAssignment::timephasedOvertimeWork() {
    if (!m_timephasedOvertimeWork && m_timephasedWork) {
        const Duration remainingOvertimeWork = this->remainingOvertimeWork();
        const Duration remainingWork = this->remainingWork();
        double perDayFactor = remainingOvertimeWork.duration() / (remainingWork.duration() - remainingOvertimeWork.duration());
        double totalFactor = remainingOvertimeWork.duration() / remainingWork.duration();

        perDayFactor = std::isnan(perDayFactor) ? 0 : perDayFactor;
        totalFactor = std::isnan(totalFactor) ? 0 : totalFactor;

        m_timephasedOvertimeWork = std::make_unique<DefaultTimephasedWorkContainer>(*m_timephasedWork, perDayFactor, totalFactor);
    }
    return (m_timephasedOvertimeWork ? m_timephasedOvertimeWork->data() : TimephasedWorkContainer::data_type{});
}

Task *ResourceAssignment::task() {
    if(!m_task)
        m_task = parentFile().getTaskByUniqueID(taskUniqueID());
    return m_task.get();
}

std::shared_ptr<Resource> ResourceAssignment::resource() {
    return parentFile().getResourceByUniqueID(resourceUniqueID());
}

void ResourceAssignment::remove() {
    auto &list = parentFile().allResourceAssignments();
    list.remove(std::find_if(list.cbegin(), list.cend(), FinderAssignment{this}));
}

CostRateTable *ResourceAssignment::costRateTable() {
    Resource *resource = this->resource().get();
    return (!resource ? nullptr : &resource->costRateTable(static_cast<unsigned>(costRateTableIndex())));
}

ResourceAssignment::Date ResourceAssignment::start() {
    const common::any &result = getCachedValue(AssignmentField::START);
    return result.empty() ? task()->start() : result.get<Date>({});
}

ResourceAssignment::Date ResourceAssignment::finish() {
    const common::any &result = getCachedValue(AssignmentField::FINISH);
    return result.empty() ? task()->finish() : result.get<Date>({});
}

double ResourceAssignment::percentageWorkComplete() {
    const common::any &variance = getCachedValue(AssignmentField::PERCENT_WORK_COMPLETE);
    if (variance.empty()) {
        const Duration actualWork = this->actualWork();
        const double result = (actualWork.duration() * 100) / work().convertUnits(actualWork.units(), parentFile().projectProperties()).duration();
        _field_set<double>(AssignmentField::PERCENT_WORK_COMPLETE, result);
        return result;
    }
    return variance.cast<double>();
}

Duration ResourceAssignment::workVariance() {
    const common::any &variance = getCachedValue(AssignmentField::WORK_VARIANCE);
    if(variance.empty()) {
        const Duration result = Duration::sub(work(), baselineWork(), parentFile().projectProperties());
        _field_set<Duration>(AssignmentField::WORK_VARIANCE, result);
        return result;
    }
    return variance.cast<Duration>();
}

void ResourceAssignmentContainer::removed(const ResourceAssignmentPtr &assignment) {
    assignment->task()->removeResourceAssignment(assignment.get());
    if (Resource *resource = assignment->resource().get())
       resource->removeResourceAssignment(assignment.get());
}
