#include "RedEngine/Core/Event/Signal.hpp"

#include <catch2/catch.hpp>

class MyClass
{
public:
    MyClass() = default;
    ~MyClass() = default;

    int m_a{-1};

    void op(int a)
    {
        m_a = a;
    }
};

TEST_CASE("Signal/Slots connections", "[EVENT]")
{
    SECTION("Pong lambda")
    {
        bool isCalled = false;
        red::Signal<int> signal;
        signal.Connect([&](int /*i*/) { isCalled = true; }, true);

        signal.emit(1);

        REQUIRE(isCalled);
    }

    SECTION("Pong method bind")
    {
        MyClass mc;

        red::Signal<int> signal;
        signal.Connect(&MyClass::op, &mc, true);

        int a = 10;
        signal.emit(a);

        REQUIRE(mc.m_a == 10);
    }

    SECTION("Activate / Deactivate slot & signal")
    {
        MyClass mc;

        red::Signal<int> signal;
        auto slot = signal.Connect(&MyClass::op, &mc, true);

        slot.Deactivate();

        int a = 10;
        signal.emit(a);

        REQUIRE(mc.m_a == -1);

        slot.Activate();

        signal.Deactivate();

        signal.emit(a);

        REQUIRE(mc.m_a == -1);

        signal.Activate();
        signal.emit(a);
        REQUIRE(mc.m_a == 10);
    }

    SECTION("Copying slots")
    {
        MyClass mc;

        red::Signal<int> signal;

        red::Signal<int>::Slot slot;

        {
            auto firstSlot = signal.Connect(&MyClass::op, &mc, true);
            slot = firstSlot;
        }

        REQUIRE(slot.IsActive());

        signal.emit(10);

        REQUIRE(mc.m_a == 10);
    }
}