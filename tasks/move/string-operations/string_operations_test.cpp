#include <catch.hpp>
#include "allocations_checker.h"

#include <string_operations.h>

#include <atomic>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <tuple>
#include <type_traits>
#include <unistd.h>

using namespace std::string_view_literals;
using namespace std::string_literals;

template <typename T>
struct FunctionTraits;

template <typename R, typename... Args>
struct FunctionTraits<R(Args...)> {
    static constexpr bool kIsNoexcept = false;
    static constexpr size_t kNargs = sizeof...(Args);

    typedef R ResultType;

    template <size_t i>
    struct Arg {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type Type;
    };
};

// What if a student is pedantic and adds noexcept?
template <typename R, typename... Args>
struct FunctionTraits<R(Args...) noexcept> {
    static constexpr bool kIsNoexcept = true;
    static constexpr size_t kNargs = sizeof...(Args);

    typedef R ResultType;

    template <size_t i>
    struct Arg {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type Type;
    };
};

template <typename... Types, typename Func>
void ReturnTypeCheckerOr(Func*) {
    static_assert(!FunctionTraits<Func>::kIsNoexcept ||
                      !std::is_same_v<typename FunctionTraits<Func>::ResultType, std::string>,
                  "Noexcept function cannot return std::string");
    static_assert((std::is_same_v<typename FunctionTraits<Func>::ResultType, Types> || ...),
                  "Return type is incorrect!");
}

template <typename Func, size_t I, typename... Types>
void ArgCheckerOr() {
    static_assert(
        (std::is_same_v<typename FunctionTraits<Func>::template Arg<I>::Type, Types> || ...),
        "One of the argument is not correct!");
}

template <typename Func, typename... Types, size_t... I>
void AllArgCheckerOr(std::index_sequence<I...>) {
    (ArgCheckerOr<Func, I, Types...>(), ...);
}

// Checks whether all arguments are at least one of Types.
template <typename... Types, typename Func>
void ArgsCheckerOr(Func*) {
    AllArgCheckerOr<Func, Types...>(std::make_index_sequence<FunctionTraits<Func>::kNargs>{});
}

static constexpr const char kVeryLongString[] =
    "What is love? Baby don't hurt me. Don't hurt me. No more";
static_assert(std::string_view(kVeryLongString).size() > sizeof(std::string));

TEST_CASE("EndsStarts") {
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StartsWith("test", "te")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE_FALSE(StartsWith("", kVeryLongString)));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StartsWith(kVeryLongString, kVeryLongString)));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE_FALSE(StartsWith(kVeryLongString, "What is luf")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StartsWith("", "")));
    ReturnTypeCheckerOr<bool>(StartsWith);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(StartsWith);

    EXPECT_ZERO_ALLOCATIONS(REQUIRE(EndsWith("test", "st")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE_FALSE(EndsWith("", kVeryLongString)));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(EndsWith(kVeryLongString, kVeryLongString)));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE_FALSE(EndsWith(kVeryLongString, "Noo more")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(EndsWith("", "")));
    ReturnTypeCheckerOr<bool>(EndsWith);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(EndsWith);
}

TEST_CASE("Strip") {
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("st" == StripPrefix("test", "te")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StripPrefix("", kVeryLongString).empty()));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StripPrefix(kVeryLongString, kVeryLongString).empty()));
    EXPECT_ZERO_ALLOCATIONS(
        REQUIRE(kVeryLongString == StripPrefix(kVeryLongString, "What is luf")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StripPrefix("", "").empty()));
    ReturnTypeCheckerOr<std::string, std::string_view>(StripPrefix);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(StripPrefix);

    EXPECT_ZERO_ALLOCATIONS(REQUIRE("te" == StripSuffix("test", "st")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StripSuffix("", kVeryLongString).empty()));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StripSuffix(kVeryLongString, kVeryLongString).empty()));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(kVeryLongString == StripSuffix(kVeryLongString, "Noo more")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(StripSuffix("", "").empty()));
    ReturnTypeCheckerOr<std::string, std::string_view>(StripSuffix);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(StripSuffix);

    EXPECT_ZERO_ALLOCATIONS(
        REQUIRE(StripAsciiWhitespace("                                   ").empty()));
    EXPECT_ZERO_ALLOCATIONS(
        REQUIRE("some very long words, trust me, you will doom me after this" ==
                StripAsciiWhitespace("  \n\r\n\t\f   some very long words, trust me, you will doom "
                                     "me after this \n\n\t\f")));
    ReturnTypeCheckerOr<std::string, std::string_view>(StripAsciiWhitespace);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(
        StripAsciiWhitespace);

    // EASY.
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(kVeryLongString == ClippedSubstr(kVeryLongString, 0, 100500)));
    EXPECT_ZERO_ALLOCATIONS(
        REQUIRE("t is love? Baby don't hu" == ClippedSubstr(kVeryLongString, 3, 24)));
    ReturnTypeCheckerOr<std::string_view>(ClippedSubstr);
    ArgsCheckerOr<std::string_view, const std::string_view, size_t>(ClippedSubstr);
}

