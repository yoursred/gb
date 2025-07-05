#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <sys/stat.h>
#include <string.h>

#include <CLI/CLI.hpp>

#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "debugger/debug.h"

#define MS chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()


// using namespace std;
// using std::ifstream;
int main(int argc, const char* argv[]) {
    CLI::App gb("boygame emulator");
    gb.set_version_flag("--version", std::string(VERSION));


    // std::ifstream fs_rom, fs_boot;
    byte* rom;
    byte* bootrom = nullptr;
    
    bool debug = false;
    gb.add_flag("-d,--debug", debug, "Enable debugger");

    bool mooneye_debug = false;
    gb.add_flag("-m,--mooneye-debug", mooneye_debug, "Use when running mooneye tests");

    std::string rom_path;
    gb.add_option("-f,--rom,rom", rom_path, "ROM path")
        ->required()
        ->check(CLI::ExistingFile);

    std::string bootrom_path;
    auto *arg_bootrom_path = gb.add_option("-b,--bootrom", bootrom_path, "ROM path")
        ->check(CLI::ExistingFile);

    gb.set_help_flag("");
    gb.set_help_all_flag("-h,--help");
    
    CLI11_PARSE(gb, argc, argv);

    std::streampos rom_size;
    std::ifstream rom_fs;
    rom_fs.open(rom_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (rom_fs.is_open()) {
        rom_size = rom_fs.tellg();
        rom = new byte[rom_size];
        rom_fs.seekg(0, std::ios::beg);
        rom_fs.read((char *)rom, rom_size);
        rom_fs.close();
    } else {
        std::cerr << "Couldn't open ROM file " << rom_path << std::endl;
        exit(-1);
    }

    if (*arg_bootrom_path) {
        std::streampos bootrom_size;
        std::ifstream bootrom_fs;
        bootrom_fs.open(bootrom_path, std::ios::in | std::ios::binary | std::ios::ate);
        if (bootrom_fs.is_open()) {
            bootrom_size = bootrom_fs.tellg();
            bootrom = new byte[bootrom_size];
            bootrom_fs.seekg(0, std::ios::beg);
            bootrom_fs.read((char *)bootrom, bootrom_size);
            bootrom_fs.close();
        } else {
            std::cerr << "Couldn't open bootrom file " << bootrom_path << std::endl;
            exit(-1);
        }
    }



    Memory cart(bootrom, rom, rom_size);
    
    
    CPU cpu(cart);

    if (cart.boot_rom) {
        cpu.R.af = 0;
        cpu.R.bc = 0;
        cpu.R.de = 0;
        cpu.R.hl = 0;
        cpu.R.sp = 0;
        cpu.R.pc = 0;
    }

    cart.cpu = &cpu;
    PPU ppu(cart);
    Debugger dbg(cart, cpu, ppu);
    dbg.mooneye_debug = mooneye_debug;

    if (debug) {
        dbg.debug_main();
    } else {
        // TODO: BoyGame class
    }

    return 0;
}