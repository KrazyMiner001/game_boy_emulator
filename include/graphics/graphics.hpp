#pragma once

#include "cpu/cpu.hpp"
#include "graphics/oam.hpp"
#include <SDL3/SDL.h>
#include <queue>

namespace cpu {
    class CPU;
}

namespace graphics {
    typedef struct fifo_pixel {
        uint8_t color : 2;
        uint8_t palette : 1;
        bool background_priority : 1;
    };

    class Graphics {
        private:
            enum class Pixel_Fetcher_Mode {
                Get_Tile, Get_Tile_Data_Low, Get_Tile_Data_High, Sleep
            };

            enum class Mode {
                Mode_0, Mode_1, Mode_2, Mode_3
            };

            cpu::CPU* CPU;
            SDL_Renderer* renderer;
            SDL_Texture* texture;

            uint8_t LY;
            Mode mode;
            Pixel_Fetcher_Mode pixel_fetcher_mode;
            uint8_t SCY;
            uint8_t SCX;

            struct {
                bool LCD_Enable : 1;
                bool window_tile_map_area : 1;
                bool window_enable : 1;
                bool background_and_window_tile_data_area : 1;
                bool background_tile_map_area : 1;
                bool obj_size : 1;
                bool obj_enable : 1;
                bool background_and_window_enable : 1;
            } LCDC;

            struct {
                uint8_t tile_id;
                uint8_t tile_row_low;
                uint8_t tile_row_high;
                uint8_t fetcher_x;
                uint8_t window_line_counter;
                bool obj_fetcher : 1;
                bool in_window : 1;
                bool ready_to_push : 1;
                bool window_scanline : 1;
            } pixel_fetcher_memory;

            OAM_Object line_objects[10];

            uint16_t cycle_counter = 0;
            uint8_t LX = 0;
            uint8_t line_objects_index = 0;

            std::queue<fifo_pixel> background_fifo;
            std::queue<fifo_pixel> object_fifo;

            void change_mode();
            uint8_t palettize_colour_background(uint8_t color);
            uint8_t palettize_colour_object(uint8_t color, bool palette);
            uint8_t get_memory(uint16_t address);
            void set_memory(uint16_t address, uint8_t value);

        public:
            Graphics(cpu::CPU* CPU, SDL_Renderer* renderer);
            void step();

            bool should_render();
    };
}