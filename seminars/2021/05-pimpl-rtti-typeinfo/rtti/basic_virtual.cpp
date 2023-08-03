#include <string>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

struct IEvent {
    // nothing works without polymorphism
    virtual ~IEvent() = default;

    virtual void Print() {
        std::cout << "Unknown event!\n";
    }
};

struct ErrorEvent : IEvent {
    std::string what_happened;

    void Print() override {
        std::cout << "Error: " << what_happened << "\n";
    }
};

struct StartEvent : IEvent {
    uint64_t start_timestamp;

    void Print() override {
        std::cout << "Started at: " << start_timestamp << "\n";
    }
};

struct CommandEvent : IEvent {
    enum class CommandCode { DO_THIS, DO_THAT, DO_ANYTHING_ELSE } command;

    void Print() override {
        std::cout << "Received command\n";
    }
};

void Process(IEvent* event) {
    std::unique_ptr<IEvent> holder(event);

    event->Print();
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
