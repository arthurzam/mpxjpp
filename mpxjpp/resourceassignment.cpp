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

Task *ResourceAssignment::task() {
    if(!m_task)
        m_task = parentFile().getTaskByUniqueID(taskUniqueID());
    return m_task.get();
}

void ResourceAssignment::remove() {
    auto &list = parentFile().allResourceAssignments();
    list.remove(std::find_if(list.cbegin(), list.cend(), FinderAssignment{this}));
}

ResourceAssignment::Date ResourceAssignment::start() {
    const common::any &result = getCachedValue(AssignmentField::START);
    return result.empty() ? task()->start() : result.get<Date>({});
}

ResourceAssignment::Date ResourceAssignment::finish() {
    const common::any &result = getCachedValue(AssignmentField::FINISH);
    return result.empty() ? task()->finish() : result.get<Date>({});
}

void ResourceAssignmentContainer::removed(const ResourceAssignmentPtr &assignment) {
    // TODO:
//    assignment.getTask().removeResourceAssignment(assignment);
//    Resource resource = assignment.getResource();
//    if (resource != null)
//    {
//       resource.removeResourceAssignment(assignment);
//    }
}
