#include <iostream>
#include "cpu/cpu.hpp"

using namespace cpu;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Invalid command. Usage: Game_Boy_Emulator [file path]";
        return 0;
    }

    CPU cpu(argv[1]);
    
    for (int i = 0; i < 256; i++) {
        cpu.step();
    }

    std::cout << "hi";
}