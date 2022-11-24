#include "cpu/cpu.h"
#include "memory/memory.h"
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
    byte rom[0x4000];
    fs.open("roms/tetris.gb", std::ios::in | std::ios::binary);
    fs.read((char*) rom, 0x4000);
    fs.close();

    Memory cart = Memory(rom, 0x4000);
    printf("ROM cart mode: 0x%02x", cart.mode);
    // std::cout << "ROM cart mode: " << std::hex << cart.mode << std::endl;
    return 0;
}