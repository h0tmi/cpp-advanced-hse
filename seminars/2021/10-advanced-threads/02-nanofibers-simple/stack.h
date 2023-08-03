#pragma once

#include <memory>
#include <cstddef>

namespace nanofibers {

class Stack {
public:
    explicit Stack(size_t size)
        : stack_{std::make_unique<char[]>(size)}, top_{stack_.get() + size} {
    }

    void* Top() const {
        return top_;
    }

private:
    std::unique_ptr<char[]> stack_;
    char* top_;
};

}  // namespace nanofibers
