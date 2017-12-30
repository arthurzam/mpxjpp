#ifndef FIELDTYPE_H
#define FIELDTYPE_H

#include <string>
#include <functional>

#include "common/object.h"
#include "datatype.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

class Locale;

enum class FieldTypeClass {
	TASK = 0,
	RESOURCE,
	ASSIGNMENT,
	CONSTRAINT,
	PROJECT,
	UNKNOWN
};

struct FieldType {
protected:
	struct priv_field_t {
		DataType m_dataType;
		int m_unitsType = -1;
	};

	struct priv_arr_t {
		FieldTypeClass typeClass;
		struct priv_field_t *fields;
	};

	int m_value = -1;
	const struct priv_arr_t *m_arr = nullptr;

	constexpr FieldType(int value, const struct priv_arr_t *arr) :
		m_value(value), m_arr(arr)
	{ }
public:
	constexpr FieldType() = default;

	constexpr FieldTypeClass fieldTypeClass() const {
		return m_arr->typeClass;
	}

	constexpr int value() const {
		return m_value;
	}

	DataType dataType() const {
		return m_arr->fields[m_value].m_dataType;
	}

	FieldType unitsType() const {
		return FieldType(m_arr->fields[m_value].m_unitsType, m_arr);
	}

	constexpr operator bool () const {
		return m_value != -1;
	}

	constexpr FieldType &operator =(const FieldType &o) {
		m_value = o.m_value;
		m_arr = o.m_arr;
		return *this;
	}

	// CHECK: is needed?
//	virtual std::string name() const = 0;
//	virtual std::string name(Locale &locale) const = 0;
};

static_assert(std::is_polymorphic<FieldType>::value == false, "FieldType has vtable!");

namespace common {
namespace anyimpl {
template<>
struct compare_to_func<FieldType> {
	int operator()(const FieldType &a, const FieldType &b) {
		return a.value() - b.value();
	}
};
}
}

/**
 * This interface is implemented by the Task and Resource classes. It
 * defines the common methods used to set and retrieve field values
 * using their identifiers.
 */
class FieldContainer {
private:
	common::any *m_array;
protected:
	FieldContainer(int size) {
		m_array = new common::any[size];
	}
public:
	virtual ~FieldContainer() {
		delete[] m_array;
	}

	/**
	 * Set a field value.
	 *
	 * @param field field identifier
	 * @param value field value
	 */
	virtual void set(const FieldType &field, common::any value) {
		m_array[field.value()] = value;
	}

	/**
	 * Retrieve a field value. Use the cached value. Do not attempt to
	 * calculate a value if the cached value is null.
	 *
	 * @param field field identifier
	 * @return field value
	 */
	common::any &getCachedValue(const FieldType &field) const {
		return m_array[field.value()];
	}

	/**
	 * Retrieve a field value. If the cached value is null, attempt to
	 * calculate the current value.
	 *
	 * @param field field identifier
	 * @return field value
	 */
	virtual common::any &getCurrentValue(const FieldType &field) = 0;

	/*
	 * possible functions - not found as used
	 * public void addFieldListener(FieldListener listener);
	 * public void removeFieldListener(FieldListener listener);
	 */
};

// those MACROs are used for generating getter/setter in FieldContainer
// You should define FIELDTYPE_CLASS prior to using those

#define MPXJPP_FIELD_GETTER(varName, type, field, castType) \
	type varName() const { \
		return static_cast<type>(getCachedValue(FIELDTYPE_CLASS(FIELDTYPE_CLASS::field)).cast<castType>()); \
	}

#define MPXJPP_FIELD_SETTER(varName, type, field, castType) \
	void set_##varName(type varName) { \
		set(FIELDTYPE_CLASS(FIELDTYPE_CLASS::field), static_cast<castType>(varName)); \
	}

#define MPXJPP_FIELD_GETTER_SETTER(varName, type, field, castType) \
	MPXJPP_FIELD_GETTER(varName, type, field, castType) \
	MPXJPP_FIELD_SETTER(varName, type, field, castType)

#define MPXJPP_FIELD_GETTER_SETTER_NO_SET_CAST(varName, type, field, castType) \
	MPXJPP_FIELD_GETTER(varName, type, field, castType) \
	MPXJPP_FIELD_SETTER(varName, type, field, type)

}

#endif // FIELDTYPE_H