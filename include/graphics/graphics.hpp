#pragma once

#include "cpu/cpu.hpp"
#include <SDL3/SDL.h>

namespace graphics {
    class Graphics {
        private:
            cpu::CPU* CPU;
            SDL_Renderer* renderer;
            SDL_Texture* texture;

        public:
            Graphics(cpu::CPU* CPU, SDL_Renderer* renderer);
            void step();
    };
}