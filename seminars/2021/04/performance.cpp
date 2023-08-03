#include <iostream>
#include <stdexcept>

void Thrower() {
    throw std::runtime_error("All Bad");
}

int ExitCode() {
    return -1;
}

void Catcher() {
    try {
        Thrower();
    } catch (...) {
    }
}

int Checker() {
    if (ExitCode() < 0) {
        std::cout << "";  // to make it a little bit slower
        return -1;
    }
    return 100;
}

template <typename Func>
void Tester(Func func, unsigned loop) {
    for (unsigned i = 0; i != loop; ++i) {
        func();
    }
}

constexpr unsigned kCount = 10000000;

/*
run with argument 0 or 1

time ./perf 0
or
time ./perf 1
*/
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "test number expected\n";
        return 0;
    }

    bool is_first{argv[1][0] == '1'};

    if (is_first) {
        Tester(Catcher, kCount);
    } else {
        Tester(Checker, kCount);
    }
}

/*
expected:

perf 0  0.18s user 0.00s system 98% cpu 0.183 total
perf 1  15.58s user 0.00s system 99% cpu 15.587 total
*/
