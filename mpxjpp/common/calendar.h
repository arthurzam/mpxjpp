#ifndef CALENDAR_H
#define CALENDAR_H

#include <ctime>

namespace mpxjpp {
namespace common {

struct Time final {
	unsigned sec:6;   // [0-61]
	unsigned min:6;   // [0-59]
	unsigned hours:5; // [0-23]

	constexpr explicit operator std::time_t() const {
		return hours * 3600 + min * 60 + sec;
	}

	constexpr explicit operator int() const {
		return sec | (min << 6) | (hours << 12);
	}

	constexpr int compareTo(Time x) {
		return static_cast<int>(*this) - static_cast<int>(x);
	}
};
static_assert(sizeof(Time) <= sizeof(unsigned), "oversized struct size - check compiler");
static_assert(Time({1, 1, 1}).compareTo({2, 0, 2}) < 0, "Time::compareTo not working");

struct Date final {
	unsigned day:5;   // [0-31]
	unsigned month:4; // [0-12]
	unsigned year:22;

	constexpr explicit operator int() const {
		return day | (month << 5) | (year << 9);
	}

	constexpr int compareTo(Date x) {
		return static_cast<int>(*this) - static_cast<int>(x);
	}
};
static_assert(sizeof(Date) <= sizeof(unsigned), "oversized struct size - check compiler");
static_assert(Date({20, 0, 1}).compareTo({21, 1, 2}) < 0, "Date::compareTo not working");

struct Calendar final
{
private:
	mutable struct std::tm tm;
public:
	Calendar(std::time_t t);

	explicit operator std::time_t() const {
		return std::mktime(&tm);
	}
#define FIELD(name,field) \
	int name() const { \
		return tm. field; \
	}

	FIELD(seconds, tm_sec)
	FIELD(minutes, tm_min)
	FIELD(hours, tm_hour)
	FIELD(day, tm_mday)
	FIELD(month, tm_mon)
	FIELD(year, tm_year)
#undef FIELD

public:
	Calendar &operator +=(Time t) {
		tm.tm_sec += t.sec;
		tm.tm_min += t.min;
		tm.tm_hour += t.hours;
		return *this;
	}
	Calendar &operator +=(Date d) {
		tm.tm_mday += d.day;
		tm.tm_mon += d.month;
		tm.tm_year += d.year;
		return *this;
	}

	static Calendar now() {
		return Calendar(std::time(NULL));
	}

	explicit operator Time() const {
		return {(unsigned)tm.tm_sec, (unsigned)tm.tm_min, (unsigned)tm.tm_hour};
	}

	explicit operator Date() const {
		return {(unsigned)tm.tm_mday, (unsigned)tm.tm_mon, (unsigned)tm.tm_year};
	}
};

}
}
#endif // CALENDAR_H
