#pragma once

#include <algorithm>
#include <atomic>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <utility>
#include <vector>

#include <iostream>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(const Hash& hasher = Hash()) : ConcurrentHashMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentHashMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentHashMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentHashMap(int expected_size, int expected_threads_count, const Hash& hasher = Hash())
        : values_((expected_size + expected_threads_count) / expected_threads_count *
                  expected_threads_count),
          mutexes_(expected_threads_count),
          hasher_(hasher) {
    }

    bool Insert(const K& key, const V& value) {
        const auto& mutex_id = GetMutexIndex(key);
        auto guard = std::unique_lock{mutexes_[mutex_id], std::defer_lock};
        guard.lock();
        const auto& id = GetIndex(key);

        for (auto el : values_[id]) {
            if (el.first == key) {
                guard.unlock();
                return false;
            }
        }
        load_.fetch_add(1);
        values_[id].emplace_back(key, value);

        if (static_cast<double>(load_.load()) / values_.size() >= kLoadFactor) {
            guard.unlock();
            std::vector<std::unique_lock<std::mutex>> locker;
            for (size_t i = 0; i < mutexes_.size(); ++i) {
                locker.emplace_back(mutexes_[i], std::defer_lock);
                locker.back().lock();
            }

            std::vector<std::list<std::pair<K, V>>> values(values_.size() * 3);

            for (auto& list : values_) {
                while (!list.empty()) {
                    values[hasher_(list.back().first) % values.size()].emplace_back(
                        list.back().first, list.back().second);
                    list.pop_back();
                }
            }
            values_.clear();
            values_.swap(values);

            for (int i = locker.size() - 1; i >= 0; --i) {
                locker[i].unlock();
            }
        } else {
            guard.unlock();
        }
        return true;
    }

    bool Erase(const K& key) {
        const auto& mutex_id = GetMutexIndex(key);
        auto guard = std::unique_lock{mutexes_[mutex_id], std::defer_lock};
        guard.lock();

        const auto& bucket = values_[GetIndex(key)];

        auto it = std::find_if(bucket.begin(), bucket.end(),
                               [&key](const auto& pair) { return pair.first == key; });

        if (it != bucket.end()) {
            load_.fetch_sub(1);
            values_[GetIndex(key)].erase(it);
            guard.unlock();
            return true;
        } else {
            guard.unlock();
            return false;
        }
    }

    void Clear() {
        std::vector<std::unique_lock<std::mutex>> locker;
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            locker.emplace_back(mutexes_[i], std::defer_lock);
            locker.back().lock();
        }

        for (auto& list : values_) {
            while (!list.empty()) {
                list.pop_back();
            }
        }
        values_.clear();
        values_.resize(mutexes_.size());
        load_.store(0);
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            locker.back().unlock();
            locker.pop_back();
        }
    }

    std::pair<bool, V> Find(const K& key) const {
        const auto& mutex_id = GetMutexIndex(key);
        auto guard = std::unique_lock{mutexes_[mutex_id], std::defer_lock};
        guard.lock();

        const auto& bucket = values_[GetIndex(key)];

        auto it = std::find_if(bucket.begin(), bucket.end(),
                               [&key](const auto& pair) { return pair.first == key; });

        if (it != bucket.end()) {
            std::pair<bool, V> ret_p{true, it->second};
            guard.unlock();
            return ret_p;
        } else {
            guard.unlock();
            return {false, V()};
        }
    }

    const V At(const K& key) const {
        const auto& mutex_id = GetMutexIndex(key);
        auto guard = std::unique_lock{mutexes_[mutex_id], std::defer_lock};
        guard.lock();

        const auto& bucket = values_[GetIndex(key)];

        auto it = std::find_if(bucket.begin(), bucket.end(),
                               [&key](const auto& pair) { return pair.first == key; });

        if (it != bucket.end()) {
            auto ret = it->second;
            guard.unlock();
            return ret;
        }
        guard.unlock();
        throw std::out_of_range("AAAA");
    }

    size_t Size() const {
        return load_;
    }

    static const int kDefaultConcurrencyLevel;
    static const int kUndefinedSize;
    static const double kLoadFactor;

private:
    size_t GetIndex(const K& key) const {
        return hasher_(key) % values_.size();
    }

    size_t GetMutexIndex(const K& key) const {
        return hasher_(key) % mutexes_.size();
    }

private:
    std::vector<std::list<std::pair<K, V>>> values_;
    mutable std::vector<std::mutex> mutexes_;
    Hash hasher_;

    std::atomic<size_t> load_;
};

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kDefaultConcurrencyLevel = 8;

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kUndefinedSize = 8;

template <class K, class V, class Hash>
const double ConcurrentHashMap<K, V, Hash>::kLoadFactor = 0.75;
