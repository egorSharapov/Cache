#include <deque>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace Cache {
template <typename Frequency, typename Key>
class FrequencyStorage {
  private:
    using It = typename std::list<Key>::iterator;
    std::unordered_map<Frequency, std::list<Key>> m_storage;
    size_t m_min_frequency;

  public:
    void add_first_key(const Key key) {
        m_min_frequency = 1;
        m_storage[m_min_frequency].push_back(key);
    }
    void add_key(const Key key, Frequency freq) {
        m_storage[freq].push_back(key);
    }

    size_t get_min() const { return m_min_frequency; }
    It get_last(Frequency freq) { return --m_storage[freq].end(); }
    It get_first() { return m_storage[m_min_frequency].begin(); }

    void update_min() { m_min_frequency += m_storage[m_min_frequency].empty(); }

    void remove_first_key(Frequency freq) { m_storage[freq].pop_front(); }
    void remove(Frequency freq, It it) { m_storage[freq].erase(it); }
};

template <typename Key, typename Value>
class LFUCache {
  private:
    using Frequency = size_t;
    using It = typename std::list<Key>::iterator;
    struct Elem {
        Value val;
        Frequency frequency;
        It it;
    };

    std::unordered_map<Key, Elem> m_cache;
    FrequencyStorage<Frequency, Key> m_freq;
    size_t m_max_load;

  public:
    LFUCache(size_t max_load) : m_cache(max_load), m_max_load(max_load){};

    template <typename F>
    bool lookup_update(Key key, F get_page) {
        if (m_cache.find(key) == m_cache.end()) {
            if (m_cache.size() == m_max_load) {
                // delete elem with minimum frequency
                m_cache.erase(*m_freq.get_first());
                m_freq.remove_first_key(m_freq.get_min());
            }

            m_freq.add_first_key(key);
            m_cache[key] = {get_page(key), m_freq.get_min(),
                            m_freq.get_last(m_freq.get_min())}; // 5
            return false;
        }
        touch(key);
        return true;
    }

    void touch(Key key) {
        auto &elem = m_cache[key];

        // delete old frequency by iterator
        m_freq.remove(m_cache[key].frequency, elem.it);

        // update element frequency
        m_cache[key].frequency += 1;
        m_freq.add_key(key, m_cache[key].frequency);

        m_cache[key].it = m_freq.get_last(m_cache[key].frequency); // 5

        // update frequency minimum
        m_freq.update_min();
    }

    bool contains(Key key) const { return m_cache.find(key) != m_cache.end(); }

    void print() const {
        std::cout << "\nDump:\n";
        for (const auto &elem : m_cache) {
            std::cout << "[" << elem.first << "]\t" << elem.second.val << "\t("
                      << elem.second.frequency << ")"
                      << "\n";
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

    void print() const {
        std::cout << "Dump:\n";
        for (const auto &elem : m_cache) {
            std::cout << "[" << elem.first << "]\t" << elem.second << "\n";
        }
        std::cout << "\n";
    }
};
} // namespace Cache
