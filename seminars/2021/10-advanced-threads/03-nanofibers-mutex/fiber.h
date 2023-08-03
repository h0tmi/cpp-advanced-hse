#pragma once

#include "context.h"
#include "stack.h"
#include "intrusive_list.h"

#include <cassert>
#include <climits>
#include <functional>
#include <cstddef>
#include <memory>
#include <cstdio>

namespace nanofibers {

enum class EFiberState {
    Runnable,
    Suspended,  // 02-mutex: Add suspended state
    Finished,
};

class Fiber : public IntrusiveListItem<Fiber> {
    static constexpr size_t kDefaultStackSize = 4 * 4096;

public:
    explicit Fiber(std::function<void()> routine)
        : stack_{kDefaultStackSize}, routine_{std::move(routine)} {
        context_.rsp = stack_.Top();
        context_.rip = reinterpret_cast<void*>(Trampoline);
    }

    EFiberState GetState() const {
        return state_;
    }

    void SetState(EFiberState state) {
        state_ = state;
    }

    Context* GetContext() {
        return &context_;
    }

    void Run();

private:
    static void Trampoline();

private:
    EFiberState state_ = EFiberState::Runnable;
    Context context_ = {};
    Stack stack_;
    std::function<void()> routine_;
};

}  // namespace nanofibers
