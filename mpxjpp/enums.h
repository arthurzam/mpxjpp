#ifndef ENUMS_H
#define ENUMS_H

#include <string>

namespace mpxjpp {

enum class AccrueType {
    START = 1,
    END = 2,
    PRORATED = 3
};

enum class BookingType {
    COMMITTED = 0,
    PROPOSED
};

enum class CurrencySymbolPosition {
    AFTER = 0,
    BEFORE = 1,
    AFTER_WITH_SPACE = 2,
    BEFORE_WITH_SPACE = 3
};

enum class ConstraintType {
    AS_SOON_AS_POSSIBLE = 0,
    AS_LATE_AS_POSSIBLE,
    MUST_START_ON,
    MUST_FINISH_ON,
    START_NO_EARLIER_THAN,
    START_NO_LATER_THAN,
    FINISH_NO_EARLIER_THAN,
    FINISH_NO_LATER_THAN
};

enum class DateOrder {
    MDY = 0,
    DMY,
    YMD
};

enum class Day {
    __UNINITIALIZED__ = 0,
    SUNDAY = 1,
    MONDAY = 2,
    TUESDAY = 3,
    WEDNESDAY = 4,
    THURSDAY = 5,
    FRIDAY = 6,
    SATURDAY = 7
};

constexpr Day operator++(Day val) {
    unsigned v = static_cast<unsigned>(val) + 1;
    if (v > 7)
        return Day::SUNDAY;
    return static_cast<Day>(v);
}

enum class DayType {
    __UNINITIALIZED__ = 0, // CHECK: if not problematic
    NON_WORKING, // =0
    WORKING,
    DEFAULT
};

enum class EarnedValueMethod {
    PERCENT_COMPLETE = 0,
    PHYSICAL_PERCENT_COMPLETE
};

enum class FileVersion {
    VERSION_1_0 = 1,
    VERSION_3_0 = 3,
    VERSION_4_0 = 4
};

enum class RecurrenceType {
    DAILY = 1,
    WEEKLY = 4,
    MONTHLY = 8,
    YEARLY = 16
};

enum class ScheduleFrom {
    START = 0,
    FINISH = 1
};

enum class TaskMode {
    MANUALLY_SCHEDULED = 0,
    AUTO_SCHEDULED = 1
};

enum class TaskType {
    FIXED_UNITS = 0,
    FIXED_DURATION = 1,
    FIXED_WORK = 2
};

enum class WorkContour {
    FLAT = 0,
    BACK_LOADED = 1,
    FRONT_LOADED = 2,
    DOUBLE_PEAK = 3,
    EARLY_PEAK = 4,
    LATE_PEAK = 5,
    BELL = 6,
    TURTLE = 7,
    CONTOURED = 8
};

enum class WorkGroup {
    DEFAULT = 0,
    NONE = 1,
    EMAIL = 2,
    WEB = 3
};

}

namespace std {

std::string to_string(mpxjpp::Day val);
std::string to_string(mpxjpp::FileVersion val);
std::string to_string(mpxjpp::RecurrenceType val);

}

#endif // ENUMS_H
