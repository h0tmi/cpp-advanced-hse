#pragma once

#include <cstdint>
#include <iterator>
#include <utility>

template <class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {  // NOLINT
        return begin_;
    }

    Iterator end() const {  // NOLINT
        return end_;
    }

private:
    Iterator begin_, end_;
};

class ForIterator {
public:
    ForIterator(int64_t value, int64_t step = 1) : it_(value), step_(step){};

    int64_t operator*() const {
        return it_;
    }

    ForIterator& operator++() {
        it_ += step_;
        return *this;
    }

    bool operator==(const ForIterator& other) {
        return it_ == other.it_;
    }

    bool operator!=(const ForIterator& other) {
        return (step_ > 0 ? it_ < other.it_ : it_ > other.it_);
    }

private:
    int64_t it_;
    int64_t step_;
};

IteratorRange<ForIterator> Range(int64_t to) {
    return IteratorRange(ForIterator(0), ForIterator(to));
}

IteratorRange<ForIterator> Range(int64_t from, int64_t to, int64_t step = 1) {
    return IteratorRange(ForIterator(from, step), ForIterator(to, step));
}

template <typename F, typename S>
class ZipIterator {
public:
    ZipIterator(const F first, const S second) : iterator1_(first), iterator2_(second){};
    ZipIterator& operator++() {
        ++iterator1_;
        ++iterator2_;
        return *this;
    }
    auto operator*() const {
        return std::pair(*iterator1_, *iterator2_);
    }
    auto operator*() {
        return std::pair(*iterator1_, *iterator2_);
    }
    bool operator!=(const ZipIterator& other) const {
        return iterator1_ != other.iterator1_ && iterator2_ != other.iterator2_;
    }
    bool operator!=(const ZipIterator& other) {
        return iterator1_ != other.iterator1_ && iterator2_ != other.iterator2_;
    }

private:
    F iterator1_;
    S iterator2_;
};

template <class F, class S>
auto Zip(const F& first, const S& second) {
    return IteratorRange(ZipIterator(std::begin(first), std::begin(second)),
                         ZipIterator(std::end(first), std::end(second)));
}

template <typename T>
class GroupIterator {
public:
    GroupIterator(const T begin, const T end) : begin_(begin), end_(end) {
        current_end_ = begin_;

        while (current_end_ != end_ && *current_end_ == *begin_) {
            ++current_end_;
        }
    }

    IteratorRange<T> operator*() const {
        return IteratorRange<T>(begin_, current_end_);
    }

    GroupIterator& operator++() {
        if (current_end_ != end_ && *current_end_ == *begin_) {
            ++current_end_;
        }
        begin_ = current_end_;
        while (current_end_ != end_ && *current_end_ == *begin_) {
            ++current_end_;
        }
        return *this;
    }

    bool operator==(const GroupIterator& other) {
        return begin_ == other.begin_ && end_ == other.end_;
    }

    bool operator!=(const GroupIterator& other) {
        return begin_ != other.begin_ || end_ != other.end_;
    }

private:
    T begin_;
    T end_;
    T current_end_;
};

template <class T>
auto Group(const T& element) {
    return IteratorRange(GroupIterator(std::begin(element), std::end(element)),
                         GroupIterator(std::end(element), std::end(element)));
}
