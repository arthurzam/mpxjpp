#ifndef CONSTRAINTFIELD_H
#define CONSTRAINTFIELD_H

#include "fieldtype.h"

namespace mpxjpp {

struct ConstraintField final : public FieldType {

#define CONSTRAINT_FIELD_GEN(F) \
	F(UNIQUE_ID, DataType::INTEGER),\
	F(TASK1, DataType::INTEGER),\
	F(TASK2, DataType::INTEGER)


public:
	enum Fields {
		CONSTRAINT_FIELD_GEN(MPXJPP_GEN_ENUM)
	};
private:
	static struct FieldType::priv_field_t g_fields[];
	static struct FieldType::priv_arr_t g_class;
public:
	constexpr ConstraintField(int val) :
		FieldType(val, &g_class)
	{}
};

}

#endif // CONSTRAINTFIELD_H
