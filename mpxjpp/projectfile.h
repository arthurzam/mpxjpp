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
    TaskPtr addTask() override {
        return m_tasks.create();
    }
    void removeTask(const Task *task) {
        auto pos = std::find_if(m_tasks.cbegin(), m_tasks.cend(), Task::FinderTask{task});
        if (pos != m_tasks.cend())
            m_tasks.remove(pos);
    }
    void renumberTaskIDs() {
        m_tasks.renumberIDs();
    }
    void renumberResourceIDs() {
        // m_resources.renumberIDs();
    }

    TaskContainer &allTasks() {
        return m_tasks;
    }

    ProjectProperties &projectProperties() {
        return m_properties;
    }

    ProjectCalendarPtr addDefaultBaseCalendar() {
        return m_calendars.addDefaultBaseCalendar();
    }
    ProjectCalendarPtr getCalendarByName(const std::string &calendarName) const {
        return m_calendars.getByName(calendarName);
    }

    TaskPtr getTaskByID(int id) {
        return m_tasks.getByID(id);
    }

    TableContainer &tables() {
        return m_tables;
    }
    FilterContainer &filters() {
        return m_filters;
    }
    GroupContainer &groups() {
        return m_groups;
    }

    ProjectCalendarPtr defaultCalendar();
};

}

#endif // PROJECTFILE_H
