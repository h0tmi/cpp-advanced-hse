#include <gtest/gtest.h>
#include <stdexcept>

#include "call_private.h"

class Printer final {
private:
    void Print() {
        std::cout << "Private method has been called!" << std::endl;
    }
};

class Simple final {
private:
    void PrivateMethod() {
        ++counter;
    }
public:
    size_t counter = 0;
};

class Simple2 final {
    void PrivateMethod() {
        ++counter1;
    }

    void AnotherPrivateMethod() {
        ++counter2;
    }

public:
    size_t counter1 = 0;
    size_t counter2 = 0;
};

class Vault final {
    size_t GetSuperSecretValue() {
        return super_secret_;
    }
private: private: private: private: private: private:
private: private: private: private: private: private:
private: private: private: private: private: private:
private: size_t super_secret_ = 998'244'353; private:
private: private: private: private: private: private:
private: private: private: private: private: private:
private: private: private: private: private: private:
};

class Lady final {
public:
    enum class Dress { BLACK, YELLOW, PINK };
    enum class Weather { RAINY, SUNNY, CLOUDY, TORNADO };
private:
    Dress PutOnADress(Weather weather) {
        switch(weather) {
            case Weather::RAINY:
                return Dress::BLACK;
            case Weather::SUNNY:
                return Dress::YELLOW;
            case Weather::CLOUDY:
                return Dress::PINK;
            default:
                throw std::logic_error("I don't know what to put on!");
        }
    }
};

class Complex final {
public:
    Complex() = delete;
    Complex(long n, uint p) : n_(n), p_(p) {}
private:
    long Calculate(const std::string& s, std::array<char, 4> arr) {
        unsigned long overflowed = SillyOverflowPow();
        overflowed += n_ * p_ - p_ * p_ * p_;
        for (char c: arr) {
            overflowed *= (c - '0');
        }
        return (overflowed + s.size()) % 200200;
    }
    long SillyOverflowPow() {
        long n = n_;
        uint p = p_;
        unsigned long long result = 1;
        while (p-- > 0) {
            result *= n;
        }
        return result % 200200;
    }
private:
    long n_;
    uint p_;
};

class Huge final {
    std::string Constant(size_t, size_t, std::vector<double>::allocator_type, float, float, Lady) {
        return "HugeConstant";
    }
};

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

PREPARE_FOR_ROBBERY(Printer, Print, void);
TEST(CallPrivate, Printer) {
    ROB(Printer{}, Printer, Print);
}

PREPARE_FOR_ROBBERY(Simple, PrivateMethod, void);
TEST(CallPrivate, Simple) {
    Simple simple;
    ROB(simple, Simple, PrivateMethod);
    ROB(simple, Simple, PrivateMethod);
    ROB(simple, Simple, PrivateMethod);
    ASSERT_EQ(simple.counter, 3);
}

PREPARE_FOR_ROBBERY(Simple2, PrivateMethod, void);
PREPARE_FOR_ROBBERY(Simple2, AnotherPrivateMethod, void);
TEST(CallPrivate, Dublicates) {
    Simple2 simple;

    ROB(simple, Simple2, PrivateMethod);
    ROB(simple, Simple2, PrivateMethod);
    ROB(simple, Simple2, PrivateMethod);
    ASSERT_EQ(simple.counter1, 3);

    ROB(simple, Simple2, AnotherPrivateMethod);
    ROB(simple, Simple2, AnotherPrivateMethod);
    ASSERT_EQ(simple.counter2, 2);
}

PREPARE_FOR_ROBBERY(Vault, GetSuperSecretValue, size_t);
TEST(CallPrivate, Vault) {
    Vault vault;
    auto robbed = ROB(vault, Vault, GetSuperSecretValue);
    ASSERT_EQ(robbed, 998'244'353);
}

PREPARE_FOR_ROBBERY(Lady, PutOnADress, Lady::Dress, Lady::Weather);
TEST(CallPrivate, Lady) {
    Lady lady;

    auto black_dress = ROB(lady, Lady, PutOnADress, Lady::Weather::RAINY);
    auto yellow_dress = ROB(lady, Lady, PutOnADress, Lady::Weather::SUNNY);
    auto pink_dress = ROB(lady, Lady, PutOnADress, Lady::Weather::CLOUDY);

    ASSERT_EQ(black_dress, Lady::Dress::BLACK);
    ASSERT_EQ(yellow_dress, Lady::Dress::YELLOW);
    ASSERT_EQ(pink_dress, Lady::Dress::PINK);
}

PREPARE_FOR_ROBBERY(Complex, SillyOverflowPow, long);
TEST(CallPrivate, ComplexSillyOverflowPow) {
    Complex complex(77779999, 78);

    auto value = ROB(complex, Complex, SillyOverflowPow);
    static_assert(std::is_same_v<decltype(value), long>);
    ASSERT_EQ(value, 39449);
}

PREPARE_FOR_ROBBERY(Complex, Calculate, long, const std::string&, std::array<char, 4>);
TEST(CallPrivate, ComplexCalculate) {
    Complex complex(3819481, 47);

    auto value = ROB(complex, Complex, Calculate, std::string("MUAHAHAHA"), {'x', 'o', 'x', 'o'});
    static_assert(std::is_same_v<decltype(value), long>);
    ASSERT_EQ(value, 67321);
}

PREPARE_FOR_ROBBERY(Huge, Constant, std::string, size_t, size_t, std::vector<double>::allocator_type, float, float, Lady);
TEST(CallPrivate, Huge) {
    Huge huge;
    std::string huge_constant = ROB(huge, Huge, Constant, 10, 20, std::vector<double>().get_allocator(), 789.0f, 987.0f, Lady{});
    ASSERT_EQ(huge_constant, std::string("HugeConstant"));
}
