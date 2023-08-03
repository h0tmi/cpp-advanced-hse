#include <cstdlib>
#include <iostream>

#ifndef SEED
#define SEED 2344346u
#endif

const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

void GenString(char *str, int length) {
    for (int i = 0; i < length; ++i) {
        str[i] = alphabet[rand() % alphabet.size()];
    }
}

bool ReadString(std::string *line, size_t max_length) {
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

int main() {
    srand(SEED);
    const int iterations = 1e7;

    std::string password(10, 'a');
    for (int i = 0; i < iterations; ++i) {
        GenString(&password[0], password.size());
    }

    if (password != "NLXGI4NoAp") {
        std::cerr << "Wrong seed\n";
        return 0;
    }

    GenString(&password[0], password.size());
    std::string guess;
    if (!ReadString(&guess, 10u)) {
        std::cerr << "Too long string\n";
        return 0;
    }
    if (password == guess) {
        std::cerr << "wow so hacker\n";
        return 1;
    }
    return 0;
}
