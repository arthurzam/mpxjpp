#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <vector>
#include <map>

#include "mpxjpp-gens.h"
#include "fieldtype.h"
#include "listwithcallbacks.h"

#include "common/font.h"
#include "common/object.h"

namespace mpxjpp {

/**
 * This class represents a clause from a definition of a group.
 */
class GroupClause final {
    private:
        FieldType m_field;
        bool m_ascending;
        common::FontStyle m_font; // CHECK: maybe some kind of pointer
        common::Color m_cellBackgroundColor;
        common::BackgroundPattern m_pattern;
        int m_groupOn;
        common::any m_startAt;
        common::any m_groupInterval;
    public:
        MPXJPP_GETTER_SETTER(ascending, bool)
        MPXJPP_GETTER_SETTER(cellBackgroundColor, common::Color)
        MPXJPP_GETTER_SETTER(field, FieldType)
        MPXJPP_GETTER_SETTER(font, common::FontStyle)
        MPXJPP_GETTER_SETTER(groupInterval, common::any)
        MPXJPP_GETTER_SETTER(startAt, common::any)
        MPXJPP_GETTER_SETTER(groupOn, int)
        MPXJPP_GETTER_SETTER(pattern, common::BackgroundPattern)
};

class Group final {
    private:
        int m_id;
        std::string m_name;
        bool m_showSummaryTasks;
        std::vector<GroupClause> m_clauses;
    public:
        Group(int id, const std::string &name, bool showSummaryTasks)
            : m_id(id), m_name(name), m_showSummaryTasks(showSummaryTasks)
        {}

        MPXJPP_GETTER(id, int)
        MPXJPP_GETTER(name, const std::string &)
        MPXJPP_GETTER(showSummaryTasks, bool)

        MPXJPP_GETTER(clauses, const std::vector<GroupClause> &)
        void addGroupClause(const GroupClause &clause) {
            this->m_clauses.push_back(clause);
        }
};

/**
 * Manages the group definitions belonging to a project.
 */
class GroupContainer final : public ListWithCallbacks<Group> {
    private:
        std::map<std::string, int> m_groupsByName;
    protected:
        virtual void added(int index) override {
            m_groupsByName.insert({(*this)[index].name(), index});
        }
        virtual void removed(const Group &val) override {
            m_groupsByName.erase(val.name());
        }
    public:
        /**
         * Retrieve a given group by name.
         *
         * @param name group name
         * @return Group instance
         */
        Group &getByName(const std::string &name) {
            return (*this)[m_groupsByName[name]];
        }
};

}

#endif // GROUP_H
