#pragma once

#include <memory>

template <typename Callback>
class CallbackStorage {
public:
    CallbackStorage(Callback callback) {
        ::new (GetCallbackBuffer()) Callback(std::move(callback));
    }
    void* GetCallbackBuffer() {
        return static_cast<void*>(callback_buffer_);
    }
    Callback& GetCallback() {
        return *reinterpret_cast<Callback*>(GetCallbackBuffer());
    }

private:
    alignas(Callback) char callback_buffer_[sizeof(Callback)];
};

template <typename Callback>
class Defer final {
public:
    Defer(Callback callback) : callback_(std::forward<Callback>(callback)), is_deleted_(false) {
    }

    void operator()() && {
        std::move(callback_.GetCallback())();
        Deleter();
    }

    void Invoke() {
        std::move(callback_.GetCallback())();
        Deleter();
    }

    void Cancel() {
        Deleter();
    }

    ~Defer() {
        if (!is_deleted_) {
            Invoke();
        }
    }

private:
    void Deleter() {
        if (!is_deleted_) {
            callback_.GetCallback().~Callback();
        }
        is_deleted_ = true;
    }

private:
    CallbackStorage<Callback> callback_ = {};
    bool is_deleted_ = false;
};
