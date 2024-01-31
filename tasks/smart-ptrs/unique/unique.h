#pragma once

#include "compressed_pair.h"
#include "unique/deleters.h"

#include <cstddef>  // std::nullptr_t
#include <iterator>
#include <type_traits>
#include <utility>

template <typename T>
struct Slug {
    Slug() = default;

    void operator()(T* data) {
        delete data;
    }
    template <typename S>
    Slug(const Slug<S>&) {
    }

    ~Slug() = default;
};

template <typename T>
struct Slug<T[]> {
    Slug() = default;

    void operator()(T* data) {
        delete[] data;
    }

    template <typename S>
    Slug(const Slug<S>&) {
    }

    ~Slug() = default;
};
// Primary template
template <typename T, typename Deleter = Slug<T>>
class UniquePtr {
public:
    template <typename S, typename DeleterD>
    friend class UniquePtr;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr) : data_(CompressedPair<T*, Deleter>(ptr, Deleter())) {
    }
    UniquePtr(T* ptr, Deleter deleter)
        : data_(CompressedPair<T*, Deleter>(ptr, std::forward<Deleter>(deleter))) {
    }
    template <typename S = T, typename DeleterD = Deleter>
    UniquePtr(UniquePtr<S, DeleterD>&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return;
        }
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
    }
    UniquePtr(UniquePtr&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return;
        }
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s
    template <typename S, typename DeleterD>
    UniquePtr& operator=(UniquePtr<S, DeleterD>&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return *this;
        }
        Reset();
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
        return *this;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return *this;
        }
        Reset();
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) {
        Reset();
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        Reset();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        auto ptr = data_.GetFirst();
        data_.GetFirst() = nullptr;
        return ptr;
    }
    void Reset(T* ptr = nullptr) {
        auto old_ptr = data_.GetFirst();
        data_.GetFirst() = ptr;
        if (old_ptr) {
            data_.GetSecond()(old_ptr);
        }
    }
    void Swap(UniquePtr& other) {
        std::swap(data_.GetFirst(), other.data_.GetFirst());
        std::swap(data_.GetSecond(), other.data_.GetSecond());
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return data_.GetFirst();
    }
    Deleter& GetDeleter() {
        return data_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return data_.GetSecond();
    };
    explicit operator bool() const {
        if (data_.GetFirst()) {
            return true;
        }
        return false;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    std::add_lvalue_reference_t<T> operator*() const {
        return *data_.GetFirst();
    }
    T* operator->() const {
        return data_.GetFirst();
    }

private:
    CompressedPair<T*, Deleter> data_;
};

template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    template <typename S, typename DeleterD>
    friend class UniquePtr;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr) : data_(CompressedPair<T*, Deleter>(ptr, Deleter())) {
    }
    UniquePtr(T* ptr, Deleter deleter)
        : data_(CompressedPair<T*, Deleter>(ptr, std::forward<Deleter>(deleter))) {
    }
    template <typename S = T, typename DeleterD = Deleter>
    UniquePtr(UniquePtr<S, DeleterD>&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return;
        }
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
    }
    UniquePtr(UniquePtr&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return;
        }
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    template <typename S, typename DeleterD>
    UniquePtr& operator=(UniquePtr<S, DeleterD>&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return *this;
        }
        Reset();
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
        return *this;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (data_.GetFirst() == other.data_.GetFirst()) {
            return *this;
        }
        Reset();
        data_.GetFirst() = other.data_.GetFirst();
        data_.GetSecond() = std::move(other.data_.GetSecond());
        other.Release();
        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) {
        Reset();
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        Reset();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        auto ptr = data_.GetFirst();
        data_.GetFirst() = nullptr;
        return ptr;
    }
    void Reset(T* ptr = nullptr) {
        auto old_ptr = data_.GetFirst();
        data_.GetFirst() = ptr;
        if (old_ptr) {
            data_.GetSecond()(old_ptr);
        }
    }
    void Swap(UniquePtr& other) {
        std::swap(data_.GetFirst(), other.data_.GetFirst());
        std::swap(data_.GetSecond(), other.data_.GetSecond());
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return data_.GetFirst();
    }
    Deleter& GetDeleter() {
        return data_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return data_.GetSecond();
    };
    explicit operator bool() const {
        if (data_.GetFirst()) {
            return true;
        }
        return false;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    std::add_lvalue_reference_t<T> operator*() const {
        return *data_.GetFirst();
    }
    T* operator->() const {
        return data_.GetFirst();
    }
    T& operator[](size_t pos) const {
        return data_.GetFirst()[pos];
    }

private:
    CompressedPair<T*, Deleter> data_;
};
