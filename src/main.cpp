#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "debugger/dbg.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <math.h>
#include <sys/stat.h>

#define MS chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()

// using namespace std;
// using std::ifstream;
int main(void) {
    std::ifstream fs;
    byte bootrom[0x4000];
    fs.open("bootrom.bin", std::ios::in | std::ios::binary);
    fs.read((char*) bootrom, 0x100);
    fs.close();

    Memory cart = Memory(bootrom);
    CPU cpu(cart);

    dbg_main(cart, cpu);

    // dumphex(cart, 2, 0x100);

    // CPU cpu = CPU(cart);
    // PPU ppu = PPU(cart);
    // int i;
    // while (cpu.cycles < 4194304U * 1000U) {
    //     cpu.step();
    //     for (i = 0; i < cpu.current_cycles; i++) {
    //         ppu.tick();
    //     }
    // }
    // cpu.R.print_regs();
    // std::ofstream fb ("fb.bin", std::ios::binary);
    // fb.write((char*) ppu.buffer, 160 * 144);
    // fb.close();
    return 0;
}