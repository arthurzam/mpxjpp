#include "customfield.h"

#include <algorithm>

using namespace mpxjpp;

void CustomFieldLookupTable::added(int index) {
	m_parent.registerValue(&(*this)[index]);
}

void CustomFieldLookupTable::removed(const CustomFieldValueItem &item) {
	m_parent.deregisterValue(&item);
}

void CustomField::set_alias(const std::string &alias) {
	m_alias = alias;
	m_parent.registerAlias(m_field, alias);
}

CustomField &CustomFieldContainer::getCustomField(FieldType field) {
	for (auto &element : m_configMap)
		if (element->field() == field)
			return *element;
	m_configMap.push_back(std::make_unique<CustomField>(field, *this));
	return *m_configMap.front();
}

CustomFieldValueItem *CustomFieldContainer::getCustomFieldValueItemByUniqueID(int uniqueID) {
	for (CustomFieldValueItem *element : m_valueMap)
		if (element->uniqueID() == uniqueID)
			return element;
	return nullptr;
}

void CustomFieldContainer::deregisterValue(const CustomFieldValueItem *item) {
	auto it = std::find(m_valueMap.begin(), m_valueMap.end(), item);
	if (it != m_valueMap.end())
		m_valueMap.erase(it);
}

void CustomFieldContainer::registerAlias(FieldType type, const std::string &alias) {
	m_aliasMap.push_back({alias, type});
}

FieldType CustomFieldContainer::getFieldByAlias(FieldTypeClass typeClass, const std::string &alias) {
	for (const auto &element : m_aliasMap)
		if (element.second.fieldTypeClass() == typeClass && element.first == alias)
			return element.second;
	return FieldType();
}

void CustomFieldContainer::registerAliasValue(const std::string &alias, int uid, common::any value) {
	m_aliasValueMap.push_back({alias, uid, value});
}

common::any CustomFieldContainer::getAliasValue(const std::string &alias, int uid) {
	for (const auto &element : m_aliasValueMap)
		if (std::get<1>(element) == uid && std::get<0>(element) == alias)
			return std::get<2>(element);
	return common::any();
}
