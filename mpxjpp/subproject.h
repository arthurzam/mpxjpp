#ifndef SUBPROJECT_H
#define SUBPROJECT_H

#include <string>
#include <vector>
#include <algorithm>

#include "listwithcallbacks.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

class SubProject final {
	private:
		int m_taskUniqueID;
		int m_uniqueIDOffset;
		std::vector<int> m_externalTaskUniqueIDs;
		std::string m_dosFullPath;
		std::string m_fullPath;
		std::string m_dosFileName;
		std::string m_fileName;
	public:
		MPXJPP_GETTER_SETTER(dosFullPath, std::string)
		MPXJPP_GETTER_SETTER(fullPath, std::string)
		MPXJPP_GETTER_SETTER(dosFileName, std::string)
		MPXJPP_GETTER_SETTER(fileName, std::string)
		MPXJPP_GETTER_SETTER(taskUniqueID, int)
		MPXJPP_GETTER_SETTER(uniqueIDOffset, int)

		bool isExternalTask(int task) const{
			return (std::find(m_externalTaskUniqueIDs.cbegin(),
							  m_externalTaskUniqueIDs.cend(), task)
					!= m_externalTaskUniqueIDs.cend());
		}

		void addExternalTaskUniqueID(int externalTaskUniqueID) {
			m_externalTaskUniqueIDs.push_back(externalTaskUniqueID);
		}

		MPXJPP_GETTER(externalTaskUniqueIDs, const std::vector<int> &)
};

/**
 * Manages the sub projects belonging to a project.
 */
class SubProjectContainer final : public ListWithCallbacks<SubProject> {
	private:
		SubProject m_resourceSubProject;
	public:
		MPXJPP_GETTER_SETTER(resourceSubProject, const SubProject &)
};

}

#endif // SUBPROJECT_H
