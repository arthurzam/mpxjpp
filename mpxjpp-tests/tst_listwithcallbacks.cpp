#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mpxjpp/listwithcallbacks.h"

using ::testing::_;
using namespace mpxjpp;

class MockList final : public ListWithCallbacks<int> {
public:
    MOCK_METHOD1(added, void(unsigned index));
    MOCK_METHOD1(removed, void(const int &num));
    MOCK_METHOD2(replaced, void(const int &older, const int &newer));
};

TEST(ListWithCallbacksTest, AddSignal) {
    MockList list;

    EXPECT_CALL(list, added(0)) .Times(1);
    list.add(2);
    EXPECT_EQ(list[0], 2);

    EXPECT_CALL(list, added(1)) .Times(1);
    EXPECT_CALL(list, added(2)) .Times(1);
    list.add(3);
    list.add(5);
    EXPECT_EQ(list[1], 3);
    EXPECT_EQ(list[2], 5);

    EXPECT_CALL(list, added(1))
            .Times(1);
    list.add(4, 1);
    EXPECT_EQ(list[1], 4);
    EXPECT_EQ(list[2], 3);
}

TEST(ListWithCallbacksTest, RemoveSignal) {
    MockList list;
    EXPECT_CALL(list, added(_)) .Times(3);
    list.add(2);
    list.add(3);
    list.add(5);
    EXPECT_EQ(list.size(), 3);

    EXPECT_CALL(list, removed(3)) .Times(1);
    list.remove(1);
    EXPECT_CALL(list, removed(5)) .Times(1);
    list.remove(1);
    EXPECT_CALL(list, removed(2)) .Times(1);
    list.remove(0);
    EXPECT_EQ(list.size(), 0);
}

TEST(ListWithCallbacksTest, ReplaceSignal) {
    MockList list;
    EXPECT_CALL(list, added(_)) .Times(3);
    list.add(2);
    list.add(3);
    list.add(5);
    EXPECT_EQ(list.size(), 3);

    EXPECT_CALL(list, replaced(3, 6)) .Times(1);
    EXPECT_EQ(list.set(1, 6), 3);
    EXPECT_EQ(list[1], 6);
}
