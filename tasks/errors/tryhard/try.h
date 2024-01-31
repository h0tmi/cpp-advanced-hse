#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <system_error>

class TryBase {
public:
    TryBase() = default;
    template <typename Exception>
    TryBase(const Exception& exc) : exc_(std::make_exception_ptr(exc)) {
    }
    TryBase(const std::exception_ptr& exc) : exc_(exc) {
    }

protected:
    void ThrowPrivate() {
        if (exc_) {
            std::rethrow_exception(exc_);
        }
        throw std::runtime_error("No exception");
    }

    bool IsFailedPrivate() const {
        return exc_ != nullptr;
    }

protected:
    std::exception_ptr exc_ = nullptr;
};

template <class T>
class Try : protected TryBase {
public:
    Try() = default;
    Try(const T& value) : value_(value), has_value_(true) {
    }
    template <typename Exception>
    Try(const Exception& exc) : TryBase(std::make_exception_ptr(exc)) {
    }
    Try(const std::exception_ptr& exc) : TryBase(exc) {
    }

    const T& Value() {
        if (has_value_) {
            return value_;
        }
        if (exc_) {
            std::rethrow_exception(exc_);
        }
        throw std::runtime_error("Object is empty");
    }

    void Throw() {
        ThrowPrivate();
    }

    bool IsFailed() const {
        return IsFailedPrivate();
    }

private:
    T value_ = {};
    bool has_value_ = false;
};

template <>
class Try<void> : protected TryBase {
public:
    Try() = default;
    template <typename Exception>
    Try(const Exception& exc) : TryBase(std::make_exception_ptr(exc)) {
    }
    Try(const std::exception_ptr& exc) : TryBase(exc) {
    }

    void Throw() {
        ThrowPrivate();
    }

    bool IsFailed() const {
        return IsFailedPrivate();
    }
};

template <class Function, class... Args>
auto TryRun(Function func, Args... args) {
    using ReturnType = decltype(func(args...));
    try {
        if constexpr (std::is_same_v<void, ReturnType>) {
            func(args...);
            return Try<ReturnType>();
        } else {
            return Try<ReturnType>(func(args...));
        }
    } catch (const std::exception& ex) {
        return Try<ReturnType>(std::current_exception());
    } catch (const char* str) {
        return Try<ReturnType>(std::runtime_error(str));
    } catch (int error_code) {
        return Try<ReturnType>(std::system_error(error_code, std::generic_category()));
    } catch (...) {
        return Try<ReturnType>(std::runtime_error("Unknown exception"));
    }
}
