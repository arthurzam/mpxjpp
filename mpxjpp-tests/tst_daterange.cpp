#include <gtest/gtest.h>

#include "mpxjpp/daterange.h"

using mpxjpp::DateRange;
using namespace mpxjpp::common;

TEST(DateRangeTest, Constructor) {
    DateRange d;
    EXPECT_EQ(d.start, DateTime{});
    EXPECT_EQ(d.end, DateTime{});
}

TEST(DateRangeTest, CompareTo) {
    using namespace mpxjpp::common;
    DateTime a = local_days(1998_y/oct/ 5).time_since_epoch();
    DateTime b = local_days(2010_y/apr/18).time_since_epoch();
    DateTime c = local_days(2030_y/may/10).time_since_epoch();
    DateTime d = local_days(2080_y/jan/31).time_since_epoch();

    DateRange r1{a, b}, r2{c, d}, r3{a, c}, r4{a, d};

    EXPECT_TRUE(r3.compareTo(d) > 0);
    EXPECT_TRUE(r4.compareTo(d) == 0);

    EXPECT_TRUE(r1.compareTo(r2) < 0);
    EXPECT_TRUE(r1.compareTo(r4) < 0);
    EXPECT_TRUE(r3.compareTo(r4) > 0);
}
