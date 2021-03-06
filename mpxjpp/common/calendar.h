#ifndef CALENDAR_H
#define CALENDAR_H

#include "deps/date.h"
#include "object.h"

namespace mpxjpp {
namespace common {

using namespace std::chrono;
using namespace date;
using namespace std::chrono_literals;

using Date = year_month_day;
using Time = duration<int32_t>;
using DateTime = seconds;

using Calendar = DateTime;

constexpr Date EMPTY_DATE(year(0), month(0), day(0));

namespace anyimpl {
template<>
struct compare_to<seconds> {
    int operator()(seconds a, seconds b) {
        return static_cast<int>((a - b).count());
    }
};
}

constexpr inline year_month_day normalize(year_month_day ymd)
{
    ymd += months{0};
    ymd = sys_days{ymd};
    return ymd;
}

//template<>
//struct any_type_cast<Date> {
//  using type = Date;
//  using castType = int32_t;

//  static type get(const any &a, type def) {
//      union {
//          castType n;
//          type t;
//      } u;
//      u.t = def;
//        u.n = a.cast<castType>(u.n);
//      return u.t;
//  }

//  static void set(any &a, type var) {
//      union {
//          castType n;
//          type t;
//      } u;
//      u.t = var;
//      a.assign<castType>(u.n);
//  }
//};

static_assert(anyimpl::is_small<Date>, "Date isn't small enough");

//template<>
//struct any_type_cast<DateTime> {
//  using type = DateTime;
//  using castType = DateTime::rep;

//  static type get(const any &a, type def) {
//      return type(a.cast<castType>(def.count()));
//  }

//  static void set(any &a, type var) {
//      a.assign<castType>(var.count());
//  }
//};
static_assert(anyimpl::is_small<DateTime>, "DateTime isn't small enough");

}
}
#endif // CALENDAR_H
