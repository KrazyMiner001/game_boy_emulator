#pragma once
#include <cstdint>

namespace cpu {
    class CPU;

    class MemoryBus {
        friend graphics::Graphics;
    private:
        uint8_t memory[0x4000];
        uint8_t vram[0x2000];
        CPU &cpu;

    public:
        MemoryBus(CPU &cpu);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    };
}