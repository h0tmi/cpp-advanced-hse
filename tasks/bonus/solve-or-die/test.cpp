#include "status_macros.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <tuple>
#include <string_view>
#include <catch.hpp>
#include <utility>

#include "statusor.h"
#include "status.h"
#include "status_builder.h"

namespace {

absl::Status ReturnOk() {
    return absl::OkStatus();
}

absl::StatusBuilder ReturnOkBuilder() {
    return absl::StatusBuilder(absl::OkStatus());
}

absl::Status ReturnError(std::string_view msg) {
    return absl::Status(absl::StatusCode::kUnknown, msg);
}

absl::StatusBuilder ReturnErrorBuilder(std::string_view msg) {
    return absl::StatusBuilder(absl::Status(absl::StatusCode::kUnknown, msg));
}

absl::StatusOr<int> ReturnStatusOrValue(int v) {
    return v;
}

absl::StatusOr<int> ReturnStatusOrError(std::string_view msg) {
    return absl::Status(absl::StatusCode::kUnknown, msg);
}

template <class... Args>
absl::StatusOr<std::tuple<Args...>> ReturnStatusOrTupleValue(Args&&... v) {
    return std::tuple<Args...>(std::forward<Args>(v)...);
}

template <class... Args>
absl::StatusOr<std::tuple<Args...>> ReturnStatusOrTupleError(std::string_view msg) {
    return absl::Status(absl::StatusCode::kUnknown, msg);
}

absl::StatusOr<std::unique_ptr<int>> ReturnStatusOrPtrValue(int v) {
    return std::make_unique<int>(v);
}

}  // namespace

