#include <cinttypes>
#include <cstddef>
#include <deque>
#include <iterator>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Cache {
template <typename Key, typename Value>
class LFUCache {
  private:
    using freq_key = size_t;

    std::unordered_map<Key, std::pair<Value, freq_key>> m_cache;
    std::multimap<freq_key, Key> m_frequency;
    size_t m_max_load;

    void first_put(Key key, const Value &value) {
        m_cache.insert({key, {value, 1}});
        m_frequency.insert({1, key});
    }

  public:
    LFUCache(size_t max_load) : m_cache(max_load), m_max_load(max_load){};
    ~LFUCache() = default;

    // TODO lookup_update
    template <typename F>
    bool lookup_update(Key key, F get_page) {
        auto res = m_cache.find(key);
        if (res == m_cache.end()) {
            if (m_cache.size() == m_max_load) {
                // find element with minimum frequency
                auto min_freq_elem = m_frequency.begin();
                m_cache.erase(min_freq_elem->second);
                m_frequency.erase(min_freq_elem);
            }
            first_put(key, get_page(key));
            return false;
        } else {
            // update frequncy
            touch(key);
            return true;
        }
    }

    void touch(Key key) {
        std::cout << "touch!\n";
        auto res = m_cache.find(key);
        if (res != m_cache.end()) {
            auto bucket = m_frequency.equal_range(res->second.second);
            for (auto elem = bucket.first; elem != bucket.second; ++elem) {
                if (elem->second == res->first) {
                    std::cout << "found!\n";
                    m_frequency.insert({elem->first + 1, key});
                    m_frequency.erase(elem);
                    res->second.second += 1;
                    break;
                }
            }
        }
    }

    std::pair<Value, bool> get(Key key) {
        auto res = m_cache.find(key);
        if (res != m_cache.end()) {
            return {res->second, true};
        }
        return {{}, false};
    }

    void print() {
        std::cout << "\nDump:\n";
        for (const auto &elem : m_cache) {
            std::cout << "[" << elem.first << "]\t" << elem.second.first
                      << "\t(" << elem.second.second << ")"
                      << "\n";
        }
        for (const auto &elem : m_frequency) {
            std::cout << "(" << elem.first << ")\t"
                      << "[" << elem.second << "]\n";
        }
    }
};

template <typename Key, typename Value>
class IdealCache {
  private:
    std::unordered_map<Key, Value> m_cache;
    size_t m_max_load;

  public:
    IdealCache(size_t max_load) : m_max_load(max_load) {}
    ~IdealCache() = default;

    void first_put(const Key &key, const Value &value) {
        m_cache.insert({key, value});
    }
    template <typename F>
    bool lookup_update(const Key &key, F get_page,
                       const std::deque<Key> &future_pages) {
        if (m_cache.find(key) != m_cache.end()) {
            return true;
        }
        auto last_elem = future_pages.begin();
        if (m_cache.size() == m_max_load) {
            for (const auto &elem : m_cache) {
                auto res = std::find(future_pages.begin(), future_pages.end(),
                                     elem.first);
                if (res == future_pages.end()) {
                    m_cache.erase(elem.first);
                    first_put(key, get_page(key));
                    return false;
                }
                if (res > last_elem) {
                    last_elem = res;
                }
            }
            m_cache.erase(*last_elem);
        }
        first_put(key, get_page(key));
        return false;
    }
    void print() {
        std::cout << "Dump:\n";
        for (const auto &elem : m_cache) {
            std::cout << "[" << elem.first << "]\t" << elem.second << "\n";
        }
        std::cout << "\n";
    }
};
} // namespace Cache
