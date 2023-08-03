#include "context.h"
#include "fiber.h"
#include "mutex.h"
#include "scheduler.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>

int main() {
    Context ctx;
    if (SaveContext(&ctx) == ESaveContextResult::Saved) {
        printf("First\n");
        JumpContext(&ctx);
    } else {
        printf("Second\n");
    }

    nanofibers::Mutex mutex;
    nanofibers::Scheduler scheduler;
    scheduler.Run([&] {
        scheduler.Spawn([&] {
            for (int j = 0; j < 5; ++j) {
                mutex.Lock();
                for (int i = 0; i < 5; ++i) {
                    printf("{Fiber #1} Ping\n");
                    scheduler.Yield();
                }
                mutex.Unlock();
                scheduler.Yield();
            }
        });
        scheduler.Spawn([&] {
            for (int j = 0; j < 5; ++j) {
                mutex.Lock();
                for (int i = 0; i < 5; ++i) {
                    printf("{Fiber #2} Pong\n");
                    scheduler.Yield();
                }
                mutex.Unlock();
                scheduler.Yield();
            }
        });
        printf("{Fiber #0} Spawned 2 children\n");
    });
}
