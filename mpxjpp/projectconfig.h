#ifndef PROJECTCONFIG_H
#define PROJECTCONFIG_H

#include "mpxjpp-gens.h"

#include <functional>

namespace mpxjpp {

class ProjectFile;

class ProjectConfig
{
    private:
        enum Flags {
            Flags_autoWBS                = 0x1,
            Flags_autoOutlineLevel       = 0x2,
            Flags_autoOutlineNumber      = 0x4,
            Flags_autoTaskUniqueID       = 0x8,
            Flags_autoCalendarUniqueID   = 0x10,
            Flags_autoAssignmentUniqueID = 0x20,
            Flags_autoTaskID             = 0x40,
            Flags_autoResourceUniqueID   = 0x80,
            Flags_autoResourceID         = 0x100
        };

        std::reference_wrapper<ProjectFile> m_parent;
        int m_flags =
                // Flags_autoWBS is false
                Flags_autoOutlineLevel |
                Flags_autoOutlineNumber |
                Flags_autoTaskUniqueID |
                Flags_autoCalendarUniqueID |
                Flags_autoAssignmentUniqueID |
                Flags_autoTaskID |
                Flags_autoResourceUniqueID |
                Flags_autoResourceID;
        int m_taskUniqueID;
        int m_calendarUniqueID;
        int m_assignmentUniqueID;
        int m_taskID;
        int m_resourceUniqueID;
        int m_resourceID;
    public:
        ProjectConfig(ProjectFile &projectFile)
            : m_parent(projectFile)
        {}

        MPXJPP_GETTER_SETTER(taskUniqueID, int)
        MPXJPP_GETTER_SETTER(calendarUniqueID, int)
        MPXJPP_GETTER_SETTER(assignmentUniqueID, int)
        MPXJPP_GETTER_SETTER(taskID, int)
        MPXJPP_GETTER_SETTER(resourceUniqueID, int)
        MPXJPP_GETTER_SETTER(resourceID, int)

        MPXJPP_GETTER_SETTER_BITS(autoWBS, m_flags, Flags_autoWBS)
        MPXJPP_GETTER_SETTER_BITS(autoOutlineLevel, m_flags, Flags_autoOutlineLevel)
        MPXJPP_GETTER_SETTER_BITS(autoOutlineNumber, m_flags, Flags_autoOutlineNumber)
        MPXJPP_GETTER_SETTER_BITS(autoTaskUniqueID, m_flags, Flags_autoTaskUniqueID)
        MPXJPP_GETTER_SETTER_BITS(autoCalendarUniqueID, m_flags, Flags_autoCalendarUniqueID)
        MPXJPP_GETTER_SETTER_BITS(autoAssignmentUniqueID, m_flags, Flags_autoAssignmentUniqueID)
        MPXJPP_GETTER_SETTER_BITS(autoTaskID, m_flags, Flags_autoTaskID)
        MPXJPP_GETTER_SETTER_BITS(autoResourceUniqueID, m_flags, Flags_autoResourceUniqueID)
        MPXJPP_GETTER_SETTER_BITS(autoResourceID, m_flags, Flags_autoResourceID)

        int getNextTaskUniqueID() {
            return ++m_taskUniqueID;
        }

        int getNextCalendarUniqueID() {
            return ++m_calendarUniqueID;
        }

        int getNextAssignmentUniqueID() {
            return ++m_assignmentUniqueID;
        }

        int getNextTaskID() {
            return ++m_taskID;
        }

        int getNextResourceUniqueID() {
            return ++m_resourceUniqueID;
        }

        int getNextResourceID() {
            return ++m_resourceID;
        }

        void updateUniqueCounters();
};

}
#endif // PROJECTCONFIG_H
