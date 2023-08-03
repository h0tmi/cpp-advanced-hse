#pragma once

#include <racket/details/object.hpp>

#include <string>
#include <unordered_map>

namespace racket::details {

struct State {
    std::unordered_map<std::string, Object> variables;
};

}  // namespace racket::details
