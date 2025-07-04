#include "cpu/memory.hpp"
#include "cpu/cpu.hpp"

namespace cpu {
    MemoryBus::MemoryBus(CPU &cpu) : cpu(cpu) {}

    void MemoryBus::write(uint16_t address, uint8_t value) {
        if (address < 0x8000) {
            cpu.cartridge.rom->write(address, value);
        } else if (address < 0xA000) {
            vram[address - 0x8000] = value;
        } else if (address < 0xC000) {
            cpu.cartridge.ram->write(address - 0xA000, value);
        } else {
            memory[address - 0xC000] = value;
        }
    }

    uint8_t MemoryBus::read(uint16_t address) {
        if (address < 0x100) {
            if (!read(0xFF50)) {
                return dmg_boot_rom[address];
            }
        }

        if (address < 0x8000) {
            return cpu.cartridge.rom->read(address);
        } else if (address < 0xA000) {
            return vram[address - 0x8000];
        } else if (address < 0xC000) {
            return cpu.cartridge.ram->read(address - 0xA000);
        } else {
            return memory[address - 0xC000];
        }
    }
}