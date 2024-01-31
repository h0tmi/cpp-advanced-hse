#include "tokenizer.h"

Tokenizer::Tokenizer(std::istream* in) : stream_(in) {
    Next();
}

bool Tokenizer::IsEnd() {
    return std::holds_alternative<EndToken>(token_);
}

void Tokenizer::Next() {
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

Token Tokenizer::GetToken() {
    return token_;
}

char Tokenizer::SyntaxErrorChecker(char c) {
    std::string regex = "+-.'()<=>*/#!? \n";
    if (std::isdigit(c) || std::isalpha(c) || regex.find(c) != std::string::npos || c == EOF) {
        return c;
    }
    throw SyntaxError("SyntaxError");
}
