#include "string-splitter.h"

#include <iostream>

int main() {
    StringSplitter sp;
    auto result = sp.AddDelimiter(' ').AddDelimiter('\t').Split("a b\tc d");

    for (const auto& s : result) {
        std::cout << s << "\n";
    }
}
