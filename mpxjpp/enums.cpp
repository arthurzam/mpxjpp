#include "enums.h"
#include "view.h"
#include "timeunit.h"

#include "fieldtype.h"
#include "availability.h"
#include "listwithcallbacks.h"

constexpr const char *mpxjpp::TimeUnit::strs[];


namespace std {

std::string to_string(mpxjpp::Day val) {
	static constexpr const char *dayName[] = {
		"__UNINITIALIZED__",
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
	};
	return std::string(dayName[static_cast<unsigned>(val)]);
}

std::string to_string(mpxjpp::FileVersion val) {
	switch(val) {
		case mpxjpp::FileVersion::VERSION_1_0:
			return std::string("1.0");
		case mpxjpp::FileVersion::VERSION_3_0:
			return std::string("2.0");
		case mpxjpp::FileVersion::VERSION_4_0:
		default:
			return std::string("4.0");
	}
}

std::string to_string(mpxjpp::RecurrenceType val) {
	switch(val) {
		case mpxjpp::RecurrenceType::DAILY:
			return std::string("Daily");
		case mpxjpp::RecurrenceType::WEEKLY:
			return std::string("Weekly");
		case mpxjpp::RecurrenceType::MONTHLY:
			return std::string("Monthly");
		case mpxjpp::RecurrenceType::YEARLY:
			return std::string("Yearly");
		default:// CHECK: check possibility
			return std::string();
	}
}

std::string to_string(mpxjpp::ViewType val) {
	static constexpr const char *viewName[] = {
		"UNKNOWN",
		"GANTT_CHART",
		"NETWORK_DIAGRAM",
		"RELATIONSHIP_DIAGRAM",
		"TASK_FORM",
		"TASK_SHEET",
		"RESOURCE_FORM",
		"RESOURCE_SHEET",
		"RESOURCE_GRAPH",
		"TASK_DETAILS_FORM",
		"TASK_NAME_FORM",
		"RESOURCE_NAME_FORM",
		"CALENDAR",
		"TASK_USAGE",
		"RESOURCE_USAGE"
	};
	return std::string(viewName[static_cast<unsigned>(val)]);
}

}
