#pragma once

#include <vector>

template <class Functor>
class ReverseBinaryFunctor {
public:
    ReverseBinaryFunctor(const Functor& func) : func_(func){};
    template <typename T, typename S>
    bool operator()(const T& a, const S& b) {
        return !func_(a, b);
    }

private:
    Functor func_;
};

template <class Functor>
class ReverseUnaryFunctor {
public:
    ReverseUnaryFunctor(Functor func) : func_(func){};
    template <typename T>
    bool operator()(const T& a) {
        return !func_(a);
    }

private:
    Functor func_;
};

template <class Functor>
ReverseUnaryFunctor<Functor> MakeReverseUnaryFunctor(Functor functor) {
    return ReverseUnaryFunctor<Functor>(functor);
}

template <class Functor>
ReverseBinaryFunctor<Functor> MakeReverseBinaryFunctor(Functor functor) {
    return ReverseBinaryFunctor<Functor>(functor);
}
