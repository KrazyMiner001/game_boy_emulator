#include <iostream>
#include "emulator.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);

    if (argc < 2) {
        std::cout << "Invalid command. Usage: Game_Boy_Emulator [file path]";
        return 0;
    }

    GameBoyEmulator emulator(argv[1]);
    
    while (emulator.should_step) {
        emulator.step();
    }

    std::cout << "hi";
}