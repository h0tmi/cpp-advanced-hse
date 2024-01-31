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
    Tokenizer(std::istream* in) : stream_(in) {
        Next();
    }

    bool IsEnd() {
        return std::holds_alternative<EndToken>(token_);
    }

    void Next() {
        while (stream_->peek() == ' ' || stream_->peek() == '\n') {
            stream_->get();
        }
        char ind_ch = SyntaxErrorChecker(stream_->get());
        if (ind_ch == '(') {
            token_ = BracketToken::OPEN;
        } else if (ind_ch == ')') {
            token_ = BracketToken::CLOSE;
        } else if (ind_ch == '\'') {
            token_ = QuoteToken();
        } else if (ind_ch == '.') {
            token_ = DotToken();
        } else if (std::isdigit(ind_ch) ||
                   ((ind_ch == '+' || ind_ch == '-') && std::isdigit(stream_->peek()))) {
            bool positive = true;
            if (ind_ch == '+') {
                ind_ch = SyntaxErrorChecker(stream_->get());
            } else if (ind_ch == '-') {
                positive = false;
                ind_ch = SyntaxErrorChecker(stream_->get());
            }
            int value = 0;
            while (isdigit(ind_ch)) {
                value = value * 10 + (ind_ch - '0');
                ind_ch = SyntaxErrorChecker(stream_->get());
            }
            stream_->putback(ind_ch);
            if (!positive) {
                value *= -1;
            }
            token_ = ConstantToken{value};
        } else if (ind_ch == '+' || ind_ch == '-') {
            std::string symbol;
            symbol += ind_ch;
            token_ = SymbolToken(symbol);
        } else if (ind_ch != EOF && ind_ch != '\n') {
            std::string extracted;
            while (ind_ch != ' ' && ind_ch != EOF) {
                extracted += ind_ch;
                ind_ch = SyntaxErrorChecker(stream_->get());
            }
            token_ = SymbolToken(extracted);
        } else {
            token_ = EndToken();
        }
    }

    Token GetToken() {
        return token_;
    }

private:
    char SyntaxErrorChecker(char c) {
        std::string regex = "+-.'()<=>*/#!? \n";
        if (std::isdigit(c) || std::isalpha(c) || regex.find(c) != std::string::npos || c == EOF) {
            return c;
        }
        throw SyntaxError("SyntaxError");
    }
    std::istream* stream_;
    Token token_ = EndToken();
};
