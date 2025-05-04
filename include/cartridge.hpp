#pragma once

#include <cstdint>
#include <memory>

namespace cartridge {
    class CartridgeROM {
        public:
            virtual uint8_t read(uint16_t address) = 0;
            virtual void write(uint16_t address, uint8_t value) = 0;
    };

    class CartridgeRAM {
        public:
            virtual uint8_t read(uint16_t address) = 0;
            virtual void write(uint16_t address, uint8_t value) = 0;
    };
    
    class NoMBC : public CartridgeROM {
        private:
            uint8_t rom[0x8000];
        public:
            uint8_t read(uint16_t address) override;
            void write(uint16_t address, uint8_t value) override;

            NoMBC(uint8_t rom[0x8000]);
    };

    class NoRAM : public CartridgeRAM {
        public:
            uint8_t read(uint16_t address) override;
            void write(uint16_t address, uint8_t value) override;
    };

    class Cartridge {
        public:
            std::unique_ptr<CartridgeRAM> ram;
            std::unique_ptr<CartridgeROM> rom;

            Cartridge(uint8_t *rom_data, size_t size);
    };
}

