#include "registers.hpp"
#include <stdlib.h>

namespace cpu {
    Registers::Registers() {
        data = (uint8_t *) malloc(12);
    }

    Registers::~Registers() {
        delete data;
    }

    uint8_t Registers::get_a() {
        return data[0];
    }

    uint8_t Registers::get_f() {
        return data[1];
    }

    uint8_t Registers::get_b() {
        return data[2];
    }

    uint8_t Registers::get_c() {
        return data[3];
    }

    uint8_t Registers::get_d() {
        return data[4];
    }

    uint8_t Registers::get_e() {
        return data[5];
    }

    uint8_t Registers::get_h() {
        return data[6];
    }

    uint8_t Registers::get_l() {
        return data[7];
    }

    uint8_t Registers::get_r8(Register_8bit r8) {
        return data[(ptrdiff_t) r8];
    }

    uint16_t Registers::get_af() {
        return ((uint16_t *) data)[0];
    }

    uint16_t Registers::get_bc() {
        return ((uint16_t *) data)[1];
    }

    uint16_t Registers::get_de() {
        return ((uint16_t *) data)[2];
    }

    uint16_t Registers::get_hl() {
        return ((uint16_t *) data)[3];
    }

    uint16_t Registers::get_sp() {
        return ((uint16_t *) data)[4];
    }

    uint16_t Registers::get_pc() {
        return ((uint16_t *) data)[5];
    }

    uint16_t Registers::get_r16(Register_16bit r16) {
        return ((uint16_t *) data)[(ptrdiff_t) r16];
    }

    void Registers::set_a(uint8_t value) {
        data[0] = value;
    }

    void Registers::set_f(uint8_t value) {
        data[1] = value;
    }

    void Registers::set_b(uint8_t value) {
        data[2] = value;
    }

    void Registers::set_c(uint8_t value) {
        data[3] = value;
    }

    void Registers::set_d(uint8_t value) {
        data[4] = value;
    }

    void Registers::set_e(uint8_t value) {
        data[5] = value;
    }

    void Registers::set_h(uint8_t value) {
        data[6] = value;
    }

    void Registers::set_l(uint8_t value) {
        data[7] = value;
    }

    void Registers::set_r8(Register_8bit r8, uint8_t value) {
        data[(ptrdiff_t) r8] = value;
    }

    void Registers::set_af(uint16_t value) {
        ((uint16_t *) data)[0] = value;
    }

    void Registers::set_bc(uint16_t value) {
        ((uint16_t *) data)[1] = value;
    }

    void Registers::set_de(uint16_t value) {
        ((uint16_t *) data)[2] = value;
    }

    void Registers::set_hl(uint16_t value) {
        ((uint16_t *) data)[3] = value;
    }

    void Registers::set_sp(uint16_t value) {
        ((uint16_t *) data)[4] = value;
    }

    void Registers::set_pc(uint16_t value) {
        ((uint16_t *) data)[5] = value;
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
}