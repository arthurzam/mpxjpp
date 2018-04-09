#include "projectconfig.h"
#include "projectfile.h"

#include <numeric>

using namespace mpxjpp;

void ProjectConfig::updateUniqueCounters()
{
    auto maxUniqueID = [] (int a, const auto &obj) {
        return std::max(a, obj->uniqueID());
    };
    m_taskUniqueID = std::accumulate(m_parent.allTasks().cbegin(), m_parent.allTasks().cend(),
                                     m_taskUniqueID, maxUniqueID);

    m_resourceUniqueID = std::accumulate(m_parent.allResources().cbegin(), m_parent.allResources().cend(),
                                         m_resourceUniqueID, maxUniqueID);

    m_assignmentUniqueID = std::accumulate(m_parent.allResourceAssignments().cbegin(), m_parent.allResourceAssignments().cend(),
                                           m_assignmentUniqueID, maxUniqueID);

    m_calendarUniqueID = std::accumulate(m_parent.calendars().cbegin(), m_parent.calendars().cend(),
                                         m_calendarUniqueID, maxUniqueID);
}
