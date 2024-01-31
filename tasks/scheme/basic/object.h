#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include "tokenizer.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual std::string ToString() {
        throw std::runtime_error("Called deleted method of abstract class");
    }
    virtual void Bind(std::shared_ptr<Object>) {
        throw std::runtime_error("Can't bind to an abstract class");
    }
    virtual Object* GetCopy() {
        throw std::runtime_error("Not implemented");
    }
    virtual std::shared_ptr<Object> /*add*/ Evaluate() {
        throw std::runtime_error("Not implemented");
    }
    virtual void /*add*/ Apply() {
        throw std::runtime_error("Not implemented");
    }
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(const ConstantToken& token) : value_(token.value) {
    }
    int GetValue() const {
        return value_;
    }
    std::string ToString() {
        return std::to_string(value_);
    }
    void Bind(std::shared_ptr<Object>) {
        throw std::runtime_error("Can't bind to a number");
    }

private:
    int value_ = 0;
};

class Symbol : public Object {
public:
    Symbol(const SymbolToken& token) : symbol_(token.name) {
    }
    const std::string& GetName() const {
        return symbol_;
    }
    std::string ToString() {
        return symbol_;
    }
    void Bind(std::shared_ptr<Object>) {
        throw std::runtime_error("Can't bind to a symbol");
    }
private:
    std::string symbol_ = {};
};

class Cell : public Object {
public:
    Cell() {}
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_(first), second_(second) {
    }
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }
    std::string ToString() {
        return "(" + (first_ ? first_->ToString() : "") + "" + (second_ ? second_->ToString() : "") + ")";
        throw RuntimeError("Bad object");
    }
    void Bind(std::shared_ptr<Object> ptr) {
        second_ = ptr;
    }

private:
    std::shared_ptr<Object> first_ = nullptr;
    std::shared_ptr<Object> second_ = nullptr;
};

 ///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and convertion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    if (Is<T>(obj)) {
        return dynamic_pointer_cast<T>(obj);
    } 
    throw std::runtime_error("Bad cast");
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return dynamic_cast<T*>(obj.get()) != nullptr;
}
