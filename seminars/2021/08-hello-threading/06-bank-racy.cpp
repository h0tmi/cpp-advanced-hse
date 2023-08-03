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

        // FAULT INJECTION
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

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
    Bank(size_t num_clients) : accounts_(num_clients) {
    }

    size_t NumClients() const {
        return accounts_.size();
    }

    int Balance(ClientIndex client) {
        return accounts_[client].Balance();
    }

    void Deposit(ClientIndex client, int amount) {
        accounts_[client].Deposit(amount);
    }

    void Exchange(ClientIndex from, ClientIndex to, int amount) {
        accounts_[from].Withdraw(amount);
        accounts_[to].Deposit(amount);
    }

private:
    std::vector<Account> accounts_;
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
    bank.Exchange(kAlice, kBob, 15);
    for (Clients client : {kBob, kAlice}) {
        std::cout << "Client " << client << " balance: " << bank.Balance(client) << std::endl;
    }
    bank.Exchange(kBob, kAlice, 15);

    // Multi-threaded usage: BOOOOOM
    for (size_t iter = 0; true; ++iter) {
        bool success[2]{false, false};

        auto op = [&bank, &success](int tid) {
            success[tid] = false;
            try {
                bank.Exchange(kAlice, kBob, 20);
                success[tid] = true;
            } catch (NotEnoughMoney) {
                success[tid] = false;
            }
        };

        {
            std::jthread th1{op, 0};
            std::jthread th2{op, 1};
        }

        if (success[0] && success[1]) {
            std::cerr << "BOOM!" << std::endl;
            for (Clients client : {kBob, kAlice}) {
                std::cout << "Client " << client << " balance: " << bank.Balance(client)
                          << std::endl;
            }
            return 1;
        }
    }
}
