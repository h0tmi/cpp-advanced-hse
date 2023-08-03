#include <gtest/gtest.h>

#include <thread>
#include <chrono>
#include <atomic>

#include <executors.h>

typedef std::function<std::shared_ptr<Executor>()> ExecutorMaker;

struct ExecutorsTest : public testing::TestWithParam<ExecutorMaker> {
    std::shared_ptr<Executor> pool;

    ExecutorsTest() {
        pool = GetParam()();
    }
};

TEST_P(ExecutorsTest, Destructor) {
}

TEST_P(ExecutorsTest, Shutdown) {
    pool->StartShutdown();
    pool->WaitShutdown();
}

class TestTask : public Task {
public:
    bool completed = false;

    void Run() override {
        EXPECT_TRUE(!completed) << "Seems like task body was run multiple times";
        completed = true;
    }
};

class SlowTask : public Task {
public:
    std::atomic<bool> completed{false};

    void Run() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        completed = true;
    }
};

class FailingTestTask : public Task {
public:
    void Run() override {
        throw std::logic_error("Failed");
    }
};

TEST_P(ExecutorsTest, RunSingleTask) {
    auto task = std::make_shared<TestTask>();

    pool->Submit(task);

    task->Wait();

    EXPECT_TRUE(task->completed);
    EXPECT_TRUE(task->IsFinished());
    EXPECT_FALSE(task->IsCanceled());
    EXPECT_FALSE(task->IsFailed());
}

TEST_P(ExecutorsTest, RunSingleFailingTask) {
    auto task = std::make_shared<FailingTestTask>();

    pool->Submit(task);

    task->Wait();

    EXPECT_FALSE(task->IsCompleted());
    EXPECT_FALSE(task->IsCanceled());
    EXPECT_TRUE(task->IsFailed());

    EXPECT_THROW(std::rethrow_exception(task->GetError()), std::logic_error);
}

TEST_P(ExecutorsTest, CancelSingleTask) {
    auto task = std::make_shared<TestTask>();
    task->Cancel();
    task->Wait();

    EXPECT_FALSE(task->IsCompleted());
    EXPECT_TRUE(task->IsCanceled());
    EXPECT_FALSE(task->IsFailed());

    pool->Submit(task);
    task->Wait();

    EXPECT_FALSE(task->IsCompleted());
    EXPECT_TRUE(task->IsCanceled());
    EXPECT_FALSE(task->IsFailed());
}

