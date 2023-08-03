#include <iostream>
#include <unordered_set>
#include <string>
#include <ctime>
#include <cctype>
#include <random>
#include <cstdlib>

using std::string;

bool CheckUserName(const std::string& user) {
    for (char c : user) {
        if (!isalpha(c) && !isdigit(c) && c != '_') {
            return false;
        }
    }
    return true;
}

bool GetLine(std::string* line, size_t max_length) {
    line->clear();
    while (std::cin && line->size() <= max_length) {
        int symbol = std::cin.get();
        if (symbol == '\n') {
            return true;
        }
        line->push_back(symbol);
    }
    return false;
}

std::vector<std::string> GetUsers(size_t max_count) {
    std::vector<std::string> users;
    std::string cur_line;
    while (users.size() < max_count) {
        bool is_valid = GetLine(&cur_line, 15u);
        if (std::cin.eof()) {
            break;
        }
        if (!is_valid) {
            std::cout << "Too long username " << cur_line << "\n";
            exit(0);
        }
        if (cur_line.empty()) {
            break;
        }
        if (CheckUserName(cur_line)) {
            users.push_back(cur_line);
        } else {
            std::cout << "Incorrect username " << cur_line << "\n";
            exit(0);
        }
    }
    std::cout << "Read " << users.size() << " users\n";
    return users;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(30000, 1000000);
    size_t init_size = dist(gen);
    std::unordered_set<string> users(init_size);

    auto users_list = GetUsers(20000u);
    auto start_time = std::clock();

    for (const auto& user : users_list) {
        users.insert(user);
    }

    auto end_time = std::clock();
    double spent = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "Spent " << spent << " seconds of CPU time" << std::endl;
    if (spent > 0.8) {
        std::cout << "Shit happens\n";
        return 1;
    }

    return 0;
}
