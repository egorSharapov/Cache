#include "cache2.0.hpp"
#include <deque>
#include <string>

std::string get_page(int key) { return "string" + std::to_string(key); }

template <typename Key>
void print_pages(const std::deque<Key>& pages) {
    std::cout << "pages: ";
    for (const auto& page: pages) {
        std::cout << page << ", ";
    }
    std::cout << "\n";
}

int main() {
    Cache::IdealCache<int, std::string> ideal(3);

    std::deque<int> future_pages = {1, 2, 3, 1, 1, 4, 2, 3, 1};
    int page_count = future_pages.size();
    for (int i = 0; i < page_count; ++i) {
        int next_page = future_pages.front();
        future_pages.pop_front();
        print_pages(future_pages);
        ideal.lookup_update(next_page, get_page, future_pages);
        ideal.print();
    }
    return 0;
}