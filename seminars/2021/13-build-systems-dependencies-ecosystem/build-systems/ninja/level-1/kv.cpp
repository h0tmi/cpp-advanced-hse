#include "kv.hpp"

auto KVStorage::Size() const noexcept -> std::size_t {
    return storage_.size();
}

auto KVStorage::IsEmpty() const noexcept -> bool {
    return storage_.empty();
}
