#include "emulator.hpp"
#include <unistd.h>

GameBoyEmulator::GameBoyEmulator(std::string ROMPath)
    : window(SDL_CreateWindow("Game Boy Emulator", 480, 432, SDL_WINDOW_RESIZABLE))
    , renderer(SDL_CreateRenderer(window, "vulkan"))
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

    //usleep(1);

    CPU.step();
    
    GPU.step(); // 4 Dots per M-cycle
    //GPU.step();
    //GPU.step();
    //GPU.step();
}
