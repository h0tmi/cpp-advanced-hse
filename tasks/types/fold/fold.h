#pragma once

struct Sum {};

struct Prod {};

struct Concat {};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
}

class Length {};
