#include "cpu/memory.hpp"

namespace cpu {
    void MemoryBus::write(uint16_t address, uint8_t value) {
        memory[address] = value;
    }

    uint8_t MemoryBus::read(uint16_t address) {
        return memory[address];
    }
}