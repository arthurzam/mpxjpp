#ifndef FIELDTYPE_H
#define FIELDTYPE_H

#include <string>
#include <functional>

#include "common/object.h"
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

enum class DataType {
	__UNINITIALIZED__ = 0,

	STRING = 1,
	DATE = 2,
	CURRENCY = 3,
	BOOLEAN = 4,
	NUMERIC = 5,
	DURATION = 6,
	UNITS = 7,
	PERCENTAGE = 8,
	ACCRUE = 9,
	CONSTRAINT = 10,
	RATE = 11,
	PRIORITY = 12,
	RELATION_LIST = 13,
	TASK_TYPE = 14,
	RESOURCE_TYPE = 15,
	WORK = 16,
	INTEGER = 17,
	ASCII_STRING = 18,
	SHORT = 19,
	BINARY = 20,
	DELAY = 21,
	WORK_UNITS = 22,
	WORKGROUP = 23,
	GUID = 24,
	RATE_UNITS = 25,
	EARNED_VALUE_METHOD = 26,
	RESOURCE_REQUEST_TYPE = 27,
	CURRENCY_SYMBOL_POSITION = 28,
	CHAR = 29,
	DATE_ORDER = 30,
	PROJECT_TIME_FORMAT = 31,
	PROJECT_DATE_FORMAT = 32,
	SCHEDULE_FROM = 33,
	DAY = 34,
	MAP = 35,
	MPX_FILE_VERSION = 36,
	MPX_CODE_PAGE = 37,
	BOOKING_TYPE = 38,
	TIME_UNITS = 39
};

struct FieldType {
	friend class FieldContainer;
//	friend class common::anyimpl::compare_to<FieldType>;
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

	constexpr bool operator ==(const FieldType &o) {
		return m_arr == o.m_arr && m_value == o.m_value;
	}

	// CHECK: is needed?
//	virtual std::string name() const = 0;
//	virtual std::string name(Locale &locale) const = 0;

};

static_assert(std::is_polymorphic<FieldType>::value == false, "FieldType shouldn't have vtable!");

/**
 * This interface is implemented by the Task and Resource classes. It
 * defines the common methods used to set and retrieve field values
 * using their identifiers.
 */
class FieldContainer {
	/** TODO: vector<bool> optimization
	  *
	  * as all FieldType uses aren't using the value, we can extract all the booleans to an vector<bool>
	  * for better storage usement.
	  */
private:
	common::any *m_array;
protected:
	FieldContainer(int size) {
		m_array = new common::any[size];
	}

	/**
	 * Set a field value.
	 *
	 * @param field field identifier
	 * @param value field value
	 */
	template <typename T>
	void set(const FieldType &field, T value) {
		common::any_type_cast<T>::set(m_array[field.m_value], value);
	}
public:
	virtual ~FieldContainer() {
		delete[] m_array;
	}

	/**
	 * Retrieve a field value. Use the cached value. Do not attempt to
	 * calculate a value if the cached value is null.
	 *
	 * @param field field identifier
	 * @return field value
	 */
	common::any &getCachedValue(const FieldType &field) const {
		return m_array[field.m_value];
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

#define MPXJPP_FIELD_GETTER(varName, type, field) \
	type varName() const { \
		return common::any_type_cast<type>::get(getCachedValue(FIELDTYPE_CLASS(FIELDTYPE_CLASS::field)), {}); \
	}

#define MPXJPP_FIELD_SETTER(varName, type, field) \
	void set_##varName(type varName) { \
		set<type>(FIELDTYPE_CLASS(FIELDTYPE_CLASS::field), varName); \
	}

#define MPXJPP_FIELD_GETTER_SETTER(varName, type, field) \
	MPXJPP_FIELD_GETTER(varName, type, field) \
	MPXJPP_FIELD_SETTER(varName, type, field)

}

#endif // FIELDTYPE_H
