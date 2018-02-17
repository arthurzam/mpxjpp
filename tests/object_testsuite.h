#define _CXXTEST_HAVE_EH
#define _CXXTEST_HAVE_STD

#include <cxxtest/TestSuite.h>

#include <mpxjpp/common/object.h>
#include <mpxjpp/common/calendar.h>

using mpxjpp::common::any;

class ObjectAnyTest : public CxxTest::TestSuite
{
public:
    void testEmpty() {
        any obj;
        TS_ASSERT(obj.empty());
    }

    void testAssignInteger() {
        any obj(2);
        TS_ASSERT(!obj.empty());
        TS_ASSERT(obj.isType<int>());
        TS_ASSERT_EQUALS(obj.cast<int>(), 2);
    }

    void testAssignStringLiteral() {
        any obj("hello world");
        TS_ASSERT(!obj.empty());
        TS_ASSERT(obj.isType<const char *>());
        TS_ASSERT_EQUALS(obj.cast<const char *>(), std::string("hello world"));
    }

    void testBadCast() {
        any obj(2);
        TS_ASSERT_THROWS(obj.cast<double>(), mpxjpp::common::bad_any_cast);
    }

    void testCastDefault() {
        any obj;
        TS_ASSERT_EQUALS(obj.cast<int>(2), 2);
        TS_ASSERT_EQUALS(obj.cast<double>(2.0), 2.0);
        TS_ASSERT_EQUALS(obj.cast<const char *>("a"), std::string("a"));
    }

    void testAssignContinues() {
        any a;
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
        any b = 7.0;
        TS_ASSERT_EQUALS(b.cast<double>(), 7.0);
        a = std::move(b);
        TS_ASSERT_EQUALS(a.cast<double>(), 7.0);
        TS_ASSERT_EQUALS(b.cast<const char *>(), std::string("hello world"));
    }

    void testSwap() {
        any a(5), b(1.0);

        TS_ASSERT_EQUALS(a.cast<int>(), 5);
        TS_ASSERT_EQUALS(b.cast<double>(), 1.0);

        a.swap(b);

        TS_ASSERT_EQUALS(b.cast<int>(), 5);
        TS_ASSERT_EQUALS(a.cast<double>(), 1.0);
    }

    void testConstructors() {
        any a(6);
        TS_ASSERT_EQUALS(a.cast<int>(), 6);

        any b(a);
        TS_ASSERT_EQUALS(a.cast<int>(), 6);
        TS_ASSERT_EQUALS(b.cast<int>(), 6);

        any c(std::move(a));
        TS_ASSERT(a.empty());
        TS_ASSERT_EQUALS(b.cast<int>(), 6);
        TS_ASSERT_EQUALS(c.cast<int>(), 6);
    }

    void testCompareTo() {
        TS_ASSERT(any().compareTo(any()) == 0);

        TS_TRACE("comparing integers");
        any a(6), b(5), c(7), d(6);
        TS_ASSERT(a.compareTo(b) > 0);
        TS_ASSERT(a.compareTo(c) < 0);
        TS_ASSERT(a.compareTo(d) == 0);
        TS_ASSERT(a.compareTo(6) == 0);

        TS_TRACE("checking bad cast over compare");
        any e(1.0);
        TS_ASSERT_THROWS(a.compareTo(e), mpxjpp::common::bad_any_cast);
        TS_ASSERT_THROWS(a.compareTo(any()), mpxjpp::common::bad_any_cast);

        TS_TRACE("comparing strings");
        any s1(std::string("a")), s2(std::string("b")), s3(std::string("c"));
        TS_ASSERT_EQUALS(s1.cast<std::string>(), std::string("a"));
        TS_ASSERT(s1.compareTo(s2) < 0);
        TS_ASSERT(s2.compareTo(s3) < 0);
        TS_ASSERT(s3.compareTo(s1) > 0);
        TS_ASSERT(s1.compareTo(s1) == 0);

        TS_TRACE("testing throwing comparing");
        any v1(std::vector<int>{1, 2}), v2(std::vector<int>{2, 3});
        TS_ASSERT_THROWS(v1.compareTo(v2), std::invalid_argument);
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
