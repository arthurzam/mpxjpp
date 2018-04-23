#include "resource.h"
#include "projectfile.h"

#include <numeric>

using namespace mpxjpp;

Resource::Resource(ProjectFile &file) :
    FieldContainer (ResourceField::FINISH + 1), ProjectEntity (file),
    m_eventsEnabled(true) {
    set_type(ResourceType::WORK);

    ProjectConfig &config = file.projectConfig();
    if (config.autoResourceUniqueID())
        set_uniqueID(config.getNextResourceUniqueID());
    if (config.autoResourceID())
        set_uniqueID(config.getNextResourceID());
}

common::any &Resource::getCurrentValue(const FieldType &field) {
    switch (field.value()) {
        case ResourceField::COST_VARIANCE: costVariance(); break;
        case ResourceField::WORK_VARIANCE: workVariance(); break;
        case ResourceField::CV: cv(); break;
        case ResourceField::SV: sv(); break;
        case ResourceField::OVERALLOCATED: overAllocated(); break;
    }
    return getCachedValue(field);
}

void Resource::remove() {
    parentFile().removeResource(this);
}

void Resource::removeResourceAssignment(const ResourceAssignment *assignment) {
    auto iter = std::find_if(m_assignments.cbegin(), m_assignments.cend(), ResourceAssignment::FinderAssignment{assignment});
    if (iter != m_assignments.cend())
        m_assignments.erase(iter);
}

Resource::Date Resource::start() const {
    return std::accumulate(m_assignments.cbegin(), m_assignments.cend(), Date::max(),
                           [] (Date a, const auto &assignment) { return std::min(a, assignment->start());});
}

Resource::Date Resource::finish() const {
    return std::accumulate(m_assignments.cbegin(), m_assignments.cend(), Date::min(),
                           [] (Date a, const auto &assignment) { return std::max(a, assignment->finish());});
}

void Resource::set_id(int val) {
    const common::any &previous = getCachedValue(ResourceField::ID);
    if(!previous.empty())
        parentFile().allResources().unmapID(common::any_type_cast<int>::get(previous, 0));
    parentFile().allResources().mapID(val, shared_from_this());
    _field_set<int>(ResourceField::ID, val);
}

Duration Resource::workVariance() {
    const common::any &variance = getCachedValue(ResourceField::WORK_VARIANCE);
    if(variance.empty()) {
        const Duration result = Duration::sub(work(), baselineWork(), parentFile().projectProperties());
        _field_set<Duration>(ResourceField::WORK_VARIANCE, result);
        return result;
    }
    return common::any_type_cast<Duration>::get(variance, {});
}

void ResourceContainer::removed(const ResourcePtr &resource) {
    m_uniqueIDMap.erase(m_uniqueIDMap.find(resource->uniqueID()));
    m_idMap.erase(m_idMap.find(resource->id()));

    const int resourceUniqueID = resource->uniqueID();
    auto &assignments = m_mpx.allResourceAssignments();

    for (auto iter = assignments.cbegin(), end = assignments.cend(); iter != end; ) {
        if ((*iter)->resourceUniqueID() == resourceUniqueID) {
            // (*iter)->task()->removeResourceAssignment(iter->get());
            iter = assignments.remove(iter);
        } else
            ++iter;
    }

//    ProjectCalendar calendar = resource.getResourceCalendar();
//    if (calendar != null)
//       calendar.remove();
}

ResourcePtr ResourceContainer::create() {
    auto resource = std::make_shared<Resource>(m_mpx);
    add(resource);
    return resource;
}
