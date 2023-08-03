#pragma once

#include <unordered_map>
#include <mutex>
#include <functional>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(const Hash& hasher = Hash()) : ConcurrentHashMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentHashMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentHashMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentHashMap(int expected_size, int expected_threads_count, const Hash& hasher = Hash())
        :  // 0 because unordered_map doesn't contain constructor with hasher
          table_(0, hasher) {
        (void)expected_threads_count;

        if (expected_size != kUndefinedSize) {
            table_.reserve(expected_size);
        }
    }

    bool Insert(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto insert_pair = table_.emplace(key, value);
        return insert_pair.second;
    }

    bool Erase(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        return table_.erase(key);
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        table_.clear();
    }

    std::pair<bool, V> Find(const K& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = table_.find(key);
        return it == table_.end() ? std::make_pair(false, V()) : std::make_pair(true, it->second);
    }

    const V At(const K& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return table_.at(key);
    }

    size_t Size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return table_.size();
    }

    static const int kDefaultConcurrencyLevel;
    static const int kUndefinedSize;

private:
    std::unordered_map<K, V, Hash> table_;
    mutable std::mutex mutex_;
};

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kDefaultConcurrencyLevel = 8;

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kUndefinedSize = -1;
