#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

void Banner() {
    std::cout << "****************************************" << std::endl;
    std::cout << "*   Welcome to ReAL Location Service   *" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << "* 1) Register User                     *" << std::endl;
    std::cout << "* 2) Check In                          *" << std::endl;
    std::cout << "* 3) View feed                         *" << std::endl;
    std::cout << "* 4) Subscribe                         *" << std::endl;
    std::cout << "****************************************" << std::endl;
}

std::string LimitedGetline() {
    constexpr int kMaxSize = 80;
    std::string line;
    while (std::cin && line.size() < kMaxSize) {
        int symbol = std::cin.get();
        if (symbol == '\n') {
            break;
        }
        line.push_back(symbol);
    }
    return line;
}

int GetCmd() {
    std::string cmd_str = LimitedGetline();
    std::stringstream stream(cmd_str);
    int cmd;
    stream >> cmd;
    return cmd;
}

struct Location {
    double longitude, latitude;
};

struct User {
    std::string username;
    std::vector<Location> posts;

    std::vector<User*> subscribers;
    std::vector<std::pair<User*, Location*>> feed;
};

int main() {
    constexpr int kMaxUsers = 100;
    std::vector<std::unique_ptr<User>> users;
    auto get_user = [&]() -> User* {
        std::cout << "Enter username:";
        std::string username = LimitedGetline();
        for (auto& user : users) {
            if (user->username == username) {
                return user.get();
            }
        }

        std::cout << "User not found" << std::endl;
        return nullptr;
    };

    for (int i = 0; i < 1000; ++i) {
        Banner();

        switch (GetCmd()) {
            case 1: {
                if (users.size() > kMaxUsers) {
                    std::cout << "Users limit reached" << std::endl;
                    break;
                }
                std::cout << "Enter username:";
                std::string username = LimitedGetline();
                users.emplace_back(new User{});
                users.back()->posts.reserve(16);
                users.back()->username = username;
                std::cout << "Registered user " << username << std::endl;
                break;
            }
            case 2: {
                auto user = get_user();
                if (!user) {
                    break;
                }

                std::cout << "Enter your location:";
                Location location;

                std::string line = LimitedGetline();
                std::stringstream stream(line);
                stream >> location.longitude >> location.latitude;
                user->posts.emplace_back(location);
                for (auto subscriber : user->subscribers) {
                    subscriber->feed.emplace_back(user, &user->posts.back());
                }

                std::cout << "User " << user->username << " checked in" << std::endl;
                break;
            }
            case 3: {
                auto user = get_user();
                if (!user) {
                    break;
                }

                std::cout << "Feed for user " << user->username << std::endl;
                for (auto post : user->feed) {
                    std::cout << post.first->username << " is at " << post.second->longitude << " "
                              << post.second->latitude << std::endl;
                }
                break;
            }
            case 4: {
                auto user = get_user();
                if (!user) {
                    break;
                }

                auto subscribe_to = get_user();
                if (!subscribe_to) {
                    break;
                }

                subscribe_to->subscribers.push_back(user);
                std::cout << "User " << user->username << " subscribed to "
                          << subscribe_to->username << std::endl;
                break;
            }
        }
    }

    return 0;
}