TEST_P(ExecutorsTest, TaskWithSingleDependency) {
    auto task = std::make_shared<TestTask>();
    auto dependency = std::make_shared<TestTask>();

    task->AddDependency(dependency);

    pool->Submit(task);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(task->IsFinished());

    pool->Submit(dependency);

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, TaskWithSingleCompletedDependency) {
    auto task = std::make_shared<TestTask>();
    auto dependency = std::make_shared<TestTask>();

    task->AddDependency(dependency);

    pool->Submit(dependency);
    dependency->Wait();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ASSERT_FALSE(task->IsFinished());

    pool->Submit(task);
    task->Wait();
    ASSERT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, FailedDependencyIsConsideredCompleted) {
    auto task = std::make_shared<TestTask>();
    auto dependency = std::make_shared<FailingTestTask>();

    task->AddDependency(dependency);

    pool->Submit(task);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(task->IsFinished());

    pool->Submit(dependency);

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, CanceledDependencyIsConsideredCompleted) {
    auto task = std::make_shared<TestTask>();
    auto dependency = std::make_shared<TestTask>();

    task->AddDependency(dependency);

    pool->Submit(task);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(task->IsFinished());

    dependency->Cancel();

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

struct RecursiveTask : public Task {
    RecursiveTask(int n, std::shared_ptr<Executor> executor) : n_(n), executor_(executor) {
    }

    void Run() {
        if (n_ > 0) {
            executor_->Submit(std::make_shared<RecursiveTask>(n_ - 1, executor_));
        }

        if (n_ == 0) {
            executor_->StartShutdown();
        }
    }

private:
    const int n_;
    const std::shared_ptr<Executor> executor_;
};

TEST_P(ExecutorsTest, RunRecursiveTask) {
    auto task = std::make_shared<RecursiveTask>(100, pool);
    pool->Submit(task);

    pool->WaitShutdown();
}

TEST_P(ExecutorsTest, TaskWithSingleTrigger) {
    auto task = std::make_shared<TestTask>();
    auto trigger = std::make_shared<TestTask>();

    task->AddTrigger(trigger);
    pool->Submit(task);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(task->IsFinished());

    pool->Submit(trigger);

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, TaskWithSingleCompletedTrigger) {
    auto task = std::make_shared<TestTask>();
    auto trigger = std::make_shared<TestTask>();

    task->AddTrigger(trigger);
    pool->Submit(trigger);
    trigger->Wait();

    EXPECT_FALSE(task->IsFinished());

    pool->Submit(task);

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, TaskWithTwoTrigger) {
    auto task = std::make_shared<TestTask>();
    auto trigger_a = std::make_shared<TestTask>();
    auto trigger_b = std::make_shared<TestTask>();

    task->AddTrigger(trigger_a);
    task->AddTrigger(trigger_b);

    pool->Submit(task);
    pool->Submit(trigger_b);
    pool->Submit(trigger_a);

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, MultipleDependencies) {
    auto task = std::make_shared<TestTask>();
    auto dep1 = std::make_shared<TestTask>();
    auto dep2 = std::make_shared<TestTask>();

    task->AddDependency(dep1);
    task->AddDependency(dep2);

    pool->Submit(task);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(task->IsFinished());

    pool->Submit(dep1);
    dep1->Wait();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(task->IsFinished());

    pool->Submit(dep2);
    task->Wait();

    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, TaskWithSingleTimeTrigger) {
    auto task = std::make_shared<TestTask>();

    task->SetTimeTrigger(std::chrono::system_clock::now() + std::chrono::milliseconds(200));

    pool->Submit(task);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(task->IsFinished());

    task->Wait();
    EXPECT_TRUE(task->IsFinished());
}

TEST_P(ExecutorsTest, DISABLED_TaskTriggeredByTimeAndDep) {
    auto task = std::make_shared<TestTask>();
    auto dep = std::make_shared<TestTask>();

    task->AddDependency(dep);
    task->SetTimeTrigger(std::chrono::system_clock::now() + std::chrono::milliseconds(2));

    pool->Submit(task);
    pool->Submit(dep);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_TRUE(task->IsFinished());

    std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

TEST_P(ExecutorsTest, MultipleTimerTriggers) {
    auto task_a = std::make_shared<TestTask>();
    auto task_b = std::make_shared<TestTask>();

    task_a->SetTimeTrigger(std::chrono::system_clock::now() + std::chrono::milliseconds(50));
    task_b->SetTimeTrigger(std::chrono::system_clock::now() + std::chrono::milliseconds(1));

    pool->Submit(task_a);
    pool->Submit(task_b);

    task_a->Wait();
    EXPECT_TRUE(task_b->IsFinished());
}

TEST_P(ExecutorsTest, MultipleTimerTriggersWithReverseOrder) {
    auto task_a = std::make_shared<TestTask>();
    auto task_b = std::make_shared<TestTask>();

    task_a->SetTimeTrigger(std::chrono::system_clock::now() + std::chrono::milliseconds(50));
    task_b->SetTimeTrigger(std::chrono::system_clock::now() + std::chrono::milliseconds(1));

    pool->Submit(task_b);
    pool->Submit(task_a);

    task_b->Wait();
    EXPECT_FALSE(task_a->IsFinished());
}

TEST_P(ExecutorsTest, PossibleToCancelAfterSubmit) {
    std::vector<std::shared_ptr<SlowTask>> tasks;
    for (int i = 0; i < 1000; ++i) {
        auto task = std::make_shared<SlowTask>();
        tasks.push_back(task);

        pool->Submit(task);

        task->Cancel();
    }

    pool.reset();

    for (auto t : tasks) {
        if (!t->completed) {
            return;
        }
    }

    FAIL() << "Seems like Cancel() doesn't affect Submitted tasks";
}

TEST_P(ExecutorsTest, DISABLED_CancelAfterDetroyOfExecutor) {
    auto first = std::make_shared<TestTask>();
    auto second = std::make_shared<TestTask>();
    first->AddDependency(second);
    pool->Submit(first);

    pool.reset();

    first->Cancel();
    second->Cancel();
}

struct RecursiveGrowingTask : public Task {
    RecursiveGrowingTask(int n, int fanout, std::shared_ptr<Executor> executor)
        : n_(n), fanout_(fanout), executor_(executor) {
    }

    void Run() {
        if (n_ > 0) {
            for (int i = 0; i < fanout_; ++i) {
                executor_->Submit(
                    std::make_shared<RecursiveGrowingTask>(n_ - 1, fanout_, executor_));
            }
        }

        if (n_ == 0) {
            executor_->StartShutdown();
        }
    }

private:
    const int n_, fanout_;
    const std::shared_ptr<Executor> executor_;
};

TEST_P(ExecutorsTest, NoDeadlockWhenSubmittingFromTaskBody) {
    auto task = std::make_shared<RecursiveGrowingTask>(5, 10, pool);
    pool->Submit(task);

    pool->WaitShutdown();
}

INSTANTIATE_TEST_CASE_P(ThreadPool, ExecutorsTest,
                        ::testing::Values([] { return MakeThreadPoolExecutor(1); },
                                          [] { return MakeThreadPoolExecutor(2); },
                                          [] { return MakeThreadPoolExecutor(10); }));
