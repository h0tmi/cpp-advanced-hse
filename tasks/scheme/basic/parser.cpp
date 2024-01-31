#include <parser.h>
#include <memory>
#include <variant>
#include "error.h"
#include "tokenizer.h"

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    const auto& current_token = tokenizer->GetToken();
    tokenizer->Next();
    if (std::holds_alternative<QuoteToken>(current_token)) {
        return std::make_shared<Cell>(std::make_shared<Symbol>(SymbolToken("quote")),
                                      ReadQuote(tokenizer));
    }
    if (std::holds_alternative<BracketToken>(current_token) &&
        std::get<BracketToken>(current_token) == BracketToken::OPEN) {
        return ReadList(tokenizer);
    }
    if (std::holds_alternative<SymbolToken>(current_token)) {
        return std::make_shared<Symbol>(std::get<SymbolToken>(current_token));
    }
    if (std::holds_alternative<ConstantToken>(current_token)) {
        return std::make_shared<Number>(std::get<ConstantToken>(current_token));
    }
    throw SyntaxError("[READ] Didn't catch any good symbol");
}

std::shared_ptr<Object> ReadQuote(Tokenizer* tokenizer) {
    const auto& current_token = tokenizer->GetToken();
    tokenizer->Next();
    if (std::holds_alternative<QuoteToken>(current_token)) {
        return std::make_shared<Cell>(std::make_shared<Symbol>(SymbolToken("quote")),
                                      ReadQuote(tokenizer));
    }
    if (std::holds_alternative<BracketToken>(current_token) &&
        std::get<BracketToken>(current_token) == BracketToken::OPEN) {
        return ReadListQuote(tokenizer);
    }
    if (std::holds_alternative<SymbolToken>(current_token)) {
        return std::make_shared<Symbol>(std::get<SymbolToken>(current_token));
    }
    if (std::holds_alternative<ConstantToken>(current_token)) {
        return std::make_shared<Number>(std::get<ConstantToken>(current_token));
    }
    throw SyntaxError("[READ_QUOTE] Didn't catch any good symbol");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
        std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE) {
        tokenizer->Next();
        return nullptr;
    }
    const auto& list = std::make_shared<Cell>(Read(tokenizer), nullptr);
    auto current_cell = list;
    bool is_pair = false;

    while (!(std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
             std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE)) {
        if (bool is_dot = std::holds_alternative<DotToken>(tokenizer->GetToken());
            is_dot && is_pair) {
            throw SyntaxError("[READ_LIST] Too many dots");
        } else if (is_dot) {
            is_pair = true;
            tokenizer->Next();
        } else if (const auto& current_element = Read(tokenizer);
                   std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
                   std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE &&
                   is_pair) {
            is_pair = false;
            current_cell->Bind(current_element);
        } else if (is_pair) {
            throw SyntaxError("[READ_LIST] Double(or more) dots");
        } else {
            const auto& new_cell = std::make_shared<Cell>(current_element, nullptr);
            current_cell->Bind(new_cell);
            current_cell = new_cell;
        }
    }
    if (is_pair) {
        throw SyntaxError("[READ_LIST] Unexpected dot");
    }
    tokenizer->Next();
    return list;
}

std::shared_ptr<Object> ReadListQuote(Tokenizer* tokenizer) {
    if (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
        std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE) {
        tokenizer->Next();
        return std::make_shared<Cell>();
    }
    const auto& list = std::make_shared<Cell>(ReadQuote(tokenizer), nullptr);
    auto current_cell = list;
    bool is_pair = false;

    while (!(std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
             std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE)) {
        if (bool is_dot = std::holds_alternative<DotToken>(tokenizer->GetToken());
            is_dot && is_pair) {
            throw SyntaxError("[READ_LIST_QUOTE] Too many dots");
        } else if (is_dot) {
            is_pair = true;
            tokenizer->Next();
        } else if (const auto& current_element = ReadQuote(tokenizer);
                   std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
                   std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE &&
                   is_pair) {
            is_pair = false;
            current_cell->Bind(current_element);
        } else if (is_pair) {
            throw SyntaxError("[READ_LIST_QUOTE] Double(or more) dots");
        } else {
            const auto& new_cell = std::make_shared<Cell>(current_element, nullptr);
            current_cell->Bind(new_cell);
            current_cell = new_cell;
        }
    }
    if (is_pair) {
        throw SyntaxError("[READ_LIST_QUOTE] Unexpected dot");
    }
    tokenizer->Next();
    return list;
}
