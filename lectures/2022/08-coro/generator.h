#include <experimental/coroutine>

template <typename T>
struct generator {
    struct promise_type {
        generator get_return_object() {
            return {std::experimental::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::experimental::suspend_always initial_suspend() const {
            return {};
        }

        std::experimental::suspend_always final_suspend() const noexcept {
            return {};
        }

        void unhandled_exception() {
            throw 1; // panic
        }

        auto yield_value(T next) {
            value = std::move(next);
            return std::experimental::suspend_always{};
        }

        T value;
    };

    generator(std::experimental::coroutine_handle<promise_type> handle)
        : handle_{handle}
    {}

    bool next() {
        if (!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

    const T& value() {
        return handle_.promise().value;
    }

    ~generator() {
        handle_.destroy();
    }

private:
    std::experimental::coroutine_handle<promise_type> handle_;
};
