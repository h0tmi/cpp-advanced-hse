// Don't change this file, it is not sent to the server.

#pragma once

#include <cstddef>
#include <vector>
#include <memory>

class DctCalculator {
public:
    // input and output are width by width matrices, first row, then
    // the second row.
    DctCalculator(size_t width, std::vector<double> *input, std::vector<double> *output);

    void Inverse();

    ~DctCalculator();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
