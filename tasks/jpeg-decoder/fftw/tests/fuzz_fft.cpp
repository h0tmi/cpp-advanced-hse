#include <FuzzDataProvider.h>

#include <fft.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    FuzzedDataProvider provider(data, size);
    std::vector<double> input(64);
    std::vector<double> output(64);
    DctCalculator calculator(8, &input, &output);
    for (int i = 0; i < 64; ++i) {
        input[i] = provider.ConsumeFloatingPoint<double>();
    }
    calculator.Inverse();
    return 0;
}
