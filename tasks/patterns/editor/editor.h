#pragma once

class Editor {
public:
    const std::string &GetText() const {
        throw std::runtime_error("Not implemented");
    }

    void Type(char c) {
    }

    void ShiftLeft() {
    }

    void ShiftRight() {
    }

    void Backspace() {
    }

    void Undo() {
    }

    void Redo() {
    }
};
