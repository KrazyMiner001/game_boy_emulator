#include "graphics/graphics.hpp"
#include <iostream>

namespace graphics {
    Graphics::Graphics(cpu::CPU* CPU, SDL_Renderer* renderer)
        : CPU(CPU)
        , renderer(renderer)
        , texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, 160, 144))
        , mode(Mode::Mode_2)
        , pixel_fetcher_mode(Pixel_Fetcher_Mode::Get_Tile)
        {
            SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
            pixel_fetcher_memory.obj_fetcher = false;
            pixel_fetcher_memory.ready_to_push = false;
        }

    void Graphics::step() {
        SCY = get_memory(0xFF42);
        SCX = get_memory(0xFF43);

        uint8_t LCDC_byte = get_memory(0xFF40);

        LCDC.LCD_Enable = LCDC_byte & (1 << 7); //Todo do something with it
        LCDC.window_tile_map_area = (LCDC_byte & (1 << 6));
        LCDC.window_enable = (LCDC_byte % (1 << 5));
        LCDC.background_and_window_tile_data_area = (LCDC_byte & (1 << 4));
        LCDC.background_tile_map_area = (LCDC_byte & (1 << 3));
        LCDC.obj_size = (LCDC_byte & (1 << 2));
        LCDC.obj_enable = (LCDC_byte & (1 << 1));
        LCDC.background_and_window_enable = LCDC_byte & 1;

        change_mode();
        cycle_counter++;

        switch(mode) {
            case Mode::Mode_0: {

            }
            break;
            case Mode::Mode_1: {
                if (cycle_counter % 456 == 0) {
                    LY++;
                }
            }
            break;
            case Mode::Mode_2: {
                if (cycle_counter % 2 == 0) {
                    OAM_Object *oam = (OAM_Object *)(CPU->memory_bus.memory + 0x3E00);
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
            }
            break;
            case Mode::Mode_3: {
                pixel_fetcher_memory.in_window = ((LY >= get_memory(0xFF4A)) && (LX >= (get_memory(0xFF4B) - 7)));

                if (!pixel_fetcher_memory.window_scanline && pixel_fetcher_memory.in_window) {
                    pixel_fetcher_memory.window_line_counter++;
                    pixel_fetcher_memory.window_scanline = true;
                    while (!background_fifo.empty()) {
                        background_fifo.pop();
                    }
                    pixel_fetcher_mode = Pixel_Fetcher_Mode::Get_Tile;
                }

                if (cycle_counter % 2 == 0) {
                    if (pixel_fetcher_memory.ready_to_push && background_fifo.size() <= 8) {
                        if (!pixel_fetcher_memory.obj_fetcher) {
                            for (uint8_t i = 0; i < 8; i++) {
                                fifo_pixel pixel;
                                pixel.background_priority = 0;
                                pixel.palette = 0;
                                pixel.color = ((pixel_fetcher_memory.tile_row_high & (1 << (7 - i))) >> (7 - i)) | ((pixel_fetcher_memory.tile_row_low & (1 << (7 - i))) >> (7 - i));
                                background_fifo.push(pixel);
                            }
                        }

                        pixel_fetcher_memory.ready_to_push = false;
                    }

                    switch (pixel_fetcher_mode) {
                        case Pixel_Fetcher_Mode::Get_Tile: {
                            //Todo: OBJ Fetcher
                            // uint16_t fetch_address = 0b10011'00000000000;
                            // pixel_fetcher_memory.in_window = ((LY >= get_memory(0xFF4A)) && (LX >= (get_memory(0xFF4B) - 7)));
                            // if (pixel_fetcher_memory.in_window) {
                            //     fetch_address |= (LCDC.window_tile_map_area << 10)
                            //         | ((get_memory(0xFF4A) >> 3) << 5)
                            //         | (LX >> 3);
                            // } else {
                            //     fetch_address |= (LCDC.background_tile_map_area << 10)
                            //         | (((LY + SCY) >> 3) << 5)
                            //         | ((LX + SCX) >> 3);
                            // }

                            uint16_t fetch_address = 0x9800;
                            if (pixel_fetcher_memory.in_window) {
                                fetch_address |= LCDC.window_tile_map_area << 10;
                            } else {
                                fetch_address |= LCDC.background_tile_map_area << 10;
                            }

                            uint16_t offset = pixel_fetcher_memory.fetcher_x;
                            offset += (SCX >> 3) * !pixel_fetcher_memory.in_window;
                            offset &= 0x1f;
                            fetch_address += offset;

                            if (pixel_fetcher_memory.in_window) {
                                fetch_address += (((LY + SCY) & 0xFF) >> 3) << 5;
                            } else {
                                fetch_address += (pixel_fetcher_memory.window_line_counter >> 3) << 5;
                            }

                            //Now begins second dot

                            pixel_fetcher_memory.tile_id = get_memory(fetch_address); //Should always be fetching VRAM

                            //Change mode
                            pixel_fetcher_mode = Pixel_Fetcher_Mode::Get_Tile_Data_Low;
                        }
                        break;
                        case Pixel_Fetcher_Mode::Get_Tile_Data_Low: {
                            //Todo: OBJ Fetcher
                            uint16_t fetch_address = 0b100'000000000000'0;
                            fetch_address |= (pixel_fetcher_memory.tile_id << 4);
                            if (!pixel_fetcher_memory.obj_fetcher) {
                                fetch_address |= !((LCDC.background_and_window_tile_data_area) || (pixel_fetcher_memory.tile_id & 0x80)) << 12;

                                if (pixel_fetcher_memory.in_window) {
                                    fetch_address |= ((pixel_fetcher_memory.window_line_counter + SCY - 1) % 8) << 1; 
                                } else {
                                    fetch_address |= ((LY + SCY) % 8) << 1;
                                }
                            }

                            pixel_fetcher_memory.tile_row_low = get_memory(fetch_address); //Fetch address should be in VRAM;

                            //Change mode
                            pixel_fetcher_mode = Pixel_Fetcher_Mode::Get_Tile_Data_High;
                        }
                        break;
                        case Pixel_Fetcher_Mode::Get_Tile_Data_High: {
                            //Todo: OBJ Fetcher
                            uint16_t fetch_address = 0b100'000000000000'1;
                            fetch_address |= (pixel_fetcher_memory.tile_id << 4);
                            if (!pixel_fetcher_memory.obj_fetcher) {
                                fetch_address |= !((LCDC.background_and_window_tile_data_area) || (pixel_fetcher_memory.tile_id & 0x80)) << 12;

                                if (pixel_fetcher_memory.in_window) {
                                    fetch_address |= ((pixel_fetcher_memory.window_line_counter + SCY - 1) % 8) << 1; 
                                } else {
                                    fetch_address |= ((LY + SCY) % 8) << 1;
                                }
                            }

                            pixel_fetcher_memory.tile_row_high = get_memory(fetch_address); //Fetch address should be in VRAM;

                            //Change mode
                            pixel_fetcher_mode = Pixel_Fetcher_Mode::Sleep;
                        }
                        break;
                        case Pixel_Fetcher_Mode::Sleep: {
                            pixel_fetcher_memory.ready_to_push = true;

                            //Chane mode
                            pixel_fetcher_mode = Pixel_Fetcher_Mode::Get_Tile;
                            pixel_fetcher_memory.fetcher_x++;
                        }
                    }
                }

                if (pixel_fetcher_memory.obj_fetcher) {
                    break;
                }

                if (background_fifo.size() == 0) {
                    break;
                }

                fifo_pixel background_pixel = background_fifo.front();
                background_fifo.pop();

                uint8_t draw_color;
                uint8_t background_color = LCDC.background_and_window_enable ? background_pixel.color : 0b00;

                if (!object_fifo.empty()) {
                    fifo_pixel object_pixel = object_fifo.front();
                    object_fifo.pop();

                    if (object_pixel.color && (object_pixel.background_priority || !background_pixel.background_priority)) {
                        draw_color = palettize_colour_object(
                            object_pixel.color,
                            object_pixel.palette
                        );
                    } else {
                        if (background_color || !object_pixel.color) {
                            draw_color = palettize_colour_background(background_color);
                        } else {
                            draw_color = palettize_colour_object(
                                object_pixel.color,
                                object_pixel.palette
                            );
                        }
                    }
                } else {
                    draw_color = palettize_colour_background(background_color);
                }

                if (LX < 0) {
                    break;
                }

                uint8_t draw_color_8bit = 85 * draw_color; // 85 = 255/3y

                SDL_SetRenderDrawColor(
                    renderer,
                    draw_color_8bit,
                    draw_color_8bit,
                    draw_color_8bit,
                    255
                );

                SDL_RenderPoint(
                    renderer,
                    LX,
                    LY
                );
                
                LX++;
            }
            break;
        }

        set_memory(0xFF44, LY);
        uint8_t LYC = get_memory(0xFF45);

        uint8_t old_stat = get_memory(0xFF41);
        //Todo: interrupts

        set_memory(0xFF41, (old_stat & 0b11111000) | ((uint8_t) mode) | ((LYC == LY) << 2));
    }

    void Graphics::change_mode() {
        if (mode == Mode::Mode_2 && cycle_counter > 80) {
            mode = Mode::Mode_3;
            cycle_counter = 0;
            background_fifo = {};
            object_fifo = {};
            pixel_fetcher_memory.window_scanline = false;
            SDL_SetRenderTarget(renderer, texture);
        } else if (mode == Mode::Mode_3 && LX >= 160) {
            mode = Mode::Mode_0;
            pixel_fetcher_memory.fetcher_x = 0;
        } else if (mode == Mode::Mode_0 && cycle_counter >= 376) {
            if (LY > 143) {
                pixel_fetcher_memory.window_line_counter = 0;
                mode = Mode::Mode_1;
            } else {
                mode = Mode::Mode_2;
            }
            cycle_counter = 0;
            LX = 0;
            LY++;
        } else if (mode == Mode::Mode_1 && cycle_counter >= 4560) {
            mode = Mode::Mode_2;
            SDL_SetRenderTarget(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            LY = 0;
        }
    }

    uint8_t Graphics::palettize_colour_background(uint8_t color) {
        return (get_memory(0xFF47) & (0b11 << (2 * color))) >> (2 * color);
    }

    uint8_t Graphics::palettize_colour_object(uint8_t color, bool palette) {
        if (palette) {
            return (get_memory(0xFF49) & (0b11 << (2 * color))) >> (2 * color);
        } else {
            return (get_memory(0xFF48) & (0b11 << (2 * color))) >> (2 * color);
        }
    }

    uint8_t Graphics::get_memory(uint16_t address) {
        return CPU->memory_bus.read(address);
    }

    void Graphics::set_memory(uint16_t address, uint8_t value) {
        CPU->memory_bus.write(address, value);
    }
}
