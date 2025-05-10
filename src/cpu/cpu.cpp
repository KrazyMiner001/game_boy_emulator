#include "cpu/cpu.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

namespace cpu {
    CPU::CPU(std::string rom_path) 
    : memory_bus(MemoryBus(*this))
    , cartridge(initialize_cartidge(rom_path)) {}

    Cartridge CPU::initialize_cartidge(std::string path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("The file at path " + path + " failed to open");
        }

        file.seekg(0, std::ios::end);
        size_t length = file.tellg();
        file.seekg(0, std::ios::beg);

        uint8_t *ptr = (uint8_t*) malloc(length);
        file.read((char*) ptr, length);

        return cartridge::Cartridge(ptr, length);
    }

    void CPU::add_r16(Register_16bit destination, uint16_t value) {
        uint16_t register_value = registers.get_r16(destination);
        uint16_t sum = register_value + value;

        bool half_carry = (register_value & (1 << 11)) && !(sum & (1 << 11));
        bool carry = (register_value & (1 << 15)) && !(sum & (1 << 15));

        registers.set_f((half_carry << 5) | (carry << 4));
        registers.set_r16(destination, sum);
    }

    void CPU::perform_alu_operation(ALU_Instruction instruction, uint8_t value) {
        switch (instruction) {
            case ALU_Instruction::ADD_A:
            {
                uint8_t new_value = registers.get_a() + value;
                bool carry = (registers.get_a() > 0xFF - value);
                bool half_carry = (registers.get_a() & 0xF) + (value & 0xF) > 0xF;
                bool zero = (bool) new_value;

                registers.set_f(
                    (zero << 7) |
                    (half_carry << 5) |
                    (carry << 4)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::ADC_A:
            {
                uint8_t new_value = registers.get_a() + value;
                uint8_t old_carry = registers.get_f() & (1 << 4);
                bool carry = (registers.get_a() > 0xFF - old_carry - value);
                bool half_carry = (registers.get_a() & 0xF) + (value & 0xF) + old_carry > 0xF;
                bool zero = (bool) new_value;

                registers.set_f(
                    (zero << 7) |
                    (half_carry << 5) |
                    (carry << 4)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::SUB:
            {
                uint8_t new_value = registers.get_a() - value;
                bool zero = new_value;
                bool half_carry = (value & 0xF) > (registers.get_a() & 0xF);
                bool carry = value > registers.get_a();

                registers.set_f(
                    (zero << 7) |
                    (1 << 6) |
                    (half_carry << 5) |
                    (carry << 4)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::SBC_A:
            {
                uint8_t old_carry = registers.get_f() & (1 << 4);
                uint8_t new_value = registers.get_a() - value - old_carry;
                bool zero = new_value;
                bool half_carry = (value & 0xF + old_carry) > (registers.get_a() & 0xF);
                bool carry = (value + old_carry) > registers.get_a();

                registers.set_f(
                    (zero << 7) |
                    (1 << 6) |
                    (half_carry << 5) |
                    (carry << 4)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::AND:
            {
                uint8_t new_value = registers.get_a() & value;
                bool zero = new_value;
                registers.set_f(
                    (zero << 7) |
                    (1 << 5)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::XOR:
            {
                uint8_t new_value = registers.get_a() ^ value;
                bool zero = new_value;
                registers.set_f(
                    (zero << 7)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::OR:
            {
                uint8_t new_value = registers.get_a() | value;
                bool zero = new_value;
                registers.set_f(
                    (zero << 7)
                );
                registers.set_a(new_value);
            }
            break;
            case ALU_Instruction::CP:
            {
                uint8_t new_value = registers.get_a() - value;
                bool zero = new_value;
                bool half_carry = (value & 0xF) > (registers.get_a() & 0xF);
                bool carry = value > registers.get_a();

                registers.set_f(
                    (zero << 7) |
                    (1 << 6) |
                    (half_carry << 5) |
                    (carry << 4)
                );
            }
            break;
        }
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
        uint8_t instruction = read_rom();

        struct {
            unsigned int x : 2;
            unsigned int y : 3;
            unsigned int z : 3;
            unsigned int p : 2;
            unsigned int q : 1;
        } helper;
        
        helper.x = instruction >> 6;
        helper.y = instruction >> 3;
        helper.z = instruction;
        helper.p = instruction >> 4;
        helper.q = instruction >> 3;

        switch (helper.x) {
            case 0:
            {
                switch (helper.z) {
                    case 0:
                    {
                        switch (helper.y) {
                            case 0:
                                return;
                                break;
                            case 1:
                            {
                                uint16_t address = read_rom() | read_rom() << 8;
                                registers.set_sp(memory_bus.read(address) << 8 | memory_bus.read(address + 1));
                            }
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
                    }
                    break;
                    case 1:
                    {
                        switch (helper.q) {
                        case 0:
                            registers.set_r16(Registers::from_rp(helper.p), read_rom_16bit());
                            break;
                        case 1:
                            add_r16(Register_16bit::HL, registers.get_r16(Registers::from_rp(helper.p)));
                            break;
                        };
                    }
                    break;
                    case 2:
                    {
                        switch (helper.q) {
                            case 0:
                                switch (helper.p) {
                                    case 0:
                                        memory_bus.write(registers.get_bc(), registers.get_a());
                                        break;
                                    case 1:
                                        memory_bus.write(registers.get_de(), registers.get_a());
                                        break;
                                    case 2:
                                        memory_bus.write(registers.get_hl(), registers.get_a());
                                        registers.apply_r16(Register_16bit::HL, [](uint16_t value) {return value + 1;});
                                        break;
                                    case 3:
                                        memory_bus.write(registers.get_hl(), registers.get_a());
                                        registers.apply_r16(Register_16bit::HL, [](uint16_t value) {return value - 1;});
                                        break;
                                }
                                break;
                            case 1:
                                switch (helper.p) {
                                    case 0:
                                        registers.set_a(memory_bus.read(registers.get_bc()));
                                        break;
                                    case 1:
                                        registers.set_a(memory_bus.read(registers.get_de()));
                                        break;
                                    case 2:
                                        registers.set_a(memory_bus.read(registers.get_hl()));
                                        registers.apply_r16(Register_16bit::HL, [](uint16_t value) {return value + 1;});
                                        break;
                                    case 3:
                                        registers.set_a(memory_bus.read(registers.get_hl()));
                                        registers.apply_r16(Register_16bit::HL, [](uint16_t value) {return value - 1;});
                                }
                                break;
                        };
                    }
                    break;
                    case 3:
                    {
                        switch (helper.q) {
                            case 0:
                                registers.apply_r16(Registers::from_rp(helper.p), [](uint16_t value) {return value + 1;});
                                break;
                            case 1:
                                registers.apply_r16(Registers::from_rp(helper.p), [](uint16_t value) {return value - 1;});
                                break;
                        }
                    }
                    break;
                    case 4:
                    {
                        uint8_t byte;
                        if (helper.y == 6) {
                            byte = memory_bus.read(registers.get_hl());
                        } else {
                            byte = registers.get_r8(Registers::from_r(helper.y));
                        }
                        bool half_carry = (byte++ & 0b1000) && !(byte & 0b1000);
                        bool zero = byte == 0;

                        registers.set_f(zero << 7 | half_carry << 5 | (registers.get_f() & (1 << 4)));

                        if (helper.y == 6) {
                            memory_bus.write(registers.get_hl(), byte);
                        } else {
                            registers.set_r8(Registers::from_r(helper.y), byte);
                        }
                    }
                    break;
                    case 5:
                    {
                        uint8_t byte;
                        if (helper.y == 6) {
                            byte = memory_bus.read(registers.get_hl());
                        } else {
                            byte = registers.get_r8(Registers::from_r(helper.y));
                        }
                        bool half_carry = !(byte-- & 0b1000) && (byte & 0b1000);
                        bool zero = byte == 0;

                        registers.set_f(zero << 7 | half_carry << 5 | (registers.get_f() & (1 << 4)));

                        if (helper.y == 6) {
                            memory_bus.write(registers.get_hl(), byte);
                        } else {
                            registers.set_r8(Registers::from_r(helper.y), byte);
                        }
                    }
                    break;
                    case 6:
                    {
                        uint8_t immediate_byte = read_rom();
                        if (helper.y == 6) {
                            memory_bus.write(registers.get_hl(), immediate_byte);
                        } else {
                            registers.set_r8(Registers::from_r(helper.y), immediate_byte);
                        }
                    }
                    break;
                    case 7:
                    {
                        switch (helper.y) {
                            case 0:
                            {
                                uint8_t old_a = registers.get_a();
                                registers.set_f((old_a & (1 << 7)) >> 3);
                                registers.set_a(old_a << 1 | old_a >> 7);
                            }
                            break;
                            case 1:
                            {
                                uint8_t old_a = registers.get_a();
                                registers.set_f((old_a & (1)) << 4);
                                registers.set_a(old_a >> 1 | old_a << 7);
                            }
                            break;
                            case 2:
                            {
                                uint8_t old_a = registers.get_a();
                                bool old_carry = registers.get_f() & (1 << 4);

                                registers.set_a(old_a << 1 | old_carry);
                                registers.set_f((old_a & (1 << 7)) >> 3);
                            }
                            break;
                            case 3:
                            {
                                uint8_t old_a = registers.get_a();
                                bool old_carry = registers.get_f() & (1 << 4);

                                registers.set_a(old_a >> 1 | old_carry << 7);
                                registers.set_f((old_a & 1) << 4);
                            }
                            break;
                            case 4:
                            {
                                bool carry;
                                uint8_t adjustment = 0;
                                if (registers.get_f() & (1 << 6)) {
                                    if (registers.get_f() & (1 << 5)) {
                                        adjustment += 0x6;
                                    }
                                    if (registers.get_f() & (1 << 4)) {
                                        adjustment += 0x60;
                                    }
                                    registers.set_a(registers.get_a() - adjustment);
                                    carry = true;
                                } else {
                                    if (registers.get_f() & (1 << 5) || (registers.get_a() & 0xF) > 0x9) {
                                        adjustment += 0x6;
                                    }
                                    if (registers.get_f() & (1 << 4) || registers.get_a() > 0x99) {
                                        adjustment += 0x60;
                                    }
                                    registers.set_a(registers.get_a() + adjustment);
                                    carry = false;
                                }

                                registers.set_f(
                                    (registers.get_a() == 0) << 7 |
                                    (registers.get_a() & (1 << 6)) |
                                    carry << 4
                                );
                            }
                            break;
                            case 5:
                            {
                                registers.set_a(~registers.get_a());
                                registers.set_f(
                                    (registers.get_f() & (1 << 7)) |
                                    1 << 6 |
                                    1 << 5 |
                                    (registers.get_f() & (1 << 4))
                                );
                            }
                            break;
                            case 6:
                            {
                                registers.set_f(
                                    (registers.get_f() & (1 << 7)) |
                                    1 << 4
                                );
                            }
                            break;
                            case 7:
                            {
                                registers.set_f(
                                    (registers.get_f() & (1 << 7)) |
                                    ~(registers.get_f() & (1 << 4))
                                );
                            }
                            break;
                        }
                    }
                    break;
                    };
            }
            break;
            case 1:
            {
                if (helper.y == 6 && helper.z == 6) {
                    //Todo - Implement HALT;
                    break;
                }

                uint8_t from_byte;
                if (helper.z == 6) {
                    from_byte = memory_bus.read(registers.get_hl());
                } else {
                    from_byte = registers.get_r8(Registers::from_r(helper.z));
                }

                if (helper.y == 6) {
                    memory_bus.write(registers.get_hl(), from_byte);
                } else {
                    registers.set_r8(Registers::from_r(helper.y), from_byte);
                }
            }
            break;
            case 2:
            {
                uint8_t input_byte;
                if (helper.z == 6) {
                    input_byte = memory_bus.read(registers.get_hl());
                } else {
                    input_byte = registers.get_r8(Registers::from_r(helper.z));
                }
                perform_alu_operation((ALU_Instruction) helper.y, input_byte);
            }
            break;
            case 3:
            {
                switch(helper.z) {
                    case 0:
                    {
                        switch(helper.y) {
                            case 0:
                            {
                                if (!(registers.get_f() & (1 << 7))) {
                                    registers.set_pc(pop());
                                }
                            }
                            break;
                            case 1: {
                                if (registers.get_f() & (1 << 7)) {
                                    registers.set_pc(pop());
                                }
                            }
                            break;
                            case 2: {
                                if (!(registers.get_f() & (1 << 4))) {
                                    registers.set_pc(pop());
                                }
                            }
                            break;
                            case 3: {
                                if (registers.get_f() & (1 << 4)) {
                                    registers.set_pc(pop());
                                }
                            }
                            break;
                            case 4: {
                                memory_bus.write(0xFF00 + read_rom(), registers.get_a());
                            }
                            break;
                            case 5: {
                                int8_t add_byte = (int8_t) read_rom();
                                uint16_t stack_pointer = registers.get_sp();

                                bool carry = ((stack_pointer & 0xFF) > (0xFF - stack_pointer));
                                bool half_carry = (stack_pointer & 0xF) + (add_byte & 0xF) > 0xF;

                                registers.set_sp(stack_pointer + add_byte);
                                registers.set_f(
                                    (half_carry << 5) |
                                    (carry << 4)
                                );
                            }
                            break;
                            case 6: {
                                registers.set_a(memory_bus.read(0xFF00 + read_rom()));
                            }
                            break;
                            case 7: {
                                int8_t add_byte = (int8_t) read_rom();
                                uint16_t stack_pointer = registers.get_sp();

                                bool carry = ((stack_pointer & 0xFF) > (0xFF - stack_pointer));
                                bool half_carry = (stack_pointer & 0xF) + (add_byte & 0xF) > 0xF;

                                registers.set_sp(stack_pointer + add_byte);
                                registers.set_hl(registers.get_sp());
                                registers.set_f(
                                    (half_carry << 5) |
                                    (carry << 4)
                                );
                            }
                            break;
                        }
                    }
                    break;
                    case 1: {
                        if (helper.q == 0) {
                            registers.set_r16(Registers::from_rp2(helper.p), pop());
                        } else {
                            switch (helper.p) {
                                case 0: {
                                    registers.set_pc(pop());
                                }
                                break;
                                case 1: {
                                    registers.set_pc(pop());
                                    //Todo - Needs Interrupts - Enable Interrupts after this instruction
                                }
                                case 2: {
                                    registers.set_pc(registers.get_hl());
                                }
                                break;
                                case 3: {
                                    registers.set_sp(registers.get_hl());
                                }
                                break;
                            }
                        }
                    }
                    break;
                    case 2: {
                        switch(helper.y) {
                            case 0:
                            {
                                if (!(registers.get_f() & (1 << 7))) {
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 1: {
                                if (registers.get_f() & (1 << 7)) {
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 2: {
                                if (!(registers.get_f() & (1 << 4))) {
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 3: {
                                if (registers.get_f() & (1 << 4)) {
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 4: {
                                memory_bus.write(0xFF00 + registers.get_c(), registers.get_a());
                            }
                            break;
                            case 5: {
                                memory_bus.write(read_rom_16bit(), registers.get_a());
                            }
                            break;
                            case 6: {
                                registers.set_a(memory_bus.read(0xFF00 + registers.get_c()));
                            }
                            break;
                            case 7: {
                                registers.set_a(memory_bus.read(read_rom_16bit()));
                            }
                            break;
                        }
                    }
                    break;
                    case 3: {
                        switch (helper.y) {
                            case 0: {
                                registers.set_pc(read_rom_16bit());
                            }
                            break;
                            case 1: {
                                std::cerr << "A $CB instruction somehow made it to main instruction parser instead of being interpretted as a prefix.";
                            }
                            break;
                            case 6: {
                                //Todo - Requires Interrrupts - Implement DI
                            }
                            break;
                            case 7: {
                                //Todo - Requires Interrupts - Implement EI
                            }
                        }
                    }
                    break;
                    case 4: {
                        switch(helper.y) {
                            case 0:
                            {
                                if (!(registers.get_f() & (1 << 7))) {
                                    push(registers.get_pc());
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 1: {
                                if (registers.get_f() & (1 << 7)) {
                                    push(registers.get_pc());
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 2: {
                                if (!(registers.get_f() & (1 << 4))) {
                                    push(registers.get_pc());
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                            case 3: {
                                if (registers.get_f() & (1 << 4)) {
                                    push(registers.get_pc());
                                    registers.set_pc(read_rom_16bit());
                                }
                            }
                            break;
                        }
                    }
                    break;
                    case 5: {
                        if (helper.q == 0) {
                            push(registers.get_r16(Registers::from_rp2(helper.p)));
                        } else if (helper.p == 0) {
                            push(registers.get_pc());
                            registers.set_pc(read_rom_16bit());
                        }
                    }
                    break;
                    case 6: {
                        perform_alu_operation((ALU_Instruction) helper.y, read_rom());
                    }
                    break;
                    case 7: {
                        push(registers.get_pc());
                        registers.set_pc(helper.y * 8);
                    }
                    break;
                }
            }
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