#pragma once

#include "cpu/cpu.hpp"
#include "graphics/oam.hpp"
#include <SDL3/SDL.h>

namespace graphics {
    class Graphics {
        private:
            enum class Mode {
                Mode_1, Mode_2, Mode_3, Mode_4
            };

            cpu::CPU* CPU;
            SDL_Renderer* renderer;
            SDL_Texture* texture;

            uint8_t LY;
            Mode mode;
            uint8_t SCY;
            uint8_t SCX;

            struct {
                bool LCD_Enable;
                bool window_tile_map_area;
                bool window_enable;
                bool background_and_window_tile_data_area;
                bool background_tile_map_area;
                bool obj_size;
                bool obj_enable;
                bool background_and_window_enable;
            } LCDC;

            OAM_Object line_objects[10];

            uint8_t cycle_counter = 0;
            uint8_t x_coordinate;
            uint8_t line_objects_index = 0;

        public:
            Graphics(cpu::CPU* CPU, SDL_Renderer* renderer);
            void step();

            bool should_render();
    };
}