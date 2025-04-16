#pragma once

#include "registers.hpp"
#include "memory.hpp"

namespace cpu {
    class CPU {
    private:
        Registers registers;
        MemoryBus memory_bus;

    public:
        void step();

        void push(uint16_t value);
        uint16_t pop();
    };
}