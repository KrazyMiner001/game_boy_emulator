#include "graphics/graphics.hpp"

namespace graphics {
    Graphics::Graphics(cpu::CPU* CPU, SDL_Renderer* renderer)
        : CPU(CPU)
        , renderer(renderer)
        , texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, 160, 144))
        {

        }

    void Graphics::step() {

    }
}