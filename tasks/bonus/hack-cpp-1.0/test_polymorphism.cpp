#include "test_polymorphism.hpp"

#include <iostream>
#include <memory>
#include <gtest/gtest.h>

#include "polymorphism.h"

class TestPolymorphism : public ::testing::Test {
public:
    // These two functions override stdout
    // If you want to disable it and check the output youself, just comment them out
    void SetUp() override {
        old = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(old);
    }
protected:
    std::stringstream buffer;
    std::streambuf* old;
};

/*                                                 Intro                                                 */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

TEST_F(TestPolymorphism, Single) {
    auto base = std::make_unique<Base>();

    base->Test();
    TrapObject<Base::N>(*base, &Trap);
    base->Test();

    ASSERT_EQ(buffer.str(), std::string("Base\nTrap\n"));
}

TEST_F(TestPolymorphism, Derived) {
    auto derived = std::make_unique<Derived>();

    derived->Test();
    derived->BadTest();

    TrapObject<Derived::N>(*derived, &Trap);

    derived->Test();
    derived->BadTest();

    ASSERT_EQ(buffer.str(), std::string("Derived\nBadTest\nTrap\nTrap\n"));
}

TEST_F(TestPolymorphism, Final) {
    auto final_class = std::make_unique<FinalClass>();

    final_class->OhImSoVirtual();
    TrapObject<FinalClass::N>(*final_class, &Trap);
    final_class->OhImSoVirtual();

    ASSERT_EQ(buffer.str(), "VIRTUAL!!!\nVIRTUAL!!!\n");
}

/*                                                 Basic                                                 */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

TEST_F(TestPolymorphism, Triple) {
    auto triple = std::make_unique<Triple>();

    triple->First();
    triple->Second();
    triple->Third();

    TrapObject<Triple::N>(*triple, &Trap);

    triple->First();
    triple->Second();
    triple->Third();

    ASSERT_EQ(buffer.str(), std::string("First\nSecond\nThird\nTrap\nTrap\nTrap\n"));
}

TEST_F(TestPolymorphism, GuardedTriple) {
    auto triple = std::make_unique<GuardedTriple>();

    triple->CallAll();
    TrapObject<GuardedTriple::N>(*triple, &Trap);
    triple->CallAll();

    ASSERT_EQ(buffer.str(), std::string("Public\nProtected\nPrivate\nTrap\nTrap\nTrap\n"));
}

TEST_F(TestPolymorphism, NonVoidReturn) {
    auto returning = std::make_unique<ValueReturning>();

    ASSERT_EQ(returning->ReturnValue(), 42);
    TrapObject<ValueReturning::N>(*returning, &TrapInt);
    ASSERT_EQ(returning->ReturnValue(), TrapInt());
}

TEST_F(TestPolymorphism, TypeSlaughter1) {
    auto returning = std::make_unique<ValueReturning>();

    ASSERT_EQ(returning->ReturnValue(), 42);
    TrapObject<ValueReturning::N>(*returning, &Trap);
    ASSERT_NE(returning->ReturnValue(), 42);
    ASSERT_EQ(buffer.str(), std::string("Trap\n"));
}

TEST_F(TestPolymorphism, TypeSlaughter2) {
    auto returning = std::make_unique<ValueReturning>();

    ASSERT_EQ(returning->ReturnValue(), 42);
    TrapObject<ValueReturning::N>(*returning, &Sum);
    ASSERT_NE(returning->ReturnValue(), 42);
}

/*                                               AdvanceXD                                               */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

TEST_F(TestPolymorphism, VirtualDestructor1) {
    {
        auto unbreakable = std::make_unique<Indestructible>();
        auto breakable = std::make_unique<Indestructible>();

        TrapObject<Indestructible::N>(*unbreakable, &Trap);
    }

    ASSERT_EQ(buffer.str(), std::string("Destroyed\nTrap\n"));
}

TEST_F(TestPolymorphism, VirtualDestructor2) {
    {
        auto base = std::make_unique<GoodBase>();
        
        base->Test();
        TrapObject<GoodBase::N>(*base, &Trap);
        base->Test();
    }

    ASSERT_EQ(buffer.str(), std::string("GoodBase\nTrap\nTrap\n"));
}

TEST_F(TestPolymorphism, VirtualDestructor3) {
    {
        auto derived = std::make_unique<GoodDerived>();

        derived->Test();
        derived->NewVirtual();

        TrapObject<GoodDerived::N>(*derived, &Trap);

        derived->Test();
        derived->NewVirtual();
    }

    ASSERT_EQ(buffer.str(), std::string("GoodDerived\nNewVirtual\nTrap\nTrap\nTrap\n"));    
}

/*                                               Stressful                                               */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#define CALL(I) huge->Func##I()
#define CALL_ALL() \
    CALL(0); CALL(1); CALL(2); CALL(3); CALL(4); CALL(5); CALL(6); CALL(7); CALL(8); CALL(9); \
    CALL(10); CALL(11); CALL(12); CALL(13); CALL(14); CALL(15); CALL(16); CALL(17); CALL(18); CALL(19); \
    CALL(20); CALL(21); CALL(22); CALL(23); CALL(24); CALL(25); CALL(26); CALL(27); CALL(28); CALL(29); \
    CALL(30); CALL(31); CALL(32); CALL(33); CALL(34); CALL(35); CALL(36); CALL(37); CALL(38); CALL(39); \
    CALL(40); CALL(41); CALL(42); CALL(43); CALL(44); CALL(45); CALL(46); CALL(47); CALL(48); CALL(49); \
    CALL(50); CALL(51); CALL(52); CALL(53); CALL(54); CALL(55); CALL(56); CALL(57); CALL(58); CALL(59); \
    CALL(60); CALL(61); CALL(62); CALL(53); CALL(64); CALL(65); CALL(66); CALL(67); CALL(68); CALL(69); \
    CALL(70); CALL(71); CALL(72); CALL(63); CALL(74); CALL(75); CALL(76); CALL(77); CALL(78); CALL(79); \
    CALL(80); CALL(81); CALL(82); CALL(73); CALL(84); CALL(85); CALL(86); CALL(87); CALL(88); CALL(89); \
    CALL(90); CALL(91); CALL(92); CALL(83); CALL(94); CALL(95); CALL(96); CALL(97); CALL(98); CALL(99)

TEST_F(TestPolymorphism, Huge) {
    Huge* huge = new Huge();
    std::string result;
    
    CALL_ALL();
    for (size_t i = 0; i < Huge::N; ++i) {
        result += (std::to_string(i) + '\n');
    }

    TrapObject<Huge::N>(*huge, &Trap);

    CALL_ALL();
    for (size_t i = 0; i < Huge::N; ++i) {
        result += "Trap\n";
    }

    ASSERT_EQ(buffer.str(), result);
}
