#include <gtest/gtest.h>

#include "mpxjpp/common/object.h"

using namespace testing;

namespace common {

using namespace mpxjpp::common;

TEST(AnyTest, Empty) {
    any obj;
    EXPECT_TRUE(obj.empty());
}

TEST(AnyTest, AssignInteger) {
    any obj(2);
    EXPECT_FALSE(obj.empty());
    EXPECT_TRUE(obj.isType<int>());
    EXPECT_EQ(obj.cast<int>(), 2);
}

TEST(AnyTest, AssignStringLiteral) {
    any obj("hello world");
    EXPECT_FALSE(obj.empty());
    EXPECT_FALSE(obj.isType<std::string>());
    EXPECT_TRUE(obj.isType<const char *>());
    EXPECT_STREQ(obj.cast<const char *>(), "hello world");
}

TEST(AnyTest, AssignNullptr) {
    any obj(nullptr);
    EXPECT_TRUE(obj.empty());
    EXPECT_TRUE(obj.compareTo(any()) == 0);

    any ass(1);
    EXPECT_EQ(ass.cast<int>(), 1);
    ass = nullptr;
    EXPECT_TRUE(ass.empty());
}

TEST(AnyTest, BadCast) {
    any obj(2);
    EXPECT_THROW(obj.cast<double>(), bad_any_cast);
}

TEST(AnyTest, CastDefault) {
    any obj;
    EXPECT_EQ(obj.get<int>(2), 2);
    EXPECT_EQ(obj.get<double>(2.0), 2.0);
    EXPECT_STREQ(obj.get<const char *>("string"), "string");
    EXPECT_EQ(obj.get<std::string>("string"), "string");
}

TEST(AnyTest, AssignContinues) {
    any a;
    EXPECT_TRUE(a.empty());

    a = 2;
    EXPECT_FALSE(a.empty());
    EXPECT_TRUE(a.isType<int>());
    EXPECT_EQ(a.cast<int>(), 2);

    a = "hello world";
    EXPECT_FALSE(a.empty());
    EXPECT_TRUE(a.isType<const char *>());
    EXPECT_STREQ(a.cast<const char *>(), "hello world");

    any b = 7.0;
    EXPECT_EQ(b.cast<double>(), 7.0);
    a = std::move(b);
    EXPECT_EQ(a.cast<double>(), 7.0);
    EXPECT_STREQ(b.cast<const char *>(), "hello world");
}

TEST(AnyTest, Swap) {
    any a(5), b(1.0);

    EXPECT_EQ(a.cast<int>(), 5);
    EXPECT_EQ(b.cast<double>(), 1.0);

    a.swap(b);

    EXPECT_EQ(b.cast<int>(), 5);
    EXPECT_EQ(a.cast<double>(), 1.0);
}

TEST(AnyTest, Constructors) {
    any a(6);
    EXPECT_EQ(a.cast<int>(), 6);

    any b(a);
    EXPECT_EQ(a.cast<int>(), 6);
    EXPECT_EQ(b.cast<int>(), 6);

    any c(std::move(a));
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.cast<int>(), 6);
    EXPECT_EQ(c.cast<int>(), 6);
}

TEST(AnyTest, CompareTo) {
    EXPECT_TRUE(any().compareTo(any()) == 0);

    any a(6), b(5), c(7), d(6);
    EXPECT_TRUE(a.compareTo(b) > 0);
    EXPECT_TRUE(a.compareTo(c) < 0);
    EXPECT_TRUE(a.compareTo(d) == 0);
    EXPECT_TRUE(a.compareTo(6) == 0);

    any e(1.0);
    EXPECT_THROW(a.compareTo(e), bad_any_cast) << "can't compare with different type";
    EXPECT_THROW(a.compareTo(any()), bad_any_cast) << "can't compare with empty";

    any s1(std::string("a")), s2(std::string("b")), s3(std::string("c"));
    EXPECT_EQ(s1.cast<std::string>(), std::string("a"));
    EXPECT_TRUE(s1.compareTo(s2) < 0);
    EXPECT_TRUE(s2.compareTo(s3) < 0);
    EXPECT_TRUE(s3.compareTo(s1) > 0);
    EXPECT_TRUE(s1.compareTo(s1) == 0);

    any v1(std::vector<int>{1, 2}), v2(std::vector<int>{2, 3});
    EXPECT_THROW(v1.compareTo(v2), std::invalid_argument) << "undefined compareTo for std::vector";
}

TEST(AnyTest, Equals) {
    any a(5), b(5.0), c(5), d(6);

    EXPECT_TRUE(a == c);
    EXPECT_FALSE(a == d);
    EXPECT_THROW(a == b, bad_any_cast);
}

}
