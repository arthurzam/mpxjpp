#ifndef CUSTOMFIELD_H
#define CUSTOMFIELD_H

#include "common/object.h"
#include "common/uuid.h"

#include "criteria.h"
#include "fieldtype.h"
#include "mpxjpp-gens.h"
#include "listwithcallbacks.h"

namespace mpxjpp {

class CustomFieldValueItem final {
private:
    int m_uniqueID;
    common::UUID m_guid;
    common::any m_value;
    std::string m_description;
    std::vector<uint8_t> m_unknown;
    int m_parent;
public:
    CustomFieldValueItem(int uniqueID) :
        m_uniqueID(uniqueID)
    { }

    MPXJPP_GETTER(uniqueID, int)
    MPXJPP_GETTER_SETTER(value, const common::any &)
    MPXJPP_GETTER_SETTER(description, const std::string &)
    MPXJPP_GETTER_SETTER(unknown, const std::vector<uint8_t> &)
    MPXJPP_GETTER_SETTER(parent, int)
    MPXJPP_GETTER_SETTER(guid, common::UUID)
};

class CustomFieldContainer;

class CustomFieldLookupTable : public ListWithCallbacks<CustomFieldValueItem> {
private:
    CustomFieldContainer &m_parent;
protected:
    virtual void added(int index) override;
    virtual void removed(const CustomFieldValueItem &item) override;
public:
    CustomFieldLookupTable(CustomFieldContainer &parent) :
        m_parent(parent)
    { }
};

class CustomField {
private:
    FieldType m_field;
    CustomFieldContainer &m_parent;
    CustomFieldLookupTable m_table;
    GraphicalIndicator m_indicator;
    std::string m_alias;
public:
    CustomField(FieldType field, CustomFieldContainer &parent) :
        m_field(field), m_parent(parent), m_table(parent)
    { }

    MPXJPP_GETTER(field, FieldType)
    CustomFieldLookupTable &lookupTable() {
        return m_table;
    }
    GraphicalIndicator &graphicalIndicator() {
        return m_indicator;
    }
    MPXJPP_GETTER(alias, const std::string &)
    void set_alias(const std::string &alias);
};

class CustomFieldContainer {
private:
    std::vector<std::unique_ptr<CustomField>> m_configMap;
    std::vector<CustomFieldValueItem*> m_valueMap;
    std::vector<std::pair<std::string, FieldType>> m_aliasMap;
    std::vector<std::tuple<std::string, int, common::any>> m_aliasValueMap;
public:
    CustomField &getCustomField(FieldType field);
    CustomFieldValueItem *getCustomFieldValueItemByUniqueID(int uniqueID);
    void registerValue(CustomFieldValueItem *item) {
        m_valueMap.push_back(item);
    }
    void deregisterValue(const CustomFieldValueItem *item);
    void registerAlias(FieldType type, const std::string &alias);
    FieldType getFieldByAlias(FieldTypeClass typeClass, const std::string &alias);
    void registerAliasValue(const std::string &alias, int uid, common::any value);
    common::any getAliasValue(const std::string &alias, int uid);
};

}
#endif // CUSTOMFIELD_H
