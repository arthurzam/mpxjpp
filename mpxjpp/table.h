#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <string>
#include <vector>

#include "fieldtype.h"
#include "listwithcallbacks.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

struct Column final {
    enum class Align : unsigned char {
        ALIGN_LEFT = 1,
        ALIGN_CENTER = 2,
        ALIGN_RIGHT = 3
    };

    std::string title;
    FieldType fieldType;
    int width = 0;
    Align alignTitle = Align::ALIGN_RIGHT;
    Align alignData = Align::ALIGN_RIGHT;
};
static_assert(std::is_nothrow_move_constructible<Column>::value, "Column should be checked");

/**
 * This class represents the definition of a table of data from an MPP file.
 * Much of the important information held in MS Project is represented
 * in a tabular format. This class represents the attributes associated with
 * these tables. For example, the attributes of the table of data that appears
 * as the left hand part of the standard Gantt Chart view in MS Project will
 * be defined here.
 */
class Table final {
    private:
        std::string m_name;
        std::vector<Column> m_columns;
        int m_id = 0;
        bool m_resourceFlag = false;
    public:
        Table() = default;

        Table(int id, bool resourceFlag, std::string &&name) :
            m_name(std::move(name)), m_id(id), m_resourceFlag(resourceFlag)
        { }

        MPXJPP_GETTER_SETTER(id, int)
        MPXJPP_GETTER_SETTER(name, const std::string &)
        MPXJPP_GETTER_SETTER(resourceFlag, bool)

        MPXJPP_GETTER(columns, const std::vector<Column> &)

        void addColumn(const Column &c) {
            m_columns.push_back(c);
        }
        void addColumn(Column &&c) {
            m_columns.push_back(std::move(c));
        }
};
static_assert(std::is_nothrow_move_constructible<Table>::value, "Table should be checked");

class TableContainer final : public ListWithCallbacks<Table> {
    private:
        std::map<std::string, int> m_taskTablesByName, m_resourceTablesByName;

        std::map<std::string, int> &getIndex(const Table &table) {
            return table.resourceFlag() ? m_resourceTablesByName : m_taskTablesByName;
        }

    protected:
        virtual void added(int index) override {
            const Table &table = (*this)[index];
            getIndex(table).insert({table.name(), index});
        }
        virtual void removed(const Table &val) override {
            getIndex(val).erase(val.name());
        }
    public:
        /**
         * Utility method to retrieve the definition of a task table by name.
         * This method will return null if the table name is not recognised.
         *
         * @param name table name
         * @return table instance
         */
        Table &getTaskTableByName(const std::string &name) {
            return (*this)[m_taskTablesByName[name]];
        }

        /**
         * Utility method to retrieve the definition of a resource table by name.
         * This method will return null if the table name is not recognised.
         *
         * @param name table name
         * @return table instance
         */
        Table &getResourceTableByName(const std::string &name) {
            return (*this)[m_resourceTablesByName[name]];
        }
};

}

#endif // TABLE_H
