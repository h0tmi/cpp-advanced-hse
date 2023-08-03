#pragma once

#include <string>

struct VerboseTest {
    VerboseTest() {
        puts("VerboseTest()");
    }

    VerboseTest(const VerboseTest&) {
        puts("VerboseTest(const VerboseTest&)");
    }

    VerboseTest(VerboseTest&&) {
        puts("VerboseTest(VerboseTest&&)");
    }

    VerboseTest& operator=(const VerboseTest&) {
        puts("operator=(const VerboseTest&)");
        return *this;
    }

    VerboseTest& operator=(VerboseTest&&) {
        puts("operator=(VerboseTest&&)");
        return *this;
    }
};