#include <string>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

class IEventHandler;

struct IEvent {
    // nothing works without polymorphism
    virtual ~IEvent() = default;

    virtual void AcceptHandling(IEventHandler* handler) = 0;
};

struct StartEvent;
struct ErrorEvent;
struct CommandEvent;

// this is Visitor
class IEventHandler {
public:
    virtual void HandleStart(StartEvent* start_event) = 0;
    virtual void HandleError(ErrorEvent* error_event) = 0;
    virtual void HandleCommand(CommandEvent* command_event) = 0;
};

////////////////////////////////////////////////////////////////////////

struct ErrorEvent : IEvent {
    std::string what_happened;

    void AcceptHandling(IEventHandler* handler) override {
        handler->HandleError(this);  // give access to our context
    }
};

struct StartEvent : IEvent {
    uint64_t start_timestamp;

    void AcceptHandling(IEventHandler* handler) override {
        handler->HandleStart(this);
    }
};

struct CommandEvent : IEvent {
    enum class CommandCode { DO_THIS, DO_THAT, DO_ANYTHING_ELSE } command;

    void AcceptHandling(IEventHandler* handler) override {
        handler->HandleCommand(this);
    }
};

////////////////////////////////////////////////////////////////////////

class EventLogger : public IEventHandler {
    void HandleStart(StartEvent* start_event) override {
        std::cout << "Started at: " << start_event->start_timestamp << "\n";
    }

    void HandleError(ErrorEvent* error_event) override {
        std::cout << "Error: " << error_event->what_happened << "\n";
    }

    void HandleCommand(CommandEvent*) override {
        std::cout << "Received command\n";
    }
};

void Process(IEvent* event, IEventHandler* handler) {
    event->AcceptHandling(handler);
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

    EventLogger logger;

    for (const auto& event : events) {
        Process(event.get(), &logger);
    }
}
