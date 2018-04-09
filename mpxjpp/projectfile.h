#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "customfield.h"
#include "filter.h"
#include "group.h"
#include "projectcalendar.h"
#include "projectconfig.h"
#include "projectproperties.h"
#include "resourceassignment.h"
#include "resource.h"
#include "table.h"
#include "task.h"
#include "view.h"

namespace mpxjpp {

class EventManager;

class ProjectFile final : public ChildTaskContainer
{
private:
    ProjectConfig m_config;
    ProjectProperties m_properties;
    ResourceContainer m_resources;
    TaskContainer m_tasks;
    ResourceAssignmentContainer m_assignments;
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

    TaskContainer &allTasks() {
        return m_tasks;
    }
    TaskPtr addTask() override {
        return m_tasks.create();
    }
    TaskPtr getTaskByID(int id) {
        return m_tasks.getByID(id);
    }
    TaskPtr getTaskByUniqueID(int id) {
        return m_tasks.getByUniqueID(id);
    }
    void removeTask(const Task *task) {
        auto pos = std::find_if(m_tasks.cbegin(), m_tasks.cend(), Task::FinderTask{task});
        if (pos != m_tasks.cend())
            m_tasks.remove(pos);
    }
    void renumberTaskIDs() {
        m_tasks.renumberIDs();
    }

    ResourceAssignmentContainer &allResourceAssignments() {
        return m_assignments;
    }

    ResourceContainer &allResources() {
        return m_resources;
    }
    ResourcePtr addResource() {
        return m_resources.create();
    }
    ResourcePtr getResourceByID(int id) {
        return m_resources.getByID(id);
    }
    ResourcePtr getResourceByUniqueID(int id) {
        return m_resources.getByUniqueID(id);
    }
    void removeResource(const Resource *resource) {
        auto pos = std::find_if(m_resources.cbegin(), m_resources.cend(), Resource::FinderResource{resource});
        if (pos != m_resources.cend())
            m_resources.remove(pos);
    }
    void renumberResourceIDs() {
        m_resources.renumberIDs();
    }


    ProjectCalendarContainer &calendars() {
        return m_calendars;
    }
    ProjectCalendarPtr addCalendar() {
        return m_calendars.create();
    }
    ProjectCalendarPtr addDefaultBaseCalendar() {
        return m_calendars.addDefaultBaseCalendar();
    }
    ProjectCalendarPtr addDefaultDerivedCalendar() {
        return m_calendars.addDefaultDerivedCalendar();
    }
    ProjectCalendarPtr getCalendarByName(const std::string &calendarName) const {
        return m_calendars.getByName(calendarName);
    }
    ProjectCalendarPtr getCalendarByUniqueID(int id) {
        return m_calendars.getByUniqueID(id);
    }
    void removeCalendar(const ProjectCalendar *calendar) {
        auto pos = std::find_if(m_calendars.cbegin(), m_calendars.cend(), ProjectCalendar::FinderCalendar{calendar});
        if (pos != m_calendars.cend())
            m_calendars.remove(pos);
    }
    ProjectCalendarPtr defaultCalendar();
    void set_defaultCalendar(const ProjectCalendar &calendar) {
        m_properties.set_defaultCalendarName(calendar.name());
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
    ProjectProperties &projectProperties() {
        return m_properties;
    }
    ViewContainer &views() {
        return m_views;
    }
    SubProjectContainer &subProjects() {
        return m_subProjects;
    }

    void validateUniqueIDsForMicrosoftProject() {
        m_tasks.validateUniqueIDsForMicrosoftProject();
        m_resources.validateUniqueIDsForMicrosoftProject();
        m_assignments.validateUniqueIDsForMicrosoftProject();
        m_calendars.validateUniqueIDsForMicrosoftProject();
    }
    void synchronizeTaskIDToHierarchy() {
        m_tasks.synchronizeTaskIDToHierarchy();
    }
};

}

#endif // PROJECTFILE_H
