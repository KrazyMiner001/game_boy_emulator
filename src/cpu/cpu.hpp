#pragma once

#include "registers.hpp"
#include "memory.hpp"

namespace cpu {
    class CPU {
    private:
        Registers registers;
        MemoryBus memory_bus;

        uint8_t read_rom();
        void add(Register_16bit destination, uint16_t value);
        void add(Register_8bit destination, uint8_t value);

    public:
        void step();

        void push(uint16_t value);
        uint16_t pop();
    };
}