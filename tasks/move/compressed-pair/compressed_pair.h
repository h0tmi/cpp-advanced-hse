#pragma once

#include <ios>
#include <type_traits>
#include <memory>

template <typename F, typename S,
          bool = std::is_empty_v<F> & !std::is_final_v<F> & !std::is_base_of_v<S, F> &
                 !std::is_base_of_v<F, S>,
          bool = std::is_empty_v<S> & !std::is_final_v<S> & !std::is_base_of_v<F, S> &
                 !std::is_base_of_v<S, F>>
class CompressedPair {};

template <typename F, typename S>
class CompressedPair<F, S, false, false> {
public:
    CompressedPair() : first_(), second_(){};
    CompressedPair(const F& first, const S& second) : first_(first), second_(second) {
    }
    CompressedPair(F&& first, S&& second) : first_(std::move(first)), second_(std::move(second)) {
    }
    CompressedPair(const F& first, S&& second) : first_(first), second_(std::move(second)) {
    }
    CompressedPair(F&& first, const S& second) : first_(std::move(first)), second_(second) {
    }

    CompressedPair& operator=(const CompressedPair& other) {
        first_ = other.first_;
        second_ = other.second_;
        return *this;
    }
    CompressedPair& operator=(const CompressedPair&& other) {
        first_ = std::move(other.first_);
        second_ = std::move(other.second_);
        return *this;
    }
    ~CompressedPair() = default;

    const F& GetFirst() const {
        return first_;
    }

    F& GetFirst() {
        return first_;
    }

    const S& GetSecond() const {
        return second_;
    }

    S& GetSecond() {
        return second_;
    }

private:
    F first_;
    S second_;
};

template <typename F, typename S>
class CompressedPair<F, S, false, true> : private S {
public:
    CompressedPair() = default;
    CompressedPair(const F& first, [[maybe_unused]] const S& second) : first_(first) {
    }
    CompressedPair(F&& first, [[maybe_unused]] S&& second) : first_(std::move(first)) {
    }
    CompressedPair(const F& first, [[maybe_unused]] S&& second) : first_(first) {
    }
    CompressedPair(F&& first, [[maybe_unused]] const S& second) : first_(std::move(first)) {
    }

    CompressedPair& operator=(const CompressedPair& other) {
        first_ = other.first_;
        return *this;
    }
    CompressedPair& operator=(const CompressedPair&& other) {
        first_ = std::move(other.first_);
        return *this;
    }
    ~CompressedPair() = default;

    const F& GetFirst() const {
        return first_;
    }

    F& GetFirst() {
        return first_;
    }

    const S& GetSecond() const {
        return *this;
    }

    S& GetSecond() {
        return *this;
    }

private:
    F first_;
};

template <typename F, typename S>
class CompressedPair<F, S, true, false> : private F {
public:
    CompressedPair() = default;
    CompressedPair([[maybe_unused]] const F& first, const S& second) : second_(second) {
    }
    CompressedPair([[maybe_unused]] const F&& first, const S&& second)
        : second_(std::move(second)) {
    }
    CompressedPair([[maybe_unused]] const F&& first, const S& second) : second_(second) {
    }
    CompressedPair([[maybe_unused]] F& first, const S&& second) : second_(std::move(second)) {
    }

    CompressedPair& operator=(const CompressedPair& other) {
        second_ = other.second_;
        return *this;
    }
    CompressedPair& operator=(const CompressedPair&& other) {
        second_ = std::move(other.second_);
        return *this;
    }
    ~CompressedPair() = default;

    const F& GetFirst() const {
        return *this;
    }

    F& GetFirst() {
        return *this;
    }
    const S& GetSecond() const {
        return second_;
    }

    S& GetSecond() {
        return second_;
    }

private:
    S second_;
};

template <typename F, typename S>
class CompressedPair<F, S, true, true> : private F, private S {
public:
    CompressedPair() = default;
    CompressedPair([[maybe_unused]] const F& first, [[maybe_unused]] const S& second) {
    }
    CompressedPair([[maybe_unused]] F&& first, [[maybe_unused]] S&& second) {
    }
    CompressedPair([[maybe_unused]] F&& first, [[maybe_unused]] const S& second) {
    }
    CompressedPair([[maybe_unused]] const F& first, [[maybe_unused]] S&& second) {
    }

    ~CompressedPair() = default;

    F& GetFirst() {
        return *this;
    }

    S& GetSecond() {
        return *this;
    }
};
