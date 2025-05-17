#include "graphics/graphics.hpp"

namespace graphics {
    Graphics::Graphics(cpu::CPU* CPU, SDL_Renderer* renderer)
        : CPU(CPU)
        , renderer(renderer)
        , texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, 160, 144))
        {

        }

    void Graphics::step() {
        uint8_t LCDC_byte = CPU->memory_bus.read(0xFF40);

        bool LCD_Enable = LCDC_byte & (1 << 7); //Todo do something with it
        bool window_tile_map = (LCDC_byte & (1 << 6));
        bool window_enable = (LCDC_byte % (1 << 5));
        bool background_and_window_addressing_mode = (LCDC_byte & (1 << 4));
        bool bg_tile_map = (LCDC_byte & (1 << 3));
        bool obj_size = (LCDC_byte & (1 << 2));
        bool obj_enable = (LCDC_byte & (1 << 1));
        bool background_and_window_enable = LCDC_byte & 1;

        CPU->memory_bus.write(0xFF44, LY);
    }
}
