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
#include <SFML/Graphics.hpp>

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
void dumphex(Memory& data, word start, word len, std::ostream& output);

// Parsing functions

bool hex_byte(std::string lit, byte &x);
bool dec_byte(std::string lit, byte &x);
bool hex_word(std::string lit, word &x);
bool dec_word(std::string lit, word &x);
bool dec_uint(std::string lit, uint &x);
bool str(std::string lit, std::vector<std::string> const& valid);
bool str(std::string lit, size_t min, size_t max);


// void dumphex(Memory& data, word start, word len);
// void dbg_main(Memory& mem, CPU& cpu, PPU& ppu);

// void ppu_dbg_main(Memory& mem, PPU& ppu);

// void debug_thread(Debugger& dbg);

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
    
    Breakpoint(std::string rs, word value16, byte value8);
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

    sf::RenderWindow window; // = sf::RenderWindow(sf::VideoMode(160, 144), "boygame debooger - version");
    sf::Texture texture;


    Debugger(Memory& mem, CPU& cpu, PPU& ppu);

    void debug_main(int argc, const char* argv[]);
    void cpu_debug_main(int argc, const char* argv[]);
    void ppu_debug_main(int argc, const char* argv[]);
    std::vector<std::string> split_command(std::string cmd);

    private:
    void debug_thread();
    void render_thread();

    std::vector<std::string> commands = {
        "c",
        "clear",
        "continue",
        "cycles",
        "exit",
        "flags",
        "ic",
        "l",
        "lb",
        "list",
        "opc",
        "pause",
        "ppu",
        "registers",
        "state",
        "s",
        "step",
        "b",
        "break",
        "d",
        "delete",
        "dump"
    };

    std::map<std::string, void(Debugger::*)(void)> simple_commands_ptr = {
        {"c", &Debugger::cmd_continue},
        {"clear", &Debugger::cmd_clear},
        {"continue", &Debugger::cmd_continue},
        {"cycles", &Debugger::cmd_cycles},
        {"exit", &Debugger::cmd_exit},
        {"flags", &Debugger::cmd_flags},
        {"ic", &Debugger::cmd_ic},
        {"l", &Debugger::cmd_list},
        {"lb", &Debugger::cmd_list},
        {"list", &Debugger::cmd_list},
        {"opc", &Debugger::cmd_opc},
        {"pause", &Debugger::cmd_pause},
        {"ppu", &Debugger::cmd_ppu},
        {"registers", &Debugger::cmd_registers},
        {"state", &Debugger::cmd_state},
        {"s", &Debugger::cmd_step},
        {"step", &Debugger::cmd_step}
    };

    std::map<std::string, void(Debugger::*)(std::vector<std::string>)> commands_ptr = {
        {"b", &Debugger::cmd_break},
        {"break", &Debugger::cmd_break},
        {"d", &Debugger::cmd_delete},
        {"delete", &Debugger::cmd_delete},
        {"dump", &Debugger::cmd_dump}
    };

    std::vector<std::string> const break_types = {
        "pc", "af", "bc", "de", "hl",
        "a", "b", "c", "d", "e", "h", "l",
        "wread", "wwrite"
    };
    std::vector<std::string> const break16_types = {
        "pc", "af", "bc", "de", "hl",
        "wread", "wwrite"
    };
    std::vector<std::string> const reg8_types = {
        "a", "b", "c", "d", "e", "h", "l"
    };
    void cmd_break(std::vector<std::string> args);
    void cmd_clear();
    void cmd_continue();
    void cmd_cycles();
    void cmd_delete(std::vector<std::string> args);
    void cmd_dump(std::vector<std::string> args);
    void cmd_exit();
    void cmd_flags();
    void cmd_ic();
    void cmd_list();
    void cmd_opc();
    void cmd_pause();
    void cmd_ppu();
    void cmd_registers();
    void cmd_state();
    void cmd_step();
};
#endif