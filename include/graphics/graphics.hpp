#pragma once

#include "cpu/cpu.hpp"
#include <SDL3/SDL.h>

namespace graphics {
    class Graphics {
        private:
            cpu::CPU* CPU;
            SDL_Renderer* renderer;
            SDL_Texture* texture;

            uint8_t LY;

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

        public:
            Graphics(cpu::CPU* CPU, SDL_Renderer* renderer);
            void step();

            bool should_render();
    };
}