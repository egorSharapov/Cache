#include <deque>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace Cache {
template <typename Key, typename Value>
class LFUCache {
  private:
    using frequency_t = size_t;
    using list_it = typename std::list<Key>::iterator;

    struct Elem {
        Value val;
        frequency_t frequency;
        list_it it;
    };

    std::unordered_map<Key, Elem> m_cache;
    std::unordered_map<frequency_t, std::list<Key>> m_freq;
    // std::unordered_map<Key, size_t> m_time_stamp;

    size_t m_max_load;
    frequency_t m_min_freq;

  public:
    LFUCache(size_t max_load) : m_cache(max_load), m_max_load(max_load){};

    template <typename F>
    bool lookup_update(Key key, F get_page) {
        if (m_cache.find(key) == m_cache.end()) {
            if (m_cache.size() == m_max_load) {
                // delete elem with minimum frequency
                m_cache.erase(m_freq[m_min_freq].front());
                m_freq[m_min_freq].pop_front();
            }

            m_min_freq = 1;
            m_freq[m_min_freq].push_back(key);
            m_cache[key] = {get_page(key), m_min_freq,
                            --m_freq[m_min_freq].end()};
            return false;
        }
        touch(key);
        return true;
    }

    void touch(Key key) {
        auto &elem = m_cache[key];

        // delete old frequency by iterator
        m_freq[m_cache[key].frequency].erase(elem.it);
;
        // update element frequency
        m_freq[++m_cache[key].frequency].push_back(key);
        // update iterator
        m_cache[key].it = --m_freq[m_cache[key].frequency].end();

        // update frequency minimum
        m_min_freq += m_freq[m_min_freq].empty();
    }

    bool contains(Key key) { return m_cache.find(key) != m_cache.end(); }
    void print() {
        std::cout << "\nDump:\n";
        for (const auto &elem : m_cache) {
            std::cout << "[" << elem.first << "]\t" << elem.second.val << "\t("
                      << elem.second.frequency << ")"
                      << "\n";
        }
    }
    void print_keys(Key key) {
        if (m_freq.find(key) != m_freq.end()) {
            auto res = m_freq[key];
            for (const auto &elem : res) {
                std::cout << elem << "->";
            }
            std::cout << "\n";
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

        auto res = std::find(future_pages.begin(), future_pages.end(), key);
        if (res == future_pages.end()) {
            return false;
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
