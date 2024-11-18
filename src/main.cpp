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
    std::ifstream fs;
    byte* test_rom;
    struct stat buf;
    // sf::RenderWindow window;

    bool debug = false;

    if (argc == 2) {
        if (stat(argv[1], &buf) == 0) {
            test_rom = new byte[buf.st_size];
            fs.open(argv[1], std::ios::in | std::ios::binary);
            fs.read((char *) test_rom, buf.st_size);
        }
        else {
            std::cerr << "File does not exist" << std::endl;
            exit(1);
        }
    }
    else if (argc == 3) {
        if (strcmp(argv[1], "debug") || strcmp(argv[1], "d")) {
            debug = true;
            if (stat(argv[2], &buf) == 0) {
                test_rom = new byte[buf.st_size];
                fs.open(argv[2], std::ios::in | std::ios::binary);
                fs.read((char *) test_rom, buf.st_size);
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

    Memory cart = Memory(test_rom, buf.st_size);
    CPU cpu(cart);
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