#include <experimental/coroutine>


struct resumable {
    struct promise_type {
        resumable get_return_object() {
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
    };

    resumable(std::experimental::coroutine_handle<promise_type> handle)
        : handle_{handle}
    {}

    bool resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

    ~resumable() {
        handle_.destroy();
    }

private:
    std::experimental::coroutine_handle<promise_type> handle_;
};
