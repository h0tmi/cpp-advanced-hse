#pragma once

#include <algorithm>
#include <memory>
#include <typeinfo>
class Any {
public:
    Any() : ptr_(nullptr) {
    }

    template <class T>
    Any(const T& value) : ptr_(new Inner<T>(value)) {
    }

    template <class T>
    Any& operator=(const T& value) {
        Any(value).Swap(*this);
        return *this;
    }

    Any(const Any& rhs) : ptr_(rhs.Empty() ? nullptr : rhs.ptr_->Clone()) {
    }
    Any& operator=(const Any& rhs) {
        Any(rhs).Swap(*this);
        return *this;
    }
    ~Any() {
        ptr_.reset();
    }

    bool Empty() const {
        return ptr_ == nullptr;
    }

    void Clear() {
        ptr_.reset();
    }
    void Swap(Any& rhs) {
        std::swap(ptr_, rhs.ptr_);
    }

    template <class T>
    const T& GetValue() const {
        if (ptr_->TypeId() != typeid(T)) {
            throw std::bad_cast();
        }
        return dynamic_cast<Inner<T>*>(ptr_.get())->data_;
    }

private:
    struct InnerBase {
        using Pointer = std::unique_ptr<InnerBase>;
        virtual std::type_info const& TypeId() const = 0;
        virtual std::unique_ptr<InnerBase> Clone() const = 0;
        virtual ~InnerBase() = default;
    };
    template <typename T>
    struct Inner : public InnerBase {
        Inner(T data) : data_(std::move(data)) {
        }
        std::type_info const& TypeId() const override {
            return typeid(T);
        }
        std::unique_ptr<InnerBase> Clone() const override {
            return std::make_unique<Inner<T>>(data_);
        }
        T data_;
    };
    InnerBase::Pointer ptr_;
};
