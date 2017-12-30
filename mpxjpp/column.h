#ifndef COLUMN_H
#define COLUMN_H

#include <string>

#include "mpxjpp-gens.h"

namespace mpxjpp {

class FieldType;
class ProjectFile;

class Column final
{
	public:
		enum Align {
			ALIGN_LEFT = 1,
			ALIGN_CENTER = 2,
			ALIGN_RIGHT = 3
		};
	private:
		FieldType *m_fieldType = nullptr;
		int m_width = 0;
		int m_alignTitle = 0;
		int m_alignData = 0;
		std::string m_title;
		ProjectFile &m_project;
	public:
		Column(ProjectFile &project)
			: m_project(project)
		{}

		MPXJPP_GETTER_SETTER(alignData, int)
		MPXJPP_GETTER_SETTER(alignTitle, int)
		MPXJPP_GETTER_SETTER(width, int)
		MPXJPP_GETTER_SETTER(fieldType, FieldType *)

		// TODO: getTitle()
		// TODO: setTitle()

};

}

#endif // COLUMN_H
