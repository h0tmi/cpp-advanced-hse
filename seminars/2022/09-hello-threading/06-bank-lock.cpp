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
        // Variants:
        // 1. Use std::lock
        // std::lock(mutexes_[from], mutexes_[to]);
        // defer {
        //     mutexes_[from].unlock();
        //     mutexes_[to].unlock();
        // }
        //
        // 2. Use std::lock + adopt_lock
        // std::lock(mutexes_[from], mutexes_[to]);
        // std::unique_lock guard1{mutexes_[from], std::adopt_lock};
        // std::unique_lock guard2{mutexes_[to], std::adopt_lock};
        //
        // 3. Use scoped_lock with std::lock semantics
        // std::scoped_lock lock{mutexes_[from], mutexes_[to]};
        //
        //
        // 4. Lock the mutexes in the correct order manually
        // size_t firstLockIndex = std::min(from, to);
        // size_t secondLockIndex = std::max(from, to);
        // std::lock_guard guard1{mutexes_[firstLockIndex]};
        // std::lock_guard guard1{mutexes_[secondLockIndex]};

        std::scoped_lock lock{mutexes_[from], mutexes_[to]};
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
    Bank bank{kNumClients};
    bank.Deposit(kBob, 10);
    bank.Deposit(kAlice, 20);

    for (size_t iter = 0; iter < 1e5; ++iter) {
        if (iter % 10000 == 0) {
            std::cout << "Iteration #" << iter << std::endl;
        }

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
