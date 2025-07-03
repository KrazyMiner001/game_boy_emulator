#include "cartridge.hpp"
#include <cstring>
#include <stdexcept>
#include <format>

namespace cartridge {
    uint8_t NoMBC::read(uint16_t address) {
        if (address > 0x7FFF) {
            throw std::domain_error("Address " + std::format("{:x}", address) + " is outside of the cartridge's ROM");
        }
        return rom[address];
    }

    void NoMBC::write(uint16_t address, uint8_t value) {}

    NoMBC::NoMBC(uint8_t rom[0x8000]) {
        memcpy(this->rom, rom, 0x8000);
    }

    uint8_t NoRAM::read(uint16_t address) {
        //throw std::logic_error("This cartridge does not support RAM");
        return 0;
    }

    void NoRAM::write(uint16_t address, uint8_t value) {}

    Cartridge::Cartridge(uint8_t *rom_data, size_t size) {
        switch (rom_data[0x0147]) {
            case 0x00: {
                rom = std::make_unique<NoMBC>(NoMBC(rom_data));
                ram = std::make_unique<NoRAM>(NoRAM());
            }
            break;
            default: {
                throw std::invalid_argument("The ROM uses an invalid/not yet implemented cartridge type");
            }
        }
    }
}