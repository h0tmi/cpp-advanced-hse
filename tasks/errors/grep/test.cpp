#include <catch.hpp>

#include <grep.h>

#include <vector>
#include <set>
#include <tuple>
#include <memory>
#include <string>
#include <iostream>

#include <optional>

const auto kNull = std::nullopt;
using std::optional;

struct Match {
    std::string path;
    size_t line;
    size_t column;
    optional<std::string> after_match;

    bool operator==(const Match& rhs) const {
        return std::make_tuple(path, line, column, after_match) ==
               std::make_tuple(rhs.path, rhs.line, rhs.column, rhs.after_match);
    }
};

std::ostream& operator<<(std::ostream& out, const Match& m) {
    out << "File " << m.path << ", line " << m.line << ", column " << m.column << "\n";
    return out;
}

optional<std::string> MakeString(const char* s) {
    return std::make_optional<std::string>(s);
}

class CollectMatches {
public:
    CollectMatches() {
        matches_ = std::make_shared<std::vector<Match>>();
    }

    void OnError(const std::string& what) {
        std::cerr << "Fail: " << what << "\n";
    }

    void OnMatch(const std::string& path, size_t line, size_t column,
                 const optional<std::string>& after_match) {
        matches_->push_back(Match{path, line, column, after_match});
    }

    const std::vector<Match>& GetMatches() const {
        return *matches_;
    }

private:
    std::shared_ptr<std::vector<Match>> matches_;
};

TEST_CASE("Search test", "[grep]") {
    CollectMatches visitor;
    Grep("test.txt", "hello", visitor, GrepOptions());
    std::vector<Match> expected{Match{"test.txt", 1, 3, kNull}, Match{"test.txt", 1, 8, kNull},
                                Match{"test.txt", 3, 1, kNull}};

    REQUIRE(expected == visitor.GetMatches());
}

TEST_CASE("Search context test", "[grep]") {
    {
        CollectMatches visitor;
        Grep("test.txt", "lo", visitor, GrepOptions(3));
        std::vector<Match> expected{Match{"test.txt", 1, 6, MakeString("hel")},
                                    Match{"test.txt", 1, 11, MakeString("t.")},
                                    Match{"test.txt", 3, 4, MakeString("")}};

        REQUIRE(expected == visitor.GetMatches());
    }
    {
        CollectMatches visitor;
        Grep("test.txt", "a", visitor, GrepOptions(1, 2));
        std::vector<Match> expected{
            Match{"test.txt", 2, 1, MakeString("b")},  Match{"test.txt", 2, 3, MakeString("c")},
            Match{"test.txt", 5, 6, MakeString(" ")},  Match{"test.txt", 6, 8, MakeString("c")},
            Match{"test.txt", 8, 7, MakeString("b")},  Match{"test.txt", 8, 10, MakeString("")},
            Match{"test.txt", 10, 3, MakeString("a")}, Match{"test.txt", 10, 4, MakeString("a")}};

        REQUIRE(expected == visitor.GetMatches());
    }
}

TEST_CASE("Some codings fun", "[grep]") {
    auto filename = u8"тест.txt";
    {
        CollectMatches visitor;
        Grep(filename, u8"с", visitor, GrepOptions(2, 2));
        std::vector<Match> expected{Match{filename, 1, 11, MakeString(u8"ск")},
                                    Match{filename, 1, 12, MakeString(u8"ко")},
                                    Match{filename, 3, 13, MakeString(u8" к")},
                                    Match{filename, 3, 29, MakeString(u8"++")},
                                    Match{filename, 5, 22, MakeString(u8"ĕп")},
                                    Match{filename, 5, 70, MakeString(u8"та")},
                                    Match{filename, 8, 28, MakeString(u8"ок")},
                                    Match{filename, 8, 182, MakeString(u8"ам")},
                                    Match{filename, 10, 1, MakeString(u8"та")},
                                    Match{filename, 10, 50, MakeString(u8"еб")},
                                    Match{filename, 11, 57, MakeString(u8"ти")},
                                    Match{filename, 11, 78, MakeString(u8"ти")}};

        REQUIRE(expected == visitor.GetMatches());
    }
    {
        CollectMatches visitor;
        Grep(filename, u8"在", visitor, GrepOptions(2));
        std::vector<Match> expected{Match{filename, 13, 14, MakeString(u8"贝尔")},
                                    Match{filename, 13, 24, MakeString(u8"20")}};

        REQUIRE(expected == visitor.GetMatches());
    }
}

class FilesOnly {
public:
    FilesOnly() {
        files_ = std::make_shared<std::set<std::string>>();
    }

    void OnError(const std::string& what) {
        std::cerr << "Fail: " << what << "\n";
    }

    void OnMatch(const std::string& file, size_t, size_t, const optional<std::string>&) {
        files_->insert(file);
    }

    const std::set<std::string> GetFiles() const {
        return *files_;
    }

private:
    std::shared_ptr<std::set<std::string>> files_;
};

TEST_CASE("Recursive grep", "[grep]") {
    FilesOnly visitor;
    Grep("test_dir", ")", visitor, GrepOptions());

    std::set<std::string> expected{
        u8"test_dir/1/3/файлик.txt", u8"test_dir/1/3/другой файлик O_o.txt",
        u8"test_dir/1/empty",        "test_dir/1/image.jpg",
        "test_dir/another_key.zip",  "test_dir/2/key.txt",
        "test_dir/2/4/ok.txt"};

    REQUIRE(expected == visitor.GetFiles());
}
