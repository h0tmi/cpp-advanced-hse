#include <fft.h>

#include <fftw3.h>

class DctCalculator::Impl {};

DctCalculator::DctCalculator(size_t width, std::vector<double> *input, std::vector<double> *output)
    : impl_() {
    (void)width;
    (void)input;
    (void)output;
}

void DctCalculator::Inverse() {
}

DctCalculator::~DctCalculator() = default;
