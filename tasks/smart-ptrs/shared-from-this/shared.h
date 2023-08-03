#pragma once

#include "sw_fwd.h"  // Forward declaration

#include <cstddef>  // std::nullptr_t

// https://en.cppreference.com/w/cpp/memory/shared_ptr
template <typename T>
class SharedPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    SharedPtr();
    SharedPtr(std::nullptr_t);
    explicit SharedPtr(T* ptr);

    SharedPtr(const SharedPtr& other);
    SharedPtr(SharedPtr&& other);

    // Aliasing constructor
    // #8 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other, T* ptr);

    // Promote `WeakPtr`
    // #11 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    explicit SharedPtr(const WeakPtr<T>& other);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    SharedPtr& operator=(const SharedPtr& other);
    SharedPtr& operator=(SharedPtr&& other);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~SharedPtr();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset();
    void Reset(T* ptr);
    void Swap(SharedPtr& other);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const;
    T& operator*() const;
    T* operator->() const;
    size_t UseCount() const;
    explicit operator bool() const;
};

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right);

// Allocate memory only once
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args);

// Look for usage examples in tests and seminar
template <typename T>
class EnableSharedFromThis {
public:
    SharedPtr<T> SharedFromThis();
    SharedPtr<const T> SharedFromThis() const;

    WeakPtr<T> WeakFromThis() noexcept;
    WeakPtr<const T> WeakFromThis() const noexcept;
};
