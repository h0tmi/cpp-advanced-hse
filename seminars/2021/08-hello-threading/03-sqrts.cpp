#include "timer.h"

#include <cmath>
#include <future>
#include <iostream>
#include <limits>
#include <numeric>
#include <thread>
#include <vector>

struct Options {
    unsigned Begin = 0;
    unsigned End = 1'000'000'000;
    unsigned NumThreads = std::thread::hardware_concurrency();

    static Options Parse(int argc, const char* argv[]) {
        Options options;
        if (argc > 1) {
            options.End = std::atoi(argv[1]);
        }
        if (argc > 2) {
            options.NumThreads = std::atoi(argv[2]);
        }
        return options;
    }
};

void SumSqrts(const Options& ctx, int index, double* result) {
    double sum = 0;
    for (unsigned i = ctx.Begin + index; i < ctx.End; i += ctx.NumThreads) {
        sum += std::sqrt(i);
    }
    *result = sum;
}

int main(int argc, const char* argv[]) {
    Options options = Options::Parse(argc, argv);
    std::cout << "num_threads: " << options.NumThreads << ", limit: " << options.End << std::endl;

    // Map
    timer::Timer timer;
    std::vector<std::thread> workers;
    std::vector<double> results(options.NumThreads, 0.0);
    for (unsigned i = 0; i < options.NumThreads; ++i) {
        workers.emplace_back(SumSqrts, std::cref(options), i, &results[i]);
    }
    for (std::thread& t : workers) {
        t.join();
    }

    // Reduce
    double sum = std::accumulate(results.begin(), results.end(), 0.0);
    std::cout << "sum: " << sum << ", elapsed: " << timer.Elapsed() << std::endl;
}