TEST_CASE("FileSystems") {
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("/" == RemoveSlash("/")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("/danlark/is/creating/awesome/tests" ==
                                    RemoveSlash("/danlark/is/creating/awesome/tests")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("/danlark/is/creating/awesome/tests" ==
                                    RemoveSlash("/danlark/is/creating/awesome/tests/")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("/" == AddSlash("/")));
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(
        REQUIRE("/danlark/is/creating/even/more/awesome/tests/" ==
                AddSlash("/danlark/is/creating/even/more/awesome/tests")));
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(
        REQUIRE("/danlark/is/creating/even/more/awesome/tests/" ==
                AddSlash("/danlark/is/creating/even/more/awesome/tests/")));
    auto path = "/a/b"s;
    for (size_t i = 0; i < 512; ++i) {
        path += "a";
        auto slashed = path + "/";
        EXPECT_ZERO_ALLOCATIONS(REQUIRE(path == RemoveSlash(slashed)));
        EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(slashed == AddSlash(path)));
        EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(slashed == AddSlash(slashed.data())));
        EXPECT_ZERO_ALLOCATIONS(REQUIRE(path.data() == RemoveSlash(path)));
    }
    ReturnTypeCheckerOr<std::string, std::string_view>(AddSlash);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(AddSlash);
    ReturnTypeCheckerOr<std::string, std::string_view>(RemoveSlash);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(RemoveSlash);
}

TEST_CASE("FileSystemsSplits") {
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("file" == Basename("/file")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("/" == Dirname("/very_very_very_stylish_file")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(
        "iiiiiiiiiiii\0iiiiiiiiiiiiiiiiiiiiin" ==
        Basename("/dir/dir/dir/dir/dir/dir/danlark/let/me/iiiiiiiiiiii\0iiiiiiiiiiiiiiiiiiiiin")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(
        "/dir/dir/dir/dir/dir/dir/danlark/let/me" ==
        Dirname("/dir/dir/dir/dir/dir/dir/danlark/let/me/iiiiiiiiiiii\0iiiiiiiiiiiiiiiiiiiiin")));
    ReturnTypeCheckerOr<std::string, std::string_view>(Basename);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(Basename);
    ReturnTypeCheckerOr<std::string, std::string_view>(Dirname);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(Dirname);
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(
        "/a/" == CollapseSlashes("///////////////////////////////////////////////////////a//"
                                 "/////////////////////////////////////")));
    EXPECT_ONE_ALLOCATION(REQUIRE(
        "/a/b/c/d/e/f/g/h/aa/bb/cc/dd/ee/ff/gg/a/heh" ==
        CollapseSlashes(
            "/a/b/c/d/e/f/g/h/aa/bb//cc/dd/ee/ff//gg////////////////////////////////////////a//"
            "/////////////////////////////////////heh")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE("/" == CollapseSlashes("//////")));
    EXPECT_ZERO_ALLOCATIONS(REQUIRE(CollapseSlashes("").empty()));
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(
        REQUIRE(kVeryLongString == CollapseSlashes(kVeryLongString)));
    ReturnTypeCheckerOr<std::string, std::string_view>(CollapseSlashes);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(CollapseSlashes);
}

TEST_CASE("Join") {
    std::vector v = {"a"sv, "b"sv, "c"sv, "d"sv, "e"sv, "z"sv, "p"sv};
    auto ans = "ahehehehbhehehehchehehehdhehehehehehehehzhehehehp"s;
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(ans == StrJoin(v, "heheheh")));
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(
        REQUIRE(ans != StrJoin(v,
                               "hehehehhehehehhehehehhehehehhehehehhehehehhehehehhehehehhehehehhehe"
                               "hehhehehehhehehehhehehehhehehehheheheh")));
    for (int i = 0; i < 100; ++i) {
        v.emplace_back(kVeryLongString);
        ans += "heheheh"s + v.back().data();
        EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(ans == StrJoin(v, "heheheh")));
    }
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(StrJoin({}, "danlark").empty()));
    ReturnTypeCheckerOr<std::string>(StrJoin);
    ArgsCheckerOr<const std::vector<std::string>&, const std::vector<std::string_view>&,
                  std::string, std::string_view, const std::string&>(StrJoin);
}

