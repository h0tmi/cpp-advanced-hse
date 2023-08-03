#include <iostream>

template <class T>
class DoubleArray {
public:
    DoubleArray(size_t first_size, size_t second_size)
        : first_size_(first_size), second_size_(second_size) {
        puts("constructor");
        // Leaks memory all over the place.
        //    first_ = new T[first_size_];
        //    second_ = new T[second_size_];

        // Even making a working example is hard, this is just a pain in the ass, that's going to be
        // very clear during any demonstration.

        // Only works for std::bad_alloc, doesn't cover throwing constructors.
        //    first_ = new(std::nothrow) T[first_size_];
        //    second_ = new(std::nothrow) T[second_size_];
        //    if (!first_ || !second_) {
        //      Free();
        //      throw std::bad_alloc{};
        //    }

        // Works in general.
        first_ = new T[first_size_];
        try {
            second_ = new T[second_size_];
        } catch (const std::exception&) {
            delete[] first_;
            throw;
        }
    }

    // General pattern for strong exception safety:
    // Op:
    //   DangerousCodeThatMightThrow();
    //   ApplyStateChanges();

    DoubleArray& operator=(const DoubleArray& other) {
        puts("copy");
        // What if this throws?
        auto new_first = new T[other.first_size_];
        // What about this?
        auto new_second = new T[other.second_size_];
        std::copy(other.first_, other.first_ + other.first_size_, new_first);
        std::copy(other.second_, other.second_ + other.second_size_, new_second);
        // We basically need two layers of try catch expressions here :(

        Free();

        first_size_ = other.first_size_;
        second_size_ = other.second_size_;
        first_ = new_first;
        second_ = new_second;

        return *this;
    }

    // Much better, but then we could have just used smart pointers all along.
    //  DoubleArray& operator=(const DoubleArray& other) {
    //    puts("copy");
    //
    //    std::unique_ptr<T[]> first(new T[other.first_size_]);
    //    std::unique_ptr<T[]> second(new T[other.second_size_]);
    //    first_size_ = other.first_size_;
    //    second_size_ = other.second_size_;
    //    delete[] std::exchange(second_, second.release());
    //    delete[] std::exchange(first_, first.release());
    //
    //    return *this;
    //  }

    void Free() {
        delete[] second_;
        delete[] first_;
    }

    ~DoubleArray() {
        puts("destructor");
        Free();
    }

private:
    T* first_;
    size_t first_size_;
    T* second_;
    size_t second_size_;
};

struct KekType {
    KekType() {
        //    static int x = 0;
        //    if (x++ == 2) {
        //      throw std::runtime_error("I am an error!");
        //    }
    }

    static void* operator new[](std::size_t sz) {
        auto res = ::operator new[](sz);
        printf("new %zu = %p\n", sz, res);
        return res;
    }

    static void* operator new[](std::size_t sz, const std::nothrow_t& tag) {
        auto res = ::operator new[](sz, tag);
        printf("new nothrow %zu = %p\n", sz, res);
        return res;
    }

    static void operator delete[](void* ptr) noexcept {
        printf("delete %p\n", ptr);
        ::operator delete[](ptr);
    }

    static void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept {
        printf("delete nothrow %p\n", ptr);
        ::operator delete[](ptr, tag);
    }
};

int main() {
    try {
        DoubleArray<KekType> kek1(2, 3);
        DoubleArray<KekType> kek2(3, 5);
        kek2 = kek1;
    } catch (const std::exception& e) {
        puts(e.what());
    }
}