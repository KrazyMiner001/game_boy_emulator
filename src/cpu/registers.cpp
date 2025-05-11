#include "cpu/registers.hpp"
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <stdexcept>

namespace cpu {
    Registers::Registers() {
        data = (uint8_t *) malloc(12);
        memset(data, 0, 12);
    }

    Registers::~Registers() {
        delete data;
    }

    uint8_t Registers::get_a() {
        return data[(ptrdiff_t) Register_8bit::A];
    }

    uint8_t Registers::get_f() {
        return data[(ptrdiff_t) Register_8bit::F];
    }

    uint8_t Registers::get_b() {
        return data[(ptrdiff_t) Register_8bit::B];
    }

    uint8_t Registers::get_c() {
        return data[(ptrdiff_t) Register_8bit::C];
    }

    uint8_t Registers::get_d() {
        return data[(ptrdiff_t) Register_8bit::D];
    }

    uint8_t Registers::get_e() {
        return data[(ptrdiff_t) Register_8bit::E];
    }

    uint8_t Registers::get_h() {
        return data[(ptrdiff_t) Register_8bit::H];
    }

    uint8_t Registers::get_l() {
        return data[(ptrdiff_t) Register_8bit::L];
    }

    uint8_t Registers::get_r8(Register_8bit r8) {
        return data[(ptrdiff_t) r8];
    }

    uint16_t Registers::get_af() {
        return ((uint16_t *) data)[(ptrdiff_t) Register_16bit::AF];
    }

    uint16_t Registers::get_bc() {
        return ((uint16_t *) data)[(ptrdiff_t) Register_16bit::BC];
    }

    uint16_t Registers::get_de() {
        return ((uint16_t *) data)[(ptrdiff_t) Register_16bit::DE];
    }

    uint16_t Registers::get_hl() {
        return ((uint16_t *) data)[(ptrdiff_t) Register_16bit::HL];
    }

    uint16_t Registers::get_sp() {
        return ((uint16_t *) data)[(ptrdiff_t) Register_16bit::SP];
    }

    uint16_t Registers::get_pc() {
        return ((uint16_t *) data)[(ptrdiff_t) Register_16bit::PC];
    }

    uint16_t Registers::get_r16(Register_16bit r16) {
        return ((uint16_t *) data)[(ptrdiff_t) r16];
    }

    void Registers::set_a(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::A] = value;
    }

    void Registers::set_f(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::F] = value;
    }

    void Registers::set_b(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::B] = value;
    }

    void Registers::set_c(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::C] = value;
    }

    void Registers::set_d(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::D] = value;
    }

    void Registers::set_e(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::E] = value;
    }

    void Registers::set_h(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::H] = value;
    }

    void Registers::set_l(uint8_t value) {
        data[(ptrdiff_t) Register_8bit::L] = value;
    }

    void Registers::set_r8(Register_8bit r8, uint8_t value) {
        data[(ptrdiff_t) r8] = value;
    }

    void Registers::set_af(uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) Register_16bit::AF] = value;
    }

    void Registers::set_bc(uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) Register_16bit::BC] = value;
    }

    void Registers::set_de(uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) Register_16bit::DE] = value;
    }

    void Registers::set_hl(uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) Register_16bit::HL] = value;
    }

    void Registers::set_sp(uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) Register_16bit::SP] = value;
    }

    void Registers::set_pc(uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) Register_16bit::PC] = value;
    }

    void Registers::set_r16(Register_16bit r16, uint16_t value) {
        ((uint16_t *) data)[(ptrdiff_t) r16] = value;
    }

    void Registers::apply_r8(Register_8bit r8, std::function<uint8_t(uint8_t)> function) {
        data[(ptrdiff_t) r8] = function(data[(ptrdiff_t) r8]);
    }

    void Registers::apply_r16(Register_16bit r16, std::function<uint16_t(uint16_t)> function) {
        ((uint16_t *) data)[(ptrdiff_t) r16] = function(((uint16_t *) data)[(ptrdiff_t) r16]);
    }

    Register_16bit Registers::from_rp(uint8_t rp) {
        assert(rp <= 3);
        switch (rp) {
            case 0: return Register_16bit::BC;
            case 1: return Register_16bit::DE;
            case 2: return Register_16bit::HL;
            case 3: return Register_16bit::SP;
            default: throw std::invalid_argument("There is no register defined by rp: " + rp);
        }
    }

    Register_16bit Registers::from_rp2(uint8_t rp2) {
        assert(rp2 <= 3);
        switch (rp2) {
            case 0: return Register_16bit::BC;
            case 1: return Register_16bit::DE;
            case 2: return Register_16bit::HL;
            case 3: return Register_16bit::AF;
            default: throw std::invalid_argument("There is no register defiend by rp2: " + rp2);
        }
    }

    Register_8bit Registers::from_r(uint8_t r) {
        assert(r <= 7);
        assert(r != 6);
        switch (r) {
            case 0: return Register_8bit::B;
            case 1: return Register_8bit::C;
            case 2: return Register_8bit::D;
            case 3: return Register_8bit::E;
            case 4: return Register_8bit::H;
            case 5: return Register_8bit::L;
            case 6: throw std::invalid_argument("r = 6 is not a register, it refers to the byte pointed to by HL");
            case 7: return Register_8bit::A;
            default: throw std::invalid_argument("There is no register defined by r: " + r);
        }
    }
}