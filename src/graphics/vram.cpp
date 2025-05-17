#include "vram.hpp"
#include <stdexcept>

namespace graphics {
    VRAM::VRAM(cpu::MemoryBus* RAM) : RAM(RAM) {}

    uint8_t VRAM::read(uint16_t address) {
        if (address >= 0x2000) {
            throw std::invalid_argument("Address must be less than 0x2000");
        }

        return RAM->read(address + 0x8000);
    }

    void VRAM::write(uint16_t address, uint8_t value) {
        if (address >= 0x2000) {
            throw std::invalid_argument("Address must be less than 0x2000");
        }

        RAM->write(address + 0x8000, value);
    }
}