#ifndef DATERANGE_TESTSUITE_H
#define DATERANGE_TESTSUITE_H
#include <cxxtest/TestSuite.h>

#include <mpxjpp/daterange.h>

using namespace mpxjpp;
using namespace mpxjpp::common;

class DateRange_Test : public CxxTest::TestSuite {
public:
    void testConstructor() {
        DateRange d;
        TS_ASSERT_EQUALS(d.start, DateTime::zero());
    }

    void testCompare() {
        DateTime a = local_days(1998_y/oct/ 5).time_since_epoch();
        DateTime b = local_days(2010_y/apr/18).time_since_epoch();
        DateTime c = local_days(2030_y/may/10).time_since_epoch();
        DateTime d = local_days(2080_y/jan/31).time_since_epoch();

        DateRange r1{a, b}, r2{c, d}, r3{a, c}, r4{a, d};

        TS_ASSERT(r3.compareTo(d) > 0);
        TS_ASSERT(r4.compareTo(d) == 0);

        TS_ASSERT(r1.compareTo(r2) < 0);
        TS_ASSERT(r1.compareTo(r4) < 0);
        TS_ASSERT(r3.compareTo(r4) > 0);
    }
};
#endif // DATERANGE_TESTSUITE_H
