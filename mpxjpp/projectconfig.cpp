#include "projectconfig.h"
#include "projectfile.h"

#include <numeric>

using namespace mpxjpp;

void ProjectConfig::updateUniqueCounters()
{
    auto maxUniqueID = [] (int a, const auto &obj) {
        return std::max(a, obj->uniqueID());
    };
    m_taskUniqueID = std::accumulate(m_parent.get().allTasks().cbegin(), m_parent.get().allTasks().cend(),
                                     m_taskUniqueID, maxUniqueID);

    m_resourceUniqueID = std::accumulate(m_parent.get().allResources().cbegin(), m_parent.get().allResources().cend(),
                                         m_resourceUniqueID, maxUniqueID);

    m_assignmentUniqueID = std::accumulate(m_parent.get().allResourceAssignments().cbegin(), m_parent.get().allResourceAssignments().cend(),
                                           m_assignmentUniqueID, maxUniqueID);

    m_calendarUniqueID = std::accumulate(m_parent.get().calendars().cbegin(), m_parent.get().calendars().cend(),
                                         m_calendarUniqueID, maxUniqueID);
}
