#pragma once
#include <cstdint>
namespace cpu {
    class MemoryBus {
    private:
        uint8_t memory[0xFFFF];

    public:
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    };
}