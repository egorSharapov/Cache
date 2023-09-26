#include "cache2.0.hpp"
#include <string>
#include <iostream>

std::string get_page(int key) { return "page " + std::to_string(key); }


int main() {
    size_t hits = 0;
    std::cout << "LFUCache:\nIn: ";
    size_t size = 0;
    std::cin >> size;
    Cache::LFUCache<int, std::string> lfu(size);

    std::vector<int> elements;
    size_t elements_count = 0;
    std::cin >> elements_count;
    for (int i = 0; i < elements_count; ++i) {
        int element = 0;
        std::cin >> element;
        if (lfu.lookup_update(element, get_page)) {
            hits += 1;
        }
    }
    std::cout << "Out: " << hits << "\n";
    elements_count = 0;
    hits = 0;

    std::cout << "IdealCache:\nIn: ";
    std::cin >> size;
    Cache::IdealCache<int, std::string> ideal(size);

    std::deque<int> future_elements;
    std::cin >> elements_count;
    for (int i = 0; i < elements_count; ++i) {
        int element = 0;
        std::cin >> element;
        future_elements.push_back(element);
    }

    for (int i = 0 ; i < elements_count; ++i) {
        int current = future_elements.front();
        future_elements.pop_front();
        if (ideal.lookup_update(current, get_page, future_elements)) {
            hits += 1;
        }
    }
    std::cout << "Out: " << hits << "\n";
}