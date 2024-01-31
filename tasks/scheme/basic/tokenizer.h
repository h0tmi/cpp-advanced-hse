#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <complex>
#include <cstdio>
#include <exception>
#include <ios>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <variant>
#include <optional>
#include <type_traits>
#include <istream>
#include "error.h"

struct SymbolToken {
    std::string name;

    SymbolToken(const std::string& rhs) : name(rhs) {
    }

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
};
struct EndToken {
    bool operator==(const EndToken&) const {
        return true;
    }
};

using Token =
    std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, EndToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);
    bool IsEnd();
    void Next();
    Token GetToken();

private:
    char SyntaxErrorChecker(char c);

private:
    std::istream* stream_;
    Token token_ = EndToken();
};
