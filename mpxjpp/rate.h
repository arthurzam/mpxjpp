#ifndef RATE_H
#define RATE_H

#include <string>

#include "mpxjpp-gens.h"
#include "timeunit.h"

namespace mpxjpp {
class Rate final {
	private:
		double m_amount;
		TimeUnit m_units;
	public:
		constexpr Rate(double amount, TimeUnit time) :
			m_amount(amount), m_units(time)
		{}

		constexpr MPXJPP_GETTER(amount, double)
		constexpr MPXJPP_GETTER(units, TimeUnit)

		constexpr bool operator ==(const Rate &other) const {
			return m_units == other.m_units && ::abs(other.m_amount - m_amount) < 0.00001;
		}

		constexpr int compareTo(const Rate &other) const {
			if (m_units == other.m_units)
				return m_amount - other.m_amount;
			return static_cast<int>(m_units) - static_cast<int>(other.m_units);
		}
};
}

namespace std {
template<>
struct hash<mpxjpp::Rate> {
	constexpr size_t operator()(const mpxjpp::Rate &val) const {
		return (int)val.amount() + (int)val.units();
	}
};

//std::string to_string(mpxjpp::Rate val) {
//	return to_string(val.amount()) + to_string(val.units());
//}
}

#endif // RATE_H
