#pragma once

#include "cpu/registers.hpp"
#include "cartridge.hpp"
#include "cpu/memory.hpp"
#include <string>

using namespace cartridge;

namespace graphics {
    class Graphics;
}

namespace cpu {
    class CPU {
        friend class graphics::Graphics;

    private:
        enum class ALU_Instruction {
            ADD_A,
            ADC_A,
            SUB,
            SBC_A,
            AND,
            XOR,
            OR,
            CP
        };
        enum class ROT_Instruction {
            RLC, RRC, RL, RR, SLA, SRA, SWAP, SRL
        };

        Registers registers;
        MemoryBus memory_bus;
        bool IME;
        bool HALT;

        static Cartridge initialize_cartidge(std::string path);

        uint8_t read_rom();
        uint16_t read_rom_16bit();
        void add_r16(Register_16bit destination, uint16_t value);

        void perform_alu_operation(ALU_Instruction instruction, uint8_t value);
        uint8_t perform_rot_operation(ROT_Instruction instruction, uint8_t value);

    public:
        CPU(std::string rom_path);

        Cartridge cartridge;

        void step();

        void push(uint16_t value);
        uint16_t pop();
    };
}