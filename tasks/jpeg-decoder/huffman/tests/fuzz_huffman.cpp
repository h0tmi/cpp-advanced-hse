#include <FuzzDataProvider.h>

#include <huffman.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FuzzedDataProvider provider(data, size);
    auto code_lengths = provider.ConsumeBytes<uint8_t>(16);
    size_t sum = 0;
    for (const auto& x : code_lengths) {
        sum += x;
    }
    auto values = provider.ConsumeBytes<uint8_t>(sum);
    auto path = provider.ConsumeRemainingBytes<uint8_t>();
    try {
        int value = 0;
        HuffmanTree tree;
        tree.Build(code_lengths, values);
        for (const auto& v : path) {
            tree.Move(v, value);
        }
        // Rebuild for testing that older values are removed.
        tree.Build(code_lengths, values);
    } catch (...) {
    }
    return 0;
}
