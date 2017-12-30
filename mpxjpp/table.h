#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <string>
#include <vector>

#include "column.h"
#include "listwithcallbacks.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

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
		int m_id = 0;
		std::string m_name;
		bool m_resourceFlag = false;
		std::vector<Column> m_columns;
	public:
		Table() = default;

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

class TableContainer final : public ListWithCallbacks<Table> {
	private:
		std::map<std::string, int> m_taskTablesByName, m_resourceTablesByName;

		std::map<std::string, int> &getIndex(const Table &table) {
			return table.resourceFlag() ? m_resourceTablesByName : m_taskTablesByName;
		}

	protected:
		virtual void added(int index) override {
			const Table &table = m_list[index];
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
			return m_list[m_taskTablesByName[name]];
		}

		/**
		 * Utility method to retrieve the definition of a resource table by name.
		 * This method will return null if the table name is not recognised.
		 *
		 * @param name table name
		 * @return table instance
		 */
		Table &getResourceTableByName(const std::string &name) {
			return m_list[m_resourceTablesByName[name]];
		}
};

}

#endif // TABLE_H