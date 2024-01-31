#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stack>
#include <string>

class Command {
public:
    virtual bool Execute() = 0;
    virtual bool Undo() = 0;
    ~Command() = default;
};

class TypeCommand : public Command {
public:
    TypeCommand(std::string& buffer, size_t& position, char& c)
        : Command(), buffer_(buffer), position_(position), c_(c) {
    }
    bool Execute() override {
        buffer_.insert(position_, 1, c_);
        ++position_;
        return true;
    }
    bool Undo() override final {
        --position_;
        buffer_.erase(position_, 1);
        return true;
    }

private:
    std::string& buffer_;
    size_t& position_;
    char c_;
};

class ShiftTRightCommand : public Command {
public:
    ShiftTRightCommand(size_t& position, size_t buffer_size)
        : buffer_size_(buffer_size), position_(position) {
    }
    bool Execute() override final {
        if (position_ < buffer_size_) {
            ++position_;
            return true;
        }
        return false;
    }
    bool Undo() override {
        if (position_) {
            --position_;
            return true;
        }
        return false;
    }

private:
    size_t buffer_size_;
    size_t& position_;
};

class ShiftLeftCommand : public Command {
public:
    ShiftLeftCommand(size_t& position, size_t buffer_size)
        : buffer_size_(buffer_size), position_(position) {
    }
    bool Execute() override final {
        if (position_) {
            --position_;
            return true;
        }
        return false;
    }
    bool Undo() override {
        if (position_ < buffer_size_) {
            ++position_;
            return true;
        }
        return false;
    }

private:
    size_t buffer_size_;
    size_t& position_;
};

class BackspaceCommand : public Command {
public:
    BackspaceCommand(std::string& buffer, size_t& position)
        : Command(), buffer_(buffer), position_(position) {
    }
    bool Execute() override final {
        --position_;
        c_ = buffer_[position_];
        buffer_.erase(position_, 1);
        return true;
    }
    bool Undo() override {
        buffer_.insert(position_, 1, c_);
        ++position_;
        return false;
    }

private:
    std::string& buffer_;
    size_t& position_;
    char c_;
};
class Editor {
public:
    const std::string& GetText() const {
        return buffer_;
    }

    void Type(char c) {
        std::shared_ptr<Command> cmd = std::make_shared<TypeCommand>(buffer_, position_, c);
        Execute(cmd);
    }

    void ShiftLeft() {
        std::shared_ptr<Command> cmd =
            std::make_shared<ShiftLeftCommand>(position_, buffer_.size());
        Execute(cmd);
    }

    void ShiftRight() {
        std::shared_ptr<Command> cmd =
            std::make_shared<ShiftTRightCommand>(position_, buffer_.size());
        Execute(cmd);
    }

    void Backspace() {
        if (position_) {
            Execute(std::make_shared<BackspaceCommand>(buffer_, position_));
        }
    }

    void Undo() {
        if (!undo_.empty()) {
            auto cmd = undo_.top();
            undo_.pop();
            cmd->Undo();
            redo_.push(cmd);
        }
    }

    void Redo() {
        if (!redo_.empty()) {
            std::shared_ptr<Command> cmd = redo_.top();
            redo_.pop();
            cmd->Execute();
            undo_.push(cmd);
        }
    }

private:
    void Execute(std::shared_ptr<Command> cmd) {
        if (cmd->Execute()) {
            undo_.push(cmd);
        }
        while (!redo_.empty()) {
            redo_.pop();
        }
    }

private:
    std::string buffer_ = {};
    size_t position_ = 0;
    std::stack<std::shared_ptr<Command>> undo_;
    std::stack<std::shared_ptr<Command>> redo_;
};
