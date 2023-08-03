#include <memory>
#include <chrono>
#include <vector>
#include <functional>

class Task : public std::enable_shared_from_this<Task> {
public:
    virtual ~Task() {
    }

    virtual void Run() = 0;

    void AddDependency(std::shared_ptr<Task> dep);
    void AddTrigger(std::shared_ptr<Task> dep);
    void SetTimeTrigger(std::chrono::system_clock::time_point at);

    // Task::run() completed without throwing exception
    bool IsCompleted();

    // Task::run() throwed exception
    bool IsFailed();

    // Task was Canceled
    bool IsCanceled();

    // Task either completed, failed or was Canceled
    bool IsFinished();

    std::exception_ptr GetError();

    void Cancel();

    void Wait();

private:
};

template <class T>
class Future;

template <class T>
using FuturePtr = std::shared_ptr<Future<T>>;

// Used instead of void in generic code
struct Unit {};

class Executor {
public:
    void Submit(std::shared_ptr<Task> task);

    void StartShutdown();
    void WaitShutdown();

    template <class T>
    FuturePtr<T> Invoke(std::function<T()> fn);

    template <class Y, class T>
    FuturePtr<Y> Then(FuturePtr<T> input, std::function<Y()> fn);

    template <class T>
    FuturePtr<std::vector<T>> WhenAll(std::vector<FuturePtr<T>> all);

    template <class T>
    FuturePtr<T> WhenFirst(std::vector<FuturePtr<T>> all);

    template <class T>
    FuturePtr<std::vector<T>> WhenAllBeforeDeadline(std::vector<FuturePtr<T>> all,
                                                    std::chrono::system_clock::time_point deadline);
};

std::shared_ptr<Executor> MakeThreadPoolExecutor(int num_threads);

template <class T>
class Future : public Task {
public:
    T Get();

private:
};
