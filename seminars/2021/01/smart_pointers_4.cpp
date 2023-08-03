#include <iostream>
#include <memory>

// ===== std::unique_ptr =====

// Access object without any modifications to the pointer:
void UPAccess1(const std::unique_ptr<int>& ptr) {
    puts(
        "Kind of weird, we don't really need to know that the object is stored in a smart "
        "pointer.");
    *ptr = 21;
    printf("Value is %d.\n", *ptr);
}

void UPAccess2(int* ptr) {
    puts("Same semantics as UPAccess1, more general.");
    *ptr = 3;
    printf("Value is %d.\n", *ptr);
}

void UPAccess3(int& val) {
    puts("Also a good choice, if we know that the value is not null.");
    val = 666;
    printf("Value is %d.\n", val);
}

// Modify the pointer itself:
void UPModifyPointer(std::unique_ptr<int>& ptr) {
    ptr = std::make_unique<int>(5);
}

// Take ownership:
void UPTakeOwnership1(std::unique_ptr<int> ptr) {
    puts("We own the pointer now.");
}

void UPTakeOwnership2(std::unique_ptr<int>&& ptr) {
    puts("Works, but isn't as clean and concise.");
    puts("This is debatable though, it makes one less std::move call.");
}

// ===== std::shared_ptr =====

// Access object without any modifications to the pointer:
// Same gist as std::unique_ptr: take [const] (T* or T&) instead of const std::shared_ptr<T>& ptr.

void SPAccessBeCareful(std::shared_ptr<int> ptr) {
    // std::shared_ptr is small and seemingly cheap to copy, but doing this involves incrementing
    // and decrementing an atomic int, which requires internal synchronization and can be costly.
}

// Modify the pointer itself:
// Same as std::unique_ptr: take std::shared_ptr<T>& ptr.

// Share ownership:
void SPShareOwnership(std::shared_ptr<int> ptr) {  // Could be called asynchronously, for example.
    // Explicitly copying and incrementing the use count, clear from the signature.
    printf("sp use_count is %d.\n", ptr.use_count());
}

// Share ownership conditionally:
void SPShareOwnershipConditionally(const std::shared_ptr<int>& ptr) {
    if (*ptr == 5) {
        SPShareOwnership(ptr);
    } else {
        printf("sp value is %d.\n", *ptr);
    }
}

int main() {
    auto up = std::make_unique<int>(2);
    UPAccess1(up);
    UPAccess2(up.get());
    UPAccess3(*up);

    UPModifyPointer(up);
    printf("The value is now %d.", *up);

    // UPTakeOwnership1(up);  // doesn't compile, which is exactly what we want if ownership is
    // passed.
    UPTakeOwnership1(std::move(up));  // Nice and explicit.
    auto up1 = std::make_unique<int>(8);
    UPTakeOwnership2(std::move(up1));

    auto sp = std::make_shared<int>(3);
    SPShareOwnership(sp);
    SPShareOwnershipConditionally(sp);
    *sp = 5;
    SPShareOwnershipConditionally(sp);
}
