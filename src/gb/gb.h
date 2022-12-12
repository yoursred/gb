#ifndef __GB_H
#define __GB_H
/*
This does something
*/
#include "include.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "cpu/cpu.h"

class GB {
    public:
    Memory memory;
    CPU cpu;
    PPU ppu;

    GB(const char *rom_path);

    void frame();

};

#endif