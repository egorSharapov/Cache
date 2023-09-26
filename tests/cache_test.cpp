#include <gtest/gtest.h>

#include "../src/cache2.0.hpp"
#include "cache_test.hpp"

std::string get_page(int x) { return std::to_string(x); }

TEST(BasicTests, AddingTest) {
    std::vector<int> adding_values{1, 1, 2, 3, 4, 5, 6, 6, 5, 4,
                                   3, 2, 1, 3, 3, 4, 2, 2};

    Cache::LFUCache<int, std::string> lfu(5);
    Cache::CorrectLFU correct_lfu(5);

    for (const int elem : adding_values) {
        lfu.lookup_update(elem, get_page);
        correct_lfu.lookup_update(elem);
    }

    for (const int elem : correct_lfu.get_keys()) {
        EXPECT_TRUE(lfu.contains(elem));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}