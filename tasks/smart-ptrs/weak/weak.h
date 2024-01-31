#pragma once

#include "sw_fwd.h"  // Forward declaration
#include "weak/shared.h"

// https://en.cppreference.com/w/cpp/memory/weak_ptr
template <typename T>
class WeakPtr {
private:
    template <typename U>
    friend class SharedPtr;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    WeakPtr() : block_(nullptr), ptr_(nullptr) {
    }

    WeakPtr(const WeakPtr& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        if (block_) {
            block_->IncWeak();
        }
    }
    WeakPtr(WeakPtr&& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
    }
    template <typename U>
    WeakPtr(const WeakPtr<U>& other) : block_(other.block_), ptr_(other.ptr_) {
        if (block_) {
            block_->IncWeak();
        }
    }
    template <typename U>
    WeakPtr(WeakPtr<U>&& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
    }

    // Demote `SharedPtr`
    // #2 from https://en.cppreference.com/w/cpp/memory/weak_ptr/weak_ptr
    WeakPtr(const SharedPtr<T>& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        if (block_) {
            block_->IncWeak();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s
    template <typename U>
    WeakPtr& operator=(const WeakPtr<U>& other) {
        if (other.ptr_ == this->ptr_) {
            return *this;
        }
        if (block_) {
            block_->DecWeak();
        }
        block_ = other.block_;
        ptr_ = other.ptr_;
        if (block_) {
            block_->IncWeak();
        }
        return *this;
    }
    template <typename U>
    WeakPtr& operator=(WeakPtr<U>&& other) {
        if (other.ptr_ == this->ptr_) {
            return *this;
        }
        if (block_) {
            block_->DecWeak();
        }
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
        return *this;
    }
    WeakPtr& operator=(const WeakPtr& other) {
        if (other.ptr_ == this->ptr_) {
            return *this;
        }
        if (block_) {
            block_->DecWeak();
        }
        block_ = other.block_;
        ptr_ = other.ptr_;
        if (block_) {
            block_->IncWeak();
        }
        return *this;
    }
    WeakPtr& operator=(WeakPtr&& other) {
        if (other.ptr_ == this->ptr_) {
            return *this;
        }
        if (block_) {
            block_->DecWeak();
        }
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~WeakPtr() {
        Reset();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        if (block_) {
            block_->DecWeak();
        }
        block_ = nullptr;
        ptr_ = nullptr;
    }
    void Swap(WeakPtr& other) {
        std::swap(block_, other.block_);
        std::swap(ptr_, other.ptr_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    size_t UseCount() const {
        return block_ ? block_->GetCnt() : 0;
    }
    bool Expired() const {
        return block_ == nullptr || block_->GetCnt() == 0;
    }
    SharedPtr<T> Lock() const {
        if (Expired()) {
            return SharedPtr<T>();
        }
        return SharedPtr<T>(*this);
    };

private:
    BaseBlock* block_ = nullptr;
    T* ptr_ = nullptr;
};
