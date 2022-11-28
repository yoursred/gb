#ifndef __GBDBG_H
#define __GBDBG_H
#include "cpu/cpu.h"
#include "memory/memory.h"

#define DBG_STOPPED 0
#define DBG_RUNNING 1
#define DBG_PAUSED 2
#define DBG_END 4

#define COUT_HEX_WORD_DS(x) "$" << std::setfill('0') << std::setw(4) << std::hex << x
#define COUT_HEX_WORD(x) std::setfill('0') << std::setw(4) << std::hex << x
#define COUT_HEX_BYTE(x) std::setfill('0') << std::setw(2) << std::hex << x


// void dumphex(Memory& data, word start, word len);
void dbg_main(Memory& mem, CPU& cpu);

struct Breakpoint {
    byte type;
    word address;
    Breakpoint(word address);
    bool match(CPU& cpu);
};
#endif