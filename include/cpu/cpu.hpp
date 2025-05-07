#pragma once

#include "cpu/registers.hpp"
#include "cartridge.hpp"
#include "cpu/memory.hpp"
#include <string>

using namespace cartridge;

namespace cpu {
    class CPU {
    private:
        Registers registers;
        MemoryBus memory_bus;

        static Cartridge initialize_cartidge(std::string path);

        uint8_t read_rom();
        uint16_t read_rom_16bit();
        void add_r16(Register_16bit destination, uint16_t value);
        void add_r8(Register_8bit destination, uint8_t value);

    public:
        CPU(std::string rom_path);

        Cartridge cartridge;

        void step();

        void push(uint16_t value);
        uint16_t pop();
    };
}