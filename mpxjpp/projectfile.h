#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "customfield.h"
#include "filter.h"
#include "group.h"
#include "projectcalendar.h"
#include "projectconfig.h"
#include "projectproperties.h"
#include "resourceassignment.h"
#include "table.h"
#include "task.h"
#include "view.h"

namespace mpxjpp {

class ResourceContainer;
class EventManager;
class ResourceAssignmentContainer;

class ProjectFile final : public ChildTaskContainer
{
private:
    ProjectConfig m_config;
    ProjectProperties m_properties;
    ResourceContainer *m_resources;
    TaskContainer m_tasks;
    std::vector<TaskPtr> m_childTasks;
    ResourceAssignmentContainer *m_assignments;
    ProjectCalendarContainer m_calendars;
    TableContainer m_tables;
    FilterContainer m_filters;
    GroupContainer m_groups;
    SubProjectContainer m_subProjects;
    ViewContainer m_views;
    EventManager *m_eventManager;
    CustomFieldContainer m_customFields;
public:
    ProjectConfig &projectConfig() {
        return m_config;
    }
    TaskPtr addTask() {
        return m_tasks.add();
    }
    void renumberTaskIDs() {
        m_tasks.renumberIDs();
    }
};

}

#endif // PROJECTFILE_H
