#include "defer.h"

#include <catch.hpp>
#include <utility>

TEST_CASE("Basic cleanup") {
    int set_value1 = 12;
    int checker_num1 = 0;
    {
        Defer num_saver = [set_value1, &checker_num1] { checker_num1 = set_value1; };
        static_assert(std::is_final_v<decltype(num_saver)>, "Should mark as final");
        ++set_value1;
        REQUIRE(checker_num1 == 0);
    }
    REQUIRE(checker_num1 == 12);

    int set_value2 = 12;
    int checker_num2 = 0;
    {
        Defer num_saver1 = [set_value2, &checker_num2] { checker_num2 = set_value2; };
        Defer num_saver2 = [&checker_num2] { checker_num2 *= 2; };
        REQUIRE(checker_num2 == 0);
    }
    REQUIRE(checker_num2 == 12);

    int set_value3 = 12;
    int checker_num3 = 0;
    {
        Defer num_saver1 = [&checker_num3] { checker_num3 *= 2; };
        Defer num_saver2 = [set_value3, &checker_num3] { checker_num3 = set_value3; };
        REQUIRE(checker_num3 == 0);
    }
    REQUIRE(checker_num3 == 24);
}

TEST_CASE("Cancelling cleanup") {
    int set_value1 = 12;
    int checker_num1 = 0;
    {
        Defer num_saver1 = [&checker_num1] { checker_num1 *= 2; };
        Defer num_saver2 = [set_value1, &checker_num1] { checker_num1 = set_value1; };
        REQUIRE(checker_num1 == 0);

        std::move(num_saver1).Cancel();
        REQUIRE(checker_num1 == 0);
    }
    REQUIRE(checker_num1 == 12);

    int set_value2 = 12;
    int checker_num2 = 0;
    {
        Defer num_saver1 = [&checker_num2] { checker_num2 *= 2; };
        Defer num_saver2 = [set_value2, &checker_num2] { checker_num2 = set_value2; };
        REQUIRE(checker_num2 == 0);

        std::move(num_saver2).Cancel();
        REQUIRE(checker_num2 == 0);
    }
    REQUIRE(checker_num2 == 0);
}

TEST_CASE("Invoking cleanup") {
    int set_value1 = 12;
    int checker_num1 = 0;
    {
        Defer num_saver = [set_value1, &checker_num1] { checker_num1 = set_value1; };
        REQUIRE(checker_num1 == 0);

        std::move(num_saver).Invoke();
        ++set_value1;
        REQUIRE(checker_num1 == 12);
    }

    int set_value2 = 12;
    int checker_num2 = 0;
    {
        Defer num_saver = [set_value2, &checker_num2] { checker_num2 = set_value2; };
        REQUIRE(checker_num2 == 0);

        ++set_value2;
        std::move(num_saver).Invoke();
        REQUIRE(checker_num2 == 12);
    }

    int set_value3 = 12;
    int checker_num3 = 0;
    {
        Defer num_saver1 = [&checker_num3] { checker_num3 *= 2; };
        Defer num_saver2 = [set_value3, &checker_num3] { checker_num3 = set_value3; };
        REQUIRE(checker_num3 == 0);

        std::move(num_saver2).Invoke();
    }
    REQUIRE(checker_num3 == 24);
}

class FunctorClass {
    using Callback = std::function<void()>;

public:
    explicit FunctorClass(Callback callback) : callback_(std::move(callback)) {
    }

    FunctorClass(FunctorClass&& other) : callback_(std::exchange(other.callback_, Callback())) {
    }

    FunctorClass(const FunctorClass&) = delete;

    FunctorClass& operator=(const FunctorClass&) = delete;

    FunctorClass& operator=(FunctorClass&&) = delete;

    void operator()() const& = delete;

    void operator()() && {
        CHECK(callback_);
        callback_();
        callback_ = nullptr;
    }

private:
    Callback callback_;
};

int destruction_count = 0;

struct FunctorClassFactory {
    template <typename Callback>
    static FunctorClass AsCallback(Callback callback) {
        return FunctorClass(std::move(callback));
    }
};

struct DestructionCounter {
    void operator()() {
    }

    ~DestructionCounter() {
        ++destruction_count;
    }
};

TEST_CASE("CancelDestroys") {
    {
        Defer cleanup = FunctorClassFactory::AsCallback(DestructionCounter());
        destruction_count = 0;

        std::move(cleanup).Cancel();
        REQUIRE(destruction_count == 1);  // Cancel destroys
    }

    REQUIRE(destruction_count == 1);  // Canceled cleanup does not double destroy
}

TEST_CASE("Invoke") {
    bool called = false;

    {
        Defer cleanup = FunctorClassFactory::AsCallback([&called] { called = true; });
        REQUIRE_FALSE(called);  // Constructor shouldn't invoke the callback

        std::move(cleanup).Invoke();
        REQUIRE(called);  // Invoke should invoke the callback

        called = false;  // Reset tracker before destructor runs
    }

    REQUIRE_FALSE(called);  // Destructor shouldn't invoke the callback
}
