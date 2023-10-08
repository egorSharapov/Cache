#include "cache2.0.hpp"
#include <string>
#include <iostream>

std::string get_page(int key) { return "page " + std::to_string(key); }

int main() {
    size_t hits = 0;
    size_t size = 0;
    std::cin >> size;
    Cache::LFUCache<int, std::string> lfu(size);

    std::vector<int> elements;
    size_t elements_count = 0;
    std::cin >> elements_count;
    for (int i = 0; i < elements_count; ++i) {
        int element = 0;
        std::cin >> element;
        hits += lfu.lookup_update(element, get_page);
    }
    std::cout << hits << "\n";

    return 0;
}