TEST_CASE("ReadN") {
    std::string_view almost_urandom = "/dev/urandom\0/haha"sv;
    almost_urandom.remove_suffix(5);
    for (int i = 0; i < 100; ++i) {
        auto once = ReadN(almost_urandom.data(), 100);
        EXPECT_ZERO_ALLOCATIONS(REQUIRE(!once.empty()));
    }
    std::string urandom = std::string(almost_urandom);
    for (int i = 0; i < 100; ++i) {
        auto once = ReadN(urandom, 100);
        EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(once != ReadN(urandom, 100)));
    }
    ReturnTypeCheckerOr<std::string, std::string_view>(ReadN);
    ArgsCheckerOr<const std::string&, size_t>(ReadN);
}

TEST_CASE("Split") {
    {
        std::vector<std::string> expected{"aba", "caba", "1"};
        EXPECT_ONE_ALLOCATION(auto result = StrSplit("aba caba 1", " "); REQUIRE(std::equal(
            expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{"aba"};
        EXPECT_ONE_ALLOCATION(auto result = StrSplit("aba", " "); REQUIRE(std::equal(
            expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{""};
        EXPECT_ONE_ALLOCATION(auto result = StrSplit("", " "); REQUIRE(std::equal(
            expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{"", ""};
        EXPECT_ONE_ALLOCATION(
            auto result = StrSplit("full match", "full match");
            REQUIRE(std::equal(expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{"", ""};
        EXPECT_ONE_ALLOCATION(
            auto result = StrSplit(kVeryLongString, kVeryLongString);
            REQUIRE(std::equal(expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{"just", "", "a", "test", ""};
        EXPECT_ONE_ALLOCATION(auto result = StrSplit("just  a test ", " "); REQUIRE(std::equal(
            expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{"hello", "world,no split here", "", "1", ""};
        EXPECT_ONE_ALLOCATION(
            auto result = StrSplit("hello, world,no split here, , 1, ", ", ");
            REQUIRE(std::equal(expected.begin(), expected.end(), result.begin(), result.end())));
    }
    {
        std::vector<std::string> expected{"", "a", "b c", "def", "g h "};
        EXPECT_ONE_ALLOCATION(
            auto result = StrSplit("  a  b c  def  g h ", "  ");
            REQUIRE(std::equal(expected.begin(), expected.end(), result.begin(), result.end())));
    }
    ReturnTypeCheckerOr<std::vector<std::string>, std::vector<std::string_view>>(StrSplit);
    ArgsCheckerOr<const std::string&, std::string_view, const std::string_view>(StrSplit);
}

TEST_CASE("StrCat") {
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(
        REQUIRE("abc 5 abcd 12" == StrCat("abc ", 5u, " abcd "sv, 12ull)));
    std::string s = kVeryLongString;
    EXPECT_ONE_ALLOCATION(REQUIRE(
        "What is love? Baby don't hurt me. Don't hurt me. No more1112What is love? Baby don't "
        "hurt me. Don't hurt me. No more0" == StrCat(s, 11l, 12u, kVeryLongString, 0ull)));
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(
        REQUIRE("-9223372036854775808" == StrCat(std::numeric_limits<long long>::min())));
    EXPECT_ONE_ALLOCATION(
        REQUIRE("-922337203685477580809223372036854775807-2147483648" ==
                StrCat(std::numeric_limits<long long>::min(), 0,
                       std::numeric_limits<long long>::max(), std::numeric_limits<int>::min())));
    EXPECT_ONE_ALLOCATION(
        REQUIRE("18446744073709551615042949672952147483647" ==
                StrCat(std::numeric_limits<unsigned long long>::max(), 0,
                       std::numeric_limits<unsigned int>::max(), std::numeric_limits<int>::max())));
    EXPECT_NO_MORE_THAN_ONE_ALLOCATION(REQUIRE(StrCat().empty()));
    ReturnTypeCheckerOr<std::string, std::string_view>(StrCat<int>);

    std::string result = StrCat(kVeryLongString, 1);
    std::string large_string(result.capacity() + 1, 'a');
    EXPECT_ONE_ALLOCATION(StrCat(large_string, 1));
}
