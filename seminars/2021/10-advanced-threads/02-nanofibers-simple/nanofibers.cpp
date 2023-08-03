#include "context.h"
#include "fiber.h"
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

    nanofibers::Scheduler scheduler;
    scheduler.Run([&] {
        scheduler.Spawn([&] {
            for (int i = 0; i < 5; ++i) {
                printf("{Fiber #1} Ping\n");
                scheduler.Yield();
            }
        });
        scheduler.Spawn([&] {
            for (int i = 0; i < 5; ++i) {
                printf("{Fiber #2} Pong\n");
                scheduler.Yield();
            }
        });
        printf("{Fiber #0} Spawned 2 children\n");
    });
}
