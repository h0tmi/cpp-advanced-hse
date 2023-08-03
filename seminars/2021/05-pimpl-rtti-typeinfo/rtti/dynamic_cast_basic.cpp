#include <string>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

struct IEvent {
    // nothing works without polymorphism
    virtual ~IEvent() = default;
};

struct ErrorEvent : IEvent {
    std::string what_happened;
};

struct StartEvent : IEvent {
    uint64_t start_timestamp;
};

struct CommandEvent : IEvent {
    enum class CommandCode { DO_THIS, DO_THAT, DO_ANYTHING_ELSE } command;
};

void Process(IEvent* event) {
    std::unique_ptr<IEvent> holder(event);

    auto error_event = dynamic_cast<ErrorEvent*>(event);
    if (error_event) {
        std::cout << "Error: " << error_event->what_happened << "\n";
        return;
    }

    auto start_event = dynamic_cast<StartEvent*>(event);
    if (start_event) {
        std::cout << "Started at: " << start_event->start_timestamp << "\n";
        return;
    }

    auto command_event = dynamic_cast<CommandEvent*>(event);
    if (command_event) {
        std::cout << "Received command\n";
        return;
    }

    std::cout << "Unknown command!\n";
}

StartEvent* GetStartEvent() {
    auto se = new StartEvent;
    se->start_timestamp = 1;
    return se;
}

ErrorEvent* GetErrorEvent() {
    auto ee = new ErrorEvent;
    ee->what_happened = "i don't know";
    return ee;
}

CommandEvent* GetCommandEvent() {
    auto ce = new CommandEvent;
    ce->command = CommandEvent::CommandCode::DO_THAT;
    return ce;
}

int main() {
    std::vector<IEvent*> events{GetStartEvent(), GetCommandEvent(), GetErrorEvent()};

    for (IEvent* event : events) {
        Process(event);
    }
}
