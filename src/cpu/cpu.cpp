#include "cpu.hpp"

namespace cpu {
    void CPU::add_r16(Register_16bit destination, uint16_t value) {
        uint16_t register_value = registers.get_r16(destination);
        uint16_t sum = register_value + value;

        bool half_carry = (register_value & (1 << 11)) && !(sum & (1 << 11));
        bool carry = (register_value & (1 << 15)) && !(sum & (1 << 15));

        registers.set_f((half_carry << 5) | (carry << 4));
        registers.set_r16(destination, sum);
    }

    void CPU::add_r8(Register_8bit destination, uint8_t value) {

    }

    uint8_t CPU::read_rom() {
        uint8_t byte = memory_bus.read(registers.get_pc());
        registers.apply_r16(Register_16bit::PC, [](uint16_t pc){return pc + 1;});
        return byte;
    }

    uint16_t CPU::read_rom_16bit() {
        uint16_t byte = memory_bus.read(registers.get_pc()) | (memory_bus.read(registers.get_pc() + 1) << 8);
        registers.apply_r16(Register_16bit::PC, [](uint16_t pc){return pc + 2;});
        return byte;
    }

    void CPU::step() {
        uint8_t byte = read_rom();

        struct {
            unsigned int x : 2;
            unsigned int y : 3;
            unsigned int z : 3;
            unsigned int p : 2;
            unsigned int q : 1;
        } helper;
        
        helper.x = byte >> 6;
        helper.y = byte >> 3;
        helper.z = byte;
        helper.p = byte >> 4;
        helper.q = byte >> 3;

        switch (helper.x) {
            case 0:
                switch (helper.z) {
                    case 0:
                        switch (helper.y) {
                            case 0:
                                return;
                                break;
                            case 1:
                                uint16_t address = read_rom() | read_rom() << 8;
                                registers.set_sp(memory_bus.read(address) << 8 | memory_bus.read(address + 1));
                                break;
                            case 2:
                                //todo - implement stop
                                break;
                            case 3:
                                registers.set_pc(registers.get_sp() + read_rom());
                                break;
                            case 4:
                                if (!(registers.get_f() & 0b10000000)) {
                                    registers.set_pc(registers.get_sp() + ((int8_t) read_rom()));
                                }
                                break;
                            case 5:
                                if (registers.get_f() & 0b10000000) {
                                    registers.set_pc(registers.get_sp() + ((int8_t) read_rom()));
                                }
                                break;
                            case 6:
                                if (!(registers.get_f() & 0b00010000)) {
                                    registers.set_pc(registers.get_sp() + ((int8_t) read_rom()));
                                }
                                break;
                            case 7:
                                if (registers.get_f() & 0b00010000) {
                                    registers.set_pc(registers.get_sp() + ((int8_t) read_rom()));
                                }
                                break;
                        };
                        break;
                    case 1:
                        switch (helper.q) {
                        case 0:
                            registers.set_r16(Registers::from_rp(helper.p), read_rom_16bit());
                            break;
                        case 1:
                            add_r16(Register_16bit::HL, registers.get_r16(Registers::from_rp(helper.p)));
                            break;
                        };
                };
                break;
            case 1:
                switch (helper.z) {

                };
                break;
            case 2:
                switch (helper.z) {

                };
                break;
            case 3:
                switch (helper.z) {

                };
                break;
        };
    }

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