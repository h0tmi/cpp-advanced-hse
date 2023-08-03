#pragma once

#include <racket/details/state.hpp>

#include <iostream>

namespace racket {

class Interpreter {
public:
    Interpreter();

    auto Run(const std::string& expression) -> std::string;

private:
    details::State state_;
};

}  // namespace racket
