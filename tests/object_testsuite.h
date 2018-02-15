#define _CXXTEST_HAVE_EH
#define _CXXTEST_HAVE_STD

#include <cxxtest/TestSuite.h>

#include <mpxjpp/common/object.h>
#include <mpxjpp/common/calendar.h>

class ObjectAnyTest : public CxxTest::TestSuite
{
public:
    void testEmpty() {
        mpxjpp::common::any obj;
        TS_ASSERT(obj.empty());
    }

    void testAssignInteger() {
        mpxjpp::common::any obj(2);
        TS_ASSERT(!obj.empty());
        TS_ASSERT(obj.isType<int>());
        TS_ASSERT_EQUALS(obj.cast<int>(), 2);
    }

    void testAssignStringLiteral() {
        mpxjpp::common::any obj("hello world");
        TS_ASSERT(!obj.empty());
        TS_ASSERT(obj.isType<const char *>());
        TS_ASSERT_EQUALS(obj.cast<const char *>(), std::string("hello world"));
    }

    void testBadCast() {
        mpxjpp::common::any obj(2);
        TS_ASSERT_THROWS(obj.cast<double>(), mpxjpp::common::bad_any_cast);
    }

    void testCastDefault() {
        mpxjpp::common::any obj;
        TS_ASSERT_EQUALS(obj.cast<int>(2), 2);
        TS_ASSERT_EQUALS(obj.cast<double>(2.0), 2.0);
        TS_ASSERT_EQUALS(obj.cast<const char *>("a"), std::string("a"));
    }

    void testAssignContinues() {
        mpxjpp::common::any a;
        TS_ASSERT(a.empty());

        TS_TRACE("assigning integer");
        a = 2;
        TS_ASSERT(!a.empty());
        TS_ASSERT(a.isType<int>());
        TS_ASSERT_EQUALS(a.cast<int>(), 2);

        TS_TRACE("assigning string literal");
        a = "hello world";
        TS_ASSERT(!a.empty());
        TS_ASSERT(a.isType<const char *>());
        TS_ASSERT_EQUALS(a.cast<const char *>(), std::string("hello world"));

        TS_TRACE("assigning integer as move");
        mpxjpp::common::any b = 7.0;
        TS_ASSERT_EQUALS(b.cast<double>(), 7.0);
        a = std::move(b);
        TS_ASSERT_EQUALS(a.cast<double>(), 7.0);
        TS_ASSERT(b.empty());
    }

    void testSwap() {
        mpxjpp::common::any a(5), b(1.0);

        TS_ASSERT_EQUALS(a.cast<int>(), 5);
        TS_ASSERT_EQUALS(b.cast<double>(), 1.0);

        a.swap(b);

        TS_ASSERT_EQUALS(b.cast<int>(), 5);
        TS_ASSERT_EQUALS(a.cast<double>(), 1.0);
    }

    void testConstructors() {
        mpxjpp::common::any a(6);
        TS_ASSERT_EQUALS(a.cast<int>(), 6);

        mpxjpp::common::any b(a);
        TS_ASSERT_EQUALS(a.cast<int>(), 6);
        TS_ASSERT_EQUALS(b.cast<int>(), 6);

        mpxjpp::common::any c(std::move(a));
        TS_ASSERT_EQUALS(b.cast<int>(), 6);
        TS_ASSERT_EQUALS(c.cast<int>(), 6);
        TS_ASSERT(a.empty());
    }

    void testCompareTo() {
        mpxjpp::common::any a(6), b(5), c(7), d(6);
        TS_ASSERT(a.compareTo(b) > 0);
        TS_ASSERT(a.compareTo(c) < 0);
        TS_ASSERT(a.compareTo(d) == 0);
        TS_ASSERT(a.compareTo(6) == 0);

        mpxjpp::common::any e(1.0);
        TS_ASSERT_THROWS(a.compareTo(e), mpxjpp::common::bad_any_cast);
        TS_ASSERT_THROWS(a.compareTo(mpxjpp::common::any()), mpxjpp::common::bad_any_cast);
    }

    void testCalendarCast() {
        using namespace mpxjpp::common;
        any a;

        Date d1 = 1998_y/jan/1;
        any_type_cast<Date>::set(a, d1);
        TS_ASSERT_EQUALS(any_type_cast<Date>::get(a, EMPTY_DATE), d1);

        DateTime dt1 = local_days(1998_y/jan/1).time_since_epoch();
        any_type_cast<DateTime>::set(a, dt1);
        TS_ASSERT_EQUALS(any_type_cast<DateTime>::get(a, DateTime::zero()), dt1);
    }
};
