#include "burn.hpp"

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>

template <typename T>
class UnboundedBlockingQueue {
public:
    bool Put(T value) {
        auto guard = std::lock_guard{mutex_};
        if (stopped_) {
            return false;
        }
        buffer_.push_back(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    std::optional<T> Take() {
        auto guard = std::unique_lock{mutex_};
        not_empty_.wait(guard, [this] { return stopped_ || !buffer_.empty(); });
        if (stopped_ && buffer_.empty()) {
            return std::nullopt;
        }
        T result = std::move(buffer_.front());
        buffer_.pop_front();
        return result;
    }

    void Close() {
        CloseImpl(/*clear=*/false);
    }

    void Cancel() {
        CloseImpl(/*clear=*/true);
    }

private:
    void CloseImpl(bool clear) {
        auto guard = std::lock_guard{mutex_};
        stopped_ = true;
        if (clear) {
            buffer_.clear();
        }
        not_empty_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable not_empty_;

    bool stopped_{false};
    std::deque<T> buffer_;
};

////////////////////////////////////////////////////////////////////////////////

class ThreadPool {
public:
    using Task = std::function<void()>;

public:
    ThreadPool(size_t num_threads) {
        workers_.reserve(num_threads);
        while (num_threads-- > 0) {
            workers_.emplace_back([this] { Run(); });
        }
    }

    void Submit(Task task) {
        queue_.Put(std::move(task));
    }

    void Join() {
        queue_.Close();
        for (auto& t : workers_) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    ~ThreadPool() {
        Join();
    }

private:
    void Run() {
        while (auto task = queue_.Take()) {
            (*task)();
        }
    }

private:
    UnboundedBlockingQueue<Task> queue_;
    std::vector<std::jthread> workers_;
};

////////////////////////////////////////////////////////////////////////////////

int main() {
    ThreadPool pool{/*num_threads=*/4};
    for (int i = 0; i < 4000; ++i) {
        pool.Submit([] { BurnCpuFor(std::chrono::milliseconds(10)); });
    }
    pool.Join();
}
