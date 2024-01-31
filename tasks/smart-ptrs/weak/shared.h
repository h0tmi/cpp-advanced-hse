#pragma once

#include "sw_fwd.h"  // Forward declaration

#include <cstddef>  // std::nullptr_t
#include <memory>

class BaseBlock {
public:
    BaseBlock() {
        instance_cnt_ = 1;
        weak_instance_cnt_ = 0;
    }
    void Inc() {
        ++instance_cnt_;
    }
    void Dec() {
        --instance_cnt_;
        if (instance_cnt_ == 0) {
            UniversalDeleter();
            if (weak_instance_cnt_ == 0) {
                WeakUniversalDeleter();
            }
        }
    }
    void IncWeak() {
        ++weak_instance_cnt_;
    }
    void DecWeak() {
        --weak_instance_cnt_;
        if (instance_cnt_ == 0 && weak_instance_cnt_ == 0) {
            WeakUniversalDeleter();
        }
    }
    size_t GetCnt() {
        return instance_cnt_;
    }
    size_t GetWeakCnt() {
        return instance_cnt_;
    }

    virtual ~BaseBlock() {
    }

protected:
    virtual void UniversalDeleter() = 0;
    virtual void WeakUniversalDeleter() = 0;

private:
    size_t instance_cnt_ = 1;
    size_t weak_instance_cnt_ = 0;
};

template <typename T>
class SimpleBlock : public BaseBlock {
public:
    SimpleBlock() : BaseBlock(), data_(nullptr) {
    }
    SimpleBlock(T* ptr) : BaseBlock(), data_(ptr) {
    }
    ~SimpleBlock() {
    }

private:
    void UniversalDeleter() override {
        delete data_;
    }
    void WeakUniversalDeleter() override {
        delete this;
    }

private:
    T* data_ = nullptr;
};

template <typename T>
class MakeSharedBlock : public BaseBlock {
public:
    MakeSharedBlock() : BaseBlock() {
        ptr_ = new (data_) T();
    }
    template <typename... Args>
    MakeSharedBlock(Args&&... args) : BaseBlock() {
        ptr_ = new (data_) T(std::forward<Args>(args)...);
    }
    T* GetRawPointer() {
        return ptr_;
    }
    ~MakeSharedBlock() {
    }

private:
    void UniversalDeleter() override {
        reinterpret_cast<T*>(&data_)->~T();
    }
    void WeakUniversalDeleter() override {
        delete this;
    }

private:
    char data_[sizeof(T)] = {};
    T* ptr_;
};

// https://en.cppreference.com/w/cpp/memory/shared_ptr
template <typename T>
class SharedPtr {
private:
    template <typename S>
    friend class SharedPtr;

    template <typename S>
    friend class WeakPtr;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    SharedPtr() {
        block_ = nullptr;
        ptr_ = nullptr;
    }
    SharedPtr(std::nullptr_t) : SharedPtr<T>() {
    }
    template <typename U = T>
    SharedPtr(MakeSharedBlock<U>* block) : block_(block), ptr_(block->GetRawPointer()) {
    }
    template <typename U = T>
    explicit SharedPtr(U* ptr) : block_(new SimpleBlock<U>(ptr)), ptr_(ptr) {
    }
    template <typename U>
    SharedPtr(const SharedPtr<U>& other) : block_(other.block_), ptr_(other.ptr_) {
        if (block_) {
            block_->Inc();
        }
    }

    SharedPtr(const SharedPtr& other) : block_(other.block_), ptr_(other.ptr_) {
        if (block_) {
            block_->Inc();
        }
    }
    template <typename U>
    SharedPtr(SharedPtr<U>&& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
    }
    SharedPtr(SharedPtr&& other) {
        std::swap(block_, other.block_);
        std::swap(ptr_, other.ptr_);
    }

    // Aliasing constructor
    // #8 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other, T* ptr) {
        block_ = other.block_;
        ptr_ = ptr;
        if (block_) {
            block_->Inc();
        }
    }

    // Promote `WeakPtr`
    // #11 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    explicit SharedPtr(const WeakPtr<T>& other) {
        if (other.Expired()) {
            throw BadWeakPtr();
        } else {
            block_ = other.block_;
            ptr_ = other.ptr_;
            if (block_) {
                block_->Inc();
            }
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    template <typename U>
    SharedPtr& operator=(const SharedPtr<U>& other) {
        if (other == *this) {
            return *this;
        }
        Reset();
        block_ = other.block_;
        ptr_ = other.ptr_;
        if (block_) {
            block_->Inc();
        }
        return *this;
    }

    template <typename U>
    SharedPtr& operator=(SharedPtr<U>&& other) {
        if (other == *this) {
            return *this;
        }
        Reset();
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /// Non template
    SharedPtr& operator=(const SharedPtr& other) {
        if (other == *this) {
            return *this;
        }
        Reset();
        block_ = other.block_;
        ptr_ = other.ptr_;
        if (block_) {
            block_->Inc();
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if (other == *this) {
            return *this;
        }
        Reset();
        std::swap(block_, other.block_);
        std::swap(ptr_, other.ptr_);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~SharedPtr() {
        if (block_) {
            block_->Dec();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        if (block_) {
            block_->Dec();
        }
        block_ = nullptr;
        ptr_ = nullptr;
    }
    template <typename U = T>
    void Reset(U* ptr) {
        if (block_) {
            block_->Dec();
        }
        block_ = new SimpleBlock<U>(ptr);
        ptr_ = ptr;
    }
    void Swap(SharedPtr& other) {
        std::swap(block_, other.block_);
        std::swap(ptr_, other.ptr_);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return ptr_;
    }
    T& operator*() const {
        return *ptr_;
    }
    T* operator->() const {
        return ptr_;
    };
    size_t UseCount() const {
        return (block_ ? block_->GetCnt() : 0);
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    };

private:
    BaseBlock* block_ = nullptr;
    T* ptr_ = nullptr;
};

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right) {
    return left.Get() == right.Get();
};

// Allocate memory only once
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new MakeSharedBlock<T>(std::forward<Args>(args)...));
}

// Look for usage examples in tests
template <typename T>
class EnableSharedFromThis {
public:
    SharedPtr<T> SharedFromThis();
    SharedPtr<const T> SharedFromThis() const;

    WeakPtr<T> WeakFromThis() noexcept;
    WeakPtr<const T> WeakFromThis() const noexcept;
};
