#ifndef TIMEUNIT_H
#define TIMEUNIT_H

#include <string>

namespace mpxjpp {

struct TimeUnit final {
public:
	static constexpr unsigned MINUTES = 0,
							  HOURS = 1,
							  DAYS = 2,
							  WEEKS = 3,
							  MONTHS = 4,
							  PERCENT = 5,
							  YEARS = 6,
							  ELAPSED_MINUTES = 7,
							  ELAPSED_HOURS = 8,
							  ELAPSED_DAYS = 9,
							  ELAPSED_WEEKS = 10,
							  ELAPSED_MONTHS = 11,
							  ELAPSED_YEARS = 12,
							  ELAPSED_PERCENT = 13;
private:
	static constexpr const char *strs[] = {
		"m",
		"h",
		"d",
		"w",
		"mo",
		"%",
		"y",
		"em",
		"eh",
		"ed",
		"ew",
		"emo",
		"ey",
		"e%"
	};
	int m_value;

public:
	constexpr TimeUnit(int type) :
		m_value(type)
	{}
	constexpr explicit operator int() const {
		return m_value;
	}

	constexpr explicit operator const char *() const {
		return TimeUnit::strs[m_value];
	}

	constexpr bool operator ==(const TimeUnit &other) const {
		return other.m_value == m_value;
	}

	constexpr bool operator !=(const TimeUnit &other) const {
		return other.m_value != m_value;
	}
};

}

namespace std {

inline std::string to_string(mpxjpp::TimeUnit val) {
	return std::string((const char *)val);
}

}
#endif // TIMEUNIT_H
