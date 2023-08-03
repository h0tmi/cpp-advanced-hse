#include <huffman.h>

#include <catch.hpp>

#include <cstdint>
#include <stdexcept>
#include <vector>

TEST_CASE("Huffman default") {
    HuffmanTree tree;
    int dummy = 0;
    REQUIRE_THROWS_AS(tree.Move(1, dummy), std::invalid_argument);
}

TEST_CASE("Huffman too big") {
    HuffmanTree tree;
    std::vector<uint8_t> code_lengths;
    std::vector<uint8_t> values;
    int sum = 0;
    for (int i = 0; i < 17; ++i) {
        code_lengths.push_back(i);
        sum += i;
    }
    values.resize(sum);
    REQUIRE_THROWS_AS(tree.Build(code_lengths, values), std::invalid_argument);
}

TEST_CASE("Huffman ok rebuild") {
    HuffmanTree tree;
    for (int length = 0; length < 16; ++length) {
        std::vector<uint8_t> code_lengths;
        std::vector<uint8_t> values;
        int sum = 0;
        for (int i = 0; i < length; ++i) {
            code_lengths.push_back(i);
            sum += i;
        }
        values.resize(sum);
        tree.Build(code_lengths, values);
    }
}

TEST_CASE("Huffman fat") {
    HuffmanTree tree;
    for (int length = 1; length < 13; ++length) {
        std::vector<uint8_t> code_lengths;
        std::vector<uint8_t> values;
        int sum = 0;
        for (int i = 0; i < length; ++i) {
            if (i == length / 2) {
                code_lengths.push_back((1u << (i + 1)) + 1);
            } else {
                code_lengths.push_back(i);
            }
            sum += code_lengths.back();
        }
        values.resize(sum);
        REQUIRE_THROWS_AS(tree.Build(code_lengths, values), std::invalid_argument);
    }
}

TEST_CASE("Huffman short") {
    HuffmanTree tree;
    for (int length = 1; length < 16; ++length) {
        std::vector<uint8_t> code_lengths;
        std::vector<uint8_t> values;
        int sum = 0;
        for (int i = 1; i <= length; ++i) {
            code_lengths.push_back(i);
            sum += i;
        }
        values.resize(sum - 1, 1);
        REQUIRE_THROWS_AS(tree.Build(code_lengths, values), std::invalid_argument);
    }
}

TEST_CASE("Huffman real") {
    std::vector<uint8_t> code_lengths{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<uint8_t> values{6, 5, 4, 2};
    HuffmanTree tree;
    tree.Build(code_lengths, values);
    int x = 42;
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 6);
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE(x == 6);
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 5);
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE(x == 5);
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 4);
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 2);
    REQUIRE_THROWS_AS(([&tree, &x]() {
                          for (int i = 0; i < 32; ++i) {
                              bool is_terminal = tree.Move(1, x);
                              REQUIRE_FALSE(is_terminal);
                          }
                      }()),
                      std::invalid_argument);
}

TEST_CASE("Huffman sufferrings") {
    std::vector<uint8_t> code_lengths{0, 3, 0, 1, 4, 1, 3, 4, 2, 2, 1, 4, 2, 1, 2, 7};
    std::vector<uint8_t> values{1,  2,  3,  4,  5,  6,  17,  18, 19, 7,  33, 34, 0,
                                8,  20, 35, 49, 50, 9,  21,  36, 22, 51, 65, 66, 23,
                                81, 37, 82, 52, 67, 38, 113, 10, 83, 97, 114};
    HuffmanTree tree;
    tree.Build(code_lengths, values);
    int x = 0;
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(0, x));
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 34);

    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(0, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE(tree.Move(1, x));
    REQUIRE(x == 6);

    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(0, x));
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 9);

    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(1, x));
    REQUIRE_FALSE(tree.Move(0, x));
    REQUIRE_FALSE(tree.Move(0, x));
    REQUIRE(tree.Move(0, x));
    REQUIRE(x == 67);
}
