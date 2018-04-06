#include "resourceassignment.h"


mpxjpp::ResourceAssignment::ResourceAssignment(mpxjpp::ProjectFile &file, std::shared_ptr<mpxjpp::Task> task) :
    ProjectEntity(file), FieldContainer(AssignmentField::FINISH + 1), m_task(std::move(task)) {
    // TODO: continue
}
