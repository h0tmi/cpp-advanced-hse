#include <catch.hpp>
#include <immutable_vector.h>

#include <string>
#include <vector>
#include <random>

template <typename T>
std::vector<T> GetValues(const ImmutableVector<T>& data) {
    std::vector<T> result;
    result.reserve(data.Size());
    for (size_t i = 0; i < data.Size(); ++i) {
        result.push_back(data.Get(i));
    }
    return result;
}

std::vector<int> MakeRange(int count) {
    std::vector<int> result(count);
    for (int i = 0; i < count; ++i) {
        result[i] = i;
    }
    return result;
}

TEST_CASE("Constructors", "[vector]") {
    ImmutableVector<int> empty;
    REQUIRE(0u == empty.Size());

    ImmutableVector<int> from_list{1, 2, 3};
    REQUIRE(3u == from_list.Size());
    std::vector<int> to_check{1, 2, 3};
    REQUIRE(to_check == GetValues(from_list));

    std::vector<std::string> origin{"aba", "caba"};
    ImmutableVector<std::string> range(origin.begin(), origin.end());
    REQUIRE(2u == range.Size());
    REQUIRE(origin == GetValues(range));
}

TEST_CASE("ChangeSize", "[vector]") {
    const int iterations_count = 1000;
    ImmutableVector<int> data;
    std::vector<ImmutableVector<int>> versions;
    versions.reserve(iterations_count);
    for (int i = 0; i < iterations_count; ++i) {
        data = data.PushBack(i);
        versions.push_back(data);
        REQUIRE(versions.back().Size() == i + 1);
        REQUIRE(MakeRange(i + 1) == GetValues(versions.back()));
    }
    for (int i = 0; i < iterations_count; ++i) {
        data = data.PopBack();
        REQUIRE(data.Size() == iterations_count - i - 1);
        REQUIRE(MakeRange(iterations_count - i - 1) == GetValues(data));
    }
}

TEST_CASE("Immutable", "[vector]") {
    ImmutableVector<int> vector_one{1, 2, 3};
    std::vector<int> to_check_one{1, 2, 3};
    REQUIRE(to_check_one == GetValues(vector_one));

    auto vector_two = vector_one.Set(0, 10);
    std::vector<int> to_check_two{10, 2, 3};
    REQUIRE(to_check_two == GetValues(vector_two));

    REQUIRE(to_check_one == GetValues(vector_one));
}

TEST_CASE("SetGet", "[vector]") {
    const int iterations_count = 1000;
    ImmutableVector<int> data(50);
    std::vector<int> vector_data(50);
    std::mt19937 gen(7346475);
    std::uniform_int_distribution<int> dist(0, data.Size() - 1);
    for (int i = 0; i < iterations_count; ++i) {
        int index = dist(gen);
        data = data.Set(index, i + 1);
        vector_data[index] = i + 1;
        for (int j = 0; j < 10; ++j) {
            int check_index = dist(gen);
            REQUIRE(vector_data[check_index] == data.Get(check_index));
        }
    }
    REQUIRE(vector_data == GetValues(data));
}

TEST_CASE("PushBack", "[vector]") {
    ImmutableVector<int> my_data;
    std::vector<int> ok_data;
    for (int i = 0; i < 100; ++i) {
        ok_data.push_back(i);
        my_data = my_data.PushBack(i);
        REQUIRE(ok_data == GetValues(my_data));
    }
}

TEST_CASE("Mix", "[vector]") {
    const int iterations_count = 1000;
    auto fill = MakeRange(iterations_count);
    ImmutableVector<int> data(fill.begin(), fill.end());
    std::mt19937 gen(7464754);
    for (int j = 0; j < 10; ++j) {
        auto my_data(data);
        std::uniform_int_distribution<int> dist(0, my_data.Size() - 1);
        for (int k = 0; k < iterations_count; ++k) {
            int index = dist(gen);
            REQUIRE(index == my_data.Get(index));
        }
    }

    for (int i = 0; i < iterations_count; ++i) {
        data = data.PopBack();
    }

    REQUIRE(data.Size() == 0u);
}

TEST_CASE("BigTest", "[vector]") {
    const int iterations_count = 100000;
    ImmutableVector<int> data;
    std::mt19937 gen(93475);
    std::vector<int> indices(iterations_count);
    for (int i = 0; i < iterations_count; ++i) {
        data = data.PushBack(i);
        indices[i] = i;
    }
    std::shuffle(indices.begin(), indices.end(), gen);
    for (int i = 0; i < iterations_count / 2; ++i) {
        data = data.Set(indices[i], -1);
    }

    for (size_t i = 0; i < data.Size(); ++i) {
        int cur_value = data.Get(indices[i]);
        if (i < iterations_count / 2) {
            REQUIRE(cur_value == -1);
        } else {
            REQUIRE(cur_value == indices[i]);
        }
    }
}
