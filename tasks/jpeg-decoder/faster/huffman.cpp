#include <huffman.h>

class HuffmanTree::Impl {};

HuffmanTree::HuffmanTree() = default;

void HuffmanTree::Build(const std::vector<uint8_t> &code_lengths,
                        const std::vector<uint8_t> &values) {
    (void)code_lengths;
    (void)values;
}

bool HuffmanTree::Move(bool bit, int &value) {
    (void)bit;
    (void)value;

    return true;
}

HuffmanTree::HuffmanTree(HuffmanTree &&) = default;

HuffmanTree &HuffmanTree::operator=(HuffmanTree &&) = default;

HuffmanTree::~HuffmanTree() = default;
