#ifndef VIEW_H
#define VIEW_H

#include <string>

#include "listwithcallbacks.h"
#include "mpxjpp-gens.h"
#include "viewstate.h"

namespace mpxjpp {
/**
 * The enumeration of the valid types of view.
 */
enum class ViewType {
	UNKNOWN = 0,
	GANTT_CHART = 1,
	NETWORK_DIAGRAM = 2,
	RELATIONSHIP_DIAGRAM = 3,
	TASK_FORM = 4,
	TASK_SHEET = 5,
	RESOURCE_FORM = 6,
	RESOURCE_SHEET = 7,
	RESOURCE_GRAPH = 8,
	TASK_DETAILS_FORM = 10,
	TASK_NAME_FORM = 11,
	RESOURCE_NAME_FORM = 12,
	CALENDAR = 13,
	TASK_USAGE = 14,
	RESOURCE_USAGE = 15
};

/**
 * This interface represents a view of a set of project data that has been
 * instantiated within an MS Project file. View data is instantiated when a user
 * first looks at a view in MS Project. Each "screen" in MS Project, for example
 * the Gantt Chart, the Resource Sheet and so on are views. If a user has not
 * looked at a view (for example the Resource Usage view), information about
 * that view will not be present in the MPP file.
 */
class View {
	public:
		/**
		 * This method is used to retrieve the unique view identifier. This
		 * value identifies the view within the file. It does not identify
		 * the type of view represented by an instance of this class.
		 *
		 * @return view identifier
		 */
		virtual int id() = 0;

		/**
		 * This method is used to retrieve the view name. Note that internally
		 * in MS Project the view name will contain an ampersand (&) used to
		 * flag the letter that can be used as a shortcut for this view. The
		 * ampersand is stripped out by MPXJ.
		 *
		 * @return view name
		 */
		virtual std::string name() = 0;

		/**
		 * Retrieves the view type.
		 *
		 * @return view type
		 */
		virtual ViewType getType() = 0;
};

class ViewContainer : public ListWithCallbacks<View> {
	private:
		ViewState m_viewState;
	public:
		MPXJPP_GETTER_SETTER(viewState, ViewState)
};

}

namespace std {
std::string to_string(mpxjpp::ViewType val);
}

#endif // VIEW_H
