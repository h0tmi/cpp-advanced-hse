#include "test_type_rich.h"

#include <iostream>
#include <vector>
#include <memory>

struct MyStruct {
    VerboseTest& GetHugeField() & {
        return HugeField;
    }

    VerboseTest&& GetHugeField() && {
        return std::move(HugeField);
    }

    VerboseTest HugeField;
};

int main() {
    auto field = MyStruct{}.GetHugeField();

    auto my_struct = MyStruct{};
    auto field2 = my_struct.GetHugeField();
}
