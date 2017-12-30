#include "calendar.h"

#include <cstring>

using namespace mpxjpp;
using namespace mpxjpp::common;

Calendar::Calendar(time_t t) {
	::memcpy(&tm, std::localtime(&t), sizeof(tm));
}
