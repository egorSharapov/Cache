#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace Cache {
class CorrectLFU {
  private:
    struct CacheElem {
        int key;
        size_t frequency;
    };

    std::vector<CacheElem> m_cache;
    size_t m_max_load;

    void first_put(int key) { m_cache.push_back({key, 1}); }

  public:
    CorrectLFU(size_t max_load) : m_max_load(max_load){};
    ~CorrectLFU() = default;

    // TODO lookup_update
    bool lookup_update(int key) {
        for (const auto &elem : m_cache) {
            if (elem.key == key) {
                touch(key);
                return true;
            }
        }
        if (m_cache.size() == m_max_load) {
            // find element with minimum frequency;
            auto min = std::min_element(
                m_cache.begin(), m_cache.end(),
                [](const CacheElem &lhs, const CacheElem &rhs) {
                    return lhs.frequency < rhs.frequency;
                });
            m_cache.erase(min);
        }
        first_put(key);
        return false;
    }

    void touch(int key) {
        for (auto &elem : m_cache) {
            if (elem.key == key) {
                elem.frequency += 1;
            }
        }
    }
    std::vector<int> get_keys() {
        std::vector<int> res;
        for (const auto &elem : m_cache) {
            res.push_back(elem.key);
        }
        return res;
    }

    void print() {
        std::cout << "\nDump:\n";
        for (const auto &elem : m_cache) {
            std::cout << "[" << elem.key << "]\t(" << elem.frequency << ")"
                      << "\n";
        }
    }
};

} // namespace Cache
