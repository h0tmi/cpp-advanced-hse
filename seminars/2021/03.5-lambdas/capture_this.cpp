#include <iostream>

class MaybeLogger {
public:
    auto MakeCallback() const {
        return [=, *this] {
            // this
            // &(*this)
            std::cout << data_ << std::endl;
        };
    }

    ~MaybeLogger() {
        std::cout << "Goodnight Sweet Prince" << std::endl;
    }

private:
    std::string data_{"Important log data"};
};

auto MakeLogCallback() {
    auto logger = MaybeLogger{};
    return logger.MakeCallback();
}

auto main() -> int {
    std::cout << std::endl;

    auto log_cb = MakeLogCallback();
    log_cb();

    std::cout << std::endl;

    // https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change
}
