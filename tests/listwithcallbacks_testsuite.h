#ifndef LISTWITHCALLBACKS_TESTSUITE_H
#define LISTWITHCALLBACKS_TESTSUITE_H

#include <cxxtest/TestSuite.h>

#include <mpxjpp/listwithcallbacks.h>

class ListWithCallbacks_Test : public CxxTest::TestSuite {
private:
    class SubListTest : public mpxjpp::ListWithCallbacks<int> {
        protected:
            virtual void added(int index) override {
                throw int(at(index));
            }
            virtual void removed(const int &num) override {
                throw int(num);
            }
            virtual void replaced(const int &older, const int &newer) override {
                throw int(newer * 0x10000 + older);
            }
    };
public:
    void testAddSignal() {
        SubListTest s;
        TS_ASSERT_THROWS_EQUALS(s.add(2), int n, n, 2);
        TS_ASSERT_EQUALS(s.size(), 1);
        TS_ASSERT_THROWS_EQUALS(s.add(3), int n, n, 3);
        TS_ASSERT_THROWS_EQUALS(s.add(5), int n, n, 5);
        TS_ASSERT_EQUALS(s.size(), 3);

        TS_ASSERT_THROWS_EQUALS(s.add(4, 1), int n, n, 4);
        TS_ASSERT_EQUALS(s.at(1), 4);
        TS_ASSERT_EQUALS(s.at(2), 3);
    }

    void testRemoveSignal() {
        SubListTest s;
        TS_ASSERT_THROWS_ANYTHING(s.add(2));
        TS_ASSERT_THROWS_ANYTHING(s.add(3));
        TS_ASSERT_THROWS_ANYTHING(s.add(5));
        TS_ASSERT_EQUALS(s.size(), 3);

        TS_ASSERT_THROWS_EQUALS(s.remove(1), int n, n, 3);
        TS_ASSERT_EQUALS(s.size(), 2);
        TS_ASSERT_THROWS_EQUALS(s.remove(1), int n, n, 5);
        TS_ASSERT_EQUALS(s.size(), 1);
        TS_ASSERT_THROWS_EQUALS(s.remove(0), int n, n, 2);
        TS_ASSERT(s.empty());
    }

    void testReplaceSignal() {
        SubListTest s;
        TS_ASSERT_THROWS_ANYTHING(s.add(2));
        TS_ASSERT_THROWS_ANYTHING(s.add(3));
        TS_ASSERT_THROWS_ANYTHING(s.add(5));
        TS_ASSERT_EQUALS(s.size(), 3);

        TS_ASSERT_THROWS_EQUALS(s.set(1, 6), int n, n, 6 * 0x10000 + 3);
        TS_ASSERT_EQUALS(s.at(1), 6);
    }
};
#endif // LISTWITHCALLBACKS_TESTSUITE_H
