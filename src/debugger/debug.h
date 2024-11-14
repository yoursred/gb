#ifndef __GBDBG_H
#define __GBDBG_H
#include "include.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "memory/memory.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define DBG_STOPPED 0
#define DBG_RUNNING 1
#define DBG_PAUSED 2
#define DBG_END 4

// What is this shitty naming?
// #define RWATCH_STR(r) case r: ss << #r; break;
// #define RWATCH_STR_IF(r) if (rs == #r) {watch = r;}

#define RWATCH_STR_CASE(r) case r: ss << #r; break; // Used for string output
#define RWATCH_STR_IF(r) if (rs == #r) {watch = r;} //


// Output functions
std::string print_instruction(CPU& cpu);
void dumphex(Memory& data, word start, word len);


// void dumphex(Memory& data, word start, word len);
// void dbg_main(Memory& mem, CPU& cpu, PPU& ppu);

// void ppu_dbg_main(Memory& mem, PPU& ppu);

enum rwatch {
    pc = 0x10, af = 0x11, bc = 0x12, de = 0x13, hl = 0x14,
    a = 0, b = 1, c = 2, d = 3, e = 4, h = 5, l = 6,
    wread = 0x100, wmem = 0x101, wwrite = 0x110
};

/* TODO: inherit from this struct to allow for any kind of breakpoint
low priority because the current ones pretty much cover everything*/
struct Breakpoint {
    rwatch watch;
    
    word address16;
    word value16;
    byte value8;
    
    Breakpoint(std::string rs, word value, byte mem_value);
    Breakpoint(rwatch watch, word value);
    // Breakpoint(rwatch watch, word address); // break on read/write, redundant ^^^
    Breakpoint(rwatch watch, word address, byte value); // break on mem match, not all memory addresses will change due to write
    Breakpoint(rwatch watch, byte value);
    
    bool match(CPU& cpu);
    std::string str();
};


class Debugger {
    public:
    std::ostream &output = std::cout;

    byte state = DBG_STOPPED;
    std::vector<Breakpoint> breakpoints;

    std::string cmd;
    std::stringstream cmd_ss;
    std::string token;
    word x, y;
    int i;

    Memory& mem;
    CPU& cpu;
    PPU& ppu;


    Debugger(Memory& mem, CPU& cpu, PPU& ppu);

    void debug_main(int argc, const char* argv[]);
    void ppu_debug_main(int argc, const char* argv[]);
    int split_command(std::string cmd, std::map<std::string, std::string> &tokenized);

    private:
    void cmd_break(std::map<std::string, std::string> args);

};
#endif