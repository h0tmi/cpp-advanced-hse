#include <functional>
#include <string>

struct Status {
    std::string message;
};

auto Error() -> Status {
    return Status{"Nothing works anymore"};
}

auto main() -> int {
    std::function<void()> do_work = []() -> Status {
        // ...
        return Error();
    };

    // Compiles...
    do_work();
    // ... and does not return anything.
    auto status = do_work();
}
