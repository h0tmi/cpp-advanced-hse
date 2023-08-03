#pragma once

struct Context {
    void* rip;
    void* rsp;

    void* rbp;
    void* rbx;

    void* r12;
    void* r13;
    void* r14;
    void* r15;
};

enum class ESaveContextResult : int {
    Saved = 0,
    Resumed = 1,
};

// Implemented in context.S
extern "C" ESaveContextResult SaveContext(Context* ctx) __attribute__((returns_twice));
extern "C" void JumpContext(Context* ctx) __attribute__((noreturn));
