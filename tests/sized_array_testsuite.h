#define _CXXTEST_HAVE_EH
#define _CXXTEST_HAVE_STD

#include <cxxtest/TestSuite.h>

#include <mpxjpp/common/sized_array.h>

class SizedArrayTest : public CxxTest::TestSuite
{
public:
    void testAllocateIntVector() {
        using mpxjpp::common::sized_array;

        std::vector<int> vec = {1, 2, 3};
        sized_array<int> *ptr = new (3) sized_array<int>(vec);
        sized_array<int> &ref = *ptr;
        TS_ASSERT_EQUALS(ref.size(), 3);
        TS_ASSERT_EQUALS(ref[0], 1);
        TS_ASSERT_EQUALS(ref[1], 2);
        TS_ASSERT_EQUALS(ref[2], 3);
        delete ptr;
    }

    void testAllocateStringVector() {
        using mpxjpp::common::sized_array;

        std::vector<std::string> vec{"a", "b", "c"};
        sized_array<std::string> *ptr = new (vec) sized_array<std::string>(vec);
        sized_array<std::string> &ref = *ptr;
        TS_ASSERT_EQUALS(ref[0], std::string("a"));
        TS_ASSERT_EQUALS(ref[1], std::string("b"));
        TS_ASSERT_EQUALS(ref[2], std::string("c"));
        ptr->emplace(1, "d");
        TS_ASSERT_EQUALS(ref[1], std::string("d"));
        delete ptr;
    }

    void testAllocateStringVectorConvert() {
        using mpxjpp::common::sized_array;

        std::vector<const char *> vec{"a", "b", "c"};
        sized_array<std::string> *ptr = new (vec) sized_array<std::string>(vec);
        sized_array<std::string> &ref = *ptr;
        TS_ASSERT_EQUALS(ref[0], std::string("a"));
        TS_ASSERT_EQUALS(ref[1], std::string("b"));
        TS_ASSERT_EQUALS(ref[2], std::string("c"));
        delete ptr;
    }

    void testCopyAllocate() {
        using mpxjpp::common::sized_array;

        std::vector<const char *> vec{"a", "b", "c"};
        sized_array<std::string> *first = new (vec) sized_array<std::string>(vec);

        sized_array<std::string> *second = new (*first) sized_array<std::string>(*first);
        {
            sized_array<std::string> &ref = *second;
            TS_ASSERT_EQUALS(ref[0], std::string("a"));
            TS_ASSERT_EQUALS(ref[1], std::string("b"));
            TS_ASSERT_EQUALS(ref[2], std::string("c"));
        }

        delete first;
        {
            sized_array<std::string> &ref = *second;
            TS_ASSERT_EQUALS(ref[0], std::string("a"));
            TS_ASSERT_EQUALS(ref[1], std::string("b"));
            TS_ASSERT_EQUALS(ref[2], std::string("c"));
        }

        delete second;
    }

    void testCtorDtor() {
        using mpxjpp::common::sized_array;

        static int counter = 0;
        struct item_counter {
            item_counter() {
                counter += 1;
            }
            ~item_counter() {
                counter -= 1;
            }
        };
        sized_array<item_counter> *g = new (5) sized_array<item_counter>(5);
        TS_ASSERT_EQUALS(counter, 5);
        g->emplace(0);
        TS_ASSERT_EQUALS(counter, 5);
        delete g;
        TS_ASSERT_EQUALS(counter, 0);
    }
};
