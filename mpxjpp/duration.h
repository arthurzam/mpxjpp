#ifndef DURATION_H
#define DURATION_H

#include "mpxjpp-gens.h"
#include "timeunit.h"

namespace mpxjpp {

class ProjectProperties;

class Duration final {
	private:
		double m_duration;
		TimeUnit m_units;
	public:
		constexpr Duration(double duration, TimeUnit type)
			: m_duration(duration), m_units(type)
		{}

		constexpr MPXJPP_GETTER(duration, double)
		constexpr MPXJPP_GETTER(units, TimeUnit)

		/**
		 * This method provides an <i>approximate</i> conversion between duration
		 * units. It does take into account the project defaults for number of hours
		 * in a day and a week, but it does not take account of calendar details.
		 * The results obtained from it should therefore be treated with caution.
		 *
		 * @param duration duration value
		 * @param fromUnits units to convert from
		 * @param toUnits units to convert to
		 * @param minutesPerDay number of minutes per day
		 * @param minutesPerWeek number of minutes per week
		 * @param daysPerMonth number of days per month
		 * @return new Duration instance
		 */
		static constexpr Duration convertUnits(
				double duration,
				TimeUnit fromUnits,
				TimeUnit toUnits,
				double minutesPerDay,
				double minutesPerWeek,
				double daysPerMonth ) {
			switch ((int)fromUnits) {
				case TimeUnit::YEARS:
					duration *= (minutesPerWeek * 52);
					break;
				case TimeUnit::ELAPSED_YEARS:
					duration *= (60 * 24 * 7 * 52);
					break;
				case TimeUnit::MONTHS:
					duration *= (minutesPerDay * daysPerMonth);
					break;
				case TimeUnit::ELAPSED_MONTHS:
					duration *= (60 * 24 * 30);
					break;
				case TimeUnit::WEEKS:
					duration *= minutesPerWeek;
					break;
				case TimeUnit::ELAPSED_WEEKS:
					duration *= (60 * 24 * 7);
					break;
				case TimeUnit::DAYS:
					duration *= minutesPerDay;
					break;
				case TimeUnit::ELAPSED_DAYS:
					duration *= (60 * 24);
					break;
				case TimeUnit::HOURS:
				case TimeUnit::ELAPSED_HOURS:
					duration *= 60;
					break;
			}
			switch ((int)toUnits)
			{
				case TimeUnit::MINUTES:
				case TimeUnit::ELAPSED_MINUTES:
					break;
				case TimeUnit::HOURS:
				case TimeUnit::ELAPSED_HOURS:
					duration /= 60;
					break;
				case TimeUnit::DAYS:
					if (minutesPerDay != 0)
						duration /= minutesPerDay;
					else
						duration = 0;
					break;
				case TimeUnit::ELAPSED_DAYS:
					duration /= (60 * 24);
					break;
				case TimeUnit::WEEKS:
					if (minutesPerWeek != 0)
						duration /= minutesPerWeek;
					else
						duration = 0;
					break;
				case TimeUnit::ELAPSED_WEEKS:
					duration /= (60 * 24 * 7);
					break;
				case TimeUnit::MONTHS:
					if (minutesPerDay != 0 && daysPerMonth != 0)
						duration /= (minutesPerDay * daysPerMonth);
					else
						duration = 0;
					break;
				case TimeUnit::ELAPSED_MONTHS:
					duration /= (60 * 24 * 30);
					break;
				case TimeUnit::YEARS:
					if (minutesPerWeek != 0)
						duration /= (minutesPerWeek * 52);
					else
						duration = 0;
					break;
				case TimeUnit::ELAPSED_YEARS:
					duration /= (60 * 24 * 7 * 52);
					break;
			}
			return Duration(duration, toUnits);
		}

		static Duration convertUnits(double duration, TimeUnit fromUnits, TimeUnit toUnits, ProjectProperties &defaults);

		inline Duration convertUnits(TimeUnit type, ProjectProperties &defaults) const {
			return convertUnits(m_duration, m_units, type, defaults);
		}

		constexpr bool operator ==(const Duration &other) {
			return m_units == other.m_units &&
					::abs(m_duration - other.m_duration) < 0.00001;
		}

		int compareTo(Duration rhs) const {
			if (m_units != rhs.m_units) {
				rhs = convertUnits(rhs.m_duration, rhs.m_units, m_units, (8 * 60), (5 * 8 * 60), 20);
			}
			return ::abs(m_duration - rhs.m_duration) < 0.00001 ? 0 : m_duration < rhs.m_duration ? -1 : 1;
		}
};

static_assert(std::is_polymorphic<Duration>::value == false, "Duration has vtable!");

}

namespace std {
template<>
struct hash<mpxjpp::Duration> {
	size_t operator()(const mpxjpp::Duration &val) const {
		return static_cast<int>(val.units()) + (int)val.duration();
	}
};
}

#endif // DURATION_H