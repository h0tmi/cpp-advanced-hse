#pragma once

#include "compressed_pair.h"

#include <cstddef>  // std::nullptr_t

struct Slug {};

// Primary template
template <typename T, typename Deleter = Slug>
class UniquePtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr);
    UniquePtr(T* ptr, Deleter deleter);

    UniquePtr(UniquePtr&& other);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr& operator=(UniquePtr&& other);
    UniquePtr& operator=(std::nullptr_t);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release();
    void Reset(T* ptr = nullptr);
    void Swap(UniquePtr& other);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const;
    Deleter& GetDeleter();
    const Deleter& GetDeleter() const;
    explicit operator bool() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    T& operator*() const;
    T* operator->() const;
};

// Specialization for arrays
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter>;
