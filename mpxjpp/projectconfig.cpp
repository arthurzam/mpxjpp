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

    m_calendarUniqueID = std::accumulate(m_parent.calendars().cbegin(), m_parent.calendars().cend(),
                                         m_calendarUniqueID, maxUniqueID);

    /* TODO:
     *  - Update resource unique IDs
     *  - Update assignment unique IDs
     */
}
