#include <cstring>
#include <iostream>
#include <vector>
#include <string_view>

// What do you need to store for a string?
// begin, size, capacity = 24 bytes

// Yet we don't really want to do any allocations for small strings, these can be very expensive.
// Solution: let's store small strings inside the object itself.

// Long string:
// [1 bit flag equal to 1 | 63 bits for capacity][8 bytes][8 bytes]
// Short strings:
// [1 bit flag equal to 0 | 7 bits for capacity][23 bytes for string + \0]
// In the end: SSO in libc works for strings of length <= 22.

// What you need to watch out for: when a short string is moved the address of the underlying
// string changes!

int main() {
    std::string kek = "hello world!!!!!!!!!!!";
    char data[24];
    std::memcpy(data, &kek, sizeof(kek));
    printf("is_long: %d\n", data[0] & 1);
    printf("size: %d\n", data[0] >> 1);  // Not completely relevant for long strings anymore.
    for (int i = 1; i < 24; ++i) {
        printf("%c", data[i]);
    }
    puts("");

    // IMPORTANT: Delete the code below if we end up making a crash-me problem about this.
    std::vector<std::string> s{{"kek"}};
    std::string_view sw(s[0]);
    for (int i = 0; i < 30; ++i) {
        s.emplace_back("bye-bye-bye");
    }
    std::cout << sw << "\n";
}