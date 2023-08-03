#include <benchmark/benchmark.h>

#include <executors.h>

class EmptyTask : public Task {
public:
    virtual void Run() override {
    }
};

static void BenchmarkSimpleSubmit(benchmark::State& state) {
    auto executor = MakeThreadPoolExecutor(state.range(0));
    for (auto _ : state) {
        auto task = std::make_shared<EmptyTask>();
        executor->Submit(task);
        task->Wait();
    }
}

BENCHMARK(BenchmarkSimpleSubmit)->Arg(1)->Arg(2)->Arg(4);

static void BenchmarkFanoutFanin(benchmark::State& state) {
    auto executor = MakeThreadPoolExecutor(state.range(0));
    for (auto _ : state) {
        auto first_task = std::make_shared<EmptyTask>();
        auto last_task = std::make_shared<EmptyTask>();

        for (int i = 0; i < state.range(1); i++) {
            auto middle_task = std::make_shared<EmptyTask>();
            middle_task->AddDependency(first_task);
            last_task->AddDependency(middle_task);

            executor->Submit(middle_task);
        }

        executor->Submit(first_task);
        executor->Submit(last_task);

        last_task->Wait();
    }
}

BENCHMARK(BenchmarkFanoutFanin)
    ->Args({1, 1})
    ->Args({1, 10})
    ->Args({1, 100})
    ->Args({2, 1})
    ->Args({2, 10})
    ->Args({2, 100})
    ->Args({10, 1})
    ->Args({10, 10})
    ->Args({10, 100});

class Latch {
public:
    Latch(size_t count) : counter_(count) {
    }

    void Wait() {
        std::unique_lock<std::mutex> guard(lock_);
        done_.wait(guard, [this] { return counter_ == 0; });
    }

    void Signal() {
        std::unique_lock<std::mutex> guard(lock_);
        counter_--;
        if (counter_ == 0) {
            done_.notify_all();
        }
    }

private:
    std::mutex lock_;
    std::condition_variable done_;
    size_t counter_;
};

class LatchSignaler : public Task {
public:
    LatchSignaler(Latch* latch) : latch_(latch) {
    }

    virtual void Run() override {
        latch_->Signal();
    }

private:
    Latch* latch_;
};

static void BenchmarkScalableTimers(benchmark::State& state) {
    auto executor = MakeThreadPoolExecutor(state.range(0));

    for (auto _ : state) {
        Latch latch(state.range(1));
        auto at = std::chrono::system_clock::now() + std::chrono::milliseconds(5);

        for (size_t i = 0; i < static_cast<size_t>(state.range(1)); i++) {
            auto task = std::make_shared<LatchSignaler>(&latch);

            task->SetTimeTrigger(at + std::chrono::microseconds((i * i) % 1000));
            executor->Submit(task);
        }

        latch.Wait();
    }
}

BENCHMARK(BenchmarkScalableTimers)
    ->Args({1, 100000})
    ->Args({2, 100000})
    ->Args({5, 100000})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
