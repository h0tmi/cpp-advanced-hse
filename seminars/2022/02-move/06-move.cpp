#include "test_type_rich.h"

#include <iostream>
#include <vector>
#include <memory>

template <typename T>
class VectorWrapper {
public:
    VectorWrapper() {
        data_.reserve(100);
    }

    void PushBack(const T& val) {
        data_.push_back(val);
    }

    //  void PushBack(T&& val) {
    //    data_.push_back(std::move(val));
    //  }

    template <typename U>
    void PushBackUniversalReference(U&& val) {
        data_.push_back(std::forward<U>(val));
    }

    void PushBackByValue(T val) {
        data_.push_back(std::move(val));
    }

private:
    std::vector<T> data_;
};

int main() {
    VectorWrapper<VerboseTest> v;
    VerboseTest test;
    puts("");
    v.PushBack(test);
    puts("");
    v.PushBack(VerboseTest{});  // We want the move constructor to be called here.
    puts("");
    v.PushBackUniversalReference(VerboseTest{});
    puts("");
    v.PushBackUniversalReference(test);
    puts("");
    v.PushBackByValue(test);  // extra move-constructor
    puts("");
    v.PushBackByValue(std::move(test));  // extra move-constructor
    puts("");
    v.PushBackByValue(VerboseTest{});
}