TEST_CASE("Works") {
    auto func = []() -> absl::Status {
        ASSIGN_OR_RETURN(int value1, ReturnStatusOrValue(1));
        REQUIRE(1 == value1);
        ASSIGN_OR_RETURN(const int value2, ReturnStatusOrValue(2));
        REQUIRE(2 == value2);
        ASSIGN_OR_RETURN(const int& value3, ReturnStatusOrValue(3));
        REQUIRE(3 == value3);
        ASSIGN_OR_RETURN(int value4, ReturnStatusOrError("EXPECTED"));
        value4 = 0;  // fix unused error
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithCommasInType") {
    auto func = []() -> absl::Status {
        ASSIGN_OR_RETURN((std::tuple<int, int> t1), ReturnStatusOrTupleValue(1, 1));
        REQUIRE((std::tuple{1, 1}) == t1);
        ASSIGN_OR_RETURN((const std::tuple<int, std::tuple<int, int>, int> t2),
                         ReturnStatusOrTupleValue(1, std::tuple{1, 1}, 1));
        REQUIRE((std::tuple{1, std::tuple{1, 1}, 1}) == t2);
        ASSIGN_OR_RETURN((std::tuple<int, std::tuple<int, int>, int> t3),
                         (ReturnStatusOrTupleError<int, std::tuple<int, int>, int>("EXPECTED")));
        t3 = {};  // fix unused error
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithStructuredBindings") {
    auto func = []() -> absl::Status {
        ASSIGN_OR_RETURN((const auto& [t1, t2, t3, t4, t5]),
                         ReturnStatusOrTupleValue(std::tuple{1, 1}, 1, 2, 3, 4));
        REQUIRE((std::tuple{1, 1}) == t1);
        REQUIRE(1 == t2);
        REQUIRE(2 == t3);
        REQUIRE(3 == t4);
        REQUIRE(4 == t5);
        ASSIGN_OR_RETURN(int t6, ReturnStatusOrError("EXPECTED"));
        t6 = 0;
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithParenthesesAndDereference") {
    auto func = []() -> absl::Status {
        int integer;
        int* pointer_to_integer = &integer;
        ASSIGN_OR_RETURN((*pointer_to_integer), ReturnStatusOrValue(1));
        REQUIRE(1 == integer);
        ASSIGN_OR_RETURN(*pointer_to_integer, ReturnStatusOrValue(2));
        REQUIRE(2 == integer);
        // Make the test where the order of dereference matters and treat the
        // parentheses.
        pointer_to_integer--;
        int** pointer_to_pointer_to_integer = &pointer_to_integer;
        ASSIGN_OR_RETURN((*pointer_to_pointer_to_integer)[1], ReturnStatusOrValue(3));
        REQUIRE(3 == integer);
        ASSIGN_OR_RETURN(int t1, ReturnStatusOrError("EXPECTED"));
        t1 = 0;
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithAppend") {
    auto fail_test_if_called = []() -> std::string {
        std::abort();
        return "FAILURE";
    };
    auto func = [&]() -> absl::Status {
        int value;
        ASSIGN_OR_RETURN(value, ReturnStatusOrValue(1), _ << fail_test_if_called());
        ASSIGN_OR_RETURN(value, ReturnStatusOrError("EXPECTED A"), _ << "EXPECTED B");
        return ReturnOk();
    };

    REQUIRE(func().message().find("EXPECTED A") != std::string_view::npos);
    REQUIRE(func().message().find("EXPECTED B") != std::string_view::npos);
}

TEST_CASE("WorksWithAdaptorFunc") {
    auto fail_test_if_called = [](absl::StatusBuilder builder) {
        std::abort();
        return builder;
    };
    auto adaptor = [](absl::StatusBuilder builder) { return builder << "EXPECTED B"; };
    auto func = [&]() -> absl::Status {
        int value;
        ASSIGN_OR_RETURN(value, ReturnStatusOrValue(1), fail_test_if_called(_));
        ASSIGN_OR_RETURN(value, ReturnStatusOrError("EXPECTED A"), adaptor(_));
        return ReturnOk();
    };

    REQUIRE(func().message().find("EXPECTED A") != std::string_view::npos);
    REQUIRE(func().message().find("EXPECTED B") != std::string_view::npos);
}

TEST_CASE("WorksWithThirdArgumentAndCommas") {
    auto fail_test_if_called = [](absl::StatusBuilder builder) {
        std::abort();
        return builder;
    };
    auto adaptor = [](absl::StatusBuilder builder) { return builder << "EXPECTED B"; };
    auto func = [&]() -> absl::Status {
        ASSIGN_OR_RETURN((const auto& [t1, t2, t3]), ReturnStatusOrTupleValue(1, 2, 3),
                         fail_test_if_called(_));
        REQUIRE(t1 == 1);
        REQUIRE(t2 == 2);
        REQUIRE(t3 == 3);
        ASSIGN_OR_RETURN((const auto& [t4, t5, t6]),
                         (ReturnStatusOrTupleError<int, int, int>("EXPECTED A")), adaptor(_));
        // Silence errors about the unused values.
        static_cast<void>(t4);
        static_cast<void>(t5);
        static_cast<void>(t6);
        return ReturnOk();
    };
    REQUIRE(func().message().find("EXPECTED A") != std::string_view::npos);
    REQUIRE(func().message().find("EXPECTED B") != std::string_view::npos);
}

TEST_CASE("WorksWithAppendIncludingLocals") {
    auto func = [&](const std::string& str) -> absl::Status {
        int value;
        ASSIGN_OR_RETURN(value, ReturnStatusOrError("EXPECTED A"), _ << str);
        return ReturnOk();
    };
    REQUIRE(func("EXPECTED B").message().find("EXPECTED A") != std::string_view::npos);
    REQUIRE(func("EXPECTED B").message().find("EXPECTED B") != std::string_view::npos);
}

TEST_CASE("WorksForExistingVariable") {
    auto func = []() -> absl::Status {
        int value = 1;
        ASSIGN_OR_RETURN(value, ReturnStatusOrValue(2));
        REQUIRE(2 == value);
        ASSIGN_OR_RETURN(value, ReturnStatusOrValue(3));
        REQUIRE(3 == value);
        ASSIGN_OR_RETURN(value, ReturnStatusOrError("EXPECTED"));
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("UniquePtrWorks") {
    auto func = []() -> absl::Status {
        ASSIGN_OR_RETURN(std::unique_ptr<int> ptr, ReturnStatusOrPtrValue(1));
        REQUIRE(*ptr == 1);
        return ReturnError("EXPECTED");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("UniquePtrWorksForExistingVariable") {
    auto func = []() -> absl::Status {
        std::unique_ptr<int> ptr;
        ASSIGN_OR_RETURN(ptr, ReturnStatusOrPtrValue(1));
        REQUIRE(*ptr == 1);

        ASSIGN_OR_RETURN(ptr, ReturnStatusOrPtrValue(2));
        REQUIRE(*ptr == 2);
        return ReturnError("EXPECTED");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("Chain") {
    auto func1 = []() -> absl::StatusOr<std::unique_ptr<int>> {
        std::unique_ptr<int> ptr;
        ASSIGN_OR_RETURN(ptr, ReturnStatusOrPtrValue(1));
        return ptr;
    };
    auto func2 = []() -> absl::StatusOr<std::unique_ptr<int>> {
        return absl::Status(absl::StatusCode::kInternal, "msg");
    };

    auto func3 = [=]() -> absl::StatusOr<std::unique_ptr<int>> {
        std::unique_ptr<int> ptr;
        ASSIGN_OR_RETURN(ptr, func1());
        ASSIGN_OR_RETURN(ptr, func2());
        return ptr;
    };

    auto func4 = [=]() -> absl::StatusOr<std::unique_ptr<int>> {
        std::unique_ptr<int> ptr;
        ASSIGN_OR_RETURN(ptr, func3());
        ASSIGN_OR_RETURN(ptr, func2());
        return ptr;
    };

    absl::StatusOr<std::unique_ptr<int>> result = func4();
    REQUIRE(absl::StatusCode::kInternal == result.status().code());
}

TEST_CASE("WorksGood") {
    auto func = []() -> absl::Status {
        RETURN_IF_ERROR(ReturnOk());
        RETURN_IF_ERROR(ReturnOk());
        RETURN_IF_ERROR(ReturnError("EXPECTED"));
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithBuilder") {
    auto func = []() -> absl::Status {
        RETURN_IF_ERROR(ReturnOkBuilder());
        RETURN_IF_ERROR(ReturnOkBuilder());
        RETURN_IF_ERROR(ReturnErrorBuilder("EXPECTED"));
        return ReturnErrorBuilder("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithLambda") {
    auto func = []() -> absl::Status {
        RETURN_IF_ERROR([] { return ReturnOk(); }());
        RETURN_IF_ERROR([] { return ReturnError("EXPECTED"); }());
        return ReturnError("ERROR");
    };

    REQUIRE(func().message() == "EXPECTED");
}

TEST_CASE("WorksWithReturnIfErrorAppend") {
    auto fail_test_if_called = []() -> std::string {
        std::abort();
        return "FAILURE";
    };
    auto func = [&]() -> absl::Status {
        RETURN_IF_ERROR(ReturnOk()) << fail_test_if_called();
        RETURN_IF_ERROR(ReturnError("EXPECTED A")) << "EXPECTED B";
        return absl::OkStatus();
    };

    REQUIRE(func().message().find("EXPECTED A") != std::string_view::npos);
    REQUIRE(func().message().find("EXPECTED B") != std::string_view::npos);
}

TEST_CASE("ReturnIfErrorWorksWithVoidReturnAdaptor") {
    int code = 0;
    int phase = 0;
    auto adaptor = [&](absl::Status) -> void { code = phase; };
    auto func = [&]() -> void {
        phase = 1;
        RETURN_IF_ERROR(ReturnOk()).With(adaptor);
        phase = 2;
        RETURN_IF_ERROR(ReturnError("EXPECTED A")).With(adaptor);
        phase = 3;
    };

    func();
    REQUIRE(phase == 2);
    REQUIRE(code == 2);
}
