#include "emulator.hpp"

GameBoyEmulator::GameBoyEmulator(std::string ROMPath)
    : window(SDL_CreateWindow("Game Boy Emulator", 500, 500, 0))
    , renderer(SDL_CreateRenderer(window, NULL))
    , CPU(cpu::CPU(ROMPath))
    , GPU(graphics::Graphics(&CPU, renderer))
    , should_step(true)
{

}

void GameBoyEmulator::step() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            should_step = false;
        }
    }

    CPU.step();
    
    GPU.step(); // 4 Dots per M-cycle
    GPU.step();
    GPU.step();
    GPU.step();
}
