#pragma once
#include <cstdint>

namespace graphics {
    struct OAM_Object {
        uint8_t y_position;
        uint8_t x_position;
        uint8_t tile_index;
        struct {
            bool priority : 1;
            bool y_flip : 1;
            bool x_flip : 1;
            bool dmg_pallete : 1;
            int unused : 4;
        } flags;
    };
}