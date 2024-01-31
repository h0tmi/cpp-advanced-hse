#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include "object.h"

template <typename T = void>
class Action {

private:
    std::string name_;
    std::function<T> func_;
};

class Interpreter {
public:
    std::string Run(const std::string&);
    std::shared_ptr<Object> Evaluate();

private:
    std::unordered_map<std::string, std::shared_ptr<Action<>>()> action_register_;
};
