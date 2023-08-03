#pragma once

#include <atomic>
#include <unordered_map>
#include <memory>
#include <mutex>

#include "hazard_ptr.h"

template <class K, class V>
class SyncMap {
public:
    bool Lookup(const K& key, V* value);

    bool Insert(const K& key, const V& value);

    SyncMap();

    ~SyncMap();
};
