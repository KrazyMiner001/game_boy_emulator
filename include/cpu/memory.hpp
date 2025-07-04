#pragma once
#include <cstdint>

namespace graphics {
    class Graphics;
}

namespace cpu {
    class CPU;

    class MemoryBus {
        friend class graphics::Graphics;
    private:
        uint8_t memory[0x4000];
        uint8_t vram[0x2000];
        CPU &cpu;
        static constexpr uint8_t dmg_boot_rom[256] = {0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0xe, 0x11, 0x3e, 0x80, 0x32, 0xe2, 0xc, 0x3e, 0xf3, 0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, 0x47, 0x11, 0x4, 0x1, 0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x0, 0xcd, 0x96, 0x0, 0x13, 0x7b, 0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x0, 0x6, 0x8, 0x1a, 0x13, 0x22, 0x23, 0x5, 0x20, 0xf9, 0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99, 0xe, 0xc, 0x3d, 0x28, 0x8, 0x32, 0xd, 0x20, 0xf9, 0x2e, 0xf, 0x18, 0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x4, 0x1e, 0x2, 0xe, 0xc, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0xd, 0x20, 0xf7, 0x1d, 0x20, 0xf2, 0xe, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62, 0x28, 0x6, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x6, 0x7b, 0xe2, 0xc, 0x3e, 0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x5, 0x20, 0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x6, 0x4, 0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17, 0x5, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9, 0xce, 0xed, 0x66, 0x66, 0xcc, 0xd, 0x0, 0xb, 0x3, 0x73, 0x0, 0x83, 0x0, 0xc, 0x0, 0xd, 0x0, 0x8, 0x11, 0x1f, 0x88, 0x89, 0x0, 0xe, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63, 0x6e, 0xe, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e, 0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x21, 0x4, 0x1, 0x11, 0xa8, 0x0, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20, 0xf5, 0x6, 0x19, 0x78, 0x86, 0x23, 0x5, 0x20, 0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x1, 0xe0, 0x50};
        static constexpr uint8_t sgb_boot_rom[256] = {0x31, 0xfe, 0xff, 0x3e, 0x30, 0xe0, 0x0, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0xe, 0x11, 0x3e, 0x80, 0x32, 0xe2, 0xc, 0x3e, 0xf3, 0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, 0x47, 0x21, 0x5f, 0xc0, 0xe, 0x8, 0xaf, 0x32, 0xd, 0x20, 0xfc, 0x11, 0x4f, 0x1, 0x3e, 0xfb, 0xe, 0x6, 0xf5, 0x6, 0x0, 0x1a, 0x1b, 0x32, 0x80, 0x47, 0xd, 0x20, 0xf8, 0x32, 0xf1, 0x32, 0xe, 0xe, 0xd6, 0x2, 0xfe, 0xef, 0x20, 0xea, 0x11, 0x4, 0x1, 0x21, 0x10, 0x80, 0x1a, 0xcd, 0xd3, 0x0, 0xcd, 0xd4, 0x0, 0x13, 0x7b, 0xfe, 0x34, 0x20, 0xf3, 0x11, 0xe6, 0x0, 0x6, 0x8, 0x1a, 0x13, 0x22, 0x23, 0x5, 0x20, 0xf9, 0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99, 0xe, 0xc, 0x3d, 0x28, 0x8, 0x32, 0xd, 0x20, 0xf9, 0x2e, 0xf, 0x18, 0xf3, 0x3e, 0x91, 0xe0, 0x40, 0x21, 0x0, 0xc0, 0xe, 0x0, 0x3e, 0x0, 0xe2, 0x3e, 0x30, 0xe2, 0x6, 0x10, 0x1e, 0x8, 0x2a, 0x57, 0xcb, 0x42, 0x3e, 0x10, 0x20, 0x2, 0x3e, 0x20, 0xe2, 0x3e, 0x30, 0xe2, 0xcb, 0x1a, 0x1d, 0x20, 0xef, 0x5, 0x20, 0xe8, 0x3e, 0x20, 0xe2, 0x3e, 0x30, 0xe2, 0xcd, 0xc2, 0x0, 0x7d, 0xfe, 0x60, 0x20, 0xd2, 0xe, 0x13, 0x3e, 0xc1, 0xe2, 0xc, 0x3e, 0x7, 0xe2, 0x18, 0x3a, 0x16, 0x4, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x1e, 0x0, 0x1d, 0x20, 0xfd, 0x15, 0x20, 0xf2, 0xc9, 0x4f, 0x6, 0x4, 0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17, 0x5, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9, 0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3e, 0x1, 0xe0, 0x50};

    public:
        MemoryBus(CPU &cpu);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    };
}