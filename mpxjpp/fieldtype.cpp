#include "assignmentfield.h"
#include "constraintfield.h"
#include "projectfield.h"
#include "resourcefield.h"
#include "taskfield.h"

using namespace mpxjpp;

struct FieldType::priv_field_t AssignmentField::g_fields[] = {
   ASSIGNMENT_FIELD_GEN(MPXJPP_GEN_VALUE)
};

struct FieldType::priv_arr_t AssignmentField::g_class = {
	FieldTypeClass::ASSIGNMENT,
	AssignmentField::g_fields
};

struct FieldType::priv_field_t ConstraintField::g_fields[] = {
   CONSTRAINT_FIELD_GEN(MPXJPP_GEN_VALUE)
};

struct FieldType::priv_arr_t ConstraintField::g_class = {
	FieldTypeClass::CONSTRAINT,
	ConstraintField::g_fields
};

struct FieldType::priv_field_t ProjectField::g_fields[] = {
   PROJECT_FIELD_GEN(MPXJPP_GEN_VALUE)
};

struct FieldType::priv_arr_t ProjectField::g_class = {
	FieldTypeClass::PROJECT,
	ProjectField::g_fields
};

struct FieldType::priv_field_t ResourceField::g_fields[] = {
   RESOURCE_FIELD_GEN(MPXJPP_GEN_VALUE)
};

struct FieldType::priv_arr_t ResourceField::g_class = {
	FieldTypeClass::RESOURCE,
	ResourceField::g_fields
};

struct FieldType::priv_field_t TaskField::g_fields[] = {
   TASK_FIELD_GEN(MPXJPP_GEN_VALUE)
};

struct FieldType::priv_arr_t TaskField::g_class = {
	FieldTypeClass::TASK,
	TaskField::g_fields
};
