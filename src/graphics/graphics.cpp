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
                if (cycle_counter > 80) {
                    cycle_counter = 0;
                    mode = Mode::Mode_3;
                    break;
                }
                if (cycle_counter % 2 == 0) {
                    OAM_Object *oam = (OAM_Object *)(CPU->memory_bus.memory + 0xFE00);
                    OAM_Object object = oam[cycle_counter / 2];

                    if (
                        (object.x_position > 0) &&
                        (LY + 16 >= object.y_position) &&
                        (LY + 16 < object.y_position + 8 + 8 * LCDC.obj_size) &&
                        (line_objects_index < 10)
                    ) {
                        line_objects[line_objects_index] = object;
                        line_objects_index++;
                    }
                }
                cycle_counter++;
            }
            break;
            case Mode::Mode_3: {
                if (cycle_counter < SCX % 8) {
                    break;
                }
                cycle_counter++;
            }
            break;
            case Mode::Mode_4: {
                         
            }
            break;
        }
    }
}
