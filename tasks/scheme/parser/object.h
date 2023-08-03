#pragma once

#include <memory>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    int GetValue() const;
};

class Symbol : public Object {
public:
    const std::string& GetName() const;
};

class Cell : public Object {
public:
    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and convertion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj);

template <class T>
bool Is(const std::shared_ptr<Object>& obj);
