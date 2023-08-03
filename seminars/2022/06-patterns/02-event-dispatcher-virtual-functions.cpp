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
    event->Print();
}

auto GetStartEvent() {
    auto se = std::make_unique<StartEvent>();
    se->start_timestamp = 1;
    return se;
}

auto GetErrorEvent() {
    auto ee = std::make_unique<ErrorEvent>();
    ee->what_happened = "i don't know";
    return ee;
}

auto GetCommandEvent() {
    auto ce = std::make_unique<CommandEvent>();
    ce->command = CommandEvent::CommandCode::DO_THAT;
    return ce;
}

int main() {
    std::vector<std::unique_ptr<IEvent>> events;

    events.push_back(GetStartEvent());
    events.push_back(GetCommandEvent());
    events.push_back(GetErrorEvent());

    for (const auto& event : events) {
        Process(event.get());
    }
}
