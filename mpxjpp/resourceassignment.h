#ifndef RESOURCEASSIGNMENT_H
#define RESOURCEASSIGNMENT_H

#include <ctime>
#include <string>
#include <vector>

#include "assignmentfield.h"
#include "fieldtype.h"
#include "mpxjpp-gens.h"
#include "projectentity.h"
#include "timephased.h"

namespace mpxjpp {

class ProjectFile;
class Task;

class ResourceAssignmentWorkgroupFields final {
private:
	std::string m_messageUniqueID;
	std::string m_scheduleID;
	std::time_t m_updateStart;
	std::time_t m_updateFinish;
	unsigned char m_confirmed:1;
	unsigned char m_responsePending:1;
public:
	MPXJPP_GETTER_SETTER(messageUniqueID, const std::string &)
	MPXJPP_GETTER_SETTER(scheduleID, const std::string &)
	MPXJPP_GETTER_SETTER(updateStart, std::time_t)
	MPXJPP_GETTER_SETTER(updateFinish, std::time_t)
	MPXJPP_GETTER_SETTER(confirmed, bool)
	MPXJPP_GETTER_SETTER(responsePending, bool)
};

enum class ResourceType {
	MATERIAL,
	WORK,
	COST
};

class ResourceAssignment final : public ProjectEntity, public ProjectEntityWithUniqueID, public FieldContainer
{
private:
	std::shared_ptr<Task> m_task;

	bool m_eventsEnabled = true;
//	DefaultTimephasedWorkContainer m_timephasedWork;
	std::vector<TimephasedCost> m_timephasedCost;

	std::unique_ptr<TimephasedWorkContainer> m_timephasedActualWork;
	std::vector<TimephasedCost> m_timephasedActualCost;

	std::unique_ptr<TimephasedWorkContainer> m_timephasedOvertimeWork;
	std::unique_ptr<TimephasedWorkContainer> m_timephasedActualOvertimeWork;

	ResourceAssignmentWorkgroupFields m_workgroup;
public:
	ResourceAssignment(ProjectFile &file, std::shared_ptr<Task> task);

	int getUniqueID() override {
		return getCachedValue(AssignmentField(AssignmentField::UNIQUE_ID)).cast<int>();
	}

	void setUniqueID(int id) override {
		set(AssignmentField(AssignmentField::UNIQUE_ID), common::any(id));
	}
};

}

#endif // RESOURCEASSIGNMENT_H
