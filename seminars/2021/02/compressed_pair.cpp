#include <iostream>

// Working example, modulo default constructors etc.
// Since we don't want to show the solution, here are the key points that *need* to be shown:
//   - The signature below, talk about why all the template parameters are needed.
//   - Remind how to specialize templates.
//   - For empty non-final classes: derive and return *this pointer.
//   - For other cases (default): store as member.
// We want to try to outline the code here.

template <typename T, size_t I, bool = std::is_empty_v<T> && !std::is_final_v<T>>
struct CompressedPairElement {
    // Constructors.
    // Getters: return value.
    T value;
};

template <typename T, size_t I>
struct CompressedPairElement<T, I, true> : public T {
    // Constructors.
    // Getters: return *this.
};

template <typename F, typename S>
class CompressedPair : private CompressedPairElement<F, 0>, private CompressedPairElement<S, 1> {
    using First = CompressedPairElement<F, 0>;
    using Second = CompressedPairElement<S, 1>;

public:
    // Constructors.
    // Getters for first and second.
};

int main() {
    CompressedPair<int, std::allocator<int>> kek1(2, std::allocator<int>{});
    std::pair<int, std::allocator<int>> kek2(2, std::allocator<int>{});
    printf("%zu\n", sizeof(kek1));  // We want this to be 4.
    printf("%zu\n", sizeof(kek2));  // This is 8.

    // Usage in standard structures: allocator can be efficiently stored with one of the other
    // fields, same goes for deleters in unique_ptr.
    return 0;
}