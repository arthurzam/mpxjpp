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

FieldType::FieldType(unsigned value, FieldTypeClass typeClass) {
    unsigned maxVal;
    switch (typeClass) {
        case FieldTypeClass::UNKNOWN:
            m_arr = nullptr;
            maxVal = value;
            break;
        case FieldTypeClass::ASSIGNMENT:
            m_arr = &AssignmentField::g_class;
            maxVal = AssignmentField::FINISH;
            break;
        case FieldTypeClass::CONSTRAINT:
            m_arr = &ConstraintField::g_class;
            maxVal = ConstraintField::TASK2;
            break;
        case FieldTypeClass::PROJECT:
            m_arr = &ProjectField::g_class;
            maxVal = ProjectField::FINISH_DATE;
            break;
        case FieldTypeClass::RESOURCE:
            m_arr = &ResourceField::g_class;
            maxVal = ResourceField::FINISH;
            break;
        case FieldTypeClass::TASK:
            m_arr = &TaskField::g_class;
            maxVal = TaskField::FINISH;
            break;
    }
    if (value > maxVal)
        throw std::invalid_argument("value is out of bounds for this class type");
    m_value = static_cast<int>(value);
}
