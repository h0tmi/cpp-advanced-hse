/*
    Example of aliasing constructor usage.
    Some structs (`Message`) are received from somewhere using `GetMessage()`, then passed to
   buffered printer. The printer accepts text lines, not messages - so aliasing ctor is helpful
   here.
*/

#include <memory>
#include <random>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

/////////////////////////////////////////////////////////////////////

static constexpr size_t kLineMaxSize = 63;

using TextLine = std::array<char, kLineMaxSize + 1>;

struct Message {
    Message(TextLine line) : id(++last_id), sent_time(time(nullptr)), line(line) {
    }

    const int id;
    time_t sent_time;
    TextLine line;

    static inline int last_id = 0;
};

/////////////////////////////////////////////////////////////////////

class Bufferer {
public:
    Bufferer(size_t buffer_size) : buffer_size_(buffer_size) {
    }

    void AddNewLine(std::shared_ptr<TextLine> line) {
        lines_.push_back(std::move(line));
        if (lines_.size() >= buffer_size_) {
            Flush();
        }
    }

private:
    size_t buffer_size_ = 0;
    std::vector<std::shared_ptr<TextLine>> lines_;

    void Flush() {
        for (const auto& line : lines_) {
            if (line) {
                puts(&(*line)[0]);
            } else {
                std::cout << "<empty line>" << std::endl;
            }
        }
        lines_.clear();
    }
};

/////////////////////////////////////////////////////////////////////

std::shared_ptr<Message> GetMessage() {
    std::this_thread::sleep_for(10ms);

    static std::mt19937 gen;
    std::uniform_int_distribution<int> dist(0, 25);

    TextLine line;
    for (size_t i = 0; i < kLineMaxSize; ++i) {
        if (int c = dist(gen); c > 0) {
            line[i] = 'a' + c;
        } else {
            break;
        }
    }

    return std::make_shared<Message>(line);
}

void UpdateLastMessageTime(time_t time) {
    static time_t last_message_time;
    last_message_time = time;

    if (static_cast<size_t>(last_message_time) % 42 == 0) {
        std::cout << "Lucky time!" << std::endl;
    }
}

/////////////////////////////////////////////////////////////////////

int main() {
    Bufferer bufferer(16);

    while (true) {
        auto message = GetMessage();

        // Aliasing ctor
        bufferer.AddNewLine({message, &message->line});

        // `message` is still needed
        UpdateLastMessageTime(message->sent_time);

        // `message` is dropped, but inner object should stay alive until bufferer drops it
    }
}
