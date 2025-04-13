#pragma once
#include <cstdint>

namespace cpu {
    enum class Register_8bit {
        A, B, C, D, E, H, L, Flags
    };

    enum class Register_16bit {
        AF, BC, DE, HL, SP, PC
    };

    class Registers {
    private:
        uint8_t data[12];

    public:
        Registers();

        uint8_t get_a();
        uint8_t get_b();
        uint8_t get_c();
        uint8_t get_d();
        uint8_t get_e();
        uint8_t get_flags();
        
        uint8_t get_h();
        uint8_t get_l();

        uint8_t get_r8(Register_8bit r8);
        
        uint16_t get_af();
        uint16_t get_bc();
        uint16_t get_de();
        uint16_t get_hl();
        
        uint16_t get_sp();
        uint16_t get_pc();

        uint16_t get_r16(Register_16bit r16);
    };
}