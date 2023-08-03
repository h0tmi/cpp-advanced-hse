#pragma once

#include <exception>

// Instead of std::bad_weak_ptr
class BadWeakPtr : public std::exception {};

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;
