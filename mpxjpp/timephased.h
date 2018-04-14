#ifndef TIMEPHASEDITEM_H
#define TIMEPHASEDITEM_H

#include <ctime>
#include <vector>
#include <functional>

#include "common/calendar.h"
#include "duration.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

class ProjectCalendar;

template <typename T>
class TimephasedItem {
        static_assert(std::is_same<T, double>::value || std::is_same<T, Duration>::value,
                      "TimephasedItem should be used with either double or Duration types");
    private:
        T m_totalAmount;
        T m_amountPerDay;
        common::DateTime m_start;
        common::DateTime m_finish;
        bool m_modified;
    public:
        TimephasedItem() = default;
        TimephasedItem(const TimephasedItem &) = default;
        TimephasedItem(common::DateTime start, common::DateTime finish, bool modified, T totalAmount, T amountPerDay)
            : m_totalAmount(totalAmount), m_amountPerDay(amountPerDay),
              m_start(start), m_finish(finish), m_modified(modified)
        {}

        TimephasedItem(const TimephasedItem &other, double totalFactor, double perDayFactor) :
            TimephasedItem(other.start(), other.finish(), other.modified(),
                           other.totalAmount() * totalFactor, other.amountPerDay() * perDayFactor)
        {}

        MPXJPP_GETTER_SETTER(start, common::DateTime)
        MPXJPP_GETTER_SETTER(totalAmount, T)
        MPXJPP_GETTER_SETTER(finish, common::DateTime)
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
        using data_type = std::vector<TimephasedItem<T>>;

        virtual const data_type &data() const = 0;
        virtual bool hasData() const = 0;
};

template <typename T>
using TimephasedItemNormaliser = std::function<void(ProjectCalendar &calendar, std::vector<TimephasedItem<T>> &list)>;

using TimephasedCost = TimephasedItem<double>;
using TimephasedCostContainer = TimephasedItemContainer<double>;
using TimephasedCostNormaliser = TimephasedItemNormaliser<double>;

using TimephasedCost = TimephasedItem<double>;
using TimephasedWorkContainer = TimephasedItemContainer<Duration>;
using TimephasedWorkNormaliser = TimephasedItemNormaliser<Duration>;

template <typename T>
class DefaultTimephasedItemContainer final : public TimephasedItemContainer<T> {
    using data_type = typename TimephasedItemContainer<T>::data_type;
private:
    data_type m_data;
    ProjectCalendar &m_calendar;
    TimephasedItemNormaliser<T> m_normaliser;
    bool m_raw;
public:
    DefaultTimephasedItemContainer(ProjectCalendar &calendar, TimephasedItemNormaliser<T> normaliser,
                                   const data_type &data, bool raw) :
        m_data(data), m_calendar(calendar), m_normaliser(normaliser), m_raw(raw)
    { }

    DefaultTimephasedItemContainer(const DefaultTimephasedItemContainer &source, double perDayFactor, double totalFactor) :
        m_calendar(source.m_calendar), m_normaliser(source.m_normaliser), m_raw(source.m_raw) {
        m_data.reserve(source.m_data.size());
        for (const auto &item : source.m_data)
            m_data.emplace_back(item, totalFactor, perDayFactor);
    }

    const data_type &data() const override {
        return m_data;
    }

    bool hasData() const override {
        return !m_data.empty();
    }
};

using DefaultTimephasedCostContainer = DefaultTimephasedItemContainer<double>;
using DefaultTimephasedWorkContainer = DefaultTimephasedItemContainer<Duration>;

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
