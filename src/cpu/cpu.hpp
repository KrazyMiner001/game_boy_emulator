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
    };
}