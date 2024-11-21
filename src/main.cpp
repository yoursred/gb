#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "debugger/debug.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <math.h>
#include <sys/stat.h>
#include <string.h>
// #include <SFML/Graphics.hpp>

#define MS chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()


// using namespace std;
// using std::ifstream;
int main(int argc, const char* argv[]) {
    // TODO: command line arguments
    // std::stringstream log;
    // std::stringstream doctor_log;
    std::ifstream fs_rom, fs_boot;
    byte* rom;
    byte* bootrom;
    bool bootrom_enable = false;
    bool rom_enable = true;
    struct stat buf_rom;
    struct stat buf_boot;
    // sf::RenderWindow window;

    bool debug = false;

    if (argc == 2) {
        if (stat(argv[1], &buf_rom) == 0) {
            rom = new byte[buf_rom.st_size];
            fs_rom.open(argv[1], std::ios::in | std::ios::binary);
            fs_rom.read((char *) rom, buf_rom.st_size);
        }
        else {
            std::cerr << "File does not exist" << std::endl;
            exit(1);
        }
    }
    else if (argc == 3) {
        if (std::string("debug") == std::string(argv[1])|| std::string("d") == std::string(argv[1])) {
            debug = true;
            if (stat(argv[2], &buf_rom) == 0) {
                rom = new byte[buf_rom.st_size];
                fs_rom.open(argv[2], std::ios::in | std::ios::binary);
                fs_rom.read((char *) rom, buf_rom.st_size);
            }
            else {
                std::cerr << "File does not exist" << std::endl;
                exit(1);
            }
        }
        else if (std::string("bootrom") == std::string(argv[1])) {
            debug = true;
            if (stat(argv[2], &buf_boot) == 0) {
                bootrom = new byte[buf_boot.st_size];
                fs_rom.open(argv[2], std::ios::in | std::ios::binary);
                fs_rom.read((char *) bootrom, buf_boot.st_size);
                bootrom_enable = true;
                rom_enable = false;
            }
            else {
                std::cerr << "File does not exist" << std::endl;
                exit(1);
            }
        }
        else {
            std::cerr << "Unknown option: " << argv[1] << std::endl;
            exit(1);
        }
    }
    else if (argc == 4) {
        if (std::string("debug") == std::string(argv[1])|| std::string("d") == std::string(argv[1])) {
            debug = true;
            if (stat(argv[2], &buf_rom) == 0) {
                rom = new byte[buf_rom.st_size];
                fs_rom.open(argv[2], std::ios::in | std::ios::binary);
                fs_rom.read((char *) rom, buf_rom.st_size);
                fs_rom.close();
            }
            else {
                std::cerr << "File does not exist" << std::endl;
                exit(1);
            }
            if (stat(argv[3], &buf_boot) == 0) {
                std::cout << buf_boot.st_size << std::endl;
                if (buf_boot.st_size == 0x100) {
                    bootrom = new byte[buf_boot.st_size];
                    fs_boot.open(argv[3], std::ios::in | std::ios::binary);
                    fs_boot.read((char *) bootrom, buf_boot.st_size);
                    bootrom_enable = true;
                } else {
                    std::cerr << "Invalid bootrom size" << std::endl;
                    exit(1);
                }
            }
            else {
                std::cerr << "File does not exist" << std::endl;
                exit(1);
            }
        }
        else {
            std::cerr << "Unknown option: " << argv[1] << std::endl;
            exit(1);
        }
    }
    else {
        std::cerr << "Argument error" << std::endl;
        exit(1);
    }

    // if (!bootrom_enable)
    // else
    // Memory cart = Memory(bootrom);
    Memory cart = Memory(bootrom, rom, buf_rom.st_size);
    CPU cpu(cart);
    cpu.R.pc = 0;
    cpu.new_pc = 0;
    cpu.R.af = 0;
    cpu.R.bc = 0;
    cpu.R.de = 0;
    cpu.R.hl = 0;
    cpu.R.sp = 0;
    cart.cpu = &cpu;
    PPU ppu(cart);
    Debugger dbg(cart, cpu, ppu);

    // dbg.split_command("help test");
    if (debug) {
        dbg.debug_main(argc, argv);
    }


    // if (debug) {
    //     dbg_main(cart, cpu, ppu);
    // }
    // else {
    //     for (size_t i = 0; i < 325783; i++) {
    //     // while (1) {
    //         doctor_log << cpu.log();
    //         #ifdef __GHOST_DEBUG
    //         if (cart.mem_writes.size() != cpu.memory_writes) {
    //             // std::cout << cpu.log();
    //             // std::cout << "CPU tracked writes: " << std::dec << cpu.memory_writes << std::endl;
    //             // std::cout << "MMU tracked writes: " << std::dec << cart.mem_writes.size() << std::endl;
    //             for (auto &&w: cart.mem_writes) {
    //                 log << std::dec << w.cpu_steps << ",0x" << COUT_HEX_WORD(w.address) << ",0x" << COUT_HEX_BYTE(w.value) << "\n";
    //             }
    //             // out << log.rdbuf();
    //             // out.close();
    //             // return -1;
    //         }
    //         #endif
    //         cart.cpu_steps = i;
    //         cpu.step();
    //     }
    //     // std::cout << "CPU tracked writes: " << std::dec << cpu.memory_writes << std::endl;
    //     // std::cout << "MMU tracked writes: " << std::dec << cart.mem_writes.size() << std::endl;
    //     for (auto &&w: cart.mem_writes) {
    //         log << std::dec << w.cpu_steps << ",0x" << COUT_HEX_WORD(w.address) << ",0x" << COUT_HEX_BYTE(w.value) << "\n";
    //     }

    //     out << log.rdbuf();
    //     out.close();
    //     out_dr << doctor_log.rdbuf();
    //     out_dr.close();
    // }
    // while (1) {
    //     cpu.step();
    //     // A:00 F:11 B:22 C:33 D:44 E:55 H:66 L:77 SP:8888 PC:9999 PCMEM:AA,BB,CC,DD
    //     log
    // }
    
    return 0;
}