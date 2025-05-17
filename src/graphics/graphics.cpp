#include "graphics/graphics.hpp"

namespace graphics {
    Graphics::Graphics(cpu::CPU* CPU, SDL_Renderer* renderer)
        : CPU(CPU)
        , renderer(renderer)
        , texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, 160, 144))
        , mode(Mode::Mode_2)
        {

        }

    void Graphics::step() {
        SCY = CPU->memory_bus.read(0xFF42);
        SCX = CPU->memory_bus.read(0xFF43);

        uint8_t LCDC_byte = CPU->memory_bus.read(0xFF40);

        LCDC.LCD_Enable = LCDC_byte & (1 << 7); //Todo do something with it
        LCDC.window_tile_map_area = (LCDC_byte & (1 << 6));
        LCDC.window_enable = (LCDC_byte % (1 << 5));
        LCDC.background_and_window_tile_data_area = (LCDC_byte & (1 << 4));
        LCDC.background_tile_map_area = (LCDC_byte & (1 << 3));
        LCDC.obj_size = (LCDC_byte & (1 << 2));
        LCDC.obj_enable = (LCDC_byte & (1 << 1));
        LCDC.background_and_window_enable = LCDC_byte & 1;

        switch(mode) {
            case Mode::Mode_1: {
                         
            }
            break;
            case Mode::Mode_2: {

            }
            break;
            case Mode::Mode_3: {
                         
            }
            break;
            case Mode::Mode_4: {
                         
            }
            break;
        }

        CPU->memory_bus.write(0xFF44, LY);
    }
}
