#include <catch.hpp>
#include <editor.h>

TEST_CASE("Basic text typing", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.Type('b');
    editor.Type('c');
    REQUIRE(editor.GetText() == "abc");
}

TEST_CASE("Shift cursor", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.Type('b');
    editor.ShiftLeft();
    editor.Type('c');
    editor.ShiftRight();
    editor.Type('d');
    REQUIRE(editor.GetText() == "acbd");
}

TEST_CASE("Cursor boundaries check", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.ShiftLeft();
    editor.ShiftLeft();
    editor.Type('b');
    editor.ShiftRight();
    editor.ShiftRight();
    editor.Type('c');
    REQUIRE(editor.GetText() == "bac");
}

TEST_CASE("Backspace", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.Type('b');
    editor.Type('c');
    editor.Backspace();
    REQUIRE(editor.GetText() == "ab");

    editor.ShiftLeft();
    editor.Backspace();
    editor.Backspace();
    editor.Type('c');
    REQUIRE(editor.GetText() == "cb");
}

TEST_CASE("Undo/Redo typing", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.Type('b');
    editor.Type('c');
    editor.Undo();
    REQUIRE(editor.GetText() == "ab");
    editor.Undo();
    REQUIRE(editor.GetText() == "a");

    editor.Redo();
    REQUIRE(editor.GetText() == "ab");
    editor.Redo();
    REQUIRE(editor.GetText() == "abc");
}

TEST_CASE("Undo/Redo are safe", "[Editor]") {
    Editor editor;
    editor.Redo();
    editor.Undo();
    editor.Type('a');
    REQUIRE(editor.GetText() == "a");
    editor.Redo();
    editor.Undo();
    editor.Undo();
    REQUIRE(editor.GetText().empty());
}

TEST_CASE("Type after Undo discards last Type", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.Type('b');
    editor.Undo();
    editor.Type('c');
    editor.Redo();
    REQUIRE(editor.GetText() == "ac");
}

TEST_CASE("Undo/Redo ShiftLeft", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.ShiftLeft();
    editor.Undo();
    editor.Type('b');
    REQUIRE(editor.GetText() == "ab");

    editor.ShiftLeft();
    editor.Undo();
    editor.Redo();
    editor.Type('c');

    REQUIRE(editor.GetText() == "acb");
}

TEST_CASE("Undo/Redo ShiftRight", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.ShiftLeft();
    editor.ShiftRight();
    editor.Undo();
    editor.Type('b');
    REQUIRE(editor.GetText() == "ba");

    editor.ShiftRight();
    editor.Undo();
    editor.Redo();
    editor.Type('c');
    REQUIRE(editor.GetText() == "bac");
}

TEST_CASE("Undo/Redo Backspace", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.Type('b');
    editor.Backspace();
    editor.Undo();
    REQUIRE(editor.GetText() == "ab");

    editor.Redo();
    REQUIRE(editor.GetText() == "a");
}

TEST_CASE("AntiStupid test", "[Editor]") {
    Editor editor;
    for (int i = 0; i < 1000000; ++i) {
        editor.Type('a');
    }
}

TEST_CASE("Command does nothing - no undo", "[Editor]") {
    Editor editor;
    editor.Type('a');
    editor.ShiftRight();
    editor.Undo();
    REQUIRE(editor.GetText().empty());
}
