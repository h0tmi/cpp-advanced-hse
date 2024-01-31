#include "lru_cache.h"

LruCache::LruCache(size_t max_size) : max_size_(max_size) {
}

void LruCache::Set(const std::string& key, const std::string& value) {
    auto it = cache_addreses_.find(key);
    if (it != cache_addreses_.end()) {
        cache_.push_front({key, value});
        cache_.erase(it->second);
        it->second = cache_.begin();
        return;
    }

    if (cache_.size() == max_size_) {
        cache_addreses_.erase(cache_.back().first);
        cache_.pop_back();
    }

    cache_.push_front({key, value});
    cache_addreses_.emplace(key, cache_.begin());
}

bool LruCache::Get(const std::string& key, std::string* value) {
    auto it = cache_addreses_.find(key);

    if (it == cache_addreses_.end()) {
        return false;
    }

    *value = it->second->second;
    cache_.erase(it->second);
    cache_.push_front({key, *value});
    it->second = cache_.begin();

    return true;
}
