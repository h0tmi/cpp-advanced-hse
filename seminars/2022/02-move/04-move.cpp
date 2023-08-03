#include "test_type_rich.h"

#include <iostream>

VerboseTest Foo1() {
    return VerboseTest{};
}

VerboseTest Foo2() {
    VerboseTest kek;
    return kek;
}

VerboseTest Foo3() {
    VerboseTest kek;
    // Prevents copy-elision (more like move-elision in this case).
    return std::move(kek);
}

VerboseTest Foo4() {
    VerboseTest kek;
    VerboseTest& keker = kek;
    // This object might be referenced somewhere else outside this function, e.g. T& ref =
    // passed_arg.
    return keker;
}

VerboseTest Foo5() {
    VerboseTest kek;
    VerboseTest& keker = kek;
    // We can force a move since we know this is actually a safe local variable.
    return std::move(keker);
}

// TL; DR: return std::move(something) is rarely needed and can harm performance, if the extra
// move is expensive.

int main() {
    { VerboseTest ex1 = Foo1(); }
    puts("");
    { VerboseTest ex2 = Foo2(); }
    puts("");
    { VerboseTest ex3 = Foo3(); }
    puts("");
    { VerboseTest ex4 = Foo4(); }
    puts("");
    { VerboseTest ex5 = Foo5(); }
}
