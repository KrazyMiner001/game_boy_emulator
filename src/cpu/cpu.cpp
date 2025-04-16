#include "cpu.hpp"

namespace cpu {
    void CPU::push(uint16_t value) {
        registers.apply_r16(Register_16bit::SP, [](uint16_t sp){return sp - 1;});
        memory_bus.write(registers.get_sp(), value >> 8);
        registers.apply_r16(Register_16bit::SP, [](uint16_t sp){return sp - 1;});
        memory_bus.write(registers.get_sp(), value & 0x00FF);
    }

    uint16_t CPU::pop() {
        uint16_t value = 0;
        value |= memory_bus.read(registers.get_sp());
        registers.apply_r16(Register_16bit::SP, [](uint16_t sp){return sp + 1;});
        value |= memory_bus.read(registers.get_sp()) << 8;
        registers.apply_r16(Register_16bit::SP, [](uint16_t sp){return sp - 1;});
        return value;
    }
}