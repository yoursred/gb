#include "gb.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>

#define MS chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()

using namespace std;
int main(void) {
    Gameboy gb;
    
    // Loading the program into memory
    ifstream rom ("bootrom.gb", ios::binary);
    streampos size = rom.tellg();
    rom.seekg(0);
    rom.read((char *)gb.mem, size);

    cout << "Loaded file successfully" << endl;

    // Running the program
    uint64_t time_start = MS;
    while (*gb.R.pc != 0xFFFF) {
        gb.step();
        gb.R.print_regs();
        // fflush(stdout);
    }
    uint64_t time_end = MS;
    uint64_t duration = time_end - time_start;

    printf("\nTime: %luns\n", duration);
    printf("Cycles: %lu\n", gb.cycles);
    printf("Instructions: %lu\n", gb.instructions);
    printf("Frequency: %.4lfMHz\n", (gb.cycles * 1000.0F) / duration);

}