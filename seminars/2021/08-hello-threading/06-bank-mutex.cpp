#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

class NotEnoughMoney : public std::exception {};

class Account {
public:
    void Deposit(int amount) {
        balance_ += amount;
    }

    void Withdraw(int amount) {
        if (balance_ < amount) {
            throw NotEnoughMoney{};
        }

        balance_ -= amount;
    }

    int Balance() const {
        return balance_;
    }

private:
    int balance_ = 0;
};

using ClientIndex = size_t;

class Bank {
public:
    Bank(size_t num_clients) : accounts_(num_clients), mutexes_(num_clients) {
    }

    size_t NumClients() const {
        return accounts_.size();
    }

    int Balance(ClientIndex client) {
        auto guard = std::lock_guard{mutexes_[client]};
        return accounts_[client].Balance();
    }

    void Deposit(ClientIndex client, int amount) {
        auto guard = std::lock_guard{mutexes_[client]};
        accounts_[client].Deposit(amount);
    }

    void Exchange(ClientIndex from, ClientIndex to, int amount) {
        auto guard1 = std::lock_guard{mutexes_[from]};
        auto guard2 = std::lock_guard{mutexes_[to]};
        accounts_[from].Withdraw(amount);
        accounts_[to].Deposit(amount);
    }

private:
    std::vector<Account> accounts_;
    std::vector<std::mutex> mutexes_;
};

////////////////////////////////////////////////////////////////////////////////

enum Clients : int {
    kBob = 0,
    kAlice = 1,
    kNumClients,
};

#include <iostream>

int main() {
    // Single-threaded usage: everything is OK
    Bank bank{kNumClients};
    bank.Deposit(kBob, 10);
    bank.Deposit(kAlice, 20);

    // Multi-threaded usage: BOOOOOM
    for (size_t iter = 0; true; ++iter) {
        std::cout << "Iteration #" << iter << std::endl;

        auto op = [&bank](Clients from, Clients to) {
            try {
                bank.Exchange(from, to, 10);
                bank.Exchange(to, from, 10);
            } catch (NotEnoughMoney) {
            }
        };

        {
            std::jthread th1{op, kAlice, kBob};
            std::jthread th2{op, kBob, kAlice};
        }
    }
}
