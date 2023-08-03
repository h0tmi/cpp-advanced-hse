#include "kv.hpp"
#include "strings.hpp"

#include <iostream>

auto main() -> int {
    auto storage = KVStorage{};

    // Store data.

    storage.Set("ttl", int{42});
    storage.Set("X-Accept", std::string{" Accept Please "});

    // Print data.

    std::cout << "TTL is " << storage.GetUnsafe<int>("ttl") << std::endl
              << "X-Accept header is `"
              << strings::TrimCopy(storage.GetUnsafe<std::string>("X-Accept")) << "`" << std::endl;
}
