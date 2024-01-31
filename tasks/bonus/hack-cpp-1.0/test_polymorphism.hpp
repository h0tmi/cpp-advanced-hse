#pragma once

#include <iostream>
#include <cstdlib>
#include <stdexcept>

void Trap() {
    std::cout << "Trap" << std::endl;
}

int TrapInt() {
    return 100;
}

int Sum(int a, int b) {
    return a + b;
}

/*                                                 Intro                                                 */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

class Base {
public:
    virtual void Test() {
        std::cout << "Base" << std::endl;
    }
    static constexpr size_t N = 1;
};

class Derived : public Base {
public:
    void Test() override {
        std::cout << "Derived" << std::endl;
    }
    virtual void BadTest() {
        std::cout << "BadTest" << std::endl;
    }
    static constexpr size_t N = 2;
};

class FinalClass final {
public:
    virtual void OhImSoVirtual() {
        std::cout << "VIRTUAL!!!" << std::endl;
    }
    static constexpr size_t N = 1;
};

/*                                                 Basic                                                 */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

class Triple {
public:
    virtual void First() {
        std::cout << "First" << std::endl;
    }
    virtual void Second() {
        std::cout << "Second" << std::endl;
    }
    virtual void Third() {
        std::cout << "Third" << std::endl;
    }
    static constexpr size_t N = 3;
};

class GuardedTriple {
public:
    static constexpr size_t N = 3;

    void CallAll() {
        Public();
        Protected();
        Private();
    }

public:
    virtual void Public() {
        std::cout << "Public" << std::endl;
    }
protected:
    virtual void Protected() {
        std::cout << "Protected" << std::endl;
    }
private:
    virtual void Private() {
        std::cout << "Private" << std::endl;
    }
};

class ValueReturning {
public:
    virtual int ReturnValue() {
        return 42;
    }
    static constexpr size_t N = 1;
};

/*                                               AdvanceXD                                               */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

class Indestructible {
public:
    virtual ~Indestructible() {
        std::cout << "Destroyed" << std::endl;
    }
    static constexpr size_t N = 1;
};

class GoodBase {
public:
    virtual void Test() {
        std::cout << "GoodBase" << std::endl;
    }
    virtual ~GoodBase() {
        std::cout << "BaseDestroyed" << std::endl;
    }
    static constexpr size_t N = 2;
};

class GoodDerived : public GoodBase {
public:
    void Test() override {
        std::cout << "GoodDerived" << std::endl;
    }
    void DerivedFunc() {
        throw std::logic_error("Impossible... How did you even call me?");
    }
    virtual void NewVirtual() {
        std::cout << "NewVirtual" << std::endl;
    }
    static constexpr size_t N = 3;
};

class MultiDerived : public GoodBase, Indestructible {
public:
    void Test() override {
        std::cout << "MultiDerived" << std::endl;
    }
    virtual void Multi() {
        std::cout << "Multi" << std::endl;
    }
    static constexpr size_t N = 10;
};

/*                                               Stressful                                               */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#define __STRINGIFY__(X) #X
#define STRINGIFY(X) __STRINGIFY__(X)
#define __CONCAT__(X, Y) X##Y
#define CONCAT(X, Y) __CONCAT__(X, Y)
#define ADD_CNT(X) CONCAT(X, __COUNTER__)
#define FIX(FUNC) virtual void ADD_CNT(FUNC)() { std::cout << std::to_string(call_id++) << std::endl; }
#define HUNDRED(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \
    FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) FIX(X) \


class Huge {
public:
    HUNDRED(Func)
    static constexpr size_t N = 100;
private:
    size_t call_id = 0;
};
