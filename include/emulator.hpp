#pragma once
#include "cpu/cpu.hpp"
#include "graphics/graphics.hpp"
#include <string>

class GameBoyEmulator {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;

        cpu::CPU CPU;
        graphics::Graphics GPU;

    public:
        bool should_step;

        GameBoyEmulator(std::string ROMPath);
        void step();
};