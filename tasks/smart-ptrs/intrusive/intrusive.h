#pragma once

#include <cstddef> // for std::nullptr_t
#include <utility> // for std::exchange / std::swap


class SimpleCounter {
public:
    size_t IncRef();
    size_t DecRef();
    size_t RefCount() const;

private:
    size_t count_ = 0;
};

struct DefaultDelete {
    template <typename T>
    static void Destroy(T* object) {
        delete object;
    }
};

template <typename Derived, typename Counter, typename Deleter>
class RefCounted {
public:
    // Increase reference counter.
    void IncRef();

    // Decrease reference counter.
    // Destroy object using Deleter when the last instance dies.
    void DecRef();

    // Get current counter value (the number of strong references).
    size_t RefCount() const;

private:
    Counter counter_;
};

template <typename Derived, typename D = DefaultDelete>
using SimpleRefCounted = RefCounted<Derived, SimpleCounter, D>;

template <typename T>
class IntrusivePtr {
    template <typename Y>
    friend class IntrusivePtr;

public:
    // Constructors
    IntrusivePtr();
    IntrusivePtr(std::nullptr_t);
    IntrusivePtr(T* ptr);

    template <typename Y>
    IntrusivePtr(const IntrusivePtr<Y>& other);

    template <typename Y>
    IntrusivePtr(IntrusivePtr<Y>&& other);

    IntrusivePtr(const IntrusivePtr& other);
    IntrusivePtr(IntrusivePtr&& other);

    // `operator=`-s
    IntrusivePtr& operator=(const IntrusivePtr& other);
    IntrusivePtr& operator=(IntrusivePtr&& other);

    // Destructor
    ~IntrusivePtr();

    // Modifiers
    void Reset();
    void Reset(T* ptr);
    void Swap(IntrusivePtr& other);

    // Observers
    T* Get() const;
    T& operator*() const;
    T* operator->() const;
    size_t UseCount() const;
    explicit operator bool() const;
};

template <typename T, typename ...Args>
IntrusivePtr<T> MakeIntrusive(Args&& ...args);
