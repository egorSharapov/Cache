#include <gtest/gtest.h>
#include <cache3.0.hpp>

#include "cache_test.hpp"

std::string get_page(int x) { return std::to_string(x); }

TEST(LFU, AddingTest) {
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

TEST(LFU, HitTest1) {
    std::vector<int> adding_values{1, 2, 1, 2, 1, 2, 1, 2, 1, 2};

    Cache::LFUCache<int, std::string> lfu(4);

    int hits = 0;
    for (const int elem : adding_values) {
        hits += lfu.lookup_update(elem, get_page);
    }

    EXPECT_EQ(hits, 8);
}

TEST(LFU, HitTest2) {
    std::vector<int> adding_values{1, 2, 2, 1, 5, 4, 2, 1, 10, 6, 7, 8, 10, 11, 12, 5};

    Cache::LFUCache<int, std::string> lfu(8);

    int hits = 0;
    for (const int elem : adding_values) {
        hits += lfu.lookup_update(elem, get_page);
    }

    EXPECT_EQ(hits, 5);
}

TEST(Ideal, HitTest1) {
    std::deque<int> future_elements {1, 2, 3, 4, 1, 2, 5, 5, 2, 4, 3, 4, 1};
    Cache::IdealCache<int, std::string> ideal(4); 
    int total = future_elements.size();
    int hits = 0;
    for (int i = 0 ; i < total; ++i) {
        int current = future_elements.front();
        future_elements.pop_front();
        hits += ideal.lookup_update(current, get_page, future_elements);
    }

    EXPECT_EQ(hits, 7);
}

TEST(Ideal, HitTest2) {
    std::deque<int> future_elements {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    Cache::IdealCache<int, std::string> ideal(4); 
    int total = future_elements.size();
    int hits = 0;
    for (int i = 0 ; i < total; ++i) {
        int current = future_elements.front();
        future_elements.pop_front();
        hits += ideal.lookup_update(current, get_page, future_elements);
    }

    EXPECT_EQ(hits, 7);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}