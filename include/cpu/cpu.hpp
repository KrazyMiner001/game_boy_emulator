#pragma once

#include "cpu/registers.hpp"
#include "cpu/memory.hpp"

namespace cpu {
    class CPU {
    private:
        Registers registers;
        MemoryBus memory_bus;

        uint8_t read_rom();
        uint16_t read_rom_16bit();
        void add_r16(Register_16bit destination, uint16_t value);
        void add_r8(Register_8bit destination, uint8_t value);

    public:
        void step();

        void push(uint16_t value);
        uint16_t pop();
    };
}