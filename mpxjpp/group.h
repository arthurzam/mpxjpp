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
 * This struct represents a clause from a definition of a group.
 */
struct GroupClause final {
public:
    FieldType field;
    bool ascending;
    common::FontStyle font; // CHECK: maybe some kind of pointer
    common::Color cellBackgroundColor;
    common::BackgroundPattern pattern;
    int groupOn;
    common::any startAt;
    common::any groupInterval;
};

class Group final {
    private:
        std::string m_name;
        std::vector<GroupClause> m_clauses;
        int m_id;
        bool m_showSummaryTasks;
    public:
        Group(int id, const std::string &name, bool showSummaryTasks)
            : m_name(name), m_id(id), m_showSummaryTasks(showSummaryTasks)
        {}
        Group(int id, std::string &&name, bool showSummaryTasks)
            : m_name(std::move(name)), m_id(id), m_showSummaryTasks(showSummaryTasks)
        {}

        MPXJPP_GETTER(id, int)
        MPXJPP_GETTER(name, const std::string &)
        MPXJPP_GETTER(showSummaryTasks, bool)

        MPXJPP_GETTER(clauses, const std::vector<GroupClause> &)
        void addGroupClause(const GroupClause &clause) {
            this->m_clauses.push_back(clause);
        }
        template<typename... Args>
        void emplaceGroupClause(Args&&... args) {
            this->m_clauses.emplace_back(std::forward<Args>(args)...);
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
