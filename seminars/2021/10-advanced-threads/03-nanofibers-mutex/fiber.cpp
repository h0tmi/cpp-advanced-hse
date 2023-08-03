#include "fiber.h"
#include "scheduler.h"

namespace nanofibers {

void Fiber::Trampoline() {
    GetCurrentFiber()->Run();
}

void Fiber::Run() {
    assert(GetState() == EFiberState::Runnable);
    routine_();
    GetCurrentScheduler()->Terminate();
}

}  // namespace nanofibers
