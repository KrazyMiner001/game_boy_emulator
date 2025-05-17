#pragma once

#include "cpu/memory.hpp"

namespace graphics {
    class VRAM {
        private:
            cpu::MemoryBus* RAM;

        public:
            VRAM(cpu::MemoryBus* RAM);
            uint8_t read(uint16_t address);
            void write(uint16_t address, uint8_t value);
    };
}