#ifndef TIMEPHASEDITEM_H
#define TIMEPHASEDITEM_H

#include <ctime>
#include <vector>

#include "duration.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

class ProjectCalendar;

template <typename T>
class TimephasedItem {
	private:
		std::time_t m_start;
		T m_totalAmount;
		std::time_t m_finish;
		T m_amountPerDay;
		bool m_modified;
	public:
		TimephasedItem() = default;
		TimephasedItem(const TimephasedItem &) = default;
		TimephasedItem(std::time_t start, std::time_t finish, bool modified, T totalAmount, T amountPerDay)
			: m_start(start), m_totalAmount(totalAmount), m_finish(finish),
			  m_amountPerDay(amountPerDay), m_modified(modified)
		{}

		MPXJPP_GETTER_SETTER(start, std::time_t)
		MPXJPP_GETTER_SETTER(totalAmount, T)
		MPXJPP_GETTER_SETTER(finish, std::time_t)
		MPXJPP_GETTER_SETTER(amountPerDay, T)
		MPXJPP_GETTER_SETTER(modified, bool)

		bool operator ==(const TimephasedItem<T> &other) {
			return this->m_start == other.m_start &&
				   this->m_finish == other.m_finish &&
				   this->m_totalAmount == other.m_totalAmount &&
				   this->m_amountPerDay == other.m_amountPerDay;
		}
};

template <typename T>
class TimephasedItemContainer {
	public:
		virtual const std::vector<TimephasedItem<T>> &data() const = 0;
		virtual bool hasData() const = 0;
};

template <typename T>
class TimephasedItemNormaliser {
public:
	virtual void normalise(ProjectCalendar &calendar, std::vector<TimephasedItem<T>> & list) = 0;
};

using TimephasedCost = TimephasedItem<double>;
using TimephasedCostContainer = TimephasedItemContainer<TimephasedCost>;
using TimephasedCostNormaliser = TimephasedItemNormaliser<double>;

class TimephasedWork final : public TimephasedItem<Duration> {
	public:
		TimephasedWork() = default;
		TimephasedWork(const TimephasedWork &other) = default;

		TimephasedWork(const TimephasedWork &other, double totalFactor, double perDayFactor) :
			TimephasedItem<Duration>(other.start(), other.finish(), other.modified(),
									 Duration(other.totalAmount().duration() * totalFactor, other.totalAmount().units()),
									 Duration(other.amountPerDay().duration() * perDayFactor, other.amountPerDay().units()))
		{}
};

using TimephasedWorkContainer = TimephasedItemContainer<Duration>;
using TimephasedWorkNormaliser = TimephasedItemNormaliser<Duration>;
}

namespace std {

template <typename T>
struct hash<mpxjpp::TimephasedItem<T>> {
	size_t operator()(const mpxjpp::TimephasedItem<T> &val) const {
		return static_cast<size_t>(val.start() + val.finish() +
								   std::hash<T>(val.totalAmount()) +
								   std::hash<T>(val.amountPerDay()));
	}
};

}

#endif // TIMEPHASEDITEM_H
