#pragma once
#include <cstdint>
#include <functional>

namespace cpu {
    enum class Register_8bit {
        A = 1, 
        F = 0, 
        B = 3, 
        C = 2, 
        D = 5, 
        E = 4, 
        H = 7, 
        L = 6
    };

    enum class Register_16bit {
        AF, BC, DE, HL, SP, PC
    };

    class Registers {
        private:
            uint8_t *data;

        public:
            Registers();
            ~Registers();

            uint8_t get_a();
            uint8_t get_f();
            uint8_t get_b();
            uint8_t get_c();
            uint8_t get_d();
            uint8_t get_e();
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

            void set_a(uint8_t value);
            void set_f(uint8_t value);
            void set_b(uint8_t value);
            void set_c(uint8_t value);
            void set_d(uint8_t value);
            void set_e(uint8_t value);
            void set_h(uint8_t value);
            void set_l(uint8_t value);

            void set_r8(Register_8bit r8, uint8_t value);

            void set_af(uint16_t value);
            void set_bc(uint16_t value);
            void set_de(uint16_t value);
            void set_hl(uint16_t value);

            void set_sp(uint16_t value);
            void set_pc(uint16_t value);

            void set_r16(Register_16bit r16, uint16_t value);

            void apply_r8(Register_8bit r8, std::function<uint8_t(uint8_t)> function);
            void apply_r16(Register_16bit r16, std::function<uint16_t(uint16_t)> function);

            static Register_16bit from_rp(uint8_t rp);

            static Register_16bit from_rp2(uint8_t rp2);

            static Register_8bit from_r(uint8_t r);
    };
}