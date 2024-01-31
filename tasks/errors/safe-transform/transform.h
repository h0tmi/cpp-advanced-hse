#pragma once

#include <iterator>
#include <vector>
#include <iostream>

template <class Iterator, class Predicat, class Functor>
void TransformIf(Iterator begin, Iterator end, Predicat p, Functor f) {
    std::vector<typename Iterator::value_type> log;
    log.reserve(std::distance(begin, end));
    auto i = begin;
    try {
        while (i != end) {

            try {
                log.push_back(*i);
            } catch (...) {
            }

            if (p(*i)) {
                f(*i);
            }
            ++i;
        }
    } catch (...) {
        i = begin;
        for (const auto& val : log) {
            (*i) = val;
            ++i;
        }
        throw;
    }
}
