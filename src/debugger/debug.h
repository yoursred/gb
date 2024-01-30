#ifndef __GBDBG_H
#define __GBDBG_H
#include "include.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "memory/memory.h"
#include <string>

#define DBG_STOPPED 0
#define DBG_RUNNING 1
#define DBG_PAUSED 2
#define DBG_END 4

#define RWATCH_STR(r) case r: ss << #r; break;
#define STR_RWATCH(r) if (rs == #r) {watch = r;}

// Output functions
std::string print_instruction(CPU& cpu);
void dumphex(Memory& data, word start, word len);


// void dumphex(Memory& data, word start, word len);
void dbg_main(Memory& mem, CPU& cpu, PPU& ppu);

enum rwatch {
    pc = 0x10, af = 0x11, bc = 0x12, de = 0x13, hl = 0x14,
    a = 0, b = 1, c = 2, d = 3, e = 4, h = 5, l = 6 
};

struct Breakpoint {
    rwatch watch;
    
    word address16;
    word value16;
    byte value8;
    
    Breakpoint(std::string rs, word value);
    Breakpoint(rwatch watch, word value);
    Breakpoint(rwatch watch, word address, byte value);
    Breakpoint(rwatch watch, byte value);
    
    bool match(CPU& cpu);
    std::string str();
};
#endif