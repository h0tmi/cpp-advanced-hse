#include <cassert>
#include <condition_variable>
#include <deque>
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

int main() {
    UnboundedBlockingQueue<int> queue;
    std::jthread pusher{[&queue] {
        for (int i = 0; i < 1000000; ++i) {
            queue.Put(i);
        }
        queue.Close();
    }};

    int expected = 0;
    while (auto item = queue.Take()) {
        assert(*item == expected++);
    }
}
