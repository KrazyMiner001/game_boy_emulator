#pragma once
#include <cstdint>
namespace cpu {
    class MemoryBus {
    private:
        uint8_t memory[0x4000];
        uint8_t vram[0x2000];

    public:
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    };
}