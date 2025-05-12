project(
    'game_boy_emulator',
    'cpp',

    default_options : [
        'cpp_std=c++20'
    ],
    version : '1.0.0'
)

sdl3_dep = dependency('sdl3')

src = [
    'src/main.cpp', 
    'src/cartridge.cpp', 
    'src/cpu/cpu.cpp', 
    'src/cpu/memory.cpp', 
    'src/cpu/registers.cpp',
    'src/emulator.cpp',
    'src/graphics/graphics.cpp',

]
incdir = include_directories('include')
exe = executable('gbemu', 
    sources: src, 
    include_directories: incdir,
    dependencies: sdl3_dep
